#ifndef QUAD_H
#define QUAD_H

#include "vec2d.h"
#include "rect.h"
#include "../platform/gl.h"

class Quad {
public:
  Quad(float x, float y, float w, float h);
  Quad(Vec2D const& position, Vec2D const& horizontalDelta, Vec2D const& verticalDelta);
  Quad(Vec2D const& a, Vec2D const& b, Vec2D const& c, Vec2D const& d);

  void coordArray(GLfloat* array) const;
  
  Rect boundingRect() const;

  Vec2D a;
  Vec2D b;
  Vec2D c;
  Vec2D d;
};
#endif