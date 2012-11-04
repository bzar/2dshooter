#include "parallelanimation.h"

ParallelAnimation::ParallelAnimation() :
  CompoundAnimation()
{

}

void ParallelAnimation::animate(float const delta, Skeleton* skeleton)
{
  for(Animation::Reference& animation : animations)
  {
    animation->animate(delta, skeleton);
  }

  if((loops == INFINITE_LOOPS || loop < loops) && isFinished())
  {
    loop += 1;
    resetAnimations();
  }
}

Animation::Reference ParallelAnimation::clone() const
{
  ParallelAnimation* a = new ParallelAnimation(*this);

  for(auto i = a->animations.begin(); i != a->animations.end(); ++i) {
    *i = (*i)->clone();
  }

  return Animation::Reference(a);
}
