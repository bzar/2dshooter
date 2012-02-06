#include "interval.h"

Interval::Interval(Limit const& lower, Limit const& upper) : lower(lower), upper(upper)
{
}

Interval::Interval(float const lower, float const upper) : lower(Limit(lower)), upper(Limit(upper))
{
}

bool Interval::contains(float const value)
{
  return (value > lower.value || (lower.closed && value == lower.value))
      && (value < upper.value || (upper.closed && value == upper.value));
}
bool Interval::contains(Interval const& other)
{
  if(lower.value > other.lower.value || upper.value < other.upper.value) return false;
  if(!lower.closed && other.lower.closed && lower.value == other.lower.value) return false;
  if(!upper.closed && other.upper.closed && upper.value == other.upper.value) return false;
  return true;  
}
bool Interval::intersects(Interval const& other)
{
  if(lower.value > other.upper.value || upper.value < other.lower.value) return false;
  if((!lower.closed || !other.upper.closed) && lower.value == other.upper.value) return false;
  if((!upper.closed || !other.lower.closed) && upper.value == other.lower.value) return false;
  return true;  
}
