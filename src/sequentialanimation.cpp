#include "sequentialanimation.h"
#include <cmath>

SequentialAnimation::SequentialAnimation() :
  CompoundAnimation()
{

}

void SequentialAnimation::animate(float const delta, Skeleton* skeleton)
{
  for(Animation::Reference& animation : animations)
  {
    if(!animation->isFinished())
    {
      animation->animate(delta, skeleton);

      if((loops == INFINITE_LOOPS || loop < loops) && animation->isFinished() && isFinished())
      {
        loop += 1;
        resetAnimations();
      }
      break;
    }
  }
}

Animation::Reference SequentialAnimation::clone() const
{
  SequentialAnimation* a = new SequentialAnimation(*this);
  for(auto i = a->animations.begin(); i != a->animations.end(); ++i) {
    *i = (*i)->clone();
  }
  
  return Animation::Reference(a);
}
