#ifndef PUPPET_HH
#define PUPPET_HH

#include "skeleton.h"
#include "spritesheet.h"
#include "util/vec2d.h"
#include "util/transformation.h"
#include "qmlon.h"

#include <string>
#include <vector>
#include <memory>

class Puppet
{
public:
  struct Part
  {
    Part() : id(-1), name(), boneId(-1), frontId(-1), backId(-1), base(), tip(), z(0), transformation(), textureTransformation() {}
    int id;
    std::string name;
    int boneId;
    int frontId;
    int backId;
    Vec2D base;
    Vec2D tip;
    int z;
    Transformation transformation;
    Transformation textureTransformation;
  };

  typedef std::vector<Part> Parts;

  void initialize(qmlon::Value::Reference value);
  static void initialize(Puppet& puppet, qmlon::Value::Reference value);
  
  void update(float const delta);

  void setFlipX(bool const value);
  void setFlipY(bool const value);

  SpriteSheet& getSpriteSheet();
  Skeleton& getSkeleton();

  Part const& getPart(std::string const& name) const;
  Part const& getPart(int id) const;
  Parts const& getParts() const;

private:
  SpriteSheet spriteSheet;
  Skeleton skeleton;
  Parts parts;
  bool flipX = false;
  bool flipY = false;
};
#endif
