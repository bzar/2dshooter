#ifndef BULLET_H
#define BULLET_H

#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"
#include "vectorterraincollidable.h"
#include "gameworld.h"
#include "glhck/glhck.h"

class Bullet : public ew::Renderable, public ew::Updatable, public ew::Collidable, public ew::VectorTerrainCollidable
{
public:
  Bullet(GameWorld* world);

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);
  void collide(ew::Collidable const* other);

  Vec2D const& getPosition();
  Vec2D const& getVelocity();

  void setPosition(Vec2D const& newPosition);
  void setVelocity(Vec2D const& newVelocity);

  bool vectorTerrainCollision(Segment const& segment, Vec2D const& collisionPoint, float const timeDelta);

private:
  GameWorld* world;
  glhckObject* o;
  Vec2D position;
  Vec2D velocity;
  float life;
  bool isRicochet;
};

#endif // BULLET_H
