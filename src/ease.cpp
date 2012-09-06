#include "ease.h"

Ease::EasingFunction Ease::createInOut(EasingFunction const& in, EasingFunction const& out)
{
  return [&](float t){
    return t < 0.5 ? 0.5 * in(2 * t) : 0.5 * out(2 * (t - 0.5)) + 0.5;
  };
}

