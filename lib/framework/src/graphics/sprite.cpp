#include "sprite.h"
#include "../util/quad.h"

Sprite::Sprite(Image const& image, Vec2D const& position, Sprite* parent) : 
  image(image), origin(), position(position), rotation(0), scaleAmount(1), 
  mirrorX(false), mirrorY(false), transformation(), dirtyTransformation(true), parent(parent)
{
}

Vec2D Sprite::getOrigin() const
{
  return origin;
}

Vec2D Sprite::getPosition() const
{
  return position;
}

float Sprite::getRotation() const
{
  return rotation;
}

float Sprite::getScale() const
{
  return scaleAmount;
}

bool Sprite::getMirrorX() const
{
  return mirrorX;
}

bool Sprite::getMirrorY() const
{
  return mirrorY;
}

void Sprite::setOrigin(Vec2D const& newOrigin)
{
  origin = newOrigin;
  dirtyTransformation = true;
}

void Sprite::setPosition(Vec2D const& newPosition)
{
  position = newPosition;
  dirtyTransformation = true;
}

void Sprite::setRotation(float const newRotation)
{
  rotation = newRotation;
  dirtyTransformation = true;
}

void Sprite::setScale(float const newScale)
{
  scaleAmount = newScale;
  dirtyTransformation = true;
}

void Sprite::setMirrorX(bool const newMirrorX)
{
  mirrorX = newMirrorX;
  dirtyTransformation = true;
}

void Sprite::setMirrorY(bool const newMirrorY)
{
  mirrorY = newMirrorY;
  dirtyTransformation = true;
}

void Sprite::move(Vec2D const& delta)
{
  position.addi(delta);
  dirtyTransformation = true;
}

void Sprite::rotate(float const delta)
{
  rotation += delta;
  if(rotation >= 1.0)
  {
    rotation -= 1.0;
  }
  dirtyTransformation = true;
}

void Sprite::scale(float const amount)
{
  scaleAmount *= amount;
  dirtyTransformation = true;
}

void Sprite::render(Transformation const& view)
{
  Transformation t = getTransformation().apply(view);
  image.render(t.transform(image.quad()));
}

void Sprite::setParent(Sprite* const newParent)
{
  if(newParent != this)
  {
    parent = newParent;
  }
}
Transformation Sprite::getTransformation()
{
  if(dirtyTransformation)
  {
    updateTransformation();
    dirtyTransformation = false;
  }
  
  if(parent)
  {
    return Transformation(transformation).apply(parent->getTransformation());
  }
  
  return transformation;
}

void Sprite::updateTransformation()
{
  const float epsilon = 0.00001;
  transformation.reset();
  
  transformation.scale(mirrorX ? -1 : 1, mirrorY ? -1 : 1);
  
  if(scaleAmount < 1 - epsilon || scaleAmount > 1 + epsilon)
    transformation.scale(scaleAmount);
  
  transformation.move(origin.neg());
  
  if(rotation < -epsilon || rotation > epsilon)
    transformation.rotate(rotation);
  
  transformation.move(position);
   
}