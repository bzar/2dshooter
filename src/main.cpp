#include <SDL/SDL.h>
#include <cmath>

#include "framework.h"
#include "teststate.h"
#include "states.h"

#ifdef GLES1
  const unsigned int DISPLAY_FLAGS = SDL_SWSURFACE | SDL_FULLSCREEN;
#else
  const unsigned int DISPLAY_FLAGS = SDL_OPENGL;
#endif

void glInit()
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
  glDisable(GL_DITHER);  
}

int main( int argc, char* args[] )
{
  SDL_Init( SDL_INIT_VIDEO );
  dlCreateWindow(800, 480, 16, DISPLAY_FLAGS);
  glInit();
  
  Screen screen(800, 480);
  
  Engine engine;
  engine.addState(STATE_TEST, new TestState);
  engine.changeState(STATE_TEST);
  engine.run(60, screen);
  
  dlCloseWindow();
  SDL_Quit();

  return 0;
}
