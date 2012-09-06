#include "human.h"
#include <vector>
#include <algorithm>
#include <iostream>

ew::UID const Human::ID = ew::getUID();
std::string const Human::SKELETON_FILE = "skeletons/human.qmlon";

Human::Human(GameWorld* world) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  debugLines(glhckObjectNew()), skeleton(SKELETON_FILE)
{
  glhckObjectSetGeometryType(debugLines, GLHCK_LINES);
  skeleton.getPose("walk")->activate();
  skeleton.getPose("walk-hands")->activate();
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

  std::vector<glhckImportVertexData> debugLineData;

  for(Skeleton::Bone::Reference const& bone : skeleton.getBones())
  {
    debugLineData.push_back({
      {bone->getBase().x, bone->getBase().y, 0},
      {0, 0, 0}, {0, 0}, {255, 255, 255, 255}
    });
    debugLineData.push_back({
      {bone->getTip().x, bone->getTip().y, 0},
      {0, 0, 0}, {0, 0}, {255, 255, 255, 255}
    });
  }

  glhckObjectInsertVertexData3d(debugLines, debugLineData.size(), debugLineData.data());
}

