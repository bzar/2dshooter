#ifndef LEVEL_HH
#define LEVEL_HH

#include "util/vec2d.h"
#include "util/segment.h"

#include <string>
#include <vector>

class Level
{
public:
  struct Line {
    std::vector<Vec2D> vertices;
    std::string terrain;
    bool solid;
  };

  Level(std::string const& filename);

  std::string const& getFilename() const;
  std::string const& getName() const;
  Vec2D const& getStartPosition() const;

  std::vector<Line> const& getLines() const;

private:
  std::string filename;
  std::string name;
  Vec2D startPosition;
  std::vector<Line> lines;
};

#endif
