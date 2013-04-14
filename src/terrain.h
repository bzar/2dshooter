#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <vector>
#include <map>

class Terrain
{
public:
  struct Edge {
    Edge() : fromAngle(0), toAngle(1), offset(0), image(), width(1) {}
    float fromAngle;
    float toAngle;
    float offset;
    std::string image;
    float width;
  };

  Terrain();
  static Terrain load(std::string const& filename);

  std::string const& getFill() const;
  float getFillScale() const;
  std::vector<Edge> const& getEdges() const;


private:

  static std::map<std::string, Terrain> cache;

  std::string fill;
  float fillScale;
  std::vector<Edge> edges;
};

#endif // TERRAIN_H
