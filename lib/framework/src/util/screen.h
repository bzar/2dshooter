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

private:
  float w;
  float h;
  Transformation t;
};
#endif