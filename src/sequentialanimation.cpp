#include "sequentialanimation.h"
#include <cmath>

SequentialAnimation::SequentialAnimation(Skeleton* skeleton) :
  CompoundAnimation(skeleton)
{

}

void SequentialAnimation::animate(float const delta)
{
  for(Animation::Reference& animation : animations)
  {
    if(!animation->isFinished())
    {
      animation->animate(delta);

      if((loops == INFINITE_LOOPS || loop < loops) && animation->isFinished() && isFinished())
      {
        loop += 1;
        resetAnimations();
      }
      break;
    }
  }
}
