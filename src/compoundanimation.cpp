#include "compoundanimation.h"

CompoundAnimation::CompoundAnimation(Skeleton* skeleton) :
  skeleton(skeleton), loops(1), loop(1), animations()
{

}

bool CompoundAnimation::isFinished() const
{
  for(Animation::Reference const& animation : animations)
  {
    if(!animation->isFinished())
    {
      return false;
    }
  }

  return true;
}

void CompoundAnimation::reset()
{
  resetAnimations();
  loop = 1;
}

void CompoundAnimation::setLoops(int const value)
{
  loops = value;
}

void CompoundAnimation::addAnimation(Animation* animation)
{
  animations.push_back(Animation::Reference(animation));
}

Skeleton* CompoundAnimation::getSkeleton() const
{
  return skeleton;
}

void CompoundAnimation::resetAnimations()
{
  for(Animation::Reference const& animation : animations)
  {
    animation->reset();
  }
}
