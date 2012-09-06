#ifndef COMPOUNDANIMATION_HH
#define COMPOUNDANIMATION_HH

#include "animation.h"
#include "skeleton.h"
#include <memory>
#include <vector>

class CompoundAnimation : public Animation
{
public:
  CompoundAnimation(Skeleton* skeleton);

  virtual void animate(float const delta) = 0;
  virtual bool isFinished() const;
  virtual void reset();

  void setLoops(int const value);

  void addAnimation(Animation* animation);
  void resetAnimations();

  Skeleton* getSkeleton() const;

protected:
  Skeleton* skeleton;
  int loops;
  int loop;

  std::vector<Animation::Reference> animations;
};

#endif
