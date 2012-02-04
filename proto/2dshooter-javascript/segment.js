function Segment(a, b) {
  this.a = a;
  this.b = b;
}

Segment.prototype.delta = function() {
  return this.b.subtract(this.a);
}

Segment.prototype.intersects = function(other) {
    var selfDelta = this.delta();
    var otherDelta = other.delta();
    
    if(selfDelta.crossProduct(otherDelta) != 0) {
        var t = other.a.subtract(this.a).crossProduct(otherDelta) / selfDelta.crossProduct(otherDelta);
        var u = other.a.subtract(this.a).crossProduct(selfDelta) / selfDelta.crossProduct(otherDelta);
        if(t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            return true;
        }
    }

    return false;
}

Segment.prototype.intersectionPoint = function(other) {
    var selfDelta = this.delta();
    var otherDelta = other.delta();
    
    if(selfDelta.crossProduct(otherDelta) != 0) {
        var t = other.a.subtract(this.a).crossProduct(otherDelta) / selfDelta.crossProduct(otherDelta);
        var u = other.a.subtract(this.a).crossProduct(selfDelta) / selfDelta.crossProduct(otherDelta);
        if(t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            return this.a.add(selfDelta.scale(t));
        }
    }

    return null;
}

Segment.prototype.toString = function() {
  return "Segment: " + this.a.toString() + "-" + this.b.toString();
}



function SegmentTree(segments) {
  this.segments = segments ? segments : [];
  this.root = null;
  this.min = {x: null, y: null};
  this.max = {x: null, y: null};
}

function Limit(value, open) {
  this.value = value;
  this.open = open !== undefined ? open : false;
}

function Interval(lower, upper) {
  this.lower = lower;
  this.upper = upper;
}

Interval.prototype.contains = function(value) {
  return ((this.lower.open && this.lower.value < value) || 
          (!this.lower.open && this.lower.value <= value)) &&
         ((this.upper.open && this.upper.value > value) || 
          (!this.upper.open && this.upper.value >= value));
}

Interval.prototype.toString = function() {
  return "Interval: " + (this.lower.open ? "(" : "[") + this.lower.value + ", " + this.upper.value + (this.upper.open ? ")" : "]")
}

Interval.prototype.containsInterval = function(other) {
  if(this.lower.value > other.lower.value || this.upper.value < other.upper.value) return false;
  if(this.lower.open && !other.lower.open && this.lower.value == other.lower.value) return false;
  if(this.upper.open && !other.upper.open && this.upper.value == other.upper.value) return false;
  return true;
}

Interval.prototype.intersectsInterval = function(other) {
  if(this.lower.value > other.upper.value || this.upper.value < other.lower.value) return false;
  if((this.lower.open || other.upper.open) && this.lower.value == other.upper.value) return false;
  if((this.upper.open || other.lower.open) && this.upper.value == other.lower.value) return false;
  return true;
}


SegmentTree.prototype.Node = function(left, right) {
  this.left = left;
  this.right = right;
  this.interval = new Interval(this.left.interval.lower, this.right.interval.upper);
  this.segments = [];
}

SegmentTree.prototype.Leaf = function(interval) {
  this.interval = interval;
  this.segments = [];
}

SegmentTree.prototype.add = function(segment) {
  this.segments.push(segment);
}

