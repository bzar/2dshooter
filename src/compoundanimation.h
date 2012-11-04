#ifndef COMPOUNDANIMATION_HH
#define COMPOUNDANIMATION_HH

#include "animation.h"
#include "skeleton.h"
#include <memory>
#include <vector>

class CompoundAnimation : public Animation
{
public:
  CompoundAnimation();

  virtual void animate(float const delta, Skeleton* skeleton) = 0;
  virtual bool isFinished() const;
  virtual void reset();

  void setLoops(int const value);

  void addAnimation(Animation::Reference animation);
  void resetAnimations();

protected:
  int loops;
  int loop;

  std::vector<Animation::Reference> animations;
};

#endif
