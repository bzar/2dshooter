#ifndef SCREEN_HH
#define SCREEN_HH

#include "vec2d.h"
#include "transformation.h"

class Screen
{
public:
  Screen(float const& screenWidth, float const& screenHeight);
  float width() const;
  float height() const;
  Transformation const& transformation() const;
  float toRelativeX(float absolute) const;
  float toAbsoluteX(float relative) const;
  float toRelativeY(float absolute) const;
  float toAbsoluteY(float relative) const;
  float toRelativeWidth(float absolute) const;
  float toAbsoluteWidth(float relative) const;
  float toRelativeHeight(float absolute) const;
  float toAbsoluteHeight(float relative) const;
  Vec2D toRelative(Vec2D const& absolute) const;
  Vec2D toAbsolute(Vec2D const& relative) const;

private:
  float w;
  float h;
  Transformation t;
};
#endif