#include "shooterstate.h"
#include <SDL/SDL.h>
#include <cmath>
#include "player.h"
#include "segmentgroup.h"

ShooterState::ShooterState() : 
  Engine::State(), world(Vec2D(0, -9.8)), input(), view(), viewCenter(), newTerrainSegment(), drawingTerrain(false)
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

Vec2D windowToWorldCoords(Screen const& screen, Vec2D const& viewCenter, Vec2D const& windowCoords)
{
  return screen.windowToScreenCoordinates(windowCoords) + viewCenter - Vec2D(screen.width()/2, screen.height()/2);
}

void ShooterState::update(Screen const& screen, float const& delta)
{
  Player* player = world.getPlayer();
  input.update();

  Vec2D mousePosition = windowToWorldCoords(screen, viewCenter, input.mousePosition());
  
  if(input.drawing() && !drawingTerrain)
  { 
    newTerrainSegment.a = snapToGrid(mousePosition);
    newTerrainSegment.b = newTerrainSegment.a;
    drawingTerrain = true;
  }
  else if(input.drawing() && drawingTerrain)
  { 
    newTerrainSegment.b = snapToGrid(mousePosition);
  }
  else if(!input.drawing() && drawingTerrain)
  { 
    newTerrainSegment.b = snapToGrid(mousePosition);
    drawingTerrain = false;
    (*(world.getTerrain().begin()))->addSegment(newTerrainSegment);
  }
  
    
  if(input.jump())                     player->jump();
  if(!(input.left() || input.right())) player->stop();
  if(input.left())                     player->moveLeft();
  if(input.right())                    player->moveRight();
  if(input.shoot())                    player->shoot();
  if(input.quit())                     getEngine()->quit();

#ifdef PANDORA
  player->aimAt(player->getPosition() + input.aimDirection());
#else
  player->aimAt(mousePosition);
#endif
  viewCenter = player->getPosition();
  
  world.update(delta);
}

void ShooterState::draw(Screen const& screen)
{
  view.reset().move(Vec2D(screen.width()/2, screen.height()/2).subtracti(viewCenter));
  view.apply(screen.transformation());

  Shapes::grid(view, viewCenter.subtract(Vec2D(screen.width()/2, screen.height()/2)),
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

Vec2D ShooterState::snapToGrid(Vec2D const& value) const
{
  return Vec2D(snapToGrid(value.x), snapToGrid(value.y));
}