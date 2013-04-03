#include "skeleton.h"

#include "animation.h"
#include "skeletonanimation.h"
#include "sequentialanimation.h"
#include "parallelanimation.h"
#include "pauseanimation.h"

#include "qmloninitializer.h"
#include <fstream>
#include <iostream>
namespace
{
  SkeletonAnimation::BoneAnimator* createBoneAnimator(qmlon::Object& obj, std::string const& boneName,
                                                  float (Skeleton::Bone::*getter)() const,
                                                  void (Skeleton::Bone::*setter)(float))
  {
    bool fromSet = obj.hasProperty("from");
    bool toSet = obj.hasProperty("to");
    bool deltaSet = obj.hasProperty("delta");
    float from = fromSet ? obj.getProperty("from")->asFloat() : 0.0;
    float to = toSet ? obj.getProperty("to")->asFloat() : 0.0;
    float delta = deltaSet ? obj.getProperty("delta")->asFloat() : 0.0;

    return new SkeletonAnimation::BoneAnimator(boneName, fromSet, from, toSet, to, deltaSet, delta, getter, setter);
  }

  SkeletonAnimation::OriginAnimator* createOriginAnimator(qmlon::Object& obj, qmlon::Initializer<Vec2D>& vi)
  {
    bool fromSet = obj.hasProperty("from");
    bool toSet = obj.hasProperty("to");
    bool deltaSet = obj.hasProperty("delta");
    Vec2D from = fromSet ? qmlon::create(obj.getProperty("from")->asObject(), vi) : Vec2D();
    Vec2D to = toSet ? qmlon::create(obj.getProperty("to")->asObject(), vi) : Vec2D();
    Vec2D delta = deltaSet ? qmlon::create(obj.getProperty("delta")->asObject(), vi) : Vec2D();

    return new SkeletonAnimation::OriginAnimator(fromSet, from, toSet, to, deltaSet, delta);
  }
}

Skeleton::Bone::Bone(int const id, int const parentId) :
  id(id), parentId(parentId), name(), base(), tip(),
  angle(0), dirty(true), transformation(), children()
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

Vec2D Skeleton::Bone::getNonTransformedBase() const
{
  return base;
}

Vec2D Skeleton::Bone::getNonTransformedTip() const
{
  return tip;
}

float Skeleton::Bone::getAngle() const
{
  return angle;
}

Transformation const& Skeleton::Bone::getTransformation() const
{
  return transformation;
}

void Skeleton::Bone::setAngle(float const value)
{
  angle = value;
  dirty = true;
}

void Skeleton::Bone::changeAngle(float const delta)
{
  angle += delta;
  dirty = true;
}

void Skeleton::Bone::transform(Skeleton* skeleton, bool parentDirty)
{
  dirty |= parentDirty;
  
  if(dirty)
  {
    transformation
      .reset()
      .move(base.neg())
      .rotate(angle)
      .move(base);

    Bone* parent = parentId < 0 ? 0 : &skeleton->getBone(parentId);
    if(parent)
    {
      transformation.apply(parent->transformation);
    }
    else
    {
      transformation.move(skeleton->getOrigin().neg());

      if(skeleton->getFlipX() || skeleton->getFlipY())
      {
        transformation.scale(skeleton->getFlipX() ? -1 : 1, skeleton->getFlipY() ? -1 : 1);
      }
    }
  }

  for(int childId : children)
  {
    Bone& child = skeleton->getBone(childId);
    child.transform(skeleton, dirty);
  }

  dirty = false;
}

Skeleton::Bone::Children const& Skeleton::Bone::getChildren() const
{
  return children;
}

Skeleton::Pose::Pose(int const id) :
  id(id), name(), active(false), animations()
{
}

Skeleton::Pose::Pose(Skeleton::Pose const& other) :
  id(other.id), name(other.name), active(other.active), animations()
{
  for(Animation::Reference animation : other.animations)
  {
    animations.push_back(animation->clone());
  }
}

