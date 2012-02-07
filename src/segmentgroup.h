#ifndef SEGMENT_GROUP_H
#define SEGMENT_GROUP_H

#include <list>
#include "framework.h"

#include "shooterentity.h"

class SegmentGroup : public ShooterEntity
{
public:
  typedef std::list<Segment> SegmentList;

  SegmentGroup(Vec2D const& position, SegmentList const& segments);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
  bool query(Segment const& segment, SegmentTree::ResultHandler& handler) const;
  
private:
  class ResultHandlerHelper : public SegmentTree::ResultHandler
  {
  public:
    ResultHandlerHelper(Vec2D const& velocity, SegmentTree::ResultHandler& handler) : delta(delta), handler(handler) {}
    virtual bool handle(Segment const& segment) const;
  private:
    Vec2D const& delta;
    SegmentTree::ResultHandler& handler;
  };
  
  void drawArrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, float const r, float const g, float const b, float const lineWidth, float const tipLength, float const tipWidth);
  
  Vec2D position;
  Vec2D velocity;
  SegmentList segments;
  SegmentTree segmentTree;
};

#endif
