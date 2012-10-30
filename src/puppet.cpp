#include "puppet.h"

#include "qmlon.h"
#include <fstream>

void Puppet::initialize(qmlon::Value::Reference value) {
    initialize(*this, value);
}

void Puppet::initialize(Puppet& puppet, qmlon::Value::Reference value) {
  qmlon::Initializer<Vec2D> vi({
    {"x", qmlon::set(&Vec2D::x)},
    {"y", qmlon::set(&Vec2D::y)}
  });

  qmlon::Initializer<Part> pai({
    {"name", qmlon::set(&Part::name)},
    {"front", [&puppet](Part& part, qmlon::Value::Reference v) {
      part.frontId = puppet.spriteSheet.getSprite(v->asString()).getId();
    }},
    {"back", [&puppet](Part& part, qmlon::Value::Reference v) {
      part.backId = puppet.spriteSheet.getSprite(v->asString()).getId();
    }},
    {"base", qmlon::createSet(vi, &Part::base)},
    {"tip", qmlon::createSet(vi, &Part::base)},
    {"z", qmlon::set(&Part::z)}
  }, {});

  qmlon::Initializer<Puppet> pi({
    {"skeleton", [](Puppet& puppet, qmlon::Value::Reference v) {
      auto skeletonValue = qmlon::readFile(v->asString());
      puppet.skeleton.initialize(skeletonValue);
    }},
    {"spritesheet", [](Puppet& puppet, qmlon::Value::Reference v) {
      auto spriteSheetValue = qmlon::readFile(v->asString());
      puppet.spriteSheet.initialize(spriteSheetValue);  
    }}
  }, {
    {"Part", [&pai](Puppet& puppet, qmlon::Object* obj){
      Part part;
      pai.init(part, obj);
      part.id = puppet.parts.size();
      puppet.parts.push_back(part);
    }}
  });

  pi.init(puppet, value);
}

void Puppet::update(float const delta)
{
  skeleton.update(delta);
}


void Puppet::setFlipX(bool const value)
{
  flipX = value;
}

void Puppet::setFlipY(bool const value)
{
  flipY = value;
}


SpriteSheet& Puppet::getSpriteSheet()
{
  return spriteSheet;
}

Skeleton& Puppet::getSkeleton()
{
  return skeleton;
}


Puppet::Part const& Puppet::getPart(std::string const& name) const
{
  for(Part const& p : parts)
  {
    if(p.name == name)
    {
      return p;
    }
  }
  
  throw std::runtime_error("Could not find part!");
}

Puppet::Part const& Puppet::getPart(int id) const
{
  return parts.at(id);
}

Puppet::Parts const& Puppet::getParts() const
{
  return parts;
}

