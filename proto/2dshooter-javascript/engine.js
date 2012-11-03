var keys = {};

var KEY_CODES = {
  8: "backspace",
  32: "space",
  37: "left",
  38: "up",
  39: "right",
  40: "down",
  48: "0",
  49: "1",
  50: "2",
  51: "3",
  52: "4",
  53: "5",
  54: "6",
  55: "7",
  56: "8",
  57: "9",
  65: "a",
  66: "b",
  67: "c",
  68: "d",
  69: "e",
  70: "f",
  71: "g",
  72: "h",
  73: "i",
  74: "j",
  75: "k",
  76: "l",
  77: "m",
  78: "n",
  79: "o",
  80: "p",
  81: "q",
  82: "r",
  83: "s",
  84: "t",
  85: "u",
  86: "v",
  87: "w",
  88: "x",
  89: "y",
  90: "z"
};

var MOUSE_BUTTONS = ["left", "middle", "right"];
var mouse = {
  x: 0,
  y: 0,
  buttons: {
    left: false,
    middle: false,
    right: false
  }
};

document.onkeydown = function(e) {
  if(KEY_CODES.hasOwnProperty(e.keyCode)) {
    keys[KEY_CODES[e.keyCode]] = true;
  }
};

document.onkeyup = function(e) {
  if(KEY_CODES.hasOwnProperty(e.keyCode)) {
    keys[KEY_CODES[e.keyCode]] = false;
  }
};

function scheduleFrame(func) {
  if (window.requestAnimationFrame) window.requestAnimationFrame(func);
  else if (window.msRequestAnimationFrame) window.msRequestAnimationFrame(func);
  else if (window.mozRequestAnimationFrame) window.mozRequestAnimationFrame(func);
  else if (window.webkitRequestAnimationFrame) window.webkitRequestAnimationFrame(func);
  else alert("ERROR: requestAnimationFrame not supported!");
}

function getMousePos(canvas, evt){
  var obj = canvas;
  var top = 0;
  var left = 0;
  while (obj.tagName != "BODY") {
      top += obj.offsetTop;
      left += obj.offsetLeft;
      obj = obj.offsetParent;
  }

  // return relative mouse position
  var mouseX = evt.clientX - left + window.pageXOffset;
  var mouseY = evt.clientY - top + window.pageYOffset;
  return {x: mouseX, y: mouseY};
}

function Engine(canvasId) {
  this.canvas = document.getElementById(canvasId);
  var ctx = this.canvas.getContext("2d");
  var states = {};
  var currentState = new State();
  var running = false;
  var this_ = this;
  
  this.canvas.addEventListener("mousemove", function(evt){
    var mousePos = getMousePos(this_.canvas, evt);
    mouse.x = mousePos.x;
    mouse.y = mousePos.y;
  }, false);
  
  this.canvas.addEventListener("mousedown", function(evt){
    mouse.buttons[MOUSE_BUTTONS[evt.button]] = true;
  }, true);

  this.canvas.addEventListener("mouseup", function(evt){
    mouse.buttons[MOUSE_BUTTONS[evt.button]] = false;
  }, true);

  this.addState = function(name, state) {
    states[name] = state;
    state.engine = this;
  };
  
  this.setState = function(name) {
    if(states.hasOwnProperty(name)) {
      currentState.exit();
      currentState = states[name];
      currentState.enter();
    }
  };
  
  function process() {
    currentState.clear(ctx);
    currentState.update(ctx);
    currentState.draw(ctx);
    if(running) {
      scheduleFrame(process);
    }
  }
  
  this.run = function(fps) {
    scheduleFrame(process);
    running = true;
  };
  
  this.stop = function() {
    running = false;
  };
  
}

function State() {
  this.enter = function() {
  };
  this.exit = function() {
  };
  this.clear = function(ctx) {
  };
  this.update = function(ctx) {
  };
  this.draw = function(ctx) {
  };
}
