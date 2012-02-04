#include "quad.h"

Quad::Quad(float x, float y, float w, float h) : 
  a(x-w/2, y-h/2),
  b(x-w/2, y+h/2),
  c(x+w/2, y-h/2),
  d(x+w/2, y+h/2)
{
}

Quad::Quad(Vec2D const& position, Vec2D const& u, Vec2D const& v) : 
  a(position.subtract(u).add(v)), 
  b(position.subtract(u).subtract(v)),
  c(position.add(u).subtract(v)), 
  d(position.add(u).add(v))
{
}

Quad::Quad(Vec2D const& a, Vec2D const& b, Vec2D const& c, Vec2D const& d) : a(a), b(b), c(c), d(d)
{
}

void Quad::coordArray(GLfloat* array) const
{
  array[0] = a.x;
  array[1] = a.y;
  array[2] = b.x;
  array[3] = b.y;
  array[4] = c.x;
  array[5] = c.y;
  array[6] = d.x;
  array[7] = d.y;
}

Rect Quad::boundingRect() const
{
  float minx = a.x;
  if(b.x < minx) minx = b.x;
  if(c.x < minx) minx = c.x;
  if(d.x < minx) minx = d.x;

  float maxx = a.x;
  if(b.x > maxx) maxx = b.x;
  if(c.x > maxx) maxx = c.x;
  if(d.x > maxx) maxx = d.x;

  float miny = a.y;
  if(b.y < miny) miny = b.y;
  if(c.y < miny) miny = c.y;
  if(d.y < miny) miny = d.y;

  float maxy = a.y;
  if(b.y > maxy) maxy = b.y;
  if(c.y > maxy) maxy = c.y;
  if(d.y > maxy) maxy = d.y;

  return Rect(minx, miny, maxx - minx, maxy - miny);
}