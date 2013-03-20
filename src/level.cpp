#include "level.h"
#include "qmlon.h"
#include "qmloninitializer.h"
#include "qmloninitializershelpers.h"

Level::Level(const std::string& filename) :
  filename(filename), name(), startPosition(), segments()
{
  qmlon::Initializer<Vec2D> vi({
    {"x", qmlon::set(&Vec2D::x)},
    {"y", qmlon::set(&Vec2D::y)},
  });
    
  qmlon::Initializer<Level> li({
    {"name", qmlon::set(&Level::name)},
    {"start", qmlon::createSet(vi, &Level::startPosition)}
  }, {
    {"Line", [&](Level& level, qmlon::Object& o) {
      qmlon::Value::List const& vertices = o.getProperty("vertices")->asList();
      Vec2D current;
      Vec2D previous;
      bool first = true;
      for(qmlon::Value::Reference v : vertices)
      {
        previous = current;
        current = qmlon::create(v, vi);
        if(first)
        {
          first = false;
          continue;
        }
        
        level.segments.push_back({previous, current});
      }
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

std::list< Segment > const& Level::getSegments() const
{
  return segments;
}

Vec2D const& Level::getStartPosition() const
{
  return startPosition;
}
