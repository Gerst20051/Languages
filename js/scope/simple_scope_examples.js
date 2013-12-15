// EXAMPLE 1

window.name = "window";

action = function(greeting) {
  console.log(greeting + " " + this.name);
}

action("hello");

// hello window




// EXAMPLE 2

window.name = "window";

object = {
  name: "object",
  
  action: function(greeting) {
    console.log(greeting + " " + this.name);
  }
}

object.action("hello");

// hello object




// EXAMPLE 3

window.name = "window";

object = {
  name: "object",
  
  action: function() {
    nestedAction = function(greeting) {
      console.log(greeting + " " + this.name);
    }
    
    nestedAction("hello");
  }
}

object.action("hello");

// hello window




// EXAMPLE 4

object = {
  name: "object",

  action: function() {
    nestedAction = function(greeting) {
      console.log(greeting + " " + this.name);
    }
    
    nestedAction.call(this, "hello");
    nestedAction.apply(this, ["hello"]);
  }
}

object.action("hello");

// hello object
// hello object




// EXAMPLE 5

window.name = "the window";

alice = {
  name: "Alice"
}

eve = {
  name: "Eve",
  
  talk: function(greeting) {
    console.log(greeting + ", my name is " + this.name);
  }
}

eve.talk("yo");
eve.talk.apply(alice, ["hello"]);
eve.talk.apply(window, ["hi"]);

// yo, my name is eve
// hello, my name is alice
// hi, my name is the window




// EXAMPLE 6

Function.prototype.bind = function(scope) {
  var _function = this;
  
  return function() {
    return _function.apply(scope, arguments);
  }
}

alice = {
  name: "alice"
}

eve = {
  talk: function(greeting) {
    console.log(greeting + ", my name is " + this.name);
  }.bind(alice) // <- bound to "alice"
}

eve.talk("hello");

// hello, my name is alice




// EXAMPLE 7

Function.prototype.bind = function(scope) {
  var _function = this;
  
  return function() {
    return _function.apply(scope, arguments);
  }
}

object = {
  name: "object",
  
  action: function() {
    nestedAction = function(greeting) {
      console.log(greeting + " " + this.name);
    }.bind(this) // <- bound to "object"
    
    nestedAction("hello");
  }
}

object.action("hello");

// hello object