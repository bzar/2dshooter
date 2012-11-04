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

void CompoundAnimation::addAnimation(Animation::Reference animation)
{
  animations.push_back(animation);
}

void CompoundAnimation::resetAnimations()
{
  for(Animation::Reference const& animation : animations)
  {
    animation->reset();
  }
}
