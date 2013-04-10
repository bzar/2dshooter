#include "levelterrain.h"

ew::UID const LevelTerrain::ID = ew::getUID();

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
      addEdgePolygons(line.vertices, edge);
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

void LevelTerrain::addEdgePolygons(const std::vector<Vec2D> &vertices, const Terrain::Edge &edge)
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
      addEdgePolygon(edgeVertices, edge);
      edgeVertices.clear();
    }
  }

  if(!edgeVertices.empty())
  {
    addEdgePolygon(edgeVertices, edge);
  }
}

void LevelTerrain::addEdgePolygon(const std::vector<Vec2D> &vertices, const Terrain::Edge &edge)
{
  std::vector<glhckVertexData2f> vertexData;

  for(int i = 0; i < vertices.size(); ++i)
  {
    Vec2D const& v = vertices.at(i);

    vertexData.push_back({{v.x, v.y}, {0, 0, 0}, {i, 1}, {255,255,255,255}});

    if(i == 0)
    {
      Vec2D const& n = vertices.at(i + 1);
      Vec2D v2 = v.add(n.subtract(v).normali().uniti() * edge.width);
      vertexData.push_back({{v2.x, v2.y}, {0, 0, 0}, {i, 0}, {255,255,255,255}});
    }
    else if(i == vertices.size() - 1)
    {
      Vec2D const& p = vertices.at(i - 1);
      Vec2D v2 = v.add(v.subtract(p).normali().uniti() * edge.width);
      vertexData.push_back({{v2.x, v2.y}, {0, 0, 0}, {i, 0}, {255,255,255,255}});
    }
    else
    {
      Vec2D const& p = vertices.at(i - 1);
      Vec2D const& n = vertices.at(i + 1);
      Vec2D const nw = v.subtract(p).normali().uniti() * edge.width;
      Vec2D const d = (p - v).uniti() + (n - v).uniti();
      Vec2D v2 = v + nw.reverseProjection(d);
      vertexData.push_back({{v2.x, v2.y}, {0, 0, 0}, {i, 0}, {255,255,255,255}});
    }
  }

  glhckObject* o = glhckObjectNew();
  glhckGeometry* g = glhckObjectNewGeometry(o);
  glhckObjectMaterialFlags(o, GLHCK_MATERIAL_COLOR);
  g->type = GLHCK_TRIANGLE_STRIP;
  glhckGeometrySetVertices(g, GLHCK_VERTEX_V2F, vertexData.data(), vertexData.size());
  glhckObjectUpdate(o);

  objects.push_back(o);
}
