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
  skeleton.getBone("head")->changeAngle(-0.05);
  skeleton.getBone("thigh-right")->changeAngle(0.1);
  skeleton.getBone("thigh-left")->changeAngle(-0.1);
  skeleton.getBone("leg-right")->changeAngle(-0.1);
  skeleton.getBone("leg-left")->changeAngle(0.1);
  skeleton.getBone("arm-right")->changeAngle(0.25);
  skeleton.getBone("arm-left")->changeAngle(-0.45);
  skeleton.getBone("forearm-right")->changeAngle(0.05);
  skeleton.getBone("forearm-left")->changeAngle(-0.05);
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
  /*skeleton.getBone("a")->changeAngle(0.0025);
  skeleton.getBone("b")->changeAngle(0.005);*/
  skeleton.getBone("arm-left")->changeAngle(-0.0045);
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

