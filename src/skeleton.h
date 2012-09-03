#ifndef SKELETON_HH
#define SKELETON_HH

#include <string>
#include <vector>
#include <memory>

#include "util/vec2d.h"
#include "util/transformation.h"

class Skeleton
{
public:
  struct Bone
  {
    typedef std::shared_ptr<Bone> Reference;

    struct Position
    {
      Vec2D base;
      Vec2D tip;
    };

    Bone(int const id, Reference const parent = Reference()) :
      id(id), parent(parent), name(), position(), transformedPosition(), transformation(), children() {}

    int const id;
    Reference const parent;

    std::string name;
    Position position;
    Position transformedPosition;
    Transformation transformation;
    std::vector<Bone::Reference> children;
  };

  typedef std::vector<Bone::Reference> Bones;

  Skeleton(std::string const& filename);

  Bone::Reference const getBone(std::string const& name) const;
  Bone::Reference const getBone(int const id) const;
  Bones const& getBones() const;

  void transformBones();

private:
  static void transformBone(Bone::Reference& bone, Transformation transformation = Transformation());

  Bones bones;
};


#endif