#include "puppetentity.h"
#include <vector>
#include <algorithm>
#include <iostream>

PuppetEntity::PuppetEntity(const Puppet& p, GameWorld* world) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  debugLines(glhckObjectNew()), position(0, 0), puppet(p)
{
  glhckObjectNewGeometry(debugLines)->type = GLHCK_LINES;

  // Set initial bone geometry
  std::vector<glhckVertexData2f> debugLineData;

  for(Skeleton::Bone const& bone : puppet.getSkeleton().getBones())
  {
    debugLineData.push_back({
      {bone.getBase().x, bone.getBase().y},
      {0, 0}, {0, 0}, {255, 255, 255, 255}
    });
    debugLineData.push_back({
      {bone.getTip().x, bone.getTip().y},
      {0, 0}, {0, 0}, {255, 255, 255, 255}
    });
  }

  glhckGeometrySetVertices(glhckObjectGetGeometry(debugLines), GLHCK_VERTEX_V2F, debugLineData.data(), debugLineData.size());

}

PuppetEntity::~PuppetEntity()
{
  glhckObjectFree(debugLines);
  std::cout << "~PuppetEntity" << std::endl;
}


void PuppetEntity::render(ew::RenderContext* context)
{
  glhckObjectDraw(debugLines);
}

void PuppetEntity::update(float const delta)
{
  puppet.update(delta);
  glhckObjectPositionf(debugLines, position.x, position.y, 0);
  glhckVertexData2f* vertices = glhckObjectGetGeometry(debugLines)->vertices.v2f;
  for(int i = 0; i < puppet.getSkeleton().getBones().size(); ++i) 
  {
    Skeleton::Bone& bone = puppet.getSkeleton().getBone(i);
    Vec2D base = bone.getBase();
    Vec2D tip = bone.getTip();
    vertices[i * 2].vertex = {base.x, base.y};
    vertices[i * 2 + 1].vertex = {tip.x, tip.y};
  }
  glhckObjectUpdate(debugLines);
}


Puppet& PuppetEntity::getPuppet()
{
  return puppet;
}

void PuppetEntity::setPosition(const Vec2D& pos)
{
  position = pos;
}
