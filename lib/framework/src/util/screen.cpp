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

float Screen::toRelativeX(float absolute) const
{
  return 2 * absolute / w - 1;
}

float Screen::toAbsoluteX(float relative) const
{
  return (relative + 1) * w / 2;
}

float Screen::toRelativeY(float absolute) const
{
  return 2 * absolute / h - 1;
}

float Screen::toAbsoluteY(float relative) const
{
  return (relative + 1) * h / 2;
}

float Screen::toRelativeWidth(float absolute) const
{
  return absolute / (w / 2);
}

float Screen::toAbsoluteWidth(float relative) const
{
  return relative * (w / 2);
}

float Screen::toRelativeHeight(float absolute) const
{
  return absolute / (h / 2);
}

float Screen::toAbsoluteHeight(float relative) const
{
  return relative * (h / 2);
}

Vec2D Screen::toRelative(Vec2D const& absolute) const
{
  return Vec2D(toRelativeX(absolute.x), toRelativeY(absolute.y));
}

Vec2D Screen::toAbsolute(Vec2D const& relative) const
{
  return Vec2D(toAbsoluteX(relative.x), toAbsoluteY(relative.y));
}

