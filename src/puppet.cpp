#include "puppet.h"

#include "qmlon.h"
#include <fstream>

Puppet::Puppet(std::string const& filename) :
  spriteSheet(), skeleton(), parts(), flipX(false), flipY(false)
{
  std::ifstream ifs(filename);
  auto value = qmlon::readValue(ifs);

  skeleton.reset(new Skeleton(value->asObject()->getProperty("skeleton")->asString()));
  spriteSheet.reset(new SpriteSheet(SpriteSheet::create(value->asObject()->getProperty("spritesheet")->asString())));

  qmlon::Initializer<Part::Reference> pai({

  }, {});

  qmlon::Initializer<Puppet> pi({}, {
    {"Part", [&pai, this](Puppet& puppet, qmlon::Object* obj){
      Part::Reference part(new Part);
      pai.init(part, obj);
      parts.push_back(part);
    }}
  });

  pi.init(*this, value);
}

void Puppet::update(float const delta)
{
}


void Puppet::setFlipX(bool const value)
{
}

void Puppet::setFlipY(bool const value)
{
}


SpriteSheet::Reference const& Puppet::getSpriteSheet() const
{
}

Skeleton::Reference const& Puppet::getSkeleton() const
{
}


Puppet::Part::Reference const& Puppet::getPart(std::string const& name) const
{
}

Puppet::Part::Reference const& Puppet::getPart(int id) const
{
}

Puppet::Parts const& Puppet::getParts() const
{
}

