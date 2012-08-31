#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/collidephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "gameworld.h"

class GameState : public ew::State
{
public:
  GameState(ew::Engine* engine);

private:
  GameWorld world;
  ew::UpdatePhase update;
  ew::CollidePhase collide;
  ew::RenderPhase render;
  ew::ControlPhase control;
};
#endif
