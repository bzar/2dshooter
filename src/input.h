#ifndef INPUT_H
#define INPUT_H

#include "framework.h"
#include <fstream>

#ifdef PANDORA
#include "pandoranub.h"
#endif

class Input
{
public:
  Input();
  ~Input();

  void update();
  
  bool left() const;
  bool right() const;
  bool jump() const;
  bool shoot() const;
  bool quit() const;
  
  bool drawing() const;
  Vec2D mousePosition() const;
  
  Vec2D aimDirection() const;
  
private:
  bool _left;
  bool _right;
  bool _jump;
  bool _shoot;
  bool _quit;
  
  bool _drawing;
  Vec2D _mousePosition;
  
  Vec2D _aimDirection;
  
#ifdef PANDORA
  PandoraNub _rightNub;
#endif
};
#endif