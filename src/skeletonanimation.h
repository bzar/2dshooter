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
    virtual void execute(Skeleton* skeleton, float const progress) = 0;
    virtual void reset() = 0;
  };

  template<typename T>
  class ValueAnimator : public Animator
  {
  public:
    ValueAnimator(bool fromSet, T from,
                  bool toSet, T to,
                  bool deltaSet, T delta)
      : initialValueSet(false), initialValue(0),
        fromSet(fromSet), from(from),
        toSet(toSet), to(to),
        deltaSet(deltaSet), delta(delta)
    {}

    void execute(Skeleton* skeleton, float const progress)
    {
      if(!initialValueSet)
      {
        initialValue = getValue(skeleton);
        initialValueSet = true;
      }

      if(fromSet && toSet)
      {
        setValue(skeleton, from + (to - from) * progress);
      }
      else if(fromSet && deltaSet)
      {
        setValue(skeleton, from + delta * progress);
      }
      else if(toSet && deltaSet)
      {
        setValue(skeleton, to - delta * (1 - progress));
      }
      else if(toSet)
      {
        setValue(skeleton, initialValue + (to - initialValue) * progress);
      }
      else if(deltaSet)
      {
        setValue(skeleton, initialValue + delta * progress);
      }
    }

    void reset()
    {
      initialValueSet = false;
    }

  protected:
    virtual T getValue(Skeleton* skeleton) = 0;
    virtual void setValue(Skeleton* skeleton, T const value) = 0;

  private:
    bool initialValueSet;
    T initialValue;
    bool fromSet;
    T from;
    bool toSet;
    T to;
    bool deltaSet;
    T delta;
  };

  class BoneAnimator : public ValueAnimator<float>
  {
  public:
    BoneAnimator(std::string const& targetName,
                 bool fromSet, float from,
                 bool toSet, float to,
                 bool deltaSet, float delta,
                 float (Skeleton::Bone::*getter)() const,
                 void (Skeleton::Bone::*setter)(float));

  protected:
    float getValue(Skeleton* skeleton);
    void setValue(Skeleton* skeleton, const float value);

  private:
    std::string targetName;
    int targetId;
    float (Skeleton::Bone::*getter)() const;
    void (Skeleton::Bone::*setter)(float);
  };

  class OriginAnimator : public ValueAnimator<Vec2D>
  {
  public:
    OriginAnimator(bool fromSet, Vec2D const& from,
                 bool toSet, Vec2D const& to,
                 bool deltaSet, Vec2D const& delta);
  protected:
    Vec2D getValue(Skeleton* skeleton);
    void setValue(Skeleton* skeleton, const Vec2D value);
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
