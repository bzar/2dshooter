#ifndef LEVEL_HH
#define LEVEL_HH

#include "util/vec2d.h"
#include "util/segment.h"

#include <string>
#include <list>

class Level
{
public:
  Level(std::string const& filename);
  std::string const& getFilename() const;
  std::string const& getName() const;
  Vec2D const& getStartPosition() const;
  std::list<Segment> const& getSegments() const;
  
private:
  std::string filename;
  std::string name;
  Vec2D startPosition;
  std::list<Segment> segments;
};

#endif