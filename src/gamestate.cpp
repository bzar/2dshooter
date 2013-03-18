#include "gamestate.h"

GameState::GameState(ew::Engine* engine) :
  ew::State(engine, &world), world(),
  update(&world), collide(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext()),
  vectorTerrain(&world)
{
  setPhases({&control, &vectorTerrain, &update, &collide, &render});
}
