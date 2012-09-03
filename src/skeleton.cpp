#include "skeleton.h"
#include "qmlon.h"
#include <fstream>

Skeleton::Skeleton(std::string const& filename) :
  bones()
{
  qmlon::Initializer<Vec2D> vi({
    {"x", [](Vec2D& v, qmlon::Value::Reference value) { v.x = value->asFloat(); }},
    {"y", [](Vec2D& v, qmlon::Value::Reference value) { v.y = value->asFloat(); }}
  });

  qmlon::Initializer<Bone::Reference> bi({
    {"name", [](Bone::Reference& bone, qmlon::Value::Reference value) { bone->name = value->asString(); }},
    {"base", [&](Bone::Reference& bone, qmlon::Value::Reference value) { bone->position.base = qmlon::create(value, vi); }},
    {"tip", [&](Bone::Reference& bone, qmlon::Value::Reference value) { bone->position.tip = qmlon::create(value, vi); }},
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

void Skeleton::transformBones()
{
  for(Bone::Reference& bone : bones)
  {
    if(!bone->parent)
    {
      transformBone(bone);
    }
  }
}

void Skeleton::transformBone(Bone::Reference& bone, Transformation transformation)
{
  transformation.apply(bone->transformation).move(bone->position.base);
  bone->transformedPosition.base = transformation.transform({0 ,0});
  bone->transformedPosition.tip = transformation.transform(bone->position.tip - bone->position.base);

  for(Bone::Reference& child : bone->children)
  {
    transformBone(child, transformation);
  }
}
