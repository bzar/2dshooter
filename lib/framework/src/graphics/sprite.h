#ifndef SPRITE_HH
#define SPRITE_HH

#include "image.h"
#include "../util/vec2d.h"
#include "../util/screen.h"

class Sprite
{
public:
  Sprite(Image const& image, Vec2D const& position);
  
  Vec2D getPosition() const;
  float getRotation() const;
  float getScale() const;

  void setPosition(Vec2D const& newPosition);
  void setRotation(float const newRotation);
  void setScale(float const newScale);

  void move(Vec2D const& delta);
  void rotate(float const delta);
  void scale(float const amount);

  void render(Transformation const& view);
  void setParent(Sprite* const newParent);
  Transformation getTransformation();
  
private:
  void updateTransformation();
  
  Image image;
  Vec2D position;
  float rotation;
  float scaleAmount;
  Transformation transformation;
  bool dirtyTransformation;
  Sprite* parent;
};
#endif