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
  int windowWidth() const;
  int windowHeight() const;
  Vec2D windowToScreenCoordinates(Vec2D const& windowCoordinates) const;
  Transformation const& transformation() const;

private:
  float w;
  float h;
  Transformation t;
};
#endif