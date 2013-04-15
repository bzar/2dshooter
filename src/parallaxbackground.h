#ifndef PARALLAXBACKGROUND_H
#define PARALLAXBACKGROUND_H

#include "ew/renderable.h"
#include "ew/renderableworld.h"
#include "glhck/glhck.h"

#include <string>

class ParallaxBackground : public ew::Renderable
{
public:
  ParallaxBackground(ew::RenderableWorld* world, glhckCamera* camera, std::string const& image,
                     float width, float height, float left, float right, float top, float bottom,
                     int const zIndex = 0, int const layer = 0);
  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);

private:
  static glhckTextureParameters const TEXTURE_PARAMETERS;
  glhckCamera* camera;
  glhckObject* object;

  float width;
  float height;
  float left;
  float right;
  float top;
  float bottom;
};

#endif // PARALLAXBACKGROUND_H
