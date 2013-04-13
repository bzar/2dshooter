#ifndef POLYGON_H
#define POLYGON_H

#include "vec2d.h"
#include "triangle.h"
#include "segment.h"
#include <vector>

class Polygon
{
public:
  Polygon(const std::vector<Vec2D> &vertices);

  std::vector<Triangle> triangulate() const;
  std::vector<Segment> toSegments() const;

private:
  std::vector<Vec2D> vertices;
};

#endif // POLYGON_H
