#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec2d.h"

class Triangle
{
public:
  Triangle(Vec2D a, Vec2D b, Vec2D c);

  Vec2D a;
  Vec2D b;
  Vec2D c;
};

#endif // TRIANGLE_H
