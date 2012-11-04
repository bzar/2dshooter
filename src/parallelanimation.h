#ifndef PARALLELANIMATION_HH
#define PARALLELANIMATION_HH

#include "compoundanimation.h"
#include "skeleton.h"
#include <memory>
#include <vector>

class ParallelAnimation : public CompoundAnimation
{
public:
  ParallelAnimation();
  void animate(float const delta, Skeleton* skeleton);
  Animation::Reference clone() const;
};

#endif
