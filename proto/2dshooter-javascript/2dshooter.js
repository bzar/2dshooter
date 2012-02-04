var debug = false;

$(document).ready(function() {
  var engine = new Engine("canvas");
  engine.addState("shooter", new ShooterState());
  engine.setState("shooter");
  engine.run(60);
});
