#include "segmenttree.h"
#include "log.h"
#include <set>

namespace
{
  inline float min(float a, float b)
  {
    return a < b ? a : b;
  }
  inline float min(float a, float b, float c)
  {
    return a < b ? min(a, c) : min(b, c);
  }
  inline float max(float a, float b)
  {
    return a > b ? a : b;
  }
  inline float max(float a, float b, float c)
  {
    return a > b ? max(a, c) : max(b, c);
  }
}

SegmentTree::SegmentTree(std::list<Segment> const& segments) : root(0), segments(segments), rect(0, 0, 0, 0)
{
  construct();
}

SegmentTree::~SegmentTree()
{
  deconstruct();
}
SegmentTree& SegmentTree::operator=(SegmentTree const& other)
{
  if(&other != this)
  {
    deconstruct();
    segments = other.segments;
    construct();
  }
  return *this;
}

void SegmentTree::setSegments(std::list<Segment> const& newSegments)
{
  deconstruct();
  segments = newSegments;
  construct();
}

SegmentTree::SegmentResults const SegmentTree::query(Vec2D const& point) const
{
  DefaultResultHandler handler;
  query(point, handler);
  return handler.getResults();
}

SegmentTree::SegmentResults const SegmentTree::query(Segment const& segment) const
{
  DefaultResultHandler handler;
  query(segment, handler);
  return handler.getResults();
}

bool SegmentTree::query(Vec2D const& point, ResultHandler& handler) const
{
  return queryNode(point, handler, root);
}

bool SegmentTree::query(Segment const& segment, ResultHandler& handler) const
{
  return queryNode(segment, handler, root);
}


void SegmentTree::construct()
{
  float minx = 0;
  float miny = 0;
  float maxx = 0;
  float maxy = 0;
  bool first = true;
  
  std::set<float> endpoints;
  for(std::list<Segment>::iterator i = segments.begin(); i != segments.end(); ++i)
  {
    Segment const& segment = *i;
    minx = first ? min(segment.a.x, segment.b.x) : min(minx, segment.a.x, segment.b.x);
    miny = first ? min(segment.a.y, segment.b.y) : min(miny, segment.a.y, segment.b.y);
    maxx = first ? max(segment.a.x, segment.b.x) : max(maxx, segment.a.x, segment.b.x);
    maxy = first ? max(segment.a.y, segment.b.y) : max(maxy, segment.a.y, segment.b.y);
    first = false;
    
    bool aExists = false;
    bool bExists = false;
    for(std::set<float>::iterator j = endpoints.begin(); j != endpoints.end(); ++j)
    {
      if(*j == segment.a.x)
      {
        aExists = true;
      }
      if(*j == segment.b.x)
      {
        bExists = true;
      }
      if(aExists && bExists)
      {
        break;
      }
    }
    
    if(!aExists)
    {
      endpoints.insert(segment.a.x);
    }
    if(!bExists)
    {
      endpoints.insert(segment.b.x);
    }
  }
  
  rect = Rect(minx, miny, maxx - minx, maxy - miny);
  
  std::list<Node*> nodes;
  for(std::set<float>::iterator i = endpoints.begin(); i != endpoints.end(); ++i)
  {
    nodes.push_back(new Node(Interval(*i, *i)));
    std::set<float>::iterator next(i);
    ++next;
    if(next != endpoints.end())
    {
      nodes.push_back(new Node(Interval(Interval::Limit(*i, false), Interval::Limit(*next, false))));
    }
  }
  
  while(nodes.size() > 1)
  {
    std::list<Node*> nextNodes;
    std::list<Node*>::iterator i;
    for(i = nodes.begin(); i != nodes.end(); ++i)
    {
      std::list<Node*>::iterator next(i);
      if(++next == nodes.end())
      {
        break;
      }
      Interval interval((*i)->interval.lower, (*next)->interval.upper);
      nextNodes.push_back(new Node(interval, *i, *next));
      ++i;
    }
    
    if(i != nodes.end()) {
      nextNodes.push_back(*i);
    }
    
    nodes.swap(nextNodes);
  }
  
  root = nodes.front();

  for(std::list<Segment>::iterator i = segments.begin(); i != segments.end(); ++i) {
    insertToTree(*i, root);
  }
}

void SegmentTree::deconstruct()
{
  if(!root)
  {
    return;
  }
  
  std::set<Node*> nodes;
  nodes.insert(root);
  
  while(nodes.size() > 0)
  {
    std::set<Node*> temp;
    for(std::set<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
      Node* n = *i;
      if(n->left)
      {
        temp.insert(n->left);
      }
      if(n->right)
      {
        temp.insert(n->right);
      }
      
      delete n;
    }
    nodes.swap(temp);
  }
  
  root = 0;
}

void SegmentTree::print()
{
  if(!root)
  {
    return;
  }
  
  std::set<Node*> nodes;
  nodes.insert(root);
  
  while(nodes.size() > 0)
  {
    std::set<Node*> temp;
    for(std::set<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
      Node* n = *i;
      if(n->left)
      {
        temp.insert(n->left);
      }
      if(n->right)
      {
        temp.insert(n->right);
      }
      
      Interval in = n->interval;
      Log::debug() << (in.lower.closed ? "[" : "(") << in.lower.value << ", " << in.upper.value << (in.upper.closed ? "]" : ")");
    }
    Log::debug() << "---";
    nodes.swap(temp);
  }
}

void SegmentTree::insertToTree(Segment const& segment, Node* node)
{
  Interval interval(min(segment.a.x, segment.b.x), max(segment.a.x, segment.b.x));
  if(interval.contains(node->interval))
  {
    node->segments.push_back(&segment);
  }
  else
  {
    if(node->left != 0 && interval.intersects(node->left->interval))
    {
      insertToTree(segment, node->left);
    }
    if(node->right != 0 && interval.intersects(node->right->interval))
    {
      insertToTree(segment, node->right);
    }
  }
}

bool SegmentTree::queryNode(Vec2D const& point, ResultHandler& handler, Node* node) const
{
  if(!node)
  {
    return false;
  }
  
  for(std::list<Segment const*>::iterator i = node->segments.begin(); i != node->segments.end(); ++i)
  {
    Segment const& s = **i;
    if(min(s.a.y, s.b.y) <= point.y && max(s.a.y, s.b.y) >= point.y)
    {
      if(handler.handle(s))
      {
        return true;
      }
    }
  }
  
  if(node->left && node->left->interval.contains(point.x))
  {
    return queryNode(point, handler, node->left);
  }
  else if(node->right)
  {
    return queryNode(point, handler, node->right);
  }
  
  return false;
}

bool SegmentTree::queryNode(Segment const& segment, ResultHandler& handler, Node* node) const
{
  if(!node)
  {
    return false;
  }

  Interval yInterval(min(segment.a.y, segment.b.y), max(segment.a.y, segment.b.y));
  for(std::list<Segment const*>::iterator i = node->segments.begin(); i != node->segments.end(); ++i)
  {
    Segment const& s = **i;
    Interval syInterval(min(s.a.y, s.b.y), max(s.a.y, s.b.y));
    if(syInterval.intersects(yInterval))
    {
      if(handler.handle(s))
      {
        return true;
      }
    }
  }
  
  Interval interval(min(segment.a.x, segment.b.x), max(segment.a.x, segment.b.x));
  if(node->left && node->left->interval.intersects(interval))
  {
    if(queryNode(segment, handler, node->left))
    {
      return true;
    }
  }
  if(node->right && node->right->interval.intersects(interval))
  {
    if(queryNode(segment, handler, node->right))
    {
      return true;
    }
  }
  
  return false;
}
  
SegmentTree::DefaultResultHandler::DefaultResultHandler() : results(new SegmentList)
{
}

bool SegmentTree::DefaultResultHandler::handle(Segment const& segment)
{
  results->push_back(&segment);
  return false;
}

SegmentTree::SegmentResults SegmentTree::DefaultResultHandler::getResults()
{
  return results;
}
