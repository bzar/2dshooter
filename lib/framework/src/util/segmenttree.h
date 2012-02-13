#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "segment.h"
#include "interval.h"
#include "rect.h"
#include <list>
#include <memory>

class SegmentTree {
public:
  typedef std::list<Segment const*> SegmentList;
  typedef std::auto_ptr< SegmentList > SegmentResults;
  
  class ResultHandler
  {
  public:
    virtual bool handle(Segment const& segment) = 0;
  };
  
  class DefaultResultHandler : public ResultHandler
  {
  public:
    DefaultResultHandler();
    bool handle(Segment const& segment);
    SegmentResults getResults();
  private:
    SegmentResults results;
  };  

  SegmentTree(std::list<Segment> const& segments);
  ~SegmentTree();
  SegmentTree& operator=(SegmentTree const& other);
  void setSegments(std::list<Segment> const& newSegments);
  SegmentResults const query(Vec2D const& point) const;
  SegmentResults const query(Segment const& segment) const;
  bool query(Vec2D const& point, ResultHandler& handler) const;
  bool query(Segment const& segment, ResultHandler& handler) const;
  
private:
  struct Node
  {
    Node(Interval interval, Node* left = 0, Node* right = 0) : left(left), right(right), interval(interval), segments() {}
    bool operator<(Node const& other) { return interval.lower.value < other.interval.lower.value; }
    Node* left;
    Node* right;
    Interval interval;
    std::list<Segment const*> segments;
  };
  
  void construct();
  void deconstruct();
  void print();
  void insertToTree(Segment const& segment, Node* node);
  bool queryNode(Vec2D const& point, ResultHandler& handler, Node* node) const;
  bool queryNode(Segment const& segment, ResultHandler& handler, Node* node) const;
  
  Node* root;
  std::list<Segment> segments;
  Rect rect;
  
  
};

#endif
