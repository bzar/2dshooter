#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "segment.h"
#include "interval.h"
#include "rect.h"
#include <list>
#include <memory>
#include <functional>

class SegmentTree {
public:
  typedef std::list<Segment> SegmentList;
  typedef std::function<bool(Segment const&)> ResultHandler;

  SegmentTree(std::list<Segment> const& segments);
  ~SegmentTree();
  SegmentTree& operator=(SegmentTree const& other);
  void setSegments(std::list<Segment> const& newSegments);
  SegmentList const query(Vec2D const& point) const;
  SegmentList const query(Segment const& segment) const;
  bool query(Vec2D const& point, ResultHandler const& handler) const;
  bool query(Segment const& segment, ResultHandler const& handler) const;

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
  bool queryNode(Vec2D const& point, ResultHandler const& handler, Node* node) const;
  bool queryNode(Segment const& segment, ResultHandler const& handler, Node* node) const;

  Node* root;
  std::list<Segment> segments;
  Rect rect;
};

#endif
