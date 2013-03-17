#include "puppet.h"

#include "qmlon.h"
#include <fstream>
#include <algorithm>

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
    {"bone", [&puppet](Part& part, qmlon::Value::Reference v) {
      part.boneId = puppet.skeleton.getBone(v->asString()).getId();
    }},
    {"front", [&puppet](Part& part, qmlon::Value::Reference v) {
      part.frontId = puppet.spriteSheet.getSprite(v->asString()).getId();
    }},
    {"back", [&puppet](Part& part, qmlon::Value::Reference v) {
      part.backId = puppet.spriteSheet.getSprite(v->asString()).getId();
    }},
    {"base", qmlon::createSet(vi, &Part::base)},
    {"tip", qmlon::createSet(vi, &Part::tip)},
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
    {"Part", [&pai](Puppet& puppet, qmlon::Object& obj){
      Part part;
      pai.init(part, obj);
      part.id = puppet.parts.size();
      puppet.parts.push_back(part);
    }}
  });

  pi.init(puppet, value);
  
  for(Part& part : puppet.parts) 
  {
    puppet.updatePartPosition(part);
    puppet.updatePartImagePosition(part);
  }
}

void Puppet::update(float const delta)
{
  skeleton.update(delta);
  
  for(Part& part : parts)
  {
    updatePartPosition(part);
  }
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

Puppet::Part const& Puppet::getPart(const int id) const
{
  return parts.at(id);
}

SpriteSheet::Frame const& Puppet::getPartFrame(const int id)
{
  Part const& part = getPart(id);
  bool const flipped = flipX != flipY;
  spriteSheet.getSprite(flipped ? part.backId : part.frontId).getAnimation(0).getFrame(0);
}

Puppet::Parts const& Puppet::getParts() const
{
  return parts;
}

Puppet::PartRefs Puppet::getPartsZOrdered() const
{
  PartRefs ordered;
  for(Part const& p : parts)
  {
    ordered.push_back(&p);
  }
  
  std::stable_sort(ordered.begin(), ordered.end(), [](Part const* a, Part const* b) { 
    return a->z < b->z;
  });
  
  return ordered;
}

void Puppet::updatePartPosition(Part& part)
{
  Skeleton::Bone const& bone = skeleton.getBone(part.boneId);
  part.transformation
    .reset()
    .apply(part.textureTransformation)
    .apply(bone.getTransformation());

  part.position.topLeft = part.transformation.transform({0, 1});
  part.position.topRight = part.transformation.transform({1, 1});
  part.position.bottomLeft = part.transformation.transform({0, 0});
  part.position.bottomRight = part.transformation.transform({1, 0});
}

void Puppet::updatePartImagePosition(Puppet::Part& part)
{
  SpriteSheet::Frame const& frame = getPartFrame(part.id);
  Skeleton::Bone const& bone = skeleton.getBone(part.boneId);
  
  Transformation const invertYAxis = Transformation().scale(1, -1);
  
  Vec2D partOrientation = invertYAxis.transform(part.tip - part.base);
  Vec2D boneOrientation = bone.getNonTransformedTip() - bone.getNonTransformedBase();
  
  part.textureTransformation
    .reset()
    .scale(frame.getSize().width, frame.getSize().height)
    .move(-frame.getHotspot().x, -frame.getHotspot().y)
    .move(-part.base.x, -part.base.y)
    .apply(invertYAxis)
    .apply(Transformation::fromBase(partOrientation, partOrientation.normal()))
    .apply(Transformation::toBase(boneOrientation, boneOrientation.normal()))
    .move(bone.getNonTransformedBase());
  
  part.imagePosition.topLeft = {static_cast<float>(frame.getPosition().x), 
                                static_cast<float>(frame.getPosition().y)};
  part.imagePosition.topRight = {static_cast<float>(frame.getPosition().x) + frame.getSize().width, 
                                 static_cast<float>(frame.getPosition().y)};
  part.imagePosition.bottomLeft = {static_cast<float>(frame.getPosition().x), 
                                   static_cast<float>(frame.getPosition().y) + frame.getSize().height};
  part.imagePosition.bottomRight = {static_cast<float>(frame.getPosition().x) + frame.getSize().width, 
                                    static_cast<float>(frame.getPosition().y) + frame.getSize().height};
}
