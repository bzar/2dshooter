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



ParallaxBackground::ParallaxBackground(ew::RenderableWorld *world, glhckCamera *camera, const std::string &image, const int zIndex, const int layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), camera(camera), object(nullptr)
{
  object = glhckSpriteNewFromFile(image.data(), 1, 1, 0, &TEXTURE_PARAMETERS);
}

void ParallaxBackground::render(ew::RenderContext *context)
{
  glhckFrustum* frustum = glhckCameraGetFrustum(camera);
  float left = frustum->farCorners[GLHCK_FRUSTUM_CORNER_TOP_LEFT].x;
  float top = frustum->farCorners[GLHCK_FRUSTUM_CORNER_TOP_LEFT].y;
  float right = frustum->farCorners[GLHCK_FRUSTUM_CORNER_BOTTOM_RIGHT].x;
  float bottom = frustum->farCorners[GLHCK_FRUSTUM_CORNER_BOTTOM_RIGHT].y;

  glhckObjectPositionf(object, (left + right) / 2, (top + bottom) / 2, 0);
  glhckObjectScalef(object, right - left, top - bottom, 1);

  glhckObjectRender(object);
}
