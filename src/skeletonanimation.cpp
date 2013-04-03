#include "skeletonanimation.h"
#include "skeleton.h"
#include <cmath>

SkeletonAnimation::BoneAnimator::BoneAnimator(std::string const& targetName,
                                                bool fromSet, float from,
                                                bool toSet, float to,
                                                bool deltaSet, float delta,
                                                float (Skeleton::Bone::*getter)() const,
                                                void (Skeleton::Bone::*setter)(float))
  : ValueAnimator<float>(fromSet, from, toSet, to, deltaSet, delta),
    targetName(targetName), targetId(-1),
    getter(getter), setter(setter)
{
}

float SkeletonAnimation::BoneAnimator::getValue(Skeleton* skeleton)
{
  if(targetId == -1)
  {
    targetId = skeleton->getBone(targetName).getId();
  }

  Skeleton::Bone& bone = skeleton->getBone(targetId);
  return (bone.*getter)();
}

void SkeletonAnimation::BoneAnimator::setValue(Skeleton* skeleton, float const value)
{
  if(targetId == -1)
  {
    targetId = skeleton->getBone(targetName).getId();
  }

  Skeleton::Bone& bone = skeleton->getBone(targetId);
  return (bone.*setter)(value);
}

void SkeletonAnimation::OriginAnimator::setValue(Skeleton *skeleton, const Vec2D value)
{
  skeleton->setOrigin(value);
}

SkeletonAnimation::OriginAnimator::OriginAnimator(bool fromSet, const Vec2D &from, bool toSet, const Vec2D &to, bool deltaSet, const Vec2D &delta)
  : ValueAnimator<Vec2D>(fromSet, from, toSet, to, deltaSet, delta)
{
}

Vec2D SkeletonAnimation::OriginAnimator::getValue(Skeleton *skeleton)
{
  return skeleton->getOrigin();
}

SkeletonAnimation::SkeletonAnimation() :
  duration(1.0), time(0.0), loops(1), loop(1), animators(),
  easing(Ease::LINEAR)
{

}

SkeletonAnimation::SkeletonAnimation(const SkeletonAnimation& other) :
  duration(other.duration), time(other.time), loops(other.loops), 
  loop(other.loop), animators(other.animators), easing(other.easing)
{

}

SkeletonAnimation::SkeletonAnimation(SkeletonAnimation&& other) :
  duration(std::move(other.duration)), time(std::move(other.time)), 
  loops(std::move(other.loops)), loop(std::move(other.loop)),
  animators(std::move(other.animators)), easing(std::move(other.easing))
{

}

void SkeletonAnimation::animate(float const delta, Skeleton* skeleton)
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

  for(BoneAnimator::Reference animator : animators)
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

Animation::Reference SkeletonAnimation::clone() const
{
  return std::make_shared<SkeletonAnimation>(*this);
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
