function forEach(array, action){
 for (var i = 0; i < array.length; i++)
  action(array[i]);
}

function forEachIn(object, action) {
  for (var property in object) {
    if (object.hasOwnProperty(property))
      action(property, object[property]);
  }
}

function reduce(combine, base, array){
 forEach(array, function (element){
  base = combine(base, element);
 });
 return base;
}

function map(func, array) {
  var result = [];
  forEach(array, function (element) {
    result.push(func(element));
  });
  return result;
}

function asArray(quasiArray, start) {
  var result = [];
  for (var i = (start || 0); i < quasiArray.length; i++)
    result.push(quasiArray[i]);
  return result;
}

function partial(func) {
  var fixedArgs = asArray(arguments, 1);
  return function(){
    return func.apply(null, fixedArgs.concat(asArray(arguments)));
  };
}

function bind(func, object) {
  return function(){
    return func.apply(object, arguments);
  };
}

var functional = "hello, ";

