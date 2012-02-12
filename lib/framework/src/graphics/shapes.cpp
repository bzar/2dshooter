#include "shapes.h"
#include "../util/log.h"
#include <vector>
#include <algorithm>

void Shapes::arrow(Transformation const& view, Vec2D const& base, Vec2D const& tip, 
                   float const r, float const g, float const b, 
                   float const lineWidth, float const tipLength, float const tipWidth)
{
  
  Vec2D const delta = tip - base;
  Vec2D const normal = delta.normal().uniti();
  float const length = delta.length();
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
  
  float const a = 1.0;
  GLfloat colors[] = {
    r, g, b, a,  r, g, b, a,  r, g, b, a,  
    r, g, b, a,  r, g, b, a,  r, g, b, a,  
    r, g, b, a,  r, g, b, a,  r, g, b, a
  };
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, arrow);
  glColorPointer(4, GL_FLOAT, 0, colors);
  glDrawArrays(GL_TRIANGLES, 0, NUM_ARROW_VERTICES);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void Shapes::grid(Transformation const& view, Vec2D const& position, Vec2D const& u, Vec2D const& v,
                  Vec2D const& origin, float uGridSize, float vGridSize,
                  float const r, float const g, float const b, 
                  float const lineWidth)
{
  float const ul = u.length();
  float const vl = v.length();
  Vec2D const uu = u.unit();
  Vec2D const vu = v.unit();
  Vec2D lw = uu.scale(0.5 * lineWidth);
  Vec2D lh = vu.scale(0.5 * lineWidth);
  
  Vec2D const originOffset = position - origin;
  
  Vec2D const uOriginOffset = originOffset.projection(u);
  Vec2D const vOriginOffset = originOffset.projection(v);
  float uOffset = uOriginOffset.length() - static_cast<int>(uOriginOffset.length() / uGridSize) * uGridSize;
  float vOffset = vOriginOffset.length() - static_cast<int>(vOriginOffset.length() / vGridSize) * vGridSize;
  if(uOriginOffset.dot(u) > 0)
    uOffset = uGridSize - uOffset;
  if(vOriginOffset.dot(v) > 0)
    vOffset = vGridSize - vOffset;

  
  int const NUM_VERTICES_PER_LINE = lineWidth > 0 ? 6 : 2;
  int const NUM_VALUES_PER_VERTICE = 2;
  int const NUM_VALUES_PER_LINE = NUM_VERTICES_PER_LINE * NUM_VALUES_PER_VERTICE;
  int const NUM_VALUES_PER_COLOR = 4;
  int const NUM_VERTICAL_LINES = (ul / uGridSize + 0.5);
  int const NUM_HORIZONTAL_LINES = (vl / vGridSize + 0.5);
  int const NUM_VERTICES = (NUM_VERTICAL_LINES + NUM_HORIZONTAL_LINES)  * NUM_VERTICES_PER_LINE;
  int const VERTICE_ARRAY_SIZE = NUM_VERTICES * NUM_VALUES_PER_VERTICE;
  int const COLOR_ARRAY_SIZE = NUM_VERTICES * NUM_VALUES_PER_COLOR;

  std::vector<GLfloat> vertices(VERTICE_ARRAY_SIZE, 0.0);
  std::vector<GLfloat> colors(COLOR_ARRAY_SIZE, 0.0);
  GLfloat color[] = {r, g, b, 1.0};

  for(int i = 0; i < NUM_VERTICES; ++i)
  {
    std::copy(color, color + NUM_VALUES_PER_COLOR, colors.begin() + i*NUM_VALUES_PER_COLOR);
  }

  for(int i = 0; i < NUM_VERTICAL_LINES + NUM_HORIZONTAL_LINES; ++i)
  {
    bool vertical = i < NUM_VERTICAL_LINES;
    int j = vertical ? i : i - NUM_VERTICAL_LINES;
    
    Vec2D p1;
    Vec2D p2;
    
    if(vertical)
    {
      float x = uOffset + j * uGridSize;
      p1 = uu.scale(x) + position;
      p2 = p1 + v;
    }
    else
    {
      float y = vOffset + j * vGridSize;
      p1 = vu.scale(y) + position;
      p2 = p1 + u;
    }
    
    if(lineWidth > 0)
    {
      Vec2D const& l = vertical ? lw : lh;
      Vec2D const q1 = view.transform(p1 + l);
      Vec2D const q2 = view.transform(p1 - l);
      Vec2D const q3 = view.transform(p2 + l);
      Vec2D const q4 = view.transform(p2 - l);
      
      GLfloat lineVertices[] = {q1.x, q1.y, q2.x, q2.y, q3.x, q3.y, q2.x, q2.y, q3.x, q3.y, q4.x, q4.y};
      std::copy(lineVertices, lineVertices + NUM_VALUES_PER_LINE, vertices.begin() + i*NUM_VALUES_PER_LINE);
    }
    else
    {
      Vec2D const q1 = view.transform(p1);
      Vec2D const q2 = view.transform(p2);
      
      GLfloat lineVertices[] = {q1.x, q1.y, q2.x, q2.y};
      std::copy(lineVertices, lineVertices + NUM_VALUES_PER_LINE, vertices.begin() + i*NUM_VALUES_PER_LINE);
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, &(vertices[0]));
  glColorPointer(4, GL_FLOAT, 0, &(colors[0]));
  if(lineWidth > 0)
  {
    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
  }
  else
  {
    glDrawArrays(GL_LINES, 0, NUM_VERTICES);
  }
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
