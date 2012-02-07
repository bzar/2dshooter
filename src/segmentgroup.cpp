#include "segmentgroup.h"
#include "shooterworld.h"

SegmentGroup::SegmentGroup(Vec2D const& position, SegmentList const& segments) : ShooterEntity(), position(position), velocity(0, 0), segments(segments), segmentTree(segments)
{
}

void SegmentGroup::render(Transformation const& view)
{
  for(SegmentList::iterator i = segments.begin(); i != segments.end(); ++i)
  {
    drawArrow(view, position + i->a, position + i->b, 0.0, 0.8, 0.0, 1, 9, 7);
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

bool SegmentGroup::query(Segment const& segment, SegmentTree::ResultHandler& handler) const
{
  Segment s(segment.a - position, segment.b - position);
  ResultHandlerHelper helper(position + velocity, handler);
  return segmentTree.query(s, helper);
}

void SegmentGroup::drawArrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, float const r, float const g, float const b, float const lineWidth, float const tipLength, float const tipWidth)
{
  Vec2D const delta = tip - base;
  Vec2D const normal = delta.normal().uniti();
  float length = delta.length();
  Vec2D const c = base + delta.scale((length - tipLength) / length);
  
  Vec2D const l1 = view.transform(base + normal.scale(0.5 * lineWidth));
  Vec2D const l2 = view.transform(c + normal.scale(0.5 * lineWidth));
  Vec2D const l3 = view.transform(c - normal.scale(0.5 * lineWidth));
  Vec2D const l4 = view.transform(base - normal.scale(0.5 * lineWidth));
  
  Vec2D const t1 = view.transform(c - normal.scale(0.5 * tipWidth));
  Vec2D const t2 = view.transform(c + normal.scale(0.5 * tipWidth));
  Vec2D const t3 = view.transform(tip);
  
  int const NUM_ARROW_VERTICES = 9;
  GLfloat arrow[] = {
    l1.x, l1.y,  l2.x, l2.y,  l4.x, l4.y,  
    l4.x, l4.y,  l2.x, l2.y,  l3.x, l3.y,  
    t1.x, t1.y,  t2.x, t2.y,  t3.x, t3.y
  };

  GLfloat colors[] = {
        r, g, b,  r, g, b,  r, g, b,  
        r, g, b,  r, g, b,  r, g, b,  
        r, g, b,  r, g, b,  r, g, b,  
  };
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, arrow);
  glColorPointer(3, GL_FLOAT, 0, colors);
  glDrawArrays(GL_TRIANGLES, 0, NUM_ARROW_VERTICES);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

bool SegmentGroup::ResultHandlerHelper::handle(Segment const& segment) const
{
  Segment s(segment.a + delta, segment.a + delta);
  return handler.handle(s);
}