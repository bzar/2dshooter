#ifndef RECT_H
#define RECT_H

#include "vec2d.h"

class Rect {
public:
  Rect(float const left, float const bottom, float const w, float const h);
  Rect(Vec2D const& bottomLeft, float const w, float const h);
  Rect(Rect const& other);

  float top() const;
  float bottom() const;
  float left() const;
  float right() const;
  float verticalCenter() const;
  float horizontalCenter() const;

  float width() const;
  float height() const;

  Vec2D center() const;
  Vec2D topLeft() const;
  Vec2D topRight() const;
  Vec2D bottomLeft() const;
  Vec2D bottomRight() const;

  Rect& setTop(float const value);
  Rect& setBottom(float const value);
  Rect& setLeft(float const value);
  Rect& setRight(float const value);
  Rect& setVerticalCenter(float const value);
  Rect& setHorizontalCenter(float const value);

  Rect& setWidth(float const value);
  Rect& setHeight(float const value);

  Rect& setCenter(Vec2D const& position);
  Rect& setTopLeft(Vec2D const& position);
  Rect& setTopRight(Vec2D const& position);
  Rect& setBottomLeft(Vec2D const& position);
  Rect& setBottomRight(Vec2D const& position);

  bool intersectsWith(Rect const& other) const;

  float x;
  float y;
  float w;
  float h;
};
#endif