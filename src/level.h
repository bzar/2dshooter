#ifndef LEVEL_H
#define LEVEL_H

#include "framework.h"
#include <string>
#include <list>

class SegmentGroup;

struct Level
{
  std::string name;
  Vec2D startPosition;
  std::list<SegmentGroup*> segmentGroups;
};

Level loadLevelFromFile(std::string const& filename);

#endif
