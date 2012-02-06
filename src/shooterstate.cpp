#include "shooterstate.h"
#include <SDL/SDL.h>

#include "human.h"
#include "segmentgroup.h"

ShooterState::ShooterState() : Engine::State(), world(Vec2D(0, -0.4))
{
}

ShooterState::~ShooterState()
{
  exit();
}

void ShooterState::enter()
{
  world.addEntity(new Human(Vec2D(200, 350)));
  
  SegmentGroup::SegmentList terrainSegments;
  terrainSegments.push_back(Segment(Vec2D(-50,  50), Vec2D( 50,  50)));
  terrainSegments.push_back(Segment(Vec2D( 50,  50), Vec2D( 50, -50)));
  terrainSegments.push_back(Segment(Vec2D( 50, -50), Vec2D(-50, -50)));
  terrainSegments.push_back(Segment(Vec2D(-50, -50), Vec2D(-50,  50)));
  SegmentGroup* terrain = new SegmentGroup(Vec2D(200, 200), terrainSegments);
  world.addEntity(terrain);
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
  


    

    
