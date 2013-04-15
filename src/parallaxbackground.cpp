#include "parallaxbackground.h"

ew::UID const ParallaxBackground::ID = ew::getUID();

glhckTextureParameters const ParallaxBackground::TEXTURE_PARAMETERS = {
  .minLod = -1000.0f,
  .maxLod = 1000.0f,
  .biasLod = 0.0f,
  .baseLevel = 0,
  .maxLevel = 1000,
  .wrapS = GLHCK_WRAP_CLAMP_TO_EDGE,
  .wrapT = GLHCK_WRAP_CLAMP_TO_EDGE,
  .wrapR = GLHCK_WRAP_CLAMP_TO_EDGE,
  .minFilter = GLHCK_FILTER_NEAREST,
  .magFilter = GLHCK_FILTER_NEAREST,
  .compareMode = GLHCK_COMPARE_NONE,
  .compareFunc = GLHCK_COMPARE_LEQUAL,
  .compression = GLHCK_COMPRESSION_NONE,
  .mipmap = 0,
};

namespace
{
  float clamp(float const value, float const minValue, float const maxValue)
  {
    return value > maxValue ? maxValue : value < minValue ? minValue : value;
  }
}

ParallaxBackground::ParallaxBackground(ew::RenderableWorld *world, glhckCamera *camera, const std::string &image,
                                       float width, float height, float left, float right, float top, float bottom,
                                       const int zIndex, const int layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), camera(camera), object(nullptr),
  width(width), height(height), left(left), right(right), top(top), bottom(bottom)
{
  object = glhckSpriteNewFromFile(image.data(), width, height, 0, &TEXTURE_PARAMETERS);
}

void ParallaxBackground::render(ew::RenderContext *context)
{
  glhckObject* cameraObject = glhckCameraGetObject(camera);
  kmVec3 const* pos = glhckObjectGetPosition(cameraObject);
  float const rx = clamp((pos->x - left) / (right - left), 0, 1);
  float const ry = clamp((pos->y - bottom) / (top - bottom), 0, 1);
  float const dx = (0.5 - rx) * width/2;
  float const dy = (0.5 - ry) * height/2;

  float const x = clamp(pos->x + dx, left + width/2, right - width/2);
  float const y = clamp(pos->y + dy, bottom + height/2, top - height/2);

  glhckObjectPositionf(object, x, y, 0);
  glhckObjectRender(object);
}
