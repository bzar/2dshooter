#ifndef PUPPET_ENTITY_HH
#define PUPPET_ENTITY_HH

#include "ew/renderable.h"
#include "ew/updatable.h"
#include "puppet.h"
#include "gameworld.h"
#include "glhck/glhck.h"

#include <string>

class PuppetEntity : public ew::Renderable, public ew::Updatable
{
public:
  PuppetEntity(Puppet const& p, GameWorld* world);
  PuppetEntity(PuppetEntity const&) = delete;
  PuppetEntity(PuppetEntity&&) = delete;
  ~PuppetEntity();

  void render(ew::RenderContext* context);
  virtual void update(float const delta);
  Puppet& getPuppet();
  void setPose(std::string const& name, bool const state);
  void setFlipX(bool value);
  void setFlipY(bool value);
  void setPosition(Vec2D const& pos);
  virtual Vec2D const& getPosition() const;

private:
  static glhckTextureParameters const TEXTURE_PARAMETERS;

  void refreshVertices();

  bool dirtyVertices;
  
  bool showBoneLines;
  bool showPartLines;
  bool showParts;
  
  glhckObject* boneLines;
  glhckObject* partLines;
  glhckObject* parts;
  
  Vec2D position;
  Puppet puppet;
};

#endif
