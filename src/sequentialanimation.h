#ifndef SEQUENTIALANIMATION_HH
#define SEQUENTIALANIMATION_HH

#include "compoundanimation.h"
#include "skeleton.h"
#include <memory>
#include <vector>

class SequentialAnimation : public CompoundAnimation
{
public:
  SequentialAnimation(Skeleton* skeleton);
  void animate(float const delta);
};

#endif
