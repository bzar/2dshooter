#include "vectorterrain.h"
#include <iostream>

ew::UID const VectorTerrain::ID = ew::getUID();

VectorTerrain::VectorTerrain(GameWorld* world, std::list<Segment> const& segments, const int zIndex, const int layer): ew::Entity(world), ew::Renderable(world, zIndex, layer),
  arrows()
{
  createArrows(segments);
}

VectorTerrain::~VectorTerrain()
{

}

void VectorTerrain::render(ew::RenderContext* context)
{
  for(glhckObject* arrow : arrows)
  {
    glhckObjectDraw(arrow);
  }
}

glhckObject* VectorTerrain::createArrow(Vec2D const& base, Vec2D const& tip,
                                        unsigned char const r, unsigned char const g, unsigned char const b,
                                        float const lineWidth, float const tipLength, float const tipWidth)
{
  
  Vec2D const delta = tip - base;
  Vec2D const normal = delta.normal().uniti();
  float const length = delta.length();
  Vec2D const c = delta.scale((length - tipLength) / length);
  
  Vec2D const l1 = normal.scale(0.5 * lineWidth);
  Vec2D const l2 = c + normal.scale(0.5 * lineWidth);
  Vec2D const l3 = c - normal.scale(0.5 * lineWidth);
  Vec2D const l4 = normal.scale(0.5 * lineWidth).negi();
  
  Vec2D const t1 = c - normal.scale(0.5 * tipWidth);
  Vec2D const t2 = c + normal.scale(0.5 * tipWidth);
  Vec2D const t3 = delta;
  
  int const NUM_ARROW_VERTICES = 9;
  glhckVertexData2f arrow[] = {
    {{l1.x, l1.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{l2.x, l2.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{l4.x, l4.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    
    {{l4.x, l4.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{l2.x, l2.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{l3.x, l3.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    
    {{t1.x, t1.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{t2.x, t2.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
    {{t3.x, t3.y}, {0, 0}, {0, 0}, {r, g, b, 255}},
  };
  
  glhckObject* o = glhckObjectNew();
  glhckGeometry* geometry = glhckObjectNewGeometry(o);
  geometry->type = GLHCK_TRIANGLES;
  glhckObjectPositionf(o, base.x, base.y, 0);
  glhckObjectMaterialFlags(o, GLHCK_MATERIAL_COLOR);
  glhckGeometrySetVertices(geometry, GLHCK_VERTEX_V2F, arrow, NUM_ARROW_VERTICES);
  glhckObjectUpdate(o);
  
  return o;
}

void VectorTerrain::createArrows(const std::list< Segment >& segments)
{
  for(Segment const& segment : segments)
  {
    glhckObject* arrow = createArrow(segment.a, segment.b, 0, 255, 0, 0.5, 2.5, 2.5);
    arrows.push_back(arrow);
  }
}
