#ifndef SEGMENT_GROUP_H
#define SEGMENT_GROUP_H

#include <list>
#include "framework.h"

#include "shooterentity.h"

class SegmentGroup : public ShooterEntity
{
public:
  typedef std::list<Segment> SegmentList;

  class ResultHandler
  {
  public:
    virtual bool handle(Segment const& segment, SegmentGroup const& group) const = 0;
  };

  SegmentGroup(Vec2D const& position, SegmentList const& segments);
  void render(Transformation const& view);
  void intent(float const delta);
  void reaction(float const delta);
  void update(float const delta);
  
  Vec2D getPosition() const;
  Vec2D getVelocity() const;
  void setPosition(Vec2D const& newPosition);
  void setVelocity(Vec2D const& newVelocity);
  
  void addSegment(Segment const& segment);
  bool query(Segment const& segment, ResultHandler& handler) const;
  
private:
  class ResultHandlerHelper : public SegmentTree::ResultHandler
  {
  public:
    ResultHandlerHelper(SegmentGroup const& sg, SegmentGroup::ResultHandler& handler) : sg(sg), handler(handler) {}
    virtual bool handle(Segment const& segment) const;
  private:
    SegmentGroup const& sg;
    SegmentGroup::ResultHandler& handler;
  };
  
  friend class ResultHandlerHelper;
  
  Vec2D toLocal(Vec2D const& v) const;
  Vec2D toGlobal(Vec2D const& v) const;
  Segment toLocal(Segment const& s) const;
  Segment toGlobal(Segment const& s) const;
  
  Vec2D position;
  Vec2D velocity;
  SegmentList segments;
  SegmentTree segmentTree;
};

#endif
