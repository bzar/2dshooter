#include <SDL/SDL.h>
#include <cmath>

#include "framework.h"
#include "shooterstate.h"
#include "states.h"

#ifdef PANDORA
  const unsigned int DISPLAY_FLAGS = SDL_SWSURFACE | SDL_FULLSCREEN;
#else
  const unsigned int DISPLAY_FLAGS = SDL_OPENGL;
#endif

void glInit()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
  glDisable(GL_DITHER);  
  glShadeModel(GL_FLAT);
}

int main( int argc, char* args[] )
{
  Log::setLevel(Log::LOGLEVEL_DEBUG);
  SDL_Init( SDL_INIT_VIDEO );
  if(dlCreateWindow(800, 480, 16, DISPLAY_FLAGS))
  {
    Log::error() << dlWindowGetError();
  }
  glInit();
  
  Screen screen(800, 480);
  
  Engine engine;
  engine.addState(STATE_SHOOTER, new ShooterState);
  engine.changeState(STATE_SHOOTER);
  engine.run(60, screen);
  
  dlCloseWindow();
  SDL_Quit();

  return 0;
}
