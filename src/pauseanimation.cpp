#include "pauseanimation.h"

PauseAnimation::PauseAnimation() :
  duration(1.0), time(0.0), loops(1), loop(1)
{

}

void PauseAnimation::animate(float const delta, Skeleton* skeleton)
{
  time += delta;
  if((loops == INFINITE_LOOPS || loop < loops) && time > duration)
  {
    int n = static_cast<int>(time / duration);
    loop += n;
    time -= n * duration;
  }
}

bool PauseAnimation::isFinished() const
{
  return time >= duration;
}

void PauseAnimation::reset()
{
  time = 0.0;
  loop = 1;
}

Animation::Reference PauseAnimation::clone() const
{
  return std::make_shared<PauseAnimation>(*this);
}


void PauseAnimation::setDuration(float const value)
{
  duration = value;
}

void PauseAnimation::setLoops(int const value)
{
  loop = value;
}
