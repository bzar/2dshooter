#include "levelterrain.h"

ew::UID const LevelTerrain::ID = ew::getUID();

glhckTextureParameters const LevelTerrain::TEXTURE_PARAMETERS = {
  .minLod = -1000.0f,
  .maxLod = 1000.0f,
  .biasLod = 0.0f,
  .baseLevel = 0,
  .maxLevel = 1000,
  .wrapS = GLHCK_WRAP_REPEAT,
  .wrapT = GLHCK_WRAP_REPEAT,
  .wrapR = GLHCK_WRAP_REPEAT,
  .minFilter = GLHCK_FILTER_NEAREST,
  .magFilter = GLHCK_FILTER_NEAREST,
  .compareMode = GLHCK_COMPARE_NONE,
  .compareFunc = GLHCK_COMPARE_LEQUAL,
  .compression = GLHCK_COMPRESSION_NONE,
  .mipmap = 0,
};

LevelTerrain::LevelTerrain(GameWorld* world, Level const& level, const int zIndex, const int layer): ew::Entity(world), ew::Renderable(world, zIndex, layer),
  objects()
{
  for(Level::Line const& line : level.getLines())
  {
    Terrain terrain = Terrain::load(line.terrain);
    if(terrain.getFilled())
    {
      addFilledPolygon(line.vertices);
    }

    for(Terrain::Edge const& edge : terrain.getEdges())
    {
      addEdgePolygons(line.vertices, edge, terrain.getSpriteSheet());
    }
  }
}

LevelTerrain::~LevelTerrain()
{

}

void LevelTerrain::render(ew::RenderContext* context)
{
  for(glhckObject* o : objects)
  {
    glhckObjectDraw(o);
  }
}

void LevelTerrain::addFilledPolygon(const std::vector<Vec2D> &vertices)
{
}

void LevelTerrain::addEdgePolygons(const std::vector<Vec2D> &vertices, const Terrain::Edge &edge, SpriteSheet const& spriteSheet)
{
  std::vector<Vec2D> edgeVertices;

  for(int i = 1; i < vertices.size(); ++i)
  {
    Vec2D const& a = vertices.at(i - 1);
    Vec2D const& b = vertices.at(i);
    Vec2D const abNormal = b.subtract(a).normali().negi();
    float abNormalAngle = abNormal.angle();

    if(abNormalAngle < 0)
    {
      abNormalAngle = 1.0f + abNormalAngle;
    }

    if(abNormalAngle >= edge.fromAngle && abNormalAngle <= edge.toAngle)
    {
      if(edgeVertices.empty())
      {
        edgeVertices.push_back(a);
      }
      edgeVertices.push_back(b);
    }
    else if(!edgeVertices.empty())
    {
      addEdgePolygon(edgeVertices, edge, spriteSheet);
      edgeVertices.clear();
    }
  }

  if(!edgeVertices.empty())
  {
    addEdgePolygon(edgeVertices, edge, spriteSheet);
  }
}

void LevelTerrain::addEdgePolygon(const std::vector<Vec2D> &vertices, const Terrain::Edge &edge, SpriteSheet const& spriteSheet)
{
  std::vector<glhckVertexData2f> vertexData;

  float l = 0;

  Vec2D prev1;
  Vec2D prev2;

  for(int i = 0; i < vertices.size(); ++i)
  {
    Vec2D const& v1 = vertices.at(i);
    Vec2D v2;

    if(i == 0)
    {
      Vec2D const& n = vertices.at(i + 1);
      v2 = v1.add(n.subtract(v1).normali().uniti() * edge.width);
    }
    else if(i == vertices.size() - 1)
    {
      Vec2D const& p = vertices.at(i - 1);
      v2 = v1.add(v1.subtract(p).normali().uniti() * edge.width);
    }
    else
    {
      Vec2D const& p = vertices.at(i - 1);
      Vec2D const& n = vertices.at(i + 1);
      Vec2D const nw = v1.subtract(p).normali().uniti() * edge.width;
      Vec2D const d = (p - v1).uniti() + (n - v1).uniti();
      v2 = v1 + nw.reverseProjection(d);
    }

    if(i > 0)
    {
      float pl = l;
      l += v1.subtract(prev1).length();
      Vec2D delta = v1 - prev1;
      Vec2D direction = delta.unit();
      float l2 = (v2 - v1).dot(direction) + l;
      float pl2 = (prev2 - prev1).dot(direction) + pl;

      vertexData.push_back({{prev1.x, prev1.y}, {0, 0, 0}, {pl/edge.width, 1}, {255,255,255,255}});
      vertexData.push_back({{v2.x, v2.y}, {0, 0, 0}, {l2/edge.width, 0}, {255,255,255,255}});
      vertexData.push_back({{prev2.x, prev2.y}, {0, 0, 0}, {pl2/edge.width, 0}, {255,255,255,255}});

      vertexData.push_back({{prev1.x, prev1.y}, {0, 0, 0}, {pl/edge.width, 1}, {255,255,255,255}});
      vertexData.push_back({{v1.x, v1.y}, {0, 0, 0}, {l/edge.width, 1}, {255,255,255,255}});
      vertexData.push_back({{v2.x, v2.y}, {0, 0, 0}, {l2/edge.width, 0}, {255,255,255,255}});

    }


    prev1 = v1;
    prev2 = v2;
  }

  glhckObject* o = glhckObjectNew();
  glhckGeometry* g = glhckObjectNewGeometry(o);
  glhckTexture* tex = glhckTextureNew(spriteSheet.getImage().data(), 0, &TEXTURE_PARAMETERS);
  glhckObjectTexture(o, tex);
  glhckTextureFree(tex);
  //glhckObjectMaterialFlags(o, GLHCK_MATERIAL_COLOR);
  g->type = GLHCK_TRIANGLES;
  glhckGeometrySetVertices(g, GLHCK_VERTEX_V2F, vertexData.data(), vertexData.size());
  glhckObjectUpdate(o);

  objects.push_back(o);
}
