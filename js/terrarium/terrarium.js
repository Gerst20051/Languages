function randomElement(array){
 return array[Math.floor(Math.random() * array.length)];
}



var thePlan =
  ["#################################",
   "#      #    #           o      ##",
   "#   &                       &   #",
   "#          ##########           #",
   "##    C    #        #    ##    C#",
   "###                ##     #     #",
   "#     %     ########      #     #",
   "#                               #",
   "#                               #",
   "#                               #",
   "#                               #",
   "#   #### @                &     #",
   "#   ##       o        %         #",
   "# o  #     C        o       ### #",
   "#    #                 C        #",
   "#                               #",
   "#################################"];

function Point(x, y) {
  this.x = x;
  this.y = y;
}
Point.prototype.add = function(other) {
  return new Point(this.x + other.x, this.y + other.y);
};
Point.prototype.isEqualTo = function(other) {
  return this.x == other.x && this.y == other.y;
};
Point.prototype.toString = function(){
  return "(" + this.x + "," + this.y + ")";
};

function Grid(width, height) {
  this.width = width;
  this.height = height;
  this.cells = new Array(width * height);
}
Grid.prototype.valueAt = function(point) {
  return this.cells[point.y * this.width + point.x];
};
Grid.prototype.setValueAt = function(point, value) {
  this.cells[point.y * this.width + point.x] = value;
};
Grid.prototype.isInside = function(point) {
  return point.x >= 0 && point.y >= 0 &&
         point.x < this.width && point.y < this.height;
};
Grid.prototype.moveValue = function(from, to) {
  this.setValueAt(to, this.valueAt(from));
  this.setValueAt(from, undefined);
};
Grid.prototype.each = function(action) {
  for (var y = 0; y < this.height; y++) {
    for (var x = 0; x < this.width; x++) {
      var point = new Point(x, y);
      action(point, this.valueAt(point));
    }
  }
};


var directions = new Dictionary(
  {"n":  new Point( 0, -1),
   "ne": new Point( 1, -1),
   "e":  new Point( 1,  0),
   "se": new Point( 1,  1),
   "s":  new Point( 0,  1),
   "sw": new Point(-1,  1),
   "w":  new Point(-1,  0),
   "nw": new Point(-1, -1)});



var Square = {
  construct: function(character, tableCell) {
    this.tableCell = tableCell;
    this.background = "empty";
    
    if (character == "#"){
      this.background = "wall";
      this.tableCell.innerHTML = character;
    }
    
    if (creatureTypes.contains(character)){
      var creature = elementFromCharacter(character);
      this.background = creature.title;
      this.tableCell.innerHTML = character;
    }
    //this.tableCell.innerHTML = character;
    this.tableCell.className = this.background;
 

  },

  
  isEmpty: function() {
    return this.content == null && this.background == "empty";
  },
};

Square.moveContent = function(target) {
  target.tableCell.innerHTML = this.tableCell.innerHTML;
  target.tableCell.className = this.tableCell.className;
  this.tableCell.innerHTML = null;
  this.tableCell.className = "empty";
};
Square.clearContent = function() {
  this.tableCell.innerHTML = null;
  this.tableCell.backGround = "empty";
};

var wall = {};

function Terrarium(plan) {
  var tbody = dom("TBODY");
  this.squares = [];
  var grid = new Grid(plan[0].length, plan.length);
  for (var y = 0; y < plan.length; y++) {
    var line = plan[y];
    var tableRow = dom("TR");
    var squareRow = [];
    for (var x = 0; x < line.length; x++) {
      var tableCell = dom("TD");
      tableRow.appendChild(tableCell);
      var square = Square.create(line.charAt(x), tableCell);
      squareRow.push(square);
      tbody.appendChild(tableRow);
      
      grid.setValueAt(new Point(x, y),
                      elementFromCharacter(line.charAt(x)));
    }
    this.squares.push(squareRow);
  }
  this.table = dom("TABLE", {"class": "terrarium"}, tbody);
  this.grid = grid;
  this.player = this.getPlayer();
  addHandler(document, "keydown", method(this, "keyDown"));
  
}
Terrarium.prototype.keyDown = function(event){
 if (arrowKeyCodes.contains(event.keyCode)){
  var surroundings = this.listSurroundings(this.player.point);
  var currentSquare = this.getSquare(this.player.point);
  if (arrowKeyCodes.contains(event.keyCode)){
   var to = this.player.point.add(arrowKeyCodes.lookup(event.keyCode));
   var targetSquare = this.getSquare(to);
   if (this.grid.isInside(to) && this.grid.valueAt(to) == undefined){
    this.grid.moveValue(this.player.point, to);
    currentSquare.moveContent(targetSquare);
    this.player = this.getPlayer();
   }
  }
 }
};

Terrarium.prototype.getPlayer = function(){
 var player;
 this.grid.each(function(point,value){
  if (value != undefined && value.player){
   player = value;
   player.point = point;
   return;
  }
 });
 return player;
}

