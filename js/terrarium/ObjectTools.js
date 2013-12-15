function clone(object) {
  function OneShotConstructor(){}
  OneShotConstructor.prototype = object;
  return new OneShotConstructor();
}

Object.prototype.create = function() {
  var object = clone(this);
  if (typeof object.construct == "function")
    object.construct.apply(object, arguments);
  return object;
};

Object.prototype.extend = function(properties) {
  var result = clone(this);
  forEachIn(properties, function(name, value) {
    result[name] = value;
  });
  return result;
};

function bind(func, object) {
  return function(){
    return func.apply(object, arguments);
  };
}

function method(object, name) {
  return function() {
    object[name].apply(object, arguments);
  };
}

var object = "world!";


