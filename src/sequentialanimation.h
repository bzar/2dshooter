#ifndef SEQUENTIALANIMATION_HH
#define SEQUENTIALANIMATION_HH

#include "compoundanimation.h"
#include "skeleton.h"
#include <memory>
#include <vector>

class SequentialAnimation : public CompoundAnimation
{
public:
  SequentialAnimation();
  void animate(float const delta, Skeleton* skeleton);
  Animation* clone() const;
};

#endif
