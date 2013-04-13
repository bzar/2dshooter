#include "terrain.h"
#include "qmloninitializer.h"

std::map<std::string, Terrain> Terrain::cache;

Terrain::Terrain() :
  fill(), fillScale(1), edges()
{
}


Terrain Terrain::load(const std::string &filename)
{
  auto t = cache.find(filename);
  if(t != cache.end()) {
    return t->second;
  }

  qmlon::Initializer<Edge> ei({
    {"fromAngle", qmlon::set(&Edge::fromAngle)},
    {"toAngle", qmlon::set(&Edge::toAngle)},
    {"image", qmlon::set(&Edge::image)},
    {"width", qmlon::set(&Edge::width)},
  });

  qmlon::Initializer<Terrain> ti({
    {"fill", qmlon::set(&Terrain::fill)},
    {"fillScale", qmlon::set(&Terrain::fillScale)}
  }, {
   {"Edge", [&ei](Terrain& terrain, qmlon::Object& o) {
      terrain.edges.push_back(qmlon::create(o, ei));
   }}
  });

  qmlon::Value::Reference terrainValue = qmlon::readFile(filename);
  Terrain terrain;
  ti.init(terrain, terrainValue);

  cache[filename] = terrain;
  return terrain;
}

std::string const& Terrain::getFill() const
{
  return fill;
}

const std::vector<Terrain::Edge> &Terrain::getEdges() const
{
  return edges;
}


float Terrain::getFillScale() const
{
  return fillScale;
}
