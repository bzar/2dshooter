#include "level.h"
#include "qmlon.h"
#include "qmloninitializer.h"
#include "qmloninitializershelpers.h"

Level::Level(const std::string& filename) :
  filename(filename), name(), startPosition(), lines()
{
  qmlon::Initializer<Vec2D> vi({
    {"x", qmlon::set(&Vec2D::x)},
    {"y", qmlon::set(&Vec2D::y)},
  });

  qmlon::Initializer<Line> lii({
    {"terrain", qmlon::set(&Line::terrain)},
    {"vertices", [&vi](Line& line, qmlon::Value::Reference value) {
      qmlon::Value::List const& vertices = value->asList();
      for(qmlon::Value::Reference v : vertices)
      {
        Vec2D vertex = qmlon::create(v, vi);
        line.vertices.push_back(vertex);
      }
    }}
  });

  qmlon::Initializer<Level> li({
    {"name", qmlon::set(&Level::name)},
    {"start", qmlon::createSet(vi, &Level::startPosition)}
  }, {
    {"Line", [&](Level& level, qmlon::Object& o) {
      Line line = qmlon::create(o, lii);
      level.lines.push_back(line);
    }}
  });
  
  qmlon::Value::Reference value = qmlon::readFile(filename);
  li.init(*this, value);
}

std::string const& Level::getFilename() const
{
  return filename;
}

std::string const& Level::getName() const
{
  return name;
}

const std::vector<Level::Line> &Level::getLines() const
{
  return lines;
}

Vec2D const& Level::getStartPosition() const
{
  return startPosition;
}
