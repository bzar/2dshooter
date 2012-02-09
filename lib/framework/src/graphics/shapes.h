#ifndef SHAPES_H
#define SHAPES_H

#include "../util/transformation.h"
#include "../util/vec2d.h"
namespace Shapes
{
  void arrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, 
             float const r, float const g, float const b, 
             float const lineWidth, float const tipLength, float const tipWidth);
}
#endif