SegmentTree.prototype.construct = function() {
  // Sort endpoints and add as leaves
  var endpoints = []
  for(var i = 0; i < this.segments.length; ++i) {
    var segment = this.segments[i];
    this.min.x = Math.min(this.min.x, segment.a.x, segment.b.x);
    this.min.y = Math.min(this.min.y, segment.a.y, segment.b.y);
    this.max.x = Math.max(this.max.x, segment.a.x, segment.b.x);
    this.max.y = Math.max(this.max.y, segment.a.y, segment.b.y);

    if(endpoints.filter(function(x) { return x.interval.lower.value == segment.a.x; }).length == 0) {
      var limit = new Limit(segment.a.x);
      endpoints.push(new this.Leaf(new Interval(limit, limit)));
    }
    if(endpoints.filter(function(x) { return x.interval.lower.value == segment.b.x; }).length == 0) {
      var limit = new Limit(segment.b.x);
      endpoints.push(new this.Leaf(new Interval(limit, limit)));
    }
  }
  endpoints.sort(function(a, b) { return a.interval.lower.value - b.interval.lower.value; });
  
  var nodes = [];
  for(var i = 0; i < endpoints.length; ++i) {
    var curr = endpoints[i];
    var next = endpoints[i+1];
    nodes.push(curr);
    if(next !== undefined) {
      var interval = new Interval(new Limit(curr.interval.upper.value, true), new Limit(next.interval.lower.value, true));
      nodes.push(new this.Leaf(interval));
    }
  }
  
  // Construct binary tree
  while(nodes.length > 1) {
    var nextNodes = [];
    
    for(var i = 1; i < nodes.length; i = i + 2) {
      nextNodes.push(new this.Node(nodes[i-1], nodes[i]))
    }
    
    if(nodes.length != 1 && nodes.length % 2 == 1) {
      nextNodes.push(nodes[nodes.length - 1]);
    }
    
    nodes = nextNodes;
  }
  
  this.root = nodes[0];
  
  // Add segments to the tree
  function insertToTree(segment, node) {
    var min = Math.min(segment.a.x, segment.b.x);
    var max = Math.max(segment.a.x, segment.b.x);
    var interval = new Interval(new Limit(min), new Limit(max));
    if(interval.containsInterval(node.interval)) {
      node.segments.push(segment);
    } else {
      if(interval.intersectsInterval(node.left.interval)) {
        insertToTree(segment, node.left);
      }
      if(interval.intersectsInterval(node.right.interval)) {
        insertToTree(segment, node.right);
      }
    }
  }
  
  for(var i = 0; i < this.segments.length; ++i) {
    this.segments[i].index = i;
    insertToTree(this.segments[i], this.root);
  }
}

SegmentTree.prototype.queryPoint = function(point) {
  if(this.segments.length == 0) return [];
  
  function query(point, node) {
    var result = node.segments;
    if(node.left !== undefined) {
      if(node.left.interval.contains(point.x)) {
        result = result.concat(query(point, node.left));
      } else {
        result = result.concat(query(point, node.right));
      }
    }
    return result;
  }

  var result = query(point, this.root).filter(function(s) { 
    return Math.min(s.a.y, s.b.y) <= point.y && Math.max(s.a.y, s.b.y) >= point.y;     
  });
  
  return result;
}

SegmentTree.prototype.querySegment = function(segment) {
  if(this.segments.length == 0) return [];
    
  function query(segment, node) {
    var result = node.segments;
    var min = Math.min(segment.a.x, segment.b.x);
    var max = Math.max(segment.a.x, segment.b.x);
    var interval = new Interval(new Limit(min), new Limit(max));
    if(node.left !== undefined) {
      if(interval.intersectsInterval(node.left.interval)) {
        var childResult = query(segment, node.left);
        if(childResult.length > 0)
          result = result.concat(childResult);
      } 
      if(interval.intersectsInterval(node.right.interval)) {
        var childResult = query(segment, node.right);
        if(childResult.length > 0)
          result = result.concat(childResult);
      }
    }
    
    return result;
  }
  
  var result = query(segment, this.root).filter(function(s) { 
    return Math.min(s.a.y, s.b.y) < Math.max(segment.a.y, segment.a.y) ||
           Math.max(s.a.y, s.b.y) > Math.min(segment.a.y, segment.a.y);
  });
  
  return result;
}

SegmentTree.prototype.querySegmentForEach = function(segment, callback) {
  if(this.segments.length == 0) return [];
  
  function query(segment, node) {
    for(var i = 0; i < node.segments.length; ++i) {
      var s = node.segments[i];
      if(Math.min(s.a.y, s.b.y) < Math.max(segment.a.y, segment.a.y) ||
         Math.max(s.a.y, s.b.y) > Math.min(segment.a.y, segment.a.y)) {
        if(callback(s))
          return true;
      }
    };
    
    var min = Math.min(segment.a.x, segment.b.x);
    var max = Math.max(segment.a.x, segment.b.x);
    var interval = new Interval(new Limit(min), new Limit(max));
    if(node.left !== undefined) {
      if(interval.intersectsInterval(node.left.interval)) {
        if(query(segment, node.left))
          return true;
      } 
      if(interval.intersectsInterval(node.right.interval)) {
        if(query(segment, node.right))
          return true;
      }
    }

    return false;
  }
  
  query(segment, this.root);
}
