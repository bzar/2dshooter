#include "segment.h"

Segment::Segment(Vec2D const& a, Vec2D const& b) : a(a), b(b)
{

}

Segment::Segment(Segment const& other) : a(other.a), b(other.b)
{

}

Vec2D Segment::delta() const
{
  return b.subtract(a);
}
bool Segment::intersects(Segment const& other) const
{
  Vec2D selfDelta = delta();
  Vec2D otherDelta = other.delta();

  if(selfDelta.cross(otherDelta) != 0)
  {
    float t = other.a.subtract(a).cross(otherDelta) / selfDelta.cross(otherDelta);
    float u = other.a.subtract(a).cross(selfDelta) / selfDelta.cross(otherDelta);
    if(t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
      return true;
    }
  }

  return false;
}

Vec2D Segment::intersectionPoint(Segment const& other) const
{
  Vec2D selfDelta = delta();
  Vec2D otherDelta = other.delta();

  if(selfDelta.cross(otherDelta) != 0)
  {
    float t = other.a.subtract(a).cross(otherDelta) / selfDelta.cross(otherDelta);
    float u = other.a.subtract(a).cross(selfDelta) / selfDelta.cross(otherDelta);
    if(t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
      return a.add(selfDelta.scale(t));
    }
  }

  return Vec2D(0, 0);
}

std::ostream& operator<<(std::ostream& stream, Segment const& value)
{
  stream << "Segment(" << value.a << ", " << value.b << ")";
}
