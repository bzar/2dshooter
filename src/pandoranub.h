#ifndef PANDORA_NUB_H
#define PANDORA_NUB_H

#include <string>
#include <SDL/SDL.h>

class PandoraNub
{
public:
  enum Nub { LEFT_NUB, RIGHT_NUB };
  
  PandoraNub(Nub const nub);
  ~PandoraNub();
  
  int x();
  int y();
  
  Nub const nub;
  
  static void initialize();
  static void finalize();
  
private:
  void update();
  
  static char const* PANDORA_LEFT_NUB_CONFIG_FILE;
  static char const* PANDORA_RIGHT_NUB_CONFIG_FILE;
  static char const* PANDORA_LEFT_NUB_EVDEV_NAME;
  static char const* PANDORA_RIGHT_NUB_EVDEV_NAME;

  static std::string _currentModeLeft;
  static std::string _currentModeRight;

  int _x;
  int _y;

  SDL_Joystick* _joy;
};
#endif