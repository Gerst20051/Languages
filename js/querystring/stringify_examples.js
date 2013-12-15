// EXAMPLE 1

serialize = function(obj) {
  var str = [];
  for(var p in obj)
     str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
  return str.join("&");
}

alert(serialize({foo: "hi there", bar: "100%" }));
//serializes to: foo=hi%20there&bar=100%25




// EXAMPLE 2

// this one also converts recursive objects (using php "array" notation for the query string)
serialize = function(obj, prefix) {
    var str = [];
    for(var p in obj) {
        var k = prefix ? prefix + "[" + p + "]" : p, v = obj[p];
        str.push(typeof v == "object" ? 
            serialize(v, k) :
            encodeURIComponent(k) + "=" + encodeURIComponent(v));
    }
    return str.join("&");
}

alert(serialize({foo: "hi there", bar: { blah: 123, quux: [1, 2, 3] }}));




// EXAMPLE 3

function thingToString(thing,maxDepth,recordLevel,markArrays){
    //thing: object or array to be recursively serialized
    //maxDepth (int or false):
    // (int) how deep to go with converting objects/arrays within objs/arrays
    // (false) no limit to recursive objects/arrays within objects/arrays
    //recordLevel (boolean):
    //  true - insert "(level 1)" before transcript of members at level one (etc)
    //  false - just 
    //markArrays (boolean):
    //  insert text to indicate any members that came from arrays
    var result = "";
    if (maxDepth !== false && typeof maxDepth != 'number') {maxDepth = 3;}
    var runningDepth = 0;//Keeps track how deep we're into recursion

    //First prepare the function, so that it can call itself recursively
    function serializeAnything(thing){
        //Set path-finder values
        runningDepth += 1;
        if(recordLevel){result += "(level " + runningDepth + ")";}

        //First convert any arrays to object so they can be processed
        if (thing instanceof Array){
            var realObj = {};var key;
            if (markArrays) {realObj['type'] = "converted array";}
            for (var i = 0;i < thing.length;i++){
                if (markArrays) {key = "a" + i;} else {key = i;}
                realObj[key] = thing[i];
            }
            thing = realObj;
            console.log('converted one array to ' + typeof realObj);
            console.log(thing);
        }

        //Then deal with it
        for (var member in thing){
            if (typeof thing[member] == 'object' && runningDepth < maxDepth){
                serializeAnything(thing[member]);
                //When a sub-object/array is serialized, it will add one to
                //running depth. But when we continue to this object/array's
                //next sibling, the level must go back up by one
                runningDepth -= 1;
            } else if (maxDepth !== false && runningDepth >= maxDepth) {
                console.log('Reached bottom');
            } else 
            if (
                typeof thing[member] == "string" || 
                typeof thing[member] == 'boolean' ||
                typeof thing[member] == 'number'
            ){
                result += "(" + member + ": " + thing[member] + ") ";
            }  else {
                result += "(" + member + ": [" + typeof thing[member] + " not supported]) ";
            }
        }
    }
    //Actually kick off the serialization
    serializeAnything(thing);

    return result;

}




// EXAMPLE 4

getQueryString = function(obj)
   {
      result = "";

      for(param in obj)
         result += ( encodeURIComponent(param) + '=' + encodeURIComponent(obj[param]) + '&' );

      if(result) //it's not empty string when at least one key/value pair was added. In such case we need to remove the last '&' char
         result = result.substr(0, result.length - 1); //If length is zero or negative, substr returns an empty string [ref. http://msdn.microsoft.com/en-us/library/0esxc5wy(v=VS.85).aspx]

      return result;
   }

alert( getQueryString({foo: "hi there", bar: 123, quux: 2 }) );




// EXAMPLE 5

function QueryStringBuilder() {
    var nameValues = [];

    this.add = function(name, value) {
        nameValues.push( {name: name, value: value} );
    };

    this.toQueryString = function() {
        var segments = [], nameValue;
        for (var i = 0, len = nameValues.length; i < len; i++) {
            nameValue = nameValues[i];
            segments[i] = encodeURIComponent(nameValue.name) + "=" + encodeURIComponent(nameValue.value);
        }
        return segments.join("&");
    };
}

var qsb = new QueryStringBuilder();
qsb.add("veg", "cabbage");
qsb.add("vegCount", "5");

alert( qsb.toQueryString() );