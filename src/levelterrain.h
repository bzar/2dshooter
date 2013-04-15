#ifndef LEVELTERRAIN_H
#define LEVELTERRAIN_H

#include "gameworld.h"
#include "ew/renderable.h"
#include "level.h"
#include "terrain.h"

#include <deque>
#include "glhck/glhck.h"

class LevelTerrain : public ew::Renderable
{
public:
  LevelTerrain(GameWorld* world, Level const& level, int const zIndex = 0, int const layer = 0);
  ~LevelTerrain();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);

private:
  static glhckTextureParameters const EDGE_TEXTURE_PARAMETERS;
  static glhckTextureParameters const FILL_TEXTURE_PARAMETERS;

  void addFilledPolygon(std::vector<Vec2D> const& vertices, std::string const& image, float const scale, bool const background);
  void addEdgePolygons(std::vector<Vec2D> const& vertices, Terrain::Edge const& edge, bool const background);
  void addEdgePolygon(std::vector<Vec2D> const& vertices, Terrain::Edge const& edge, bool const background);

  std::deque<glhckObject*> objects;
};


#endif // LEVELTERRAIN_H
