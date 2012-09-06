#include "skeletonanimation.h"
#include "skeleton.h"
#include <cmath>

SkeletonAnimation::BoneAnimator::BoneAnimator(std::string const& targetName,
                                                bool fromSet, float from,
                                                bool toSet, float to,
                                                bool deltaSet, float delta,
                                                ValueSetter setter, ValueGetter getter)
  : targetName(targetName), targetId(-1),
    initialValueSet(false), initialValue(0),
    fromSet(fromSet), from(from),
    toSet(toSet), to(to),
    deltaSet(deltaSet), delta(delta),
    setter(setter), getter(getter)
{
}

void SkeletonAnimation::BoneAnimator::execute(Skeleton* skeleton, float const progress)
{
  if(targetId == -1)
  {
    targetId = skeleton->getBone(targetName)->getId();
  }

  Skeleton::Bone* bone = skeleton->getBone(targetId).get();

  if(!initialValueSet)
  {
    initialValue = getter(bone);
    initialValueSet = true;
  }

  if(fromSet && toSet)
  {
    setter(bone, from + (to - from) * progress);
  }
  else if(fromSet && deltaSet)
  {
    setter(bone, from + delta * progress);
  }
  else if(toSet && deltaSet)
  {
    setter(bone, to - delta * (1 - progress));
  }
  else if(toSet)
  {
    setter(bone, initialValue + (to - initialValue) * progress);
  }
  else if(deltaSet)
  {
    setter(bone, initialValue + delta * progress);
  }
}

void SkeletonAnimation::BoneAnimator::reset()
{
  initialValueSet = false;
}

SkeletonAnimation::SkeletonAnimation(Skeleton* skeleton) :
  skeleton(skeleton), duration(1.0), time(0.0), loops(1), loop(1), animators(),
  easing(Ease::LINEAR)
{

}

void SkeletonAnimation::animate(float const delta)
{
  time += delta;
  if(time > duration)
  {
    if(loop < loops || loops == INFINITE_LOOPS)
    {
      int n = static_cast<int>(time / duration);
      loop += n;
      time -= n * duration;
      resetAnimators();
    }
    else
    {
      time = duration;
    }
  }

  float progress = time < duration ? easing(time / duration) : 1.0;

  for(Animator::Reference animator : animators)
  {
    animator->execute(skeleton, progress);
  }
}

bool SkeletonAnimation::isFinished() const
{
  return time >= duration;
}

void SkeletonAnimation::reset()
{
  resetAnimators();
  time = 0.0;
  loop = 1;
}

void SkeletonAnimation::setEasing(Ease::EasingFunction func)
{
  easing = func;
}

void SkeletonAnimation::setDuration(float const value)
{
  duration = value;
}

float SkeletonAnimation::getDuration() const
{
  return loops != INFINITE_LOOPS ? duration * loops : INFINITY;
}

void SkeletonAnimation::setLoops(int const value)
{
  loops = value;
}

void SkeletonAnimation::addAnimator(Animator* animator)
{
  animators.push_back(Animator::Reference(animator));
}

void SkeletonAnimation::resetAnimators()
{
  for(Animator::Reference animator : animators)
  {
    animator->reset();
  }
}
