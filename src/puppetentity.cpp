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
  dirtyVertices(true),
  showBoneLines(false), showPartLines(false), showParts(true),
  boneLines(glhckObjectNew()), partLines(glhckObjectNew()), parts(glhckObjectNew()), 
  position(0, 0), puppet(p)
{
  glhckObjectAddChildren(parts, boneLines);
  glhckObjectAddChildren(parts, partLines);
  
  glhckObjectNewGeometry(boneLines)->type = GLHCK_LINES;

  glhckObjectNewGeometry(partLines)->type = GLHCK_LINES;  
  glhckObjectMaterialFlags(partLines, GLHCK_MATERIAL_COLOR);

  glhckObjectNewGeometry(parts)->type = GLHCK_TRIANGLES;
  
  glhckTexture* partsTexture = glhckTextureNew(puppet.getSpriteSheet().getImage().data(), 0, &TEXTURE_PARAMETERS);
  glhckObjectTexture(parts, partsTexture);
  glhckTextureFree(partsTexture);
}

PuppetEntity::~PuppetEntity()
{
  glhckObjectFree(boneLines);
  glhckObjectFree(partLines);
  glhckObjectFree(parts);
}


void PuppetEntity::render(ew::RenderContext* context)
{
  if(showParts)
    glhckObjectDraw(parts);
  
  if(showPartLines)
    glhckObjectDraw(partLines);
  
  if(showBoneLines)
    glhckObjectDraw(boneLines);
}

void PuppetEntity::update(float const delta)
{
  puppet.update(delta);

  glhckObjectPositionf(parts, position.x, position.y, 0);

  if(dirtyVertices)
  {
    refreshVertices();
  }
  else
  {
    if(showBoneLines)
    {
      glhckVertexData2f* boneVertices = glhckObjectGetGeometry(boneLines)->vertices.v2f;
      for(int i = 0; i < puppet.getSkeleton().getBones().size(); ++i) 
      {
        Skeleton::Bone& bone = puppet.getSkeleton().getBone(i);
        Vec2D base = bone.getBase();
        Vec2D tip = bone.getTip();
        boneVertices[i * 2].vertex = {base.x, base.y};
        boneVertices[i * 2 + 1].vertex = {tip.x, tip.y};
      }
    }
    
    if(showPartLines || showParts)
    {
      glhckVertexData2f* partVertices = glhckObjectGetGeometry(parts)->vertices.v2f;
      glhckVertexData2f* partLineVertices = glhckObjectGetGeometry(partLines)->vertices.v2f;
      Puppet::PartRefs partRefs = puppet.getPartsZOrdered();
      for(int i = 0; i < partRefs.size(); ++i) 
      {
        Puppet::Part const* part = partRefs.at(i);
        
        Vec2D const& p1 = part->position.topLeft;
        Vec2D const& p2 = part->position.bottomLeft;
        Vec2D const& p3 = part->position.bottomRight;
        Vec2D const& p4 = part->position.topRight;
        
        if(showParts)
        {
          partVertices[i * 6 + 0].vertex = {p1.x, p1.y};
          partVertices[i * 6 + 1].vertex = {p3.x, p3.y};
          partVertices[i * 6 + 2].vertex = {p2.x, p2.y};
          partVertices[i * 6 + 3].vertex = {p1.x, p1.y};
          partVertices[i * 6 + 4].vertex = {p4.x, p4.y};
          partVertices[i * 6 + 5].vertex = {p3.x, p3.y};
        }
        
        if(showPartLines)
        {
          partLineVertices[i * 8 + 0].vertex = {p1.x, p1.y};
          partLineVertices[i * 8 + 1].vertex = {p2.x, p2.y};
          partLineVertices[i * 8 + 2].vertex = {p2.x, p2.y};
          partLineVertices[i * 8 + 3].vertex = {p3.x, p3.y};
          partLineVertices[i * 8 + 4].vertex = {p3.x, p3.y};
          partLineVertices[i * 8 + 5].vertex = {p4.x, p4.y};
          partLineVertices[i * 8 + 6].vertex = {p4.x, p4.y};
          partLineVertices[i * 8 + 7].vertex = {p1.x, p1.y};
        }
      }
    }
  }

  if(showParts)
    glhckObjectUpdate(parts);
  
  if(showPartLines)
    glhckObjectUpdate(partLines);
  
  if(showBoneLines)
    glhckObjectUpdate(boneLines);
}


Puppet const& PuppetEntity::getPuppet()
{
  return puppet;
}

void PuppetEntity::setPosition(const Vec2D& pos)
{
  position = pos;
}

Vec2D const& PuppetEntity::getPosition() const
{
  return position;
}

void PuppetEntity::setPose(const std::string& name, bool const state)
{
  Skeleton::Pose& pose = puppet.getSkeleton().getPose(name);
  if(state)
  {
    pose.activate();
  }
  else
  {
    pose.deactivate();
    pose.reset();
  }
}

void PuppetEntity::setFlipX(bool value)
{
  puppet.setFlipX(value);
  dirtyVertices = true;
}

void PuppetEntity::setFlipY(bool value)
{
  puppet.setFlipY(value);
  dirtyVertices = true;
}

void PuppetEntity::refreshVertices()
{
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
    glhckTexture* texture = glhckObjectGetTexture(parts);
    int width = 0;
    int height = 0;
    glhckTextureGetInformation(texture, nullptr, &width, &height, nullptr, nullptr, nullptr, nullptr);

    auto genTexCoord = [&](Vec2D const& imagePosition) {
      return Vec2D{imagePosition.x / width, 1.0f - imagePosition.y / height};
    };
    
    Vec2D tex1 = genTexCoord(part->imagePosition.topLeft);
    Vec2D tex2 = genTexCoord(part->imagePosition.bottomLeft);
    Vec2D tex3 = genTexCoord(part->imagePosition.bottomRight);
    Vec2D tex4 = genTexCoord(part->imagePosition.topRight);

    Vec2D const& p1 = part->position.topLeft;
    Vec2D const& p2 = part->position.bottomLeft;
    Vec2D const& p3 = part->position.bottomRight;
    Vec2D const& p4 = part->position.topRight;
    
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
  
  dirtyVertices = false;
}
