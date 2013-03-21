#ifndef SEGMENT_H
#define SEGMENT_H

#include "vec2d.h"

class Segment {
public:
  Segment(Vec2D const& a = Vec2D(0, 0), Vec2D const& b = Vec2D(0, 0));
  Segment(Segment const& other);
  Vec2D delta() const;
  bool intersects(Segment const& other) const;
  Vec2D intersectionPoint(Segment const& other) const;

  Vec2D a;
  Vec2D b;
};

std::ostream& operator<<(std::ostream& stream, Segment const& value);

#endif
