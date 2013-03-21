#include "util/vec2d.h"
#include "util/transformation.h"
#include "util/segment.h"

#include <iostream>
#include <cstdlib>

int main()
{
  Vec2D v(1, 1);
  Transformation fromBaseRotation = Transformation::fromBase({0, 1}, {-1, 0});
  Transformation toBaseRotation = Transformation::toBase({0, 1}, {-1, 0});
  std::cout << "fromBaseRotation: " << fromBaseRotation << std::endl;
  std::cout << "toBaseRotation: " << toBaseRotation << std::endl;

  std::cout << "fromBaseRotation: expected Vec2D(1, -1), got " << fromBaseRotation.transform(v) << std::endl;
  std::cout << "toBaseTotation: expected Vec2D(-1, 1), got " << toBaseRotation.transform(v) << std::endl;
  std::cout << "fromBaseRotation-toBaseRotation: expected Vec2D(1, 1), got " << toBaseRotation.transform(fromBaseRotation.transform(v)) << std::endl;
  std::cout << "toBaseRotation-fromBaseRotation: expected Vec2D(1, 1), got " << fromBaseRotation.transform(toBaseRotation.transform(v)) << std::endl;
  
  Transformation fromBase = Transformation::fromBase({0, 50}, {-50, 0});
  Transformation toBase = Transformation::toBase({0, 50}, {-50, 0});
  Transformation fromToBase = Transformation::fromBase({0, 50}, {-50, 0}).apply(Transformation::toBase({25, 0}, {0, 25}));
  Transformation toFromBase = Transformation::toBase({25, 0}, {0, 25}).apply(Transformation::fromBase({0, 50}, {-50, 0}));
  std::cout << "fromBase: expected Vec2D(0.02, -0.02), got " << fromBase.transform(v) << std::endl;
  std::cout << "toBase: expected Vec2D(-50, 50), got " << toBase.transform(v) << std::endl;
  std::cout << "fromToBase: expected Vec2D(0.5, -0.5), got " << fromToBase.transform(v) << std::endl;
  std::cout << "toFromToBase: expected Vec2D(0.5, -0.5), got " << toFromBase.transform(v) << std::endl;
  
  Segment s1({0, 0}, {10, 0});
  Segment s2({0, 5}, {0, 0});
  
  std::cout << "Segments " << s1 << " and " << s2 << (s1.intersects(s2) ? " intersect" : " don't intersect") << std::endl;
  
  
  
  
  return EXIT_SUCCESS;
}