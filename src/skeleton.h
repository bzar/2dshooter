#ifndef SKELETON_HH
#define SKELETON_HH

#include <string>
#include <vector>
#include <memory>

#include "util/vec2d.h"
#include "util/transformation.h"
#include "qmlon.h"
#include "animation.h"

class Skeleton
{
public:
  class Bone
  {
  public:
    friend class Skeleton;

    typedef std::vector<int> Children;

    Bone(int const id, int const parentId = -1);

    int getId() const;
    std::string const& getName() const;
    Vec2D getBase() const;
    Vec2D getTip() const;
    float getAngle() const;

    void setAngle(float const value);
    void changeAngle(float const delta);

    void transform(Skeleton* skeleton, bool parentDirty = false);
    Children const& getChildren() const;
    
  private:
    int id;
    int parentId;
    
    std::string name;
    Vec2D base;
    Vec2D tip;

    float angle;
    bool dirty;
    Transformation transformation;
    Children children;
  };

  class Pose
  {
  public:
    friend class Skeleton;
    typedef std::vector<Animation::Reference> Animations;

    Pose(int const id);
    Pose(Pose const& other);
    Pose(Pose&& other);
    
    Pose& operator=(Pose const& other);
    Pose& operator=(Pose&& other);
    
    void activate();
    void deactivate();
    bool isActive() const;
    void animate(float const delta, Skeleton* skeleton);

  private:
    int id;
    std::string name;
    bool active;
    Animations animations;
  };

  typedef std::vector<Bone> Bones;
  typedef std::vector<Pose> Poses;

  Skeleton();
  Skeleton(Skeleton const& other);
  Skeleton(Skeleton&& other);
  ~Skeleton();
  
  void initialize(qmlon::Value::Reference value);
  static void initialize(Skeleton& skeleton, qmlon::Value::Reference value);
  
  Bone& getBone(std::string const& name);
  Bone& getBone(int const id);
  Bones& getBones();

  Pose& getPose(std::string const& name);
  Pose& getPose(int const id);
  Poses& getPoses();

  void update(float const delta);

private:
  Bones bones;
  Poses poses;
};

#endif