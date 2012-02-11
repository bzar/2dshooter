#ifndef SHAPES_H
#define SHAPES_H

#include "../util/transformation.h"
#include "../util/vec2d.h"
#include "../util/quad.h"
namespace Shapes
{
  void arrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, 
             float const r, float const g, float const b, 
             float const lineWidth, float const tipLength, float const tipWidth);
  void grid(Transformation const& view, Vec2D const& position, Vec2D const& u, Vec2D const& v,
            Vec2D const& origin, float uGridSize, float vGridSize,
            float const r, float const g, float const b, 
            float const lineWidth = -1.0);
}
#endif