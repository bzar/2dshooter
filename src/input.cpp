#include "input.h"

#include <fstream>
#include <string>
#include <SDL/SDL.h>

Input::Input() :
  _left(false), _right(false), _jump(false), _shoot(false), 
  _quit(false), _drawing(false),
  _mousePosition(), _aimDirection(1, 0)
#ifdef PANDORA
  , _rightNub(PandoraNub::RIGHT_NUB)
#endif
{
}

Input::~Input()
{  
}


void Input::update()
{
  Uint8 *keystates = SDL_GetKeyState( NULL );
  
#ifdef PANDORA
  _jump = keystates[SDLK_RSHIFT];
  _left = keystates[SDLK_LEFT];
  _right = keystates[SDLK_RIGHT];
  _shoot = keystates[SDLK_RCTRL];
  _quit = keystates[SDLK_ESCAPE];
#else
  _jump =  keystates['w'];
  _left = keystates['a'];
  _right = keystates['d'];
  _shoot = keystates[SDLK_SPACE];
  _quit = keystates['q'];
#endif

  SDL_Event event;
  
  while( SDL_PollEvent( &event ) )
  {
    if( event.type == SDL_QUIT )
    {
      _quit = true;
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN) 
    { 
      if(event.button.button == SDL_BUTTON_LEFT) 
      { 
        _drawing = true;
        _mousePosition = Vec2D(event.button.x, event.button.y);
      }
      else if( event.button.button == SDL_BUTTON_RIGHT ) 
      { 
        _shoot = true;
        _mousePosition = Vec2D(event.button.x, event.button.y);
      }
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    { 
      if( event.button.button == SDL_BUTTON_LEFT ) 
      { 
        _drawing = false;
        _mousePosition = Vec2D(event.button.x, event.button.y);
      }
      else if( event.button.button == SDL_BUTTON_RIGHT ) 
      { 
        _shoot = false;
        _mousePosition = Vec2D(event.button.x, event.button.y);
      }
    }
    else if(event.type == SDL_MOUSEMOTION)
    {
      _mousePosition = Vec2D(event.motion.x, event.motion.y);
    }
  }

  
#ifdef PANDORA
  _aimDirection = Vec2D(_rightNub.x(), -_rightNub.y());
#endif
}


bool Input::left() const
{
  return _left;
}

bool Input::right() const
{
  return _right;
}

bool Input::jump() const
{
  return _jump;
}

bool Input::shoot() const
{
  return _shoot;
}

bool Input::quit() const
{
  return _quit;
}


bool Input::drawing() const
{
  return _drawing;
}

Vec2D Input::mousePosition() const
{
  return _mousePosition;
}

Vec2D Input::aimDirection() const
{
  return _aimDirection;
}

