#ifndef PAUSEANIMATION_HH
#define PAUSEANIMATION_HH

#include "animation.h"

class PauseAnimation : public Animation
{
public:
  PauseAnimation();

  void animate(float const delta, Skeleton* skeleton);
  bool isFinished() const;
  void reset();
  Animation::Reference clone() const;
  
  void setDuration(float const value);
  void setLoops(int const value);

private:
  float duration;
  float time;
  int loops;
  int loop;
};

#endif
