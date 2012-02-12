#include "pandoranub.h"
#include <sstream>
#include <fstream>

#include "framework.h"

char const* PandoraNub::PANDORA_LEFT_NUB_CONFIG_FILE = "/proc/pandora/nub0/mode";
char const* PandoraNub::PANDORA_RIGHT_NUB_CONFIG_FILE = "/proc/pandora/nub1/mode";

std::string PandoraNub::_currentModeLeft = "";
std::string PandoraNub::_currentModeRight = "";

PandoraNub::PandoraNub(Nub const nub) :
  nub(nub), _x(0), _y(0), _joy(0)
{
  _joy = SDL_JoystickOpen(nub == LEFT_NUB ? 1 : 2);
}

PandoraNub::~PandoraNub()
{
  Log::debug() << " ~PandoraNub";
  SDL_JoystickClose(_joy);
  Log::debug() << " / ~PandoraNub";
}


int PandoraNub::x()
{
  update();
  return _x;
}

int PandoraNub::y()
{
  update();
  return _y;
}

void PandoraNub::update()
{
  _x = SDL_JoystickGetAxis(_joy, 0);
  _y = SDL_JoystickGetAxis(_joy, 1);
}

void PandoraNub::initialize()
{
  std::ifstream li(PANDORA_LEFT_NUB_CONFIG_FILE);
  li >> _currentModeLeft;
  std::ofstream lo(PANDORA_LEFT_NUB_CONFIG_FILE); 
  lo.write("absolute", 8);
  lo.close();
  
  std::ifstream ri(PANDORA_RIGHT_NUB_CONFIG_FILE);
  ri >> _currentModeRight;
  std::ofstream ro(PANDORA_RIGHT_NUB_CONFIG_FILE); 
  ro.write("absolute", 8);
  ro.close();
}

void PandoraNub::finalize()
{
  std::ofstream lo(PANDORA_LEFT_NUB_CONFIG_FILE); 
  lo.write(_currentModeLeft.c_str(), _currentModeLeft.size());
  std::ofstream ro(PANDORA_RIGHT_NUB_CONFIG_FILE); 
  ro.write(_currentModeRight.c_str(), _currentModeRight.size());  
}