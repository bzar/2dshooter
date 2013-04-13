#include "polygon.h"
#include "segmenttree.h"
#include <list>

Polygon::Polygon(const std::vector<Vec2D> &vertices) : vertices(vertices)
{
}

std::vector<Triangle> Polygon::triangulate() const
{
  std::vector<Segment> segments = toSegments();
  std::list<Segment> segmentList(segments.begin(), segments.end());
  SegmentTree segmentTree(segmentList);

  std::list<Vec2D> verticesLeft(vertices.begin(), vertices.end());
  std::vector<Triangle> triangles;

  auto ia = verticesLeft.begin();
  int wrongTurnCount = 0;

  while(verticesLeft.size() > 0 && wrongTurnCount < verticesLeft.size())
  {
    auto ib = ia;
    ib = ++ib == verticesLeft.end() ? verticesLeft.begin() : ib;
    auto ic = ib;
    ic = ++ic == verticesLeft.end() ? verticesLeft.begin() : ic;

    Vec2D const& a = *ia;
    Vec2D const& b = *ib;
    Vec2D const& c = *ic;

    Vec2D const ab = b - a;
    Vec2D const bc = c - b;

    if(ab.cross(bc) >= 0)
    {
      wrongTurnCount += 1;
      ++ia;
      continue;
    }

    wrongTurnCount = 0;

    Vec2D const delta = c - a;
    Segment ac(a + delta.scale(0.0001), a + delta.scale(0.9999));

    bool intersects = segmentTree.query(ac, [&ac](Segment const& s) {
      return s.intersects(ac);
    });

    if(intersects)
    {
      ++ia;
      continue;
    }

    triangles.push_back({a, b, c});
    ia = verticesLeft.erase(ib);
  }

  return triangles;
}

std::vector<Segment> Polygon::toSegments() const
{
  std::vector<Segment> segments;
  for(int i = 1; i <= vertices.size(); ++i)
  {
    Vec2D const& a = vertices.at(i - 1);
    Vec2D const& b = vertices.at(i % vertices.size());
    segments.push_back({a, b});
  }

  return segments;
}
