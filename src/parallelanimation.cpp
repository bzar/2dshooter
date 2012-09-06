#include "parallelanimation.h"

ParallelAnimation::ParallelAnimation(Skeleton* skeleton) :
  CompoundAnimation(skeleton)
{

}

void ParallelAnimation::animate(float const delta)
{
  for(Animation::Reference& animation : animations)
  {
    animation->animate(delta);
  }

  if((loops == INFINITE_LOOPS || loop < loops) && isFinished())
  {
    loop += 1;
    resetAnimations();
  }
}