Skeleton::Pose::Pose(Skeleton::Pose&& other) :
  id(std::move(other.id)), name(std::move(other.name)), active(std::move(other.active)), animations(std::move(other.animations))
{
}

Skeleton::Pose& Skeleton::Pose::operator=(const Skeleton::Pose& other)
{
  if(&other != this)
  {
    id = other.id;
    name = other.name;
    active = other.active;
    animations = other.animations;
  }
  
  return *this;
}

Skeleton::Pose& Skeleton::Pose::operator=(Skeleton::Pose&& other)
{
  if(&other != this)
  {
    id = other.id;
    name = std::move(other.name);
    active = other.active;
    animations = std::move(other.animations);
  }
  return *this;
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

void Skeleton::Pose::animate(const float delta, Skeleton* skeleton)
{
  if(active)
  {
    for(Animation::Reference animation : animations)
    {
      animation->animate(delta, skeleton);
    }
  }
}

void Skeleton::Pose::reset()
{
    for(Animation::Reference animation : animations)
    {
      animation->reset();
    }
}


Skeleton::Skeleton() : bones(), poses(), flipX(false), flipY(false)
{
}

Skeleton::Skeleton(const Skeleton& other) : bones(other.bones), poses(other.poses), flipX(other.flipX), flipY(other.flipY)
{
}

Skeleton::Skeleton(Skeleton&& other) : bones(std::move(other.bones)), poses(std::move(other.poses)), flipX(other.flipX), flipY(other.flipY)
{
}

Skeleton::~Skeleton()
{
}


void Skeleton::initialize(qmlon::Value::Reference value)
{
  initialize(*this, value);
}

void Skeleton::initialize(Skeleton& skeleton, qmlon::Value::Reference value)
{
  qmlon::Initializer<Vec2D> vi({
    {"x", qmlon::set(&Vec2D::x)},
    {"y", qmlon::set(&Vec2D::y)}
  });

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
    {"duration", qmlon::set(&SkeletonAnimation::setDuration)},
    {"easing", [](SkeletonAnimation& a, qmlon::Value::Reference v) {
      auto i = Ease::BY_NAME.find(v->asString());
      if(i != Ease::BY_NAME.end())
      {
        a.setEasing(i->second);
      }
    }}
  }, {
    {"Angle", [&](SkeletonAnimation& animation, qmlon::Object& obj) {
      std::string boneName = obj.getProperty("target")->asString();
      SkeletonAnimation::BoneAnimator* a = createBoneAnimator(obj, boneName, &Bone::getAngle, &Bone::setAngle);
      animation.addAnimator(a);
    }},
    {"Origin", [&](SkeletonAnimation& animation, qmlon::Object& obj) {
      SkeletonAnimation::OriginAnimator* a = createOriginAnimator(obj, vi);
      animation.addAnimator(a);
    }},
  });

  qmlon::Initializer<PauseAnimation> pai({
    {"duration", qmlon::set(&PauseAnimation::setDuration)},
  });

  qmlon::Initializer<CompoundAnimation> cai({
  }, {
    {"Animation", [&](CompoundAnimation& ca, qmlon::Object& obj) {
      SkeletonAnimation* animation = new SkeletonAnimation;
      ani.init(*animation, obj);
      ai.init(*animation, obj);
      ca.addAnimation(Animation::Reference(animation));
    }},
    {"SequentialAnimation", [&](CompoundAnimation& ca, qmlon::Object& obj) {
      SequentialAnimation* animation = new SequentialAnimation;
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      ca.addAnimation(Animation::Reference(animation));
    }},
    {"ParallelAnimation", [&](CompoundAnimation& ca, qmlon::Object& obj) {
      ParallelAnimation* animation = new ParallelAnimation;
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      ca.addAnimation(Animation::Reference(animation));
    }},
    {"PauseAnimation", [&](CompoundAnimation& ca, qmlon::Object& obj) {
      PauseAnimation* animation = new PauseAnimation;
      ani.init(*animation, obj);
      pai.init(*animation, obj);
      ca.addAnimation(Animation::Reference(animation));
    }}
  });

  qmlon::Initializer<Pose> pi({
    {"name", qmlon::set(&Pose::name)},
  }, {
    {"Animation", [&](Pose& p, qmlon::Object& obj) {
      SkeletonAnimation* animation = new SkeletonAnimation;
      ani.init(*animation, obj);
      ai.init(*animation, obj);
      p.animations.push_back(Animation::Reference(animation));
    }},
    {"SequentialAnimation", [&](Pose& p, qmlon::Object& obj) {
      SequentialAnimation* animation = new SequentialAnimation;
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      p.animations.push_back(Animation::Reference(animation));
    }},
    {"ParallelAnimation", [&](Pose& p, qmlon::Object& obj) {
      ParallelAnimation* animation = new ParallelAnimation;
      ani.init(*animation, obj);
      cai.init(*animation, obj);
      p.animations.push_back(Animation::Reference(animation));
    }},
  });

  qmlon::Initializer<Bone> bi({
    {"name", qmlon::set(&Bone::name)},
    {"base", qmlon::createSet(vi, &Bone::base)},
    {"tip", qmlon::createSet(vi, &Bone::tip)},
  });

  bi.addChildSetter("Bone", [&](Bone& bone, qmlon::Object& object) {
    int boneId = skeleton.bones.size();
    int parentId = bone.getId();
    Bone b(boneId, parentId);
    bone.children.push_back(b.getId());
    skeleton.bones.push_back(b);
    bi.init(b, object);
    skeleton.bones.at(boneId) = std::move(b);
  });

  qmlon::Initializer<Skeleton> si({}, {
    {"Bone", [&](Skeleton& skeleton, qmlon::Object& object) {
      int boneId = skeleton.bones.size();
      Bone b(boneId);
      skeleton.bones.push_back(b);
      bi.init(b, object);
      skeleton.bones.at(boneId) = std::move(b);
    }},
    {"Pose", [&](Skeleton& skeleton, qmlon::Object& object) {
      Pose p(skeleton.poses.size());
      pi.init(p, object);
      skeleton.poses.push_back(std::move(p));
      
    }},
  });

  si.init(skeleton, value);
}

