#ifndef SKELETON_HH
#define SKELETON_HH

#include <string>
#include <vector>
#include <memory>

#include "util/vec2d.h"
#include "util/transformation.h"
#include "animation.h"

class Skeleton
{
public:
  typedef std::shared_ptr<Skeleton> Reference;

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
    float getAngle() const;

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

  class Pose
  {
  public:
    friend class Skeleton;
    typedef std::shared_ptr<Pose> Reference;
    typedef std::vector<Animation::Reference> Animations;

    Pose(int const id, Skeleton* skeleton);
    void activate();
    void deactivate();
    bool isActive() const;
    void animate(float const delta);

  private:
    int const id;
    Skeleton* skeleton;
    std::string name;
    bool active;
    Animations animations;
  };

  typedef std::vector<Bone::Reference> Bones;
  typedef std::vector<Pose::Reference> Poses;

  Skeleton(std::string const& filename);

  Bone::Reference const getBone(std::string const& name) const;
  Bone::Reference const getBone(int const id) const;
  Bones const& getBones() const;

  Pose::Reference const getPose(std::string const& name) const;
  Pose::Reference const getPose(int const id) const;
  Poses const& getPoses() const;

  void update(float const delta);

private:
  Bones bones;
  Poses poses;
};


#endif