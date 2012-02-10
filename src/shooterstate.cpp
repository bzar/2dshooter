#include "shooterstate.h"
#include <SDL/SDL.h>

#include "player.h"
#include "segmentgroup.h"

ShooterState::ShooterState() : Engine::State(), player(0), world(Vec2D(0, -9.8)), view(), newTerrainSegment(), drawingTerrain(false)
{
}

ShooterState::~ShooterState()
{
  exit();
}

void ShooterState::enter()
{
  player = new Player(Vec2D(200, 350));
  world.addEntity(player);
  
  SegmentGroup::SegmentList terrainSegments;
  terrainSegments.push_back(Segment(Vec2D(150, 250), Vec2D(250, 250)));
  terrainSegments.push_back(Segment(Vec2D(250, 250), Vec2D(250, 150)));
  terrainSegments.push_back(Segment(Vec2D(250, 150), Vec2D(150, 150)));
  terrainSegments.push_back(Segment(Vec2D(150, 150), Vec2D(150, 250)));
  terrainSegments.push_back(Segment(Vec2D(100, 100), Vec2D(300, 100)));
  terrainSegments.push_back(Segment(Vec2D(300, 100), Vec2D(450, 100)));
  //terrainSegments.push_back(Segment(Vec2D(-150, -100), Vec2D(-100, -100)));
  SegmentGroup* terrain = new SegmentGroup(Vec2D(0, 0), terrainSegments);
  world.addTerrain(terrain);
  
  terrain->addSegment(Segment(Vec2D(250, 100), Vec2D(100, 100)));
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
    else if(event.type == SDL_MOUSEBUTTONDOWN) 
    { 
      if( event.button.button == SDL_BUTTON_LEFT ) 
      { 
        newTerrainSegment.a.x = event.button.x - screen.width()/2 + player->getPosition().x;
        newTerrainSegment.a.y = (screen.height() - event.button.y) - screen.height()/2 + player->getPosition().y;
        drawingTerrain = true;
      }
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    { 
      if(event.button.button == SDL_BUTTON_LEFT && drawingTerrain)
      { 
        newTerrainSegment.b.x = event.button.x - screen.width()/2 + player->getPosition().x;
        newTerrainSegment.b.y = (screen.height() - event.button.y) - screen.height()/2 + player->getPosition().y;
        drawingTerrain = false;
        (*(world.getTerrain().begin()))->addSegment(newTerrainSegment);
      }
    }
  }
  
  Uint8 *keystates = SDL_GetKeyState( NULL );
  
  if( keystates[SDLK_UP] ) 
  { 
    player->jump();
  }

  if(!(keystates[SDLK_LEFT] || keystates[SDLK_RIGHT]))
  { 
    player->stop();
  }
  else
  {
    if(keystates[SDLK_LEFT])
    {
      player->moveLeft();
    }
    if(keystates[SDLK_RIGHT])
    {
      player->moveRight();
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
  


    

    
