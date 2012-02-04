#ifndef ENGINE_H
#define ENGINE_H

#include "../util/screen.h"
#include <map>

class Engine {
public:
  class State
  {
  public:
    State() : engine(0) {};
    virtual void enter() {};
    virtual void exit() {};
    virtual void clear(Screen const& screen) {};
    virtual void update(Screen const& screen, float const& delta) {};
    virtual void draw(Screen const& screen) {};
    void setEngine(Engine* const newEngine) { engine = newEngine; }
    Engine* getEngine() const { return engine; }
  private:
    Engine* engine;
  };
  
  Engine();
  ~Engine();
  
  void addState(int const id, State* const state);
  State* getState(int const id);
  bool changeState(int const id);
  void run(int const fps, Screen const& screen);
  void quit();
private:
  typedef std::map<int, State*> StateMap;
  StateMap states;
  State* current;
  State* next;
  bool running;
};
#endif