#include "screen.h"
#include <SDL/SDL.h>

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

int Screen::windowWidth() const
{
  return SDL_GetVideoSurface()->w;

}

int Screen::windowHeight() const
{
  return SDL_GetVideoSurface()->h;
}

Vec2D Screen::windowToScreenCoordinates(Vec2D const& windowCoordinates) const
{
  return Vec2D(windowCoordinates.x * w / windowWidth(), h - windowCoordinates.y * h / windowHeight());
}

Transformation const& Screen::transformation() const
{
  return t;
}