const Skeleton::Bone &Skeleton::getBone(const std::string &name) const
{
  for(Bone const& b : bones)
  {
    if(b.name == name)
    {
      return b;
    }
  }
}

Skeleton::Bone& Skeleton::getBone(std::string const& name)
{
  for(Bone& b : bones)
  {
    if(b.name == name)
    {
      return b;
    }
  }
}

Skeleton::Bone& Skeleton::getBone(int const id)
{
  return bones.at(id);
}

Skeleton::Bones& Skeleton::getBones()
{
  return bones;
}

Skeleton::Pose& Skeleton::getPose(std::string const& name)
{
  for(Pose& b : poses)
  {
    if(b.name == name)
    {
      return b;
    }
  }
}

Skeleton::Pose& Skeleton::getPose(int const id)
{
  return poses.at(id);
}

Skeleton::Poses& Skeleton::getPoses()
{
  return poses;
}

void Skeleton::setFlipX(bool const value)
{
  if(flipX != value)
  {
    flipX = value;
    setAllBonesDirty();
  }
}

void Skeleton::setFlipY(bool const value)
{
  if(flipY != value)
  {
    flipY = value;
    setAllBonesDirty();
  }
}

bool Skeleton::getFlipX() const
{
  return flipX;
}

bool Skeleton::getFlipY() const
{
  return flipY;
}

void Skeleton::update(float const delta)
{
  for(Pose& pose : poses)
  {
    pose.animate(delta, this);
  }

  for(Bone& bone : bones)
  {
    bone.transform(this);
  }
}

void Skeleton::setOrigin(const Vec2D &value)
{
  origin = value;
  setAllBonesDirty();
}

const Vec2D &Skeleton::getOrigin() const
{
  return origin;
}

void Skeleton::setAllBonesDirty()
{
  for(Bone& bone : bones)
  {
    bone.dirty = true;
  }
}
