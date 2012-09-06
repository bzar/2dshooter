#ifndef ANIMATION_HH
#define ANIMATION_HH

#include <memory>

class Animation
{
public:
  typedef std::shared_ptr<Animation> Reference;
  static int const INFINITE_LOOPS = -1;
  virtual void animate(float const delta) = 0;
  virtual bool isFinished() const = 0;
  virtual void setLoops(int const value) = 0;
  virtual void reset() = 0;
};

#endif
