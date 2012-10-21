#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

ew::UID const Human::ID = ew::getUID();
std::string const Human::SKELETON_FILE = "skeletons/human.qmlon";
std::string const Human::SPRITESHEET_FILE = "spritesheets/stickman.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  debugLines(glhckObjectNew()), skeleton(SKELETON_FILE),
  spritesheet(SpriteSheet::create(SPRITESHEET_FILE))
{
  glhckObjectNewGeometry(debugLines)->type = GLHCK_LINES;
  skeleton.getPose("walk")->activate();
  skeleton.getPose("walk-hands")->activate();

  // Set initial bone geometry
  std::vector<glhckVertexData2f> debugLineData;

  for(Skeleton::Bone::Reference const& bone : skeleton.getBones())
  {
    debugLineData.push_back({
      {bone->getBase().x, bone->getBase().y},
      {0, 0}, {255, 255, 255, 255}
    });
    debugLineData.push_back({
      {bone->getTip().x, bone->getTip().y},
      {0, 0}, {255, 255, 255, 255}
    });
  }

  glhckGeometrySetVertices(glhckObjectGetGeometry(debugLines), GLHCK_VERTEX_V2F, debugLineData.data(), debugLineData.size());

}

Human::~Human()
{
  glhckObjectFree(debugLines);
}


void Human::render(ew::RenderContext* context)
{
  glhckObjectDraw(debugLines);
}

void Human::update(float const delta)
{
  skeleton.update(delta);

  for(int i = 0; i < skeleton.getBones().size(); ++i) 
  {
    glhckGeometry* geometry = glhckObjectGetGeometry(debugLines);
    glhckVector2f& vBase = geometry->vertices.v2f[i * 2].vertex;
    glhckVector2f& vTip = geometry->vertices.v2f[i * 2 + 1].vertex;
    Skeleton::Bone::Reference bone = skeleton.getBone(i);
    vBase = {bone->getBase().x, bone->getBase().y};
    vTip = {bone->getTip().x, bone->getTip().y};
  }
  glhckObjectUpdate(debugLines);
}

