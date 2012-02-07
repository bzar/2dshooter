#include "shooterstate.h"
#include <SDL/SDL.h>

#include "human.h"
#include "segmentgroup.h"

ShooterState::ShooterState() : Engine::State(), player(0), world(Vec2D(0, -1.0)), view()
{
}

ShooterState::~ShooterState()
{
  exit();
}

void ShooterState::enter()
{
  player = new Human(Vec2D(200, 350));
  world.addEntity(player);
  
  SegmentGroup::SegmentList terrainSegments;
  terrainSegments.push_back(Segment(Vec2D(-50,  50), Vec2D( 50,  50)));
  terrainSegments.push_back(Segment(Vec2D( 50,  50), Vec2D( 50, -50)));
  terrainSegments.push_back(Segment(Vec2D( 50, -50), Vec2D(-50, -50)));
  terrainSegments.push_back(Segment(Vec2D(-50, -50), Vec2D(-50,  50)));
  SegmentGroup* terrain = new SegmentGroup(Vec2D(200, 200), terrainSegments);
  world.addTerrain(terrain);
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
  view.reset().move(Vec2D(screen.width()/2, screen.height()/2).subtracti(player->getPosition()));
  view.apply(screen.transformation());
  world.render(view);
}
  


    

    
