#include "segmentgroup.h"
#include "shooterworld.h"

SegmentGroup::SegmentGroup(Vec2D const& position, SegmentList const& segments) : ShooterEntity(), position(position), velocity(0, 0), segments(segments), segmentTree(segments)
{
}

void SegmentGroup::render(Transformation const& view)
{
  for(SegmentList::iterator i = segments.begin(); i != segments.end(); ++i)
  {
    Shapes::arrow(view, position + i->a, position + i->b, 0.0, 0.8, 0.0, 1, 9, 7);
  }
}

void SegmentGroup::intent(float const delta)
{
  
}
void SegmentGroup::reaction(float const delta)
{
  
}
void SegmentGroup::update(float const delta)
{
}

Vec2D SegmentGroup::getPosition() const
{
  return position;
}
Vec2D SegmentGroup::getVelocity() const
{
  return velocity;
}
void SegmentGroup::setPosition(Vec2D const& newPosition)
{
  position = newPosition;
}
void SegmentGroup::setVelocity(Vec2D const& newVelocity)
{
  velocity = newVelocity;
}

void SegmentGroup::addSegment(Segment const& segment)
{
  Log::debug() << "Adding a segment";
  segments.push_back(segment);
  segmentTree.setSegments(segments);
}

bool SegmentGroup::query(Segment const& segment, ResultHandler& handler) const
{
  ResultHandlerHelper helper(*this, handler);
  return segmentTree.query(toLocal(segment), helper);
}

Vec2D SegmentGroup::toLocal(Vec2D const& v) const
{
  return v - position;
}
Vec2D SegmentGroup::toGlobal(Vec2D const& v) const
{
  return v + position;
}
Segment SegmentGroup::toLocal(Segment const& s) const
{
  return Segment(toLocal(s.a), toLocal(s.b));
}
Segment SegmentGroup::toGlobal(Segment const& s) const
{
  return Segment(toGlobal(s.a), toGlobal(s.b));
}

bool SegmentGroup::ResultHandlerHelper::handle(Segment const& segment) const
{
  return handler.handle(sg.toGlobal(segment), sg);
}