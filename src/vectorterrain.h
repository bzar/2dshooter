#ifndef VECTORTERRAIN_HH
#define VECTORTERRAIN_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "util/vec2d.h"
#include <list>
#include <vector>

#include "glhck/glhck.h"

class VectorTerrain : public ew::Renderable
{
public:
  VectorTerrain(GameWorld* world, std::list<Segment> const& segments, int const zIndex = 0, int const layer = 0);
  ~VectorTerrain();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);

private:
  static glhckObject* createArrow(Vec2D const& base, Vec2D const& tip,
                                  unsigned char const r, unsigned char const g, unsigned char const b,
                                  float const lineWidth, float const tipLength, float const tipWidth);
  
  void createArrows(std::list<Segment> const& segments);

  std::vector<glhckObject*> arrows;
};

#endif
