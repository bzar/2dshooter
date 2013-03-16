#include "puppetentity.h"
#include <vector>
#include <algorithm>
#include <iostream>

PuppetEntity::PuppetEntity(const Puppet& p, GameWorld* world) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  boneLines(glhckObjectNew()), partLines(glhckObjectNew()), position(0, 0), puppet(p)
{
  glhckObjectNewGeometry(boneLines)->type = GLHCK_LINES;
  glhckObjectNewGeometry(partLines)->type = GLHCK_LINES;
  glhckObjectMaterialFlags(partLines, GLHCK_MATERIAL_COLOR);
  
  // Set initial bone geometry
  std::vector<glhckVertexData2f> boneLineData;

  for(Skeleton::Bone const& bone : puppet.getSkeleton().getBones())
  {
    boneLineData.push_back({
      {bone.getBase().x, bone.getBase().y},
      {0, 0}, {0, 0}, {255, 255, 255, 255}
    });
    boneLineData.push_back({
      {bone.getTip().x, bone.getTip().y},
      {0, 0}, {0, 0}, {255, 255, 255, 255}
    });
  }

  glhckGeometrySetVertices(glhckObjectGetGeometry(boneLines), GLHCK_VERTEX_V2F, boneLineData.data(), boneLineData.size());

  
  std::vector<glhckVertexData2f> partLineData;

  for(Puppet::Part const& part : puppet.getParts())
  {
    
    Vec2D p1 = part.transformation.transform({0, 0});
    Vec2D p2 = part.transformation.transform({0, 1});
    Vec2D p3 = part.transformation.transform({1, 1});
    Vec2D p4 = part.transformation.transform({1, 0});
    
    // left: green, bottom: yellow, right: cyan, top: magenta
    partLineData.push_back({{p1.x, p1.y}, {0, 0}, {0, 0}, {0, 255, 0, 255}});
    partLineData.push_back({{p2.x, p2.y}, {0, 0}, {0, 0}, {0, 255, 0, 255}});
    partLineData.push_back({{p2.x, p2.y}, {0, 0}, {0, 0}, {255, 255, 0, 255}});
    partLineData.push_back({{p3.x, p3.y}, {0, 0}, {0, 0}, {255, 255, 0, 255}});
    partLineData.push_back({{p3.x, p3.y}, {0, 0}, {0, 0}, {0, 255, 255, 255}});
    partLineData.push_back({{p4.x, p4.y}, {0, 0}, {0, 0}, {0, 255, 255, 255}});
    partLineData.push_back({{p4.x, p4.y}, {0, 0}, {0, 0}, {255, 0, 255, 255}});
    partLineData.push_back({{p1.x, p1.y}, {0, 0}, {0, 0}, {255, 0, 255, 255}});
  }

  glhckGeometrySetVertices(glhckObjectGetGeometry(partLines), GLHCK_VERTEX_V2F, partLineData.data(), partLineData.size());
}

PuppetEntity::~PuppetEntity()
{
  glhckObjectFree(boneLines);
  std::cout << "~PuppetEntity" << std::endl;
}


void PuppetEntity::render(ew::RenderContext* context)
{
  glhckObjectDraw(boneLines);
  glhckObjectDraw(partLines);
}

void PuppetEntity::update(float const delta)
{
  puppet.update(delta);
  glhckObjectPositionf(boneLines, position.x, position.y, 0);
  glhckObjectPositionf(partLines, position.x, position.y, 0);
  
  glhckVertexData2f* boneVertices = glhckObjectGetGeometry(boneLines)->vertices.v2f;
  for(int i = 0; i < puppet.getSkeleton().getBones().size(); ++i) 
  {
    Skeleton::Bone& bone = puppet.getSkeleton().getBone(i);
    Vec2D base = bone.getBase();
    Vec2D tip = bone.getTip();
    boneVertices[i * 2].vertex = {base.x, base.y};
    boneVertices[i * 2 + 1].vertex = {tip.x, tip.y};
  }
  
  glhckVertexData2f* partVertices = glhckObjectGetGeometry(partLines)->vertices.v2f;
  for(int i = 0; i < puppet.getParts().size(); ++i) 
  {
    Puppet::Part const& part = puppet.getPart(i);
    SpriteSheet::Frame::Size size = puppet.getSpriteSheet().getSprite(part.frontId).getAnimation(0).getFrame(0).getSize();
    Vec2D p1 = part.transformation.transform({0, 0});
    Vec2D p2 = part.transformation.transform({0, size.height});
    Vec2D p3 = part.transformation.transform({size.width, size.height});
    Vec2D p4 = part.transformation.transform({size.width, 0});
    
    partVertices[i * 8].vertex = {p1.x, p1.y};
    partVertices[i * 8 + 1].vertex = {p2.x, p2.y};
    partVertices[i * 8 + 2].vertex = {p2.x, p2.y};
    partVertices[i * 8 + 3].vertex = {p3.x, p3.y};
    partVertices[i * 8 + 4].vertex = {p3.x, p3.y};
    partVertices[i * 8 + 5].vertex = {p4.x, p4.y};
    partVertices[i * 8 + 6].vertex = {p4.x, p4.y};
    partVertices[i * 8 + 7].vertex = {p1.x, p1.y};
  }
  
  glhckObjectUpdate(boneLines);
  glhckObjectUpdate(partLines);
}


Puppet& PuppetEntity::getPuppet()
{
  return puppet;
}

void PuppetEntity::setPosition(const Vec2D& pos)
{
  position = pos;
}
