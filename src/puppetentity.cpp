#include "puppetentity.h"
#include <vector>
#include <algorithm>
#include <iostream>

glhckTextureParameters const PuppetEntity::TEXTURE_PARAMETERS = {
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


PuppetEntity::PuppetEntity(const Puppet& p, GameWorld* world) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  boneLines(glhckObjectNew()), partLines(glhckObjectNew()), parts(glhckObjectNew()), 
  position(0, 0), puppet(p)
{
  glhckObjectNewGeometry(boneLines)->type = GLHCK_LINES;

  glhckObjectNewGeometry(partLines)->type = GLHCK_LINES;  
  glhckObjectMaterialFlags(partLines, GLHCK_MATERIAL_COLOR);

  glhckObjectNewGeometry(parts)->type = GLHCK_TRIANGLES;
  
  glhckTexture* partsTexture = glhckTextureNew(puppet.getSpriteSheet().getImage().data(), 0, &TEXTURE_PARAMETERS);
  glhckObjectTexture(parts, partsTexture);
  glhckTextureFree(partsTexture);
  
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
  
  std::vector<glhckVertexData2f> partsData;
  std::vector<glhckVertexData2f> partLineData;

  Puppet::PartRefs partRefs = puppet.getPartsZOrdered();
  for(Puppet::Part const* part : partRefs)
  {
    SpriteSheet const& spriteSheet = puppet.getSpriteSheet();
    SpriteSheet::Sprite const& sprite = spriteSheet.getSprite(part->frontId); //TODO: handle back sprites too!
    SpriteSheet::Frame const& frame = sprite.getAnimation(0).getFrame(0);
    SpriteSheet::Frame::Size const& size = frame.getSize();

    Vec2D p1 = part->transformation.transform({0, 0});
    Vec2D p2 = part->transformation.transform({0, static_cast<float>(size.height)});
    Vec2D p3 = part->transformation.transform({static_cast<float>(size.width), static_cast<float>(size.height)});
    Vec2D p4 = part->transformation.transform({static_cast<float>(size.width), 0});
    
    glhckTexture* texture = glhckObjectGetTexture(parts);
    int width = 0;
    int height = 0;
    glhckTextureGetInformation(texture, nullptr, &width, &height, nullptr, nullptr, nullptr, nullptr);

    Vec2D tex1{static_cast<float>(frame.getPosition().x) / width, 
               1.0f - static_cast<float>(frame.getPosition().y) / height};
    Vec2D tex2{static_cast<float>(frame.getPosition().x) / width, 
               1.0f - static_cast<float>(frame.getPosition().y + frame.getSize().height) / height};
    Vec2D tex3{static_cast<float>(frame.getPosition().x + frame.getSize().width) / width, 
               1.0f - static_cast<float>(frame.getPosition().y + frame.getSize().height) / height};
    Vec2D tex4{static_cast<float>(frame.getPosition().x + frame.getSize().width) / width, 
               1.0f - static_cast<float>(frame.getPosition().y) / height};

    partsData.push_back({{p1.x, p1.y}, {0, 0}, {tex1.x, tex1.y}, {255, 255, 255, 255}});
    partsData.push_back({{p3.x, p3.y}, {0, 0}, {tex3.x, tex3.y}, {255, 255, 255, 255}});
    partsData.push_back({{p2.x, p2.y}, {0, 0}, {tex2.x, tex2.y}, {255, 255, 255, 255}});
    partsData.push_back({{p1.x, p1.y}, {0, 0}, {tex1.x, tex1.y}, {255, 255, 255, 255}});
    partsData.push_back({{p4.x, p4.y}, {0, 0}, {tex4.x, tex4.y}, {255, 255, 255, 255}});
    partsData.push_back({{p3.x, p3.y}, {0, 0}, {tex3.x, tex3.y}, {255, 255, 255, 255}});

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

  glhckGeometrySetVertices(glhckObjectGetGeometry(parts), GLHCK_VERTEX_V2F, partsData.data(), partsData.size());
  glhckGeometrySetVertices(glhckObjectGetGeometry(partLines), GLHCK_VERTEX_V2F, partLineData.data(), partLineData.size());
}

PuppetEntity::~PuppetEntity()
{
  glhckObjectFree(boneLines);
}


void PuppetEntity::render(ew::RenderContext* context)
{
  glhckObjectDraw(parts);
//   glhckObjectDraw(partLines);
//   glhckObjectDraw(boneLines);
}

void PuppetEntity::update(float const delta)
{
  puppet.update(delta);
  glhckObjectPositionf(boneLines, position.x, position.y, 0);
  glhckObjectPositionf(partLines, position.x, position.y, 0);
  glhckObjectPositionf(parts, position.x, position.y, 0);
  
  glhckVertexData2f* boneVertices = glhckObjectGetGeometry(boneLines)->vertices.v2f;
  for(int i = 0; i < puppet.getSkeleton().getBones().size(); ++i) 
  {
    Skeleton::Bone& bone = puppet.getSkeleton().getBone(i);
    Vec2D base = bone.getBase();
    Vec2D tip = bone.getTip();
    boneVertices[i * 2].vertex = {base.x, base.y};
    boneVertices[i * 2 + 1].vertex = {tip.x, tip.y};
  }
  
  glhckVertexData2f* partVertices = glhckObjectGetGeometry(parts)->vertices.v2f;
  glhckVertexData2f* partLineVertices = glhckObjectGetGeometry(partLines)->vertices.v2f;
  Puppet::PartRefs partRefs = puppet.getPartsZOrdered();
  for(int i = 0; i < partRefs.size(); ++i) 
  {
    Puppet::Part const* part = partRefs.at(i);
    SpriteSheet::Frame::Size size = puppet.getSpriteSheet().getSprite(part->frontId).getAnimation(0).getFrame(0).getSize();
    Vec2D p1 = part->transformation.transform({0, 0});
    Vec2D p2 = part->transformation.transform({0, static_cast<float>(size.height)});
    Vec2D p3 = part->transformation.transform({static_cast<float>(size.width), static_cast<float>(size.height)});
    Vec2D p4 = part->transformation.transform({static_cast<float>(size.width), 0});
    
    partVertices[i * 6].vertex = {p1.x, p1.y};
    partVertices[i * 6 + 1].vertex = {p3.x, p3.y};
    partVertices[i * 6 + 2].vertex = {p2.x, p2.y};
    partVertices[i * 6 + 3].vertex = {p1.x, p1.y};
    partVertices[i * 6 + 4].vertex = {p4.x, p4.y};
    partVertices[i * 6 + 5].vertex = {p3.x, p3.y};

    partLineVertices[i * 8].vertex = {p1.x, p1.y};
    partLineVertices[i * 8 + 1].vertex = {p2.x, p2.y};
    partLineVertices[i * 8 + 2].vertex = {p2.x, p2.y};
    partLineVertices[i * 8 + 3].vertex = {p3.x, p3.y};
    partLineVertices[i * 8 + 4].vertex = {p3.x, p3.y};
    partLineVertices[i * 8 + 5].vertex = {p4.x, p4.y};
    partLineVertices[i * 8 + 6].vertex = {p4.x, p4.y};
    partLineVertices[i * 8 + 7].vertex = {p1.x, p1.y};
    
  }

  glhckObjectUpdate(parts);
  glhckObjectUpdate(partLines);
  glhckObjectUpdate(boneLines);
}


Puppet& PuppetEntity::getPuppet()
{
  return puppet;
}

void PuppetEntity::setPosition(const Vec2D& pos)
{
  position = pos;
}
