#include "screen.h"
#include <iostream>

Screen::Screen(float const& screenWidth, float const& screenHeight) : w(screenWidth), h(screenHeight), t()
{
  t.move(-w/2.0, -h/2.0).scale(2.0/w, 2.0/h);
}

float Screen::width() const
{
  return w;
}

float Screen::height() const
{
  return h;
}

Transformation const& Screen::transformation() const
{
  return t;
}

