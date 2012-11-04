#ifndef SKELETONANIMATION_HH
#define SKELETONANIMATION_HH

#include "animation.h"
#include "ease.h"

#include <memory>
#include <functional>
#include <vector>
#include <string>

#include "skeleton.h"

class SkeletonAnimation : public Animation
{
public:
  class Animator
  {
  public:
    typedef std::shared_ptr<Animator> Reference;

    Animator(std::string const& targetName,
                  bool fromSet, float from,
                  bool toSet, float to,
                  bool deltaSet, float delta,
                  float (Skeleton::Bone::*getter)() const,
                  void (Skeleton::Bone::*setter)(float));

    void execute(Skeleton* skeleton, float const progress);
    void reset();

  private:
    std::string targetName;
    int targetId;
    bool initialValueSet;
    float initialValue;
    bool fromSet;
    float from;
    bool toSet;
    float to;
    bool deltaSet;
    float delta;
    float (Skeleton::Bone::*getter)() const;
    void (Skeleton::Bone::*setter)(float);
  };


  SkeletonAnimation();
  SkeletonAnimation(SkeletonAnimation const& other);
  SkeletonAnimation(SkeletonAnimation&& other);
  
  void animate(float const delta, Skeleton* skeleton);
  bool isFinished() const;
  void reset();
  Animation::Reference clone() const;
  
  void setEasing(Ease::EasingFunction func);
  void setDuration(float const value);
  float getDuration() const;
  void setLoops(int const value);
  void addAnimator(Animator* animator);
  void resetAnimators();

private:
  float duration;
  float time;
  int loops;
  int loop;
  std::vector<Animator::Reference> animators;
  Ease::EasingFunction easing;
};

#endif
