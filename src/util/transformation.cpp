#include "transformation.h"
#include <cmath>
float const TAU = 2 * 3.14159265;

float const Transformation::INITIAL_VALUES[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
float const Transformation::ZERO_VALUES[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

namespace
{
  void copyArray(float const* from, float* to, int size)
  {
    for(int i = 0; i < size; ++i)
    {
      to[i] = from[i];
    }
  }
}

Transformation::Transformation()
{
  copyArray(INITIAL_VALUES, values, NUM_VALUES);
}

Transformation::Transformation(Transformation const& other)
{
  copyArray(other.values, values, NUM_VALUES);
}

Transformation& Transformation::reset()
{
  copyArray(INITIAL_VALUES, values, NUM_VALUES);
  return *this;
}

Transformation& Transformation::invert()
{
  float detInv = 1.0 / determinant();
  Transformation adj = adjugate();
  for(int i = 0; i < NUM_VALUES; ++i)
  {
    values[i] = adj.values[i] * detInv;
  }
  return *this;
}

Transformation& Transformation::move(Vec2D const& v)
{
  float const matrix[] = { 1.0,  0.0, v.x,
                           0.0,  1.0, v.y,
                           0.0,  0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::move(float const& x, float const& y)
{
  float const matrix[] = { 1.0,  0.0,   x,
                           0.0,  1.0,   y,
                           0.0,  0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::rotate(float const& r)
{
  float const cs = cos(r * TAU);
  float const sn = sin(r * TAU);
  float const matrix[] = {  cs, -sn, 0.0,
                            sn,  cs, 0.0,
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::scale(float const& k)
{
  float const matrix[] = {   k, 0.0, 0.0,
                           0.0,   k, 0.0,
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::scale(float const& x, float const& y)
{
  float const matrix[] = {   x, 0.0, 0.0,
                           0.0,   y, 0.0,
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::apply(Transformation const& other)
{
  return apply(other.values);
}

Transformation& Transformation::apply(float const matrix[])
{
  float old[NUM_VALUES];
  copyArray(values, old, NUM_VALUES);
  copyArray(ZERO_VALUES, values, NUM_VALUES);

  for( unsigned int m = 0; m < NUM_ROWS; ++m )
  {
    for( unsigned int n = 0; n < NUM_COLS; ++n )
    {
      for( unsigned int k = 0; k < NUM_ROWS; ++k )
      {
        values[m * NUM_ROWS + n] += matrix[m * NUM_ROWS + k] * old[k * NUM_ROWS + n];
      }
    }
  }

  return *this;
}

Vec2D Transformation::transform(Vec2D const& v) const
{
  return Vec2D(v.x * values[0] + v.y * values[1] + values[2], v.x * values[3] + v.y * values[4] + values[5]);
}

Transformation Transformation::toBase(const Vec2D& i, const Vec2D& j)
{
  Transformation t;
  float const values[NUM_VALUES] = {
    i.x, j.x, 0,
    i.y, j.y, 0,
    0,   0,   1
  };
  copyArray(values, t.values, NUM_VALUES);
  return t;
}

Transformation Transformation::fromBase(const Vec2D& i, const Vec2D& j)
{
  return toBase(i, j).invert();
}

float Transformation::determinant()
{
  return (values[0] * values[4] * values[8] + values[1] * values[5] * values[6] + values[2] * values[3] * values[7])
    - (values[2] * values[4] * values[6] + values[0] * values[5] * values[7] + values[1] * values[3] * values[8]);
}

Transformation Transformation::adjugate()
{
  Transformation t;
  t.values[0] = values[4] * values[8] - values[5] * values[7];
  t.values[1] = values[2] * values[7] - values[1] * values[8];
  t.values[2] = values[1] * values[5] - values[2] * values[4];
  t.values[3] = values[5] * values[6] - values[3] * values[8];
  t.values[4] = values[0] * values[8] - values[2] * values[6];
  t.values[5] = values[2] * values[3] - values[0] * values[5];
  t.values[6] = values[3] * values[7] - values[4] * values[6];
  t.values[7] = values[1] * values[6] - values[0] * values[7];
  t.values[8] = values[0] * values[4] - values[1] * values[3];
  return t;
}

float const* Transformation::matrix() const
{
  return values;
}

std::ostream& operator<<(std::ostream& stream, Transformation const& value)
{
  stream << "Transformation( ";
  for( unsigned int m = 0; m < value.NUM_ROWS; ++m )
  {
    stream << "{";
    for( unsigned int n = 0; n < value.NUM_COLS; ++n )
    {
      stream << value.matrix()[m*value.NUM_COLS + n] << (n < value.NUM_COLS - 1 ? ", " : "");
    }
    stream << "} ";
  }
  stream << " )";
}
