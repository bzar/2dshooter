function World(gravity) {
  this.gravity = gravity;
  this.entities = {all: [], segmentGroups: []};
}

World.prototype.add = function(entity) {
  entity.world = this;
  this.getEntityGroup("all").push(entity);
  for(var i = 0; i < entity.groups.length; ++i) {
    var group = this.getEntityGroup(entity.groups[i]);
    group.push(entity);
  }
  return entity;
}

World.prototype.remove = function(entity, groupName) {
  if(groupName === undefined) {
    this.remove(entity, "all");
    for(var i = 0; i < entity.groups.length; ++i) {
      this.remove(entity, entity.groups[i]);
    }
  } else {
    var group = this.entities[groupName];
    for(var i = 0; i < group.length; ++i) {
      if(group[i] === entity) {
        group.splice(i, 1);
        break;
      }
    }
  }
}

World.prototype.getEntityGroup = function(groupName) {
  var group = this.entities[groupName];
  if(group === undefined) {
    group = [];
    this.entities[groupName] = group;
  }  
  return group;
}

World.prototype.collideTerrain = function(entity, callback) {
  var collisions = true;
  var stop = false
  
  var relativeMotion = new Segment(new Vec2D(0, 0), new Vec2D(0, 0));
  
  var n = 0;
  var segmentGroups = this.entities.segmentGroups;
  while(collisions && !stop && ++n < 50) {
    collisions = false;
    
    for(var i = 0; i < segmentGroups.length && !collisions && !stop; ++i) {
      var sg = segmentGroups[i];
      
      var relativePosition = sg.position === null ? entity.position : entity.position.subtract(sg.position);
      var relativeVelocity = sg.velocity === null ? entity.velocity : entity.velocity.subtract(sg.velocity);
      relativeMotion.a = relativePosition;
      relativeMotion.b = relativePosition.add(relativeVelocity);
      
      sg.segments.querySegmentForEach(relativeMotion, function(segment) {
        var relativeIntersectionPoint = segment.intersectionPoint(relativeMotion);
        if(relativeIntersectionPoint !== null) {
          var segmentDelta = segment.delta();
          if(segmentDelta.crossProduct(relativeVelocity) > 0) {
            collisions = true;
            
            var absoluteSegment = sg.position === null ? segment : new Segment(segment.a.add(sg.position), segment.b.add(sg.position));
            var absoluteIntersectionPoint = sg.position === null ? relativeIntersectionPoint : relativeIntersectionPoint.add(sg.position);
            var segmentNormal = segmentDelta.normal().uniti();
            
            stop = callback(absoluteSegment, absoluteIntersectionPoint, segmentNormal, sg);

            return true;
          }
        }
        return false;
      });
      
    }
  }
  
  if(n == 50)
      console.log("infinite loop detected (point)");
  
  return stop;
}


function SquishAnalyzer() {
  this.segments = [];
  this.addSegment = function(segment) {
    var ts = segment.delta();
    var squished = false;
    for(var i = 0; i < this.segments.length; ++i) {
      var previousSegment = this.segments[i];
      if(ts.dotProduct(previousSegment) < 0 && ts.unit().dotProduct(previousSegment.unit()) < -0.75) {
        squished = true;
        break;
      }
    }
    this.segments.push(ts);
    return squished;
  }
}

  
function Entity(position, velocity) {
  this.position = position ? position.copy() : position;
  this.velocity = velocity ? velocity.copy() : velocity;
  this.collideTerrain = true;
}

Entity.prototype.groups = []
Entity.prototype.intent = function() {};
Entity.prototype.reaction = function() {
  if(this.collideTerrain) {
    var this_ = this;
    var squishAnalyzer = new SquishAnalyzer();
    this.world.collideTerrain(this, function(segment, collisionPoint, segmentNormal, segmentGroup) {
      if(squishAnalyzer.addSegment(segment)) {
        this_.squish();
        return true;
      }
      return this_.terrainCollision(segment, collisionPoint, segmentNormal, segmentGroup);
    });
  } 
};
Entity.prototype.update = function() {
  this.position.addi(this.velocity);
};

Entity.prototype.delete = function() { this.world.remove(this); };
Entity.prototype.terrainCollision = function() {};
Entity.prototype.squish = function() {};
Entity.prototype.draw = function(ctx) {};
Entity.prototype.rect = function() {
  return new Rect(this.position.x, this.position.y, 0, 0);
}

  
function MultiPointEntity(points, position, velocity) {
  Entity.call(this, position, velocity);
  this.points = points;
}
MultiPointEntity.prototype = new Entity();

