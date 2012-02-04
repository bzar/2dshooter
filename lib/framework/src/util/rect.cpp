#include "rect.h"

Rect::Rect(float const left, float const bottom, float const w, float const h) : x(left), y(bottom), w(w), h(h)
{
}

Rect::Rect(Vec2D const& position, float const w, float const h) : x(position.x), y(position.y), w(w), h(h)
{
}

Rect::Rect(Rect const& other) : x(other.x), y(other.y), w(other.w), h(other.h)
{
}


float Rect::top() const
{
  return y + h;
}

float Rect::bottom() const
{
  return y;
}

float Rect::left() const
{
  return x;
}

float Rect::right() const
{
  return x + w;
}

float Rect::verticalCenter() const
{
  return y + h / 2;
}

float Rect::horizontalCenter() const
{
  return x + w / 2;
}


float Rect::width() const
{
  return w;
}

float Rect::height() const
{
  return h;
}


Vec2D Rect::center() const
{
  return Vec2D(horizontalCenter(), verticalCenter());
}

Vec2D Rect::topLeft() const
{
  return Vec2D(left(), top());
}

Vec2D Rect::topRight() const
{
  return Vec2D(right(), top());
}

Vec2D Rect::bottomLeft() const
{
  return Vec2D(left(), bottom());
}

Vec2D Rect::bottomRight() const
{
  return Vec2D(right(), bottom());
}


Rect& Rect::setTop(float const value)
{
  y = value - h;
  return *this;
}

Rect& Rect::setBottom(float const value)
{
  y = value;
  return *this;
}

Rect& Rect::setLeft(float const value)
{
  x = value;
  return *this;
}

Rect& Rect::setRight(float const value)
{
  x = value - w;
  return *this;
}

Rect& Rect::setVerticalCenter(float const value)
{
  y = value - h/2;
  return *this;
}

Rect& Rect::setHorizontalCenter(float const value)
{
  x = value - w/2;
  return *this;
}


Rect& Rect::setWidth(float const value)
{
  w = value;
  return *this;
}

Rect& Rect::setHeight(float const value)
{
  h = value;
  return *this;
}

Rect& Rect::setCenter(Vec2D const& position)
{
  setHorizontalCenter(position.x);
  setVerticalCenter(position.y);
  return *this;
}

Rect& Rect::setTopLeft(Vec2D const& position)
{
  setLeft(position.x);
  setTop(position.y);
  return *this;
}

Rect& Rect::setTopRight(Vec2D const& position)
{
  setRight(position.x);
  setTop(position.y);
  return *this;
}

Rect& Rect::setBottomLeft(Vec2D const& position)
{
  setLeft(position.x);
  setBottom(position.y);
  return *this;
}

Rect& Rect::setBottomRight(Vec2D const& position)
{
  setRight(position.x);
  setBottom(position.y);
  return *this;
}

bool Rect::intersectsWith(Rect const& other) const
{
  return !(x > other.x + other.w || y > other.y + other.h || x + w < other.x || y + h < other.y);
}