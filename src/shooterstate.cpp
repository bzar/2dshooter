#include "shooterstate.h"
#include <SDL/SDL.h>
#include <cmath>
#include "player.h"
#include "segmentgroup.h"

ShooterState::ShooterState() : Engine::State(), world(Vec2D(0, -9.8)), view(), newTerrainSegment(), drawingTerrain(false)
{
}

ShooterState::~ShooterState()
{
  exit();
}

void ShooterState::enter()
{
  world.loadLevel("levels/test.txt");
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
  world.update(delta);

  Player* player = world.getPlayer();
  SDL_Event event;
  while( SDL_PollEvent( &event ) )
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
        newTerrainSegment.a.x = snapToGrid(newTerrainSegment.a.x);
        newTerrainSegment.a.y = snapToGrid(newTerrainSegment.a.y);
        newTerrainSegment.b = newTerrainSegment.a;
        drawingTerrain = true;
      }
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    { 
      if(event.button.button == SDL_BUTTON_LEFT && drawingTerrain)
      { 
        newTerrainSegment.b.x = event.button.x - screen.width()/2 + player->getPosition().x;
        newTerrainSegment.b.y = (screen.height() - event.button.y) - screen.height()/2 + player->getPosition().y;
        newTerrainSegment.b.x = snapToGrid(newTerrainSegment.b.x);
        newTerrainSegment.b.y = snapToGrid(newTerrainSegment.b.y);
        drawingTerrain = false;
        (*(world.getTerrain().begin()))->addSegment(newTerrainSegment);
      }
    }
    else if(event.type == SDL_MOUSEMOTION)
    { 
      if(drawingTerrain)
      { 
        newTerrainSegment.b.x = event.button.x - screen.width()/2 + player->getPosition().x;
        newTerrainSegment.b.y = (screen.height() - event.button.y) - screen.height()/2 + player->getPosition().y;
        newTerrainSegment.b.x = snapToGrid(newTerrainSegment.b.x);
        newTerrainSegment.b.y = snapToGrid(newTerrainSegment.b.y);
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
  
  if(keystates['q'])
    getEngine()->quit();
}

void ShooterState::draw(Screen const& screen)
{
  Player* player = world.getPlayer();
  view.reset().move(Vec2D(screen.width()/2, screen.height()/2).subtracti(player->getPosition()));
  view.apply(screen.transformation());

  Shapes::grid(view, player->getPosition().subtract(Vec2D(screen.width()/2, screen.height()/2)),
               Vec2D(screen.width(), 0), Vec2D(0, screen.height()),
               Vec2D(0, 0), GRID_SIZE, GRID_SIZE, 0.0, 0.3, 0.0);
  world.render(view);
  
  if(drawingTerrain)
  {
    Shapes::arrow(view, newTerrainSegment.a, newTerrainSegment.b, 0.0, 1.0, 0.0, 1, 9, 7);
  }
}

float ShooterState::snapToGrid(float const value) const
{
  return floor(value / GRID_SIZE + 0.5) * GRID_SIZE;
}