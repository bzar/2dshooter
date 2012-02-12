#include <SDL/SDL.h>

#include "framework.h"
#include "shooterstate.h"
#include "states.h"

#ifdef PANDORA
  #include "pandoranub.h"

  unsigned int const DISPLAY_FLAGS = SDL_SWSURFACE | SDL_FULLSCREEN;
  unsigned int const SDL_FLAGS = SDL_INIT_VIDEO | SDL_INIT_JOYSTICK;
#else
  unsigned int const DISPLAY_FLAGS = SDL_OPENGL;
  unsigned int const SDL_FLAGS = SDL_INIT_VIDEO;
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
  #ifdef PANDORA
    PandoraNub::initialize();
    sleep(1);
  #endif

  SDL_Init( SDL_FLAGS );

  if(dlCreateWindow(800, 480, 16, DISPLAY_FLAGS))
  {
    Log::error() << dlWindowGetError();
  }
  glInit();
  
  Screen screen(400, 240);
  
  Engine engine;
  engine.addState(STATE_SHOOTER, new ShooterState);
  engine.changeState(STATE_SHOOTER);
  engine.run(60, screen);
  
  dlCloseWindow();
  SDL_Quit();
  
  #ifdef PANDORA
    PandoraNub::finalize();
  #endif

  return 0;
}
