#include "compoundanimation.h"

CompoundAnimation::CompoundAnimation() :
  loops(1), loop(1), animations()
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

void CompoundAnimation::resetAnimations()
{
  for(Animation::Reference const& animation : animations)
  {
    animation->reset();
  }
}
