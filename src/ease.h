#ifndef EASE_HH
#define EASE_HH

#include <cmath>
#include <functional>
#include <map>
#include <string>

namespace Ease
{
  typedef std::function<float(float)> EasingFunction;
  float const PI = std::acos(-1);
  float const E = std::exp(1);

  EasingFunction createInOut(EasingFunction const& in, EasingFunction const& out);

  EasingFunction const LINEAR = [](float t){ return t; };

  EasingFunction const QUAD_IN = [](float t){ return t * t; };
  EasingFunction const QUAD_OUT = [](float t){ return 2 * t - t * t; };
  EasingFunction const QUAD_INOUT = createInOut(QUAD_IN, QUAD_OUT);

  EasingFunction const SINE_IN = [](float t){ return 1 - std::cos(t * PI / 2); };
  EasingFunction const SINE_OUT = [](float t){ return std::sin(t * PI / 2); };
  EasingFunction const SINE_INOUT = createInOut(SINE_IN, SINE_OUT);

  EasingFunction const EXP_IN = [](float t){ return std::pow(2, 10 * (t - 1)); };
  EasingFunction const EXP_OUT = [](float t){ return 1 - std::pow(2, -10 * t); };
  EasingFunction const EXP_INOUT = createInOut(EXP_IN, EXP_OUT);

  std::map<std::string const, EasingFunction> const BY_NAME = {
    {"Linear", LINEAR},
    {"QuadIn", QUAD_IN},
    {"QuadOut", QUAD_OUT},
    {"QuadInOut", QUAD_INOUT},
    {"SineIn", SINE_IN},
    {"SineOut", SINE_OUT},
    {"SineInOut", SINE_INOUT},
    {"ExpIn", EXP_IN},
    {"ExpOut", EXP_OUT},
    {"ExpInOut", EXP_INOUT},
  };
}

#endif
