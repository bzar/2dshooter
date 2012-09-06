#include "skeleton.h"

#include "animation.h"
#include "skeletonanimation.h"
#include "sequentialanimation.h"
#include "parallelanimation.h"
#include "pauseanimation.h"

#include "qmlon.h"
#include <fstream>

namespace
{
  SkeletonAnimation::Animator* createBoneAnimator(qmlon::Object* obj, std::string const& boneName,
                                                  SkeletonAnimation::BoneAnimator::ValueSetter setter,
                                                  SkeletonAnimation::BoneAnimator::ValueGetter getter)
  {
    bool fromSet = obj->hasProperty("from");
    bool toSet = obj->hasProperty("to");
    bool deltaSet = obj->hasProperty("delta");
    float from = fromSet ? obj->getProperty("from")->asFloat() : 0.0;
    float to = toSet ? obj->getProperty("to")->asFloat() : 0.0;
    float delta = deltaSet ? obj->getProperty("delta")->asFloat() : 0.0;

    return new SkeletonAnimation::BoneAnimator(boneName, fromSet, from, toSet, to, deltaSet, delta, setter, getter);
  }
}

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

float Skeleton::Bone::getAngle() const
{
  return angle;
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
  qmlon::Initializer<Animation> ani({
    {"loop", [](Animation& a, qmlon::Value::Reference v) {
      if(v->isBoolean())
      {
        a.setLoops(v->asBoolean() ? Animation::INFINITE_LOOPS : 1);
      }
      else
      {
        a.setLoops(v->asInteger());
      }
    }},
  });
  qmlon::Initializer<SkeletonAnimation> ai({
    {"duration", [](SkeletonAnimation& a, qmlon::Value::Reference v) { a.setDuration(v->asFloat()); }},
    {"easing", [](SkeletonAnimation& a, qmlon::Value::Reference v) {
      auto i = Ease::BY_NAME.find(v->asString());
      if(i != Ease::BY_NAME.end())
      {
        a.setEasing(i->second);
      }
    }}
  }, {
    {"Angle", [&](SkeletonAnimation& animation, qmlon::Object* obj) {
      std::string boneName = obj->getProperty("target")->asString();
      animation.addAnimator(createBoneAnimator(obj, boneName, [](Bone* b, float const v){ b->setAngle(v); }, [](Bone* b){ return b->getAngle(); }));
    }},
  });

  qmlon::Initializer<PauseAnimation> pai({
    {"duration", [](PauseAnimation& a, qmlon::Value::Reference v) { a.setDuration(v->asFloat()); }},
  });

  qmlon::Initializer<CompoundAnimation> cai({
  }, {
    {"Animation", [&](CompoundAnimation& ca, qmlon::Object* obj) {
      SkeletonAnimation* animation = new SkeletonAnimation(ca.getSkeleton());
      ani.init(*animation, obj);
      ai.init(*animation, obj);
      ca.addAnimation(animation);
    }},
    {"SequentialAnimation", [&](CompoundAnimation& ca, qmlon::Object* obj) {
      SequentialAnimation* animation = new SequentialAnimation(ca.getSkeleton());
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      ca.addAnimation(animation);
    }},
    {"ParallelAnimation", [&](CompoundAnimation& ca, qmlon::Object* obj) {
      ParallelAnimation* animation = new ParallelAnimation(ca.getSkeleton());
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      ca.addAnimation(animation);
    }},
    {"PauseAnimation", [&](CompoundAnimation& ca, qmlon::Object* obj) {
      PauseAnimation* animation = new PauseAnimation;
      ani.init(*animation, obj);
      pai.init(*animation, obj);
      ca.addAnimation(animation);
    }}
  });

  qmlon::Initializer<Pose::Reference> pi({
    {"name", [](Pose::Reference& pose, qmlon::Value::Reference value) { pose->name = value->asString(); }},
  }, {
    {"Animation", [&](Pose::Reference& p, qmlon::Object* obj) {
      SkeletonAnimation* animation = new SkeletonAnimation(p->skeleton);
      ani.init(*animation, obj);
      ai.init(*animation, obj);
      p->animations.push_back(Animation::Reference(animation));
    }},
    {"SequentialAnimation", [&](Pose::Reference& p, qmlon::Object* obj) {
      SequentialAnimation* animation = new SequentialAnimation(p->skeleton);
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      p->animations.push_back(Animation::Reference(animation));
    }},
    {"ParallelAnimation", [&](Pose::Reference& p, qmlon::Object* obj) {
      ParallelAnimation* animation = new ParallelAnimation(p->skeleton);
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      p->animations.push_back(Animation::Reference(animation));
    }},
  });

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
    bones.push_back(b);
    bi.init(b, object);
    bone->children.push_back(b);
  });

  qmlon::Initializer<Skeleton> si({}, {
    {"Bone", [&](Skeleton& skeleton, qmlon::Object* object) {
      Bone::Reference b(new Bone(bones.size()));
      bones.push_back(b);
      bi.init(b, object);
    }},
    {"Pose", [&](Skeleton& skeleton, qmlon::Object* object) {
      Pose::Reference p(new Pose(poses.size(), this));
      pi.init(p, object);
      poses.push_back(p);
    }},
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
