#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
public:
  struct Limit
  {
    Limit(float const value, bool closed = true) : value(value), closed(closed) {};
    float value;
    bool closed;
  };
  Interval(Limit const& lower, Limit const& upper);
  Interval(float const lower, float const upper);
  
  bool contains(float const value);
  bool contains(Interval const& other);
  bool intersects(Interval const& other);
  
  Limit lower;
  Limit upper;
};

#endif
