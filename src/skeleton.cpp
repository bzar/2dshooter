#include "skeleton.h"
#include "qmlon.h"
#include <fstream>

Skeleton::Bone::Bone(int const id, Reference const parent) :
  id(id), parent(parent), name(), base(), tip(),
  angle(0), dirty(true), hasDirtyChildren(true),
  transformation(), children()
{
}

int Skeleton::Bone::getId() const
{
  return id;
}

std::string const& Skeleton::Bone::getName() const
{
  return name;
}

Vec2D Skeleton::Bone::getBase() const
{
  return transformation.transform(base);
}

Vec2D Skeleton::Bone::getTip() const
{
  return transformation.transform(tip);
}

void Skeleton::Bone::setAngle(float const value)
{
  angle = value;
  setAsDirty();
}

void Skeleton::Bone::changeAngle(float const delta)
{
  angle += delta;
  setAsDirty();
}

void Skeleton::Bone::transform()
{
  if(dirty && parent && parent->dirty)
  {
    parent->transform();
    return;
  }

  if(dirty)
  {
    transformation
      .reset()
      .move(base.neg())
      .rotate(angle)
      .move(base);

    if(parent)
      transformation.apply(parent->transformation);

    dirty = false;
  }

  if(hasDirtyChildren)
  {
    for(Bone::Reference& child : children)
    {
      child->transform();
    }
    hasDirtyChildren = false;
  }
}

Skeleton::Bone::Children const& Skeleton::Bone::getChildren() const
{
  return children;
}

void Skeleton::Bone::setAsDirty()
{
  dirty = true;
  for(Bone::Reference& child : children)
  {
    child->setAsDirty();
  }

  setAsHasDirtyChildren();
}

void Skeleton::Bone::setAsHasDirtyChildren()
{
  if(hasDirtyChildren)
    return;

  hasDirtyChildren = true;

  if(parent)
    parent->setAsHasDirtyChildren();
}



Skeleton::Pose::Pose(int const id, Skeleton* skeleton) :
  id(id), skeleton(skeleton), name(), active(false), animations()
{
}

void Skeleton::Pose::activate()
{
  active = true;
}

void Skeleton::Pose::deactivate()
{
  active = false;
}

bool Skeleton::Pose::isActive() const
{
  return active;
}

void Skeleton::Pose::animate(float const delta)
{
  if(active)
  {
    for(Animation::Reference& animation : animations)
    {
      animation->animate(delta);
    }
  }
}


Skeleton::Skeleton(std::string const& filename) :
  bones()
{
  qmlon::Initializer<Vec2D> vi({
    {"x", [](Vec2D& v, qmlon::Value::Reference value) { v.x = value->asFloat(); }},
    {"y", [](Vec2D& v, qmlon::Value::Reference value) { v.y = value->asFloat(); }}
  });

  qmlon::Initializer<Bone::Reference> bi({
    {"name", [](Bone::Reference& bone, qmlon::Value::Reference value) { bone->name = value->asString(); }},
    {"base", [&](Bone::Reference& bone, qmlon::Value::Reference value) { bone->base = qmlon::create(value, vi); }},
    {"tip", [&](Bone::Reference& bone, qmlon::Value::Reference value) { bone->tip = qmlon::create(value, vi); }},
  });

  bi.addChildSetter("Bone", [&](Bone::Reference& bone, qmlon::Object* object) {
    Bone::Reference b(new Bone(bones.size(), bone));
    bi.init(b, object);
    bones.push_back(b);
    bone->children.push_back(b);
  });

  qmlon::Initializer<Skeleton> si({}, {
    {"Bone", [&](Skeleton& skeleton, qmlon::Object* object) {
      Bone::Reference b(new Bone(bones.size()));
      bi.init(b, object);
      bones.push_back(b);
    }}
  });

  std::ifstream ifs(filename);
  auto value = qmlon::readValue(ifs);
  si.init(*this, value);
}

Skeleton::Bone::Reference const Skeleton::getBone(std::string const& name) const
{
  for(Bone::Reference b : bones)
  {
    if(b->name == name)
    {
      return b;
    }
  }
}

Skeleton::Bone::Reference const Skeleton::getBone(int const id) const
{
  return bones.at(id);
}

Skeleton::Bones const& Skeleton::getBones() const
{
  return bones;
}

Skeleton::Pose::Reference const Skeleton::getPose(std::string const& name) const
{
  for(Pose::Reference b : poses)
  {
    if(b->name == name)
    {
      return b;
    }
  }
}

Skeleton::Pose::Reference const Skeleton::getPose(int const id) const
{
  return poses.at(id);
}

Skeleton::Poses const& Skeleton::getPoses() const
{
  return poses;
}

void Skeleton::update(float const delta)
{
  for(Pose::Reference& pose : poses)
  {
    pose->animate(delta);
  }

  for(Bone::Reference& bone : bones)
  {
    bone->transform();
  }
}
