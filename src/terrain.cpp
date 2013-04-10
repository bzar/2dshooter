#include "terrain.h"
#include "qmloninitializer.h"
#include <iostream>

std::map<std::string, Terrain> Terrain::cache;

Terrain::Terrain() :
  spriteSheet(), filled(false), edges()
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
    {"sprite", qmlon::set(&Edge::sprite)},
    {"width", qmlon::set(&Edge::width)},
  });

  qmlon::Initializer<Terrain> ti({
    {"spritesheet", [](Terrain& terrain, qmlon::Value::Reference v) {
       std::string spriteSheetFilename = v->asString();
       qmlon::Value::Reference spriteSheetValue = qmlon::readFile(spriteSheetFilename);
       terrain.spriteSheet.initialize(spriteSheetValue);
    }},
    {"filled", qmlon::set(&Terrain::filled)}
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

const SpriteSheet &Terrain::getSpriteSheet() const
{
  return spriteSheet;
}

bool Terrain::getFilled() const
{
  return filled;
}

const std::vector<Terrain::Edge> &Terrain::getEdges() const
{
  return edges;
}