MultiPointEntity.prototype.reaction = function() {
  if(this.collideTerrain) {
    var this_ = this;
    var zeroPos = this.position;
    var collisions = true;
    var n = 0;
    var stop = false;
    var squishAnalyzer = new SquishAnalyzer();
    
    while(collisions && !stop && ++n < 50) {
      collisions = false;
      for(var i = 0; i < this.points.length && !collisions && !stop; ++i) {
        this.position = zeroPos.add(this.points[i]);
        stop = this.world.collideTerrain(this, function(segment, collisionPoint, segmentNormal, segmentGroup) {
          collisions = true;
          if(squishAnalyzer.addSegment(segment)) {
            this_.squish();
            return true;
          }
          return this_.terrainCollision(this_.points[i], segment, collisionPoint, segmentNormal, segmentGroup);
        });
      }
    }
    if(n == 50)
      console.log("infinite loop detected (multipoint)");
    this.position = zeroPos;
  } 
};
  
function Actor(points, position, velocity) {
  MultiPointEntity.call(this, points, position, velocity);
  this.onGround = false;
  this.reactionSum = new Vec2D(0, 0);
}

Actor.prototype = new MultiPointEntity();
this.Actor = Actor;

Actor.prototype.reaction = function() {
  this.onGround = false;
  MultiPointEntity.prototype.reaction.call(this);
};

Actor.prototype.terrainCollision = function(point, segment, collisionPoint, segmentNormal, segmentGroup) {
  var tox = collisionPoint.subtract(this.position);
  var rest = this.position.add(this.velocity).subtract(collisionPoint);
  this.velocity.assign(tox.addi(rest.projectioni(segment.delta()))).addi(segmentNormal.scale(0.1));
  if(segmentGroup.velocity !== null) {
    this.velocity.addi(segmentGroup.velocity)
    this.reactionSum.addi(segmentGroup.velocity);
  }
  if(segmentNormal.dotProduct(this.world.gravity.unit()) < -0.5) {
    this.onGround = true;
  }
  
  return false;
}

Actor.prototype.update = function() {
  MultiPointEntity.prototype.update.call(this);
  this.velocity.subtracti(this.reactionSum);
  this.reactionSum.x = 0;
  this.reactionSum.y = 0;
};

function Player(position, velocity) {
  Actor.call(this, [new Vec2D(0, 0), new Vec2D(0, -5)], position, velocity);
  this.facingRight = true;
}
Player.prototype = new Actor();

Player.prototype.intent = function() {
  if((keys.left || keys.a) && this.velocity.x > -3) {
    this.velocity.x -= 0.2;
    this.facingRight = false;
  }
  
  if((keys.right || keys.d) && this.velocity.x < 3) {
    this.velocity.x += 0.2;
    this.facingRight = true;
  }
  
  if(!keys.left && !keys.right && !keys.a && !keys.d) {
    if(this.velocity.x < -0.5)
      this.velocity.x += 0.4;
    else if(this.velocity.x > 0.5)
      this.velocity.x -= 0.4;
    else
      this.velocity.x = 0;
  }

  if(this.onGround && (keys.up || keys.w)) {
    this.velocity.projectioni(this.world.gravity.normal());
    this.velocity.addi(this.world.gravity.scale(-20));
  }
  
  this.velocity.addi(this.world.gravity);
  
  Actor.prototype.intent.call(this);
}

Player.prototype.draw = function(ctx) {
  ctx.fillStyle = "#2d2";
  ctx.fillRect(this.position.x - 2, this.position.y - 5, 5, 5);
}
  
function Enemy(position, velocity) {
  Actor.call(this, [new Vec2D(0, 0), new Vec2D(0, -5)], position, velocity);
  this.moveLeft = true;
}
Enemy.prototype = new Actor();
Enemy.prototype.groups = ["enemies"]
Enemy.prototype.intent = function() {
  if(this.velocity.x < -0.1)
    this.moveLeft = true;
  else if(this.velocity.x > 0.1)
    this.moveLeft = false;
  else
    this.moveLeft = !this.moveLeft;
  
  if(this.moveLeft && this.velocity.x > -2) {
    this.velocity.x -= 0.2;
  } else if(!this.moveLeft && this.velocity.x < 2) {
    this.velocity.x += 0.2;
  }

  this.velocity.addi(this.world.gravity);
  
  Entity.prototype.intent.call(this);
}

Enemy.prototype.draw = function(ctx) {
  ctx.fillStyle = "#d22";
  ctx.fillRect(this.position.x - 2, this.position.y - 5, 5, 5);
}

Enemy.prototype.explode = function(direction) {
  direction = direction !== undefined ? direction : this.world.gravity.neg();
  var dirUnit = direction.unit();
  var dirUnitNormal = dirUnit.normal();
  for(var i = 0; i < 50; ++i) {
    this.world.add(new BloodParticle(this.position.copy(), dirUnitNormal.scale((Math.random()-0.5) * 6).addi(dirUnit.scale(Math.random()*8))));
  }
  this.delete();
}

Enemy.prototype.squish = function() {
  for(var i = 0; i < 20; ++i) {
    this.world.add(new BloodParticle(this.position.copy(), new Vec2D((Math.random()-0.5) * 8, (Math.random()-0.5) * 8)));
  }
  this.delete();
}

function Particle(position, velocity, life, applyGravity) {
  Entity.call(this, position, velocity);
  this.life = life;
  this.applyGravity = applyGravity;
}
Particle.prototype = new Entity();
Particle.prototype.groups = ["particles"];

Particle.prototype.intent = function() {
  if(this.applyGravity)
    this.velocity.addi(this.world.gravity);
  
  this.life -= 1;
  if(this.life <= 0) {
    this.delete();
  }
  
  Entity.prototype.intent.call(this);
}

function BloodParticle(position, velocity) {
  Particle.call(this, position, velocity, parseInt(Math.random()*50)+50, true);
  this.attachedTo = null;
}
BloodParticle.prototype = new Particle();

BloodParticle.prototype.intent = function() {
  Particle.prototype.intent.call(this);
  if(this.attachedTo !== null) {
    this.velocity.assign(this.attachedTo.velocity);
  }
}
BloodParticle.prototype.terrainCollision = function(segment, collisionPoint, segmentNormal, segmentGroup) {
  this.position = collisionPoint.add(segmentNormal.scale(0.1));
  if(segmentGroup === null) {
    this.velocity.x = 0;
    this.velocity.y = 0;
  } else {
    this.attachedTo = segmentGroup;
    this.velocity = this.attachedTo.velocity.copy();
  }
  this.applyGravity = false;
  this.collideTerrain = false;
  return true;
}

BloodParticle.prototype.draw = function(ctx) {
  ctx.fillStyle = "#f44";
  ctx.fillRect(this.position.x - 1, this.position.y - 1, 3, 3);
}

function BulletParticle(position, velocity, life) {
  Particle.call(this, position, velocity, life, false);
}
BulletParticle.prototype = new Particle();

BulletParticle.prototype.draw = function(ctx) {
  
  ctx.fillStyle = this.applyGravity ? "#555" : "#ccc";
  ctx.fillRect(this.position.x - 1, this.position.y - 1, 3, 3);
}

BulletParticle.prototype.terrainCollision = function(segment, collisionPoint, segmentNormal, segmentGroup) {
  this.position.assign(collisionPoint);
  this.velocity.subtracti(this.velocity.projection(segmentNormal).scale(1.5)).scalei(0.5);
  
  this.applyGravity = true;
  return true;
}

BulletParticle.prototype.reaction = function() {
  var enemyCenter = new Vec2D(-3, 0);
  if(!this.applyGravity) {
    var enemies = this.world.getEntityGroup("enemies");
    for(var i = 0; i < enemies.length; ++i) {
      if(enemies[i].position.add(enemyCenter).toLineSegment(this.position, this.position.add(this.velocity)).lengthSquared() <= 9) {
        enemies[i].explode(this.velocity);
        this.delete();
        break;
      }
    }
  }
  Particle.prototype.reaction.call(this);
}
  
  
  
function SegmentGroup(position, segments, movementPattern, speed) {
  Entity.call(this, position, new Vec2D(0, 0));
  this.zeroPosition = position === null ? null : position.copy();
  this.collideTerrain = false;
  this.segments = new SegmentTree(segments);
  this.segments.construct();
  this.path = movementPattern ? new Path(movementPattern) : null;
  this.speed = speed;
  this.progress = 0;
}

SegmentGroup.prototype = new Entity();
SegmentGroup.prototype.groups = ["segmentGroups"]

SegmentGroup.prototype.intent = function() {
  if(this.path !== null) {
    this.progress = this.path.convert(this.progress + this.speed);
    this.velocity = this.path.positionAt(this.progress).addi(this.zeroPosition).subtracti(this.position);
  }
  Entity.prototype.intent.call(this);
};

SegmentGroup.prototype.reaction = function() {};
SegmentGroup.prototype.update = function() {
  if(this.position !== null && this.velocity !== null) {
    Entity.prototype.update.call(this);
  }
};

SegmentGroup.prototype.draw = function(ctx) {
  ctx.strokeStyle = "#dd2";
  ctx.beginPath();
  var v1 = new Vec2D(0, 0);
  var v2 = new Vec2D(0, 0);
  for(var i = 0; i < this.segments.segments.length; ++i) {
    var s = this.segments.segments[i];
    v1.assign(s.a).addi(this.position);
    v2.assign(s.b).addi(this.position);
    ctx.moveTo(v1.x, v1.y);
    ctx.lineTo(v2.x, v2.y);
  }
  ctx.stroke();
  ctx.closePath();
};
  
SegmentGroup.prototype.rect = function() {
  if(this.position === null) {
    return null;
  } else {
    return new Rect(this.position.x + this.segments.min.x, this.position.y + this.segments.min.y, 
                    this.segments.max.x -  this.segments.min.x, this.segments.max.y -  this.segments.min.y);
  }
}
