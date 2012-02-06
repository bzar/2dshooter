#include "shooterstate.h"
#include <SDL/SDL.h>

#include "testentity.h"

ShooterState::ShooterState() : Engine::State(), world(Vec2D(0, -0.4))
{
}

ShooterState::~ShooterState()
{
  exit();
}

void ShooterState::enter()
{
  TestEntity* e1 = new TestEntity(Vec2D(250, 240));
  e1->setZIndex(1);
  world.addEntity(e1);
  
  int const n = 50;
  int k = 0;
  for(int i = 0; i < n; ++i)
  {
    if(i * i >= n)
    {
      k = i;
      break;
    }
  }
  for(int i = 0; i < n; ++i) {
    world.addEntity(e1 = new TestEntity(Vec2D((i%k) * 760 / k + 50 , (i/k) * 440 / k + 50)));
  }
  
  TestEntity* e2 = new TestEntity(Vec2D(650, 240));
  e2->setZIndex(6);
  world.addEntity(e2);
  
  world.removeEntity(e1);
}


void ShooterState::exit()
{
}

void ShooterState::clear(Screen const& screen)
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void ShooterState::update(Screen const& screen, float const& delta)
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

void ShooterState::draw(Screen const& screen)
{
  world.render(screen);
}
  


    

    
