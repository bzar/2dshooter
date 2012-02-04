function Rect(x, y, w, h) {
  this.xMin = x;
  this.xMax = x + w;
  this.yMin = y;
  this.yMax = y + h;
}

Rect.prototype.extend = function(amount) {
  var result = new Rect(0, 0, 0, 0);
  if(amount.x >= 0) {
    result.xMin = this.xMin;
    result.xMax = this.xMax + amount.x;
  } else {
    result.xMin = this.xMin + amount.x;
    result.xMax = this.xMax;
  }

  if(amount.y >= 0) {
    result.yMin = this.yMin;
    result.yMax = this.yMax + amount.y;
  } else {
    result.yMin = this.yMin + amount.y;
    result.yMax = this.yMax;
  }
  return result;
}

Rect.prototype.intersectsWith = function(other) {
  return this.xMin <= other.xMax 
      && this.xMax >= other.xMin 
      && this.yMin <= other.yMax 
      && this.yMax >= other.yMin;
}


function Path(vectors) {
  this.segments = [];
  var pos = new Vec2D(0, 0);
  this.totalLength = 0;
  for(var i = 0; i < vectors.length; ++i) {
    var nextPos = pos.add(vectors[i]);
    var segment = new Segment(pos, nextPos);
    segment.lMin = this.totalLength;
    this.totalLength += vectors[i].length();
    segment.lMax = this.totalLength;
    segment.dir = segment.delta().uniti();
    this.segments.push(segment);
    pos = nextPos;
  }
}

Path.prototype.convert = function(length) {
  if(length > this.totalLength) {
    return length - Math.floor(length/this.totalLength) * this.totalLength;
  } else {
    return length;
  }
}
Path.prototype.positionAt = function(length) {
  var l = this.convert(length);
  var i = 0; 
  var s = this.segments[i];
  while(s.lMin > l || s.lMax < l) {
    s = this.segments[++i];
  }

  return s.a.add(s.dir.scale(l - s.lMin));
}


function drawArrow(ctx, segment, arrowTipWidth, arrowTipLength) {
    if(arrowTipWidth == undefined) arrowTipWidth = 5.0;
    if(arrowTipLength == undefined) arrowTipLength = 7.0;

    ctx.beginPath();
    ctx.moveTo(segment.a.x, segment.a.y);
    ctx.lineTo(segment.b.x, segment.b.y);
    ctx.stroke();
    ctx.closePath();

    var delta = segment.delta();
    var arrowLength = delta.length();
    var normal = delta.normal().uniti();

    var tbase = segment.a.add(delta.scale(1 - arrowTipLength/arrowLength));
    var t1 = tbase.add(normal.scale(arrowTipWidth));
    var t2 = tbase.subtract(normal.scale(arrowTipWidth));

    ctx.beginPath();
    ctx.moveTo(segment.b.x, segment.b.y);
    ctx.lineTo(t1.x, t1.y);
    ctx.lineTo(t2.x, t2.y);
    ctx.moveTo(segment.b.x, segment.b.y);
    ctx.fill();
    ctx.closePath();
};
