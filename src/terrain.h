#ifndef TERRAIN_H
#define TERRAIN_H

#include "spritesheet.h"
#include <string>
#include <vector>
#include <map>

class Terrain
{
public:
  struct Edge {
    float fromAngle;
    float toAngle;
    std::string sprite;
    float width;
  };

  Terrain();
  static Terrain load(std::string const& filename);

  SpriteSheet const& getSpriteSheet() const;
  bool getFilled() const;
  std::vector<Edge> const& getEdges() const;


private:

  static std::map<std::string, Terrain> cache;

  SpriteSheet spriteSheet;
  bool filled;
  std::vector<Edge> edges;
};

#endif // TERRAIN_H
