#include "teststate.h"
#include <SDL/SDL.h>

#include "testentity.h"

TestState::TestState() : Engine::State(), world()
{
}

TestState::~TestState()
{
  exit();
}

void TestState::enter()
{
  world.addEntity(new TestEntity(Vec2D(100, 240)));
  world.addEntity(new TestEntity(Vec2D(350, 240)));
  world.addEntity(new TestEntity(Vec2D(600, 240)));
}


void TestState::exit()
{
}

void TestState::clear(Screen const& screen)
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void TestState::update(Screen const& screen, float const& delta)
{
  SDL_Event event;
  if( SDL_PollEvent( &event ) )
  {
      if( event.type == SDL_QUIT )
      {
        getEngine()->quit();
      }
      else if(event.type == SDL_KEYDOWN)
      {
        getEngine()->quit();
      }
  }
  
  world.update(delta);
}

void TestState::draw(Screen const& screen)
{
  world.render(screen);
}
  


    

    
