#include "segmentgroup.h"

SegmentGroup::SegmentGroup(Vec2D const& position, SegmentList const& segments) : ShooterWorld::ShooterEntity(), position(position), segments(segments), segmentTree(segments)
{
}

void SegmentGroup::render(Screen const& screen)
{
  for(SegmentList::iterator i = segments.begin(); i != segments.end(); ++i)
  {
    drawArrow(screen, position + i->a, position + i->b, 0.0, 0.8, 0.0, 1, 9, 7);
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

void SegmentGroup::drawArrow(Screen const& screen, Vec2D const& base, Vec2D const& tip, float const r, float const g, float const b, float const lineWidth, float const tipLength, float const tipWidth)
{
  Vec2D const delta = tip - base;
  Vec2D const normal = delta.normal().uniti();
  float length = delta.length();
  Vec2D const c = base + delta.scale((length - tipLength) / length);
  
  Transformation const& s = screen.transformation();
  Vec2D const l1 = s.transform(base + normal.scale(0.5 * lineWidth));
  Vec2D const l2 = s.transform(c + normal.scale(0.5 * lineWidth));
  Vec2D const l3 = s.transform(c - normal.scale(0.5 * lineWidth));
  Vec2D const l4 = s.transform(base - normal.scale(0.5 * lineWidth));
  
  Vec2D const t1 = s.transform(c - normal.scale(0.5 * tipWidth));
  Vec2D const t2 = s.transform(c + normal.scale(0.5 * tipWidth));
  Vec2D const t3 = s.transform(tip);
  
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