#ifndef SKELETONANIMATION_HH
#define SKELETONANIMATION_HH

#include "skeleton.h"
#include "animation.h"
#include "ease.h"

#include <memory>
#include <functional>
#include <vector>
#include <string>



class SkeletonAnimation : public Animation
{
public:
  class Animator
  {
  public:
    typedef std::shared_ptr<Animator> Reference;
    virtual void execute(Skeleton* skeleton, float const progress) = 0;
    virtual void reset() = 0;
  };

  class BoneAnimator : public Animator
  {
  public:
    typedef std::function<float(Skeleton::Bone*)> ValueGetter;
    typedef std::function<void(Skeleton::Bone*, float const)> ValueSetter;

    BoneAnimator(std::string const& targetName,
                  bool fromSet, float from,
                  bool toSet, float to,
                  bool deltaSet, float delta,
                  ValueSetter setter, ValueGetter getter);

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
    ValueSetter setter;
    ValueGetter getter;
  };


  SkeletonAnimation(Skeleton* skeleton);

  void animate(float const delta);
  bool isFinished() const;
  void reset();

  void setEasing(Ease::EasingFunction func);
  void setDuration(float const value);
  float getDuration() const;
  void setLoops(int const value);
  void addAnimator(Animator* animator);
  void resetAnimators();

private:
  Skeleton* skeleton;
  float duration;
  float time;
  int loops;
  int loop;
  std::vector<Animator::Reference> animators;
  Ease::EasingFunction easing;
};

#endif
