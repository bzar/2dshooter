#ifndef ANIMATION_HH
#define ANIMATION_HH

#include <memory>

class Skeleton;

class Animation
{
public:
  typedef std::shared_ptr<Animation> Reference;
  static int const INFINITE_LOOPS = -1;
  virtual void animate(float const delta, Skeleton* skeleton) = 0;
  virtual bool isFinished() const = 0;
  virtual void setLoops(int const value) = 0;
  virtual void reset() = 0;
  virtual Reference clone() const = 0;
};

#include "skeleton.h"

#endif
