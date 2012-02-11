#include "level.h"
#include "segmentgroup.h"

#include <fstream>
#include <sstream>

Level loadLevelFromFile(std::string const& filename)
{
  Level level = {"", Vec2D(), std::list<SegmentGroup*>()};
  std::ifstream levelFile(filename.c_str());
  std::string line;
  std::string key;
  
  while(getline(levelFile, line) && !line.empty())
  {
    std::istringstream parts(line);
    parts >> key;
    if(key == "name")
    {
      parts >> level.name;
    }
    else if(key == "start")
    {
      parts >> level.startPosition.x >> level.startPosition.y;
    }
  }

  while(getline(levelFile, line))
  {
    std::istringstream positionParts(line);
    Vec2D position;
    SegmentGroup::SegmentList segments;
    positionParts >> position.x >> position.y;
    
    while(getline(levelFile, line) && !line.empty())
    {
      std::istringstream parts(line);
      Vec2D first;
      Vec2D second;
      
      parts >> first.x >> first.y;
      while(parts >> second.x >> second.y)
      {
        segments.push_back(Segment(first, second));
        first = second;
      }      
    }
    
    if(!segments.empty())
    {
      level.segmentGroups.push_back(new SegmentGroup(position, segments));
    }
  }
  
  return level;
}