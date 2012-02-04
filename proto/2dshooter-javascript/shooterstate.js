function ShooterState() {
  var gridSize = 10;
  var n = 0;
  var terrainSegments = null;
  var background = null;
  var doubleBuffer = null;
  var dbCtx = null;
  var bgCtx = null;
  var backgroundChanged = true;
  var pause = false;
  
  var newSegment = null;  
  var player = null;
  var world = null;

  var bulletCooldown = 0;
  var bulletsPerSecond = 8;
  var terrainSegments = null;
  
  var currUpdateRect = null;
  var prevUpdateRect = null;
  var level = {
    playerStart: {x: 150, y: 50},
    gravity: {x: 0, y: 0.4},
    segmentGroups: [
      { name: "terrain", position: null, path: null, speed: null, 
        segments: [
          {a: {x: 100, y: 100}, b:{x: 200, y: 100}}, {a: {x: 200, y: 100}, b: {x: 200, y: 200}}, 
          {a: {x: 200, y: 200}, b:{x: 100, y: 200}}, {a: {x: 100, y: 200}, b: {x: 100, y: 100}}
        ]
      },
      { position: {x: 200, y: 100}, path: [{x:100}, {y:100}, {x:-100}, {y: -100}], speed: 1.0, 
        segments: [
          {a: {x: 0, y: 0}, b:{x: 100, y: 0}}, {a: {x: 100, y: 0}, b: {x: 100, y: 100}}, 
          {a: {x: 100, y: 100}, b:{x: 0, y: 100}}, {a: {x: 0, y: 100}, b: {x: 0, y: 0}}
        ]
      },
    ]
  };
  var addTerrainSegments = function(segments) {
    terrainSegments.segments.segments = terrainSegments.segments.segments.concat(segments);
    terrainSegments.segments.construct();
  }
  
  var undoTerrainSegment = function(segments) {
    terrainSegments.segments.segments.pop();
    terrainSegments.segments.construct();
  }
  this.loadLevel = function(level) {
    var namedEntities = {};
    world = new World(new Vec2D(level.gravity.x, level.gravity.y));
    
    for(var i = 0; i < level.segmentGroups.length; ++i) {
      var sgInfo = level.segmentGroups[i];
      var segments = [];
      for(var j = 0; j < sgInfo.segments.length; ++j) {
        var s = sgInfo.segments[j];
        segments.push(new Segment(new Vec2D(s.a.x, s.a.y), new Vec2D(s.b.x, s.b.y)));
      }
      var path = null;
      if(sgInfo.path !== null) {
        path = [];
        for(var j = 0; j < sgInfo.path.length; ++j) {
          var p = sgInfo.path[j];
          path.push(new Vec2D(p.x ? p.x : 0, p.y ? p.y : 0));
        }
      }
      var position = sgInfo.position === null ? null : new Vec2D(sgInfo.position.x, sgInfo.position.y)
      var sg = world.add(new SegmentGroup(position, segments, path, sgInfo.speed));
      if(sgInfo.name)
        namedEntities[sgInfo.name] = sg;
    }
    
    namedEntities.player = world.add(new Player(new Vec2D(level.playerStart.x, level.playerStart.y), new Vec2D(0, 0)));
    
    return namedEntities;
  }
  
  
  function rifle(player, world, direction) {
    var bulletPosition = player.position.add(new Vec2D(0, -3));
    var bulletVelocity = direction.add(direction.normal().scalei(0.1*(Math.random()-0.5))).scalei(20);
    world.add(new BulletParticle(bulletPosition, bulletVelocity, 100));
    return 8;
  }
  
  function shotgun(player, world, direction) {
    var bulletPosition = player.position.add(new Vec2D(0, -3));
    
    for(var i = 0; i < 10; ++i) {
      var bulletVelocity = direction.add(direction.normal().scalei(0.2*(Math.random()-0.5))).scalei(Math.random()*5 + 15);
      world.add(new BulletParticle(bulletPosition, bulletVelocity, 20));
    }
    return 40;
  }
  
  var weapon = shotgun;
  
  this.enter = function() {
    this.reset();
    doubleBuffer = document.createElement("canvas");
    background = document.createElement("canvas");
    doubleBuffer.width = this.engine.canvas.width;
    doubleBuffer.height = this.engine.canvas.height;
    background.width = this.engine.canvas.width;
    background.height = this.engine.canvas.height;
    dbCtx = doubleBuffer.getContext("2d");
    bgCtx = background.getContext("2d");
  };
  
  this.reset = function() {
    var namedEntities = this.loadLevel(level);
    terrainSegments = namedEntities.terrain;
    player = namedEntities.player;
    backgroundChanged = true;    
  }

  this.exit = function() {
    
  };

  this.clear = function(ctx) {
    dbCtx.drawImage(background, 0, 0);
  };
  
  this.update = function(ctx) {
    if(mouse.buttons.left && newSegment === null) {
      newSegment = new Segment(new Vec2D(gridSize*Math.round(mouse.x/gridSize), 
                                         gridSize*Math.round(mouse.y/gridSize)), 
                               new Vec2D(gridSize*Math.round(mouse.x/gridSize), 
                                         gridSize*Math.round(mouse.y/gridSize)));
    } else if(mouse.buttons.left && newSegment !== null) {
      newSegment.b.x = gridSize*Math.round(mouse.x/gridSize);
      newSegment.b.y = gridSize*Math.round(mouse.y/gridSize);
    } else {
      if(newSegment !== null) {
        addTerrainSegments([newSegment]);
        backgroundChanged = true;
      }
      newSegment = null;
    }
  
    debug = keys.m;
    
    if(keys.p) {
      keys.p = false;
      pause = !pause;
    }
    
    if(keys.t) {
      player.position.x = mouse.x;
      player.position.y = mouse.y;
      player.velocity.x = 0;
      player.velocity.y = 0;
    }
    
    if(keys.r) {   
      keys.r = false;
      this.reset();
    }
    if(keys.f) {
      keys.f = false;
      world.add(new Enemy(new Vec2D(mouse.x, mouse.y), new Vec2D(-1, -1)));
    }
    if(keys.i) {
      keys.i = false;
      var path = [new Vec2D(0, -100), new Vec2D(0, 100)];
      var position = new Vec2D(gridSize*Math.round(mouse.x/gridSize), gridSize*Math.round(mouse.y/gridSize));
      var segments = [
        new Segment(new Vec2D(-25, -5), new Vec2D(25, -5)),
        new Segment(new Vec2D(25, -5), new Vec2D(25, 5)),
        new Segment(new Vec2D(25, 5), new Vec2D(-25, 5)),
        new Segment(new Vec2D(-25, 5), new Vec2D(-25, -5))
      ];

      var movingSegmentGroup = world.add(new SegmentGroup(position, segments, path, 0.5));
    }
  
    if(keys.k) {
      keys.k = false;
      var path = [new Vec2D(0, -100), new Vec2D(0, 100)];
      var position = new Vec2D(gridSize*Math.round(mouse.x/gridSize), gridSize*Math.round(mouse.y/gridSize));
      var segments = [
        new Segment(new Vec2D(-25, -5), new Vec2D(25, -5)),
        new Segment(new Vec2D(25, -5), new Vec2D(25, 5)),
        new Segment(new Vec2D(25, 5), new Vec2D(-25, 5)),
        new Segment(new Vec2D(-25, 5), new Vec2D(-25, -5))
      ];

      var movingSegmentGroup = world.add(new SegmentGroup(position, segments, path, 2));
    }
    if(keys.l) {
      keys.l = false;
      var path = [new Vec2D(-100, 0), new Vec2D(100, 0)];
      var position = new Vec2D(gridSize*Math.round(mouse.x/gridSize), gridSize*Math.round(mouse.y/gridSize));
      var segments = [
        new Segment(new Vec2D(-5, -25), new Vec2D(5, -25)),
        new Segment(new Vec2D(5, -25), new Vec2D(5, 25)),
        new Segment(new Vec2D(5, 25), new Vec2D(-5, 25)),
        new Segment(new Vec2D(-5, 25), new Vec2D(-5, -25))
      ];

      var movingSegmentGroup = world.add(new SegmentGroup(position, segments, path, 0.5));
    }
  
    
    if(keys.n) {
      keys.n = false;
      var enemies = world.getEntityGroup("enemies").concat([]);
      for(var i = 0; i < enemies.length; ++i) {
        enemies[i].explode();
      }
    }
    
    if(keys.backspace) {
      keys.backspace = false;
      undoTerrainSegment();
      backgroundChanged = true;
    }
    
    if(!pause) {
      if((keys.space || keys.o) && bulletCooldown <= 0) {
        var direction = null;
        if(keys.space)
          direction = (new Vec2D(mouse.x, mouse.y)).subtracti(player.position).uniti();
        else
          direction = new Vec2D(player.facingRight ? 1 : -1 , 0);

        bulletCooldown += weapon(player, world, direction);
      }
      
      if(keys[1])
        weapon = rifle;
      if(keys[2])
        weapon = shotgun;
      
      if(bulletCooldown > 0) {
        --bulletCooldown;
      } else if(bulletCooldown < 0) {
        bulletCooldown = 0;
      }
      
      
      if((keys.down || keys.s) && player.onGround) {
        world.gravity.negi();
      }
      
      var entities = world.getEntityGroup("all");
      for(var i = 0; i < entities.length; ++i) {
        entities[i].intent();
      }
      for(var i = 0; i < entities.length; ++i) {
        entities[i].reaction();
      }
      for(var i = 0; i < entities.length; ++i) {
        entities[i].update();
      }
    }
  };
  
  this.draw = function(ctx) {
    if(backgroundChanged) {
      bgCtx.save();
      bgCtx.fillStyle = "#111";
      bgCtx.fillRect(0, 0, background.width, background.height);
      bgCtx.restore();

      bgCtx.save();
      bgCtx.strokeStyle = "#030";
      bgCtx.beginPath();

      for(var y = 1; y < Math.floor(background.height/gridSize); ++y) {
          bgCtx.moveTo(0, y*gridSize);
          bgCtx.lineTo(background.width, y*gridSize);        
      }
      
      for(var x = 1; x < Math.floor(background.width/gridSize); ++x) {
          bgCtx.moveTo(x*gridSize, 0);
          bgCtx.lineTo(x*gridSize, background.height);                
      }
      bgCtx.stroke();
      bgCtx.closePath();
      bgCtx.restore();
      
      bgCtx.save();
      bgCtx.fillStyle = "#181";
      bgCtx.strokeStyle = "#181";
      for(var i = 0; i < terrainSegments.segments.segments.length; ++i) {
        var ts = terrainSegments.segments.segments[i];
        drawArrow(bgCtx, ts, 5, 5)
      }
      bgCtx.restore();
      
      dbCtx.drawImage(background, 0, 0);
      ctx.drawImage(doubleBuffer, 0, 0);
      backgroundChanged = false;
    }
    
    dbCtx.save();
    dbCtx.fillStyle = "#3a3";
    dbCtx.strokeStyle = "#3a3";
    if(newSegment !== null) {
      drawArrow(dbCtx, newSegment, 5, 5)
    }
    dbCtx.restore();
    
    var entities = world.getEntityGroup("all");
    for(var i = 0; i < entities.length; ++i) {
      var entity = entities[i];
      if(entity !== terrainSegments)
        entity.draw(dbCtx);
    }
    
    ctx.drawImage(doubleBuffer, 0, 0);
  };
}
ShooterState.prototype = new State();
