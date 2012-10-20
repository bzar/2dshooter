#ifndef PUPPET_HH
#define PUPPET_HH

#include "skeleton.h"
#include "spritesheet.h"
#include "util/vec2d.h"
#include "util/transformation.h"

#include <string>
#include <vector>
#include <memory>

class Puppet
{
public:
  struct Part
  {
    typedef std::shared_ptr<Part> Reference;

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

  typedef std::vector<Part::Reference> Parts;

  Puppet(std::string const& filename);

  void update(float const delta);

  void setFlipX(bool const value);
  void setFlipY(bool const value);

  SpriteSheet::Reference const& getSpriteSheet() const;
  Skeleton::Reference const& getSkeleton() const;

  Part::Reference const& getPart(std::string const& name) const;
  Part::Reference const& getPart(int id) const;
  Parts const& getParts() const;

private:
  SpriteSheet::Reference spriteSheet;
  Skeleton::Reference skeleton;
  Parts parts;
  bool flipX;
  bool flipY;
};
#endif
