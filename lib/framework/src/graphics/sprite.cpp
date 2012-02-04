#include "sprite.h"
#include "../util/quad.h"

Sprite::Sprite(Image const& image, Vec2D const& position) : 
  image(image), position(position), rotation(0), scaleAmount(1), 
  transformation(), dirtyTransformation(true), parent(0)
{
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

void Sprite::render(Screen const& screen)
{
  if(dirtyTransformation)
  {
    updateTransformation();
    dirtyTransformation = false;
  }
  
  Transformation t = getTransformation().apply(screen.transformation());
  image.render(t.transform(image.quad()));
}

void Sprite::setParent(Sprite* const newParent)
{
  if(newParent != this)
  {
    parent = newParent;
  }
}
Transformation Sprite::getTransformation() const
{
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
  
  if(scaleAmount < 1 - epsilon || scaleAmount > 1 + epsilon)
    transformation.scale(scaleAmount);
    
  if(rotation < -epsilon || rotation > epsilon)
    transformation.rotate(rotation);
  
  transformation.move(position);
   
}