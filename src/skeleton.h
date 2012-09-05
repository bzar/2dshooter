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
  class Bone
  {
  public:
    friend class Skeleton;

    typedef std::shared_ptr<Bone> Reference;
    typedef std::vector<Bone::Reference> Children;

    Bone(int const id, Reference const parent = Reference());

    int getId() const;
    std::string const& getName() const;
    Vec2D getBase() const;
    Vec2D getTip() const;

    void setAngle(float const value);
    void changeAngle(float const delta);

    void transform();
    Children const& getChildren() const;

  private:
    void setAsDirty();
    void setAsHasDirtyChildren();

    int const id;
    Reference const parent;

    std::string name;
    Vec2D base;
    Vec2D tip;

    float angle;
    bool dirty;
    bool hasDirtyChildren;
    Transformation transformation;
    Children children;
  };

  typedef std::vector<Bone::Reference> Bones;

  Skeleton(std::string const& filename);

  Bone::Reference const getBone(std::string const& name) const;
  Bone::Reference const getBone(int const id) const;
  Bones const& getBones() const;

  void transformBones();

private:
  Bones bones;
};


#endif