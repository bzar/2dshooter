#include "gamestate.h"

GameState::GameState(ew::Engine* engine) :
  ew::State(engine, &world), world(),
  update(&world), collide(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &collide, &render});
}