Terrarium.prototype.processCreature = function(creature) {
  var surroundings = this.listSurroundings(creature.point);
  var currentSquare = this.getSquare(creature.point);
  var action = creature.object.act(surroundings);
  if (action.type == "move" && directions.contains(action.direction)) {
    var to = creature.point.add(directions.lookup(action.direction));
    var targetSquare = this.getSquare(to);
    if (this.grid.isInside(to) && this.grid.valueAt(to) == undefined){
      this.grid.moveValue(creature.point, to);
      currentSquare.moveContent(targetSquare);
    }
  }
  else {
    throw new Error("Unsupported action: " + action.type);
  }
};
function elementFromCharacter(character) {
  if (character == " ")
    return undefined;
  else if (character == "#")
    return wall;
  else if (creatureTypes.contains(character))
    return new (creatureTypes.lookup(character))();
  else
    throw new Error("Unknown character: " + character);
}

wall.character = "#";

function characterFromElement(element) {
  if (element == undefined)
    return " ";
  else
    return element.character;
}

Terrarium.prototype.toString = function() {
  var characters = [];
  var endOfLine = this.grid.width - 1;
  this.grid.each(function(point, value) {
    characters.push(characterFromElement(value));
    if (point.x == endOfLine)
      characters.push("\n");
  });
  return characters.join("");
};


Terrarium.prototype.listActingCreatures = function() {
  var found = [];
  this.grid.each(function(point, value) {
    if (value != undefined && value.act)
      found.push({object: value, point: point});
  });
  return found;
};
Terrarium.prototype.listSurroundings = function(center) {
  var result = {};
  var grid = this.grid;
  directions.each(function(name, direction) {
    var place = center.add(direction);
    if (grid.isInside(place))
      result[name] = characterFromElement(grid.valueAt(place));
    else
      result[name] = "#";
  });
  return result;
};

Terrarium.prototype.step = function() {
  forEach(this.listActingCreatures(),
          bind(this.processCreature, this));
  if (this.onStep)
    this.onStep();
};
Terrarium.prototype.start = function() {
  if (!this.running)
    this.running = setInterval(bind(this.step, this), 500);
};

Terrarium.prototype.stop = function() {
  if (this.running) {
    clearInterval(this.running);
    this.running = null;
  }
};

Terrarium.prototype.getSquare = function(position){
  return this.squares[position.y][position.x];
}
Terrarium.prototype.place = function(where) {
  where.appendChild(this.table);
};
Terrarium.prototype.remove = function() {
  removeElement(this.table);
};


var creatureTypes = new Dictionary();
creatureTypes.register = function(constructor) {
  this.store(constructor.prototype.character, constructor);
};

function BouncingBug() {
  this.direction = "ne";
}
BouncingBug.prototype.act = function(surroundings) {
  if (surroundings[this.direction] != " ")
    this.direction = (this.direction == "ne" ? "sw" : "ne");
  return {type: "move", direction: this.direction};
};
BouncingBug.prototype.character = "%";
BouncingBug.prototype.title = "Bouncing";
creatureTypes.register(BouncingBug);

function StupidBug() {
  this.direction = "s";
}
StupidBug.prototype.act = function(surroundings) {
  if (surroundings[this.direction] != " ")
    this.direction = (this.direction = 
                      randomElement(directions.names()));
  return {type: "move", direction: this.direction};
};
StupidBug.prototype.character = "o";
StupidBug.prototype.title = "Stupid";
creatureTypes.register(StupidBug);

function Chameleon(){
 var coin = ["heads", "tails"];
 var characters = creatureTypes.names();
 
 this.headsOrTails = coin[Math.floor(Math.random * 2)];
};
Chameleon.prototype.act = function(surroundings){
 var adjacent = surroundings[this.direction];

 if (this.headsOrTails = "heads"){
  if (adjacent != " ")
    this.direction = randomElement(directions.names());
 }
 else this.direction = randomElement(directions.names());
 
 return {type: "move", direction: this.direction};
}
Chameleon.prototype.character = "C";
Chameleon.prototype.title = "Chameleon";
creatureTypes.register(Chameleon);
 

function RandomBug(){};
RandomBug.prototype.act = function(surroundings){
 return {type: "move", 
         direction: randomElement(directions.names())};
};
RandomBug.prototype.title = "Random";
RandomBug.prototype.character = "&";
creatureTypes.register(RandomBug);

function Player(){};
Player.prototype.player = true;
Player.prototype.title = "Player";
Player.prototype.character = "@";
creatureTypes.register(Player);

var arrowKeyCodes = new Dictionary({
  37: new Point(-1, 0), // left
  38: new Point(0, -1), // up
  39: new Point(1, 0),  // right
  40: new Point(0, 1)   // down
});



function init(){
 var terrarium = new Terrarium(thePlan);
 terrarium.onStep = terrarium.place(document.body);
 terrarium.start();
}
window.onLoad = setTimeout("init()",40);
