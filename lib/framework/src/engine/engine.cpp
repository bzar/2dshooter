#include "engine.h"
#include "../platform/dlWindow.h"
#include <SDL/SDL.h>

Engine::Engine() : states(), current(0), next(0), running(false)
{
}

Engine::~Engine()
{
  for(StateMap::iterator i = states.begin(); i != states.end(); ++i)
  {
    delete i->second;
  }
}
void Engine::addState(int const id, State* const state)
{
  StateMap::iterator i = states.find(id);
  if(i != states.end()) 
    delete i->second;
  state->setEngine(this);
  states[id] = state;
}

Engine::State* Engine::getState(int const id)
{
  StateMap::iterator i = states.find(id);
  if(i != states.end()) 
  {
    return i->second;
  }
  else
  {
    return 0;
  }
}

bool Engine::changeState(int const id)
{
  StateMap::iterator i = states.find(id);
  if(i != states.end())
  {
    next = i->second;
    return true;
  }
  
  return false;
}

void Engine::run(int const fps, Screen const& screen)
{
  int timer = SDL_GetTicks();
  running = current != 0 || next != 0;
  
  while(running)
  {
    if(current != next) 
    {
      if(current)
      {
        current->exit();
      }
      current = next;
      current->enter();
    }
    
    int now = SDL_GetTicks();
    int delta = now - timer;
    timer = now;
    
    current->clear(screen);
    current->update(screen, delta/1000.0);
    current->draw(screen);
    dlSwapBuffers();

    int timeLeft = 1000/fps - (SDL_GetTicks() - timer);
    if(timeLeft > 0)
    {
        SDL_Delay(timeLeft);
    }
  }
}

void Engine::quit()
{
  if(current)
  {
    current->exit();
  }
  running = false;
}
