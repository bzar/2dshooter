#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <vector>
#include <map>

class Terrain
{
public:
  struct Edge {
    float fromAngle;
    float toAngle;
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
