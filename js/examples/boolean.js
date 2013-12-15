// Example 1

var myValue = document.myForm.IS_TRUE.value;
var isTrueSet = myValue == 'true';

// Example 2

var myBool = Boolean("false");  // == true
var myBool = !!"false";  // == true
var isTrueSet = (myValue === 'true');

// Example 3

String.prototype.bool = function() {
    return (/^true$/i).test(this);
};
alert("true".bool());

// Example 4

stringToBoolean: function(string){
        switch(string.toLowerCase()){
                case "true": case "yes": case "1": return true;
                case "false": case "no": case "0": case null: return false;
                default: return Boolean(string);
        }
}

// Example 5

var isTrueSet = (myValue.toLowerCase() === 'true');
// Also, if it's a form element checkbox, you can also detect if the checkbox is checked:
var isTrueSet = document.myForm.IS_TRUE.checked;

// Example 6

Boolean.parse = function (str) {
  switch (str.toLowerCase ()) {
    case "true":
      return true;
    case "false":
      return false;
    default:
      throw new Error ("Boolean.parse: Cannot convert string to boolean.");
  }
};

// Example 7

String.prototype.boolean = function() {
    return "true" == this; 
};

"true".boolean() // returns true "false".boolean() // returns false

// Example 8

// I think this is much universal: if (String(a) == "true") ...
// It goes:

String(true) == "true" //returns true
String(false) == "true" //returns false
String("true") == "true" //returns true
String("false") == "true" //returns false

// Example 9

if (String(a) == "true"){ /*true block*/ } else{ /*false block*/ }

// Example 10

/*
The Boolean object doesn't have a 'parse' method. Boolean("false") returns true, so that won't work.
!!"false" also returns true, so that won't work also.

If you want string "true" to return boolean true and string "false" to return boolean "false" then the simplest solution is to use eval().
eval("true") returns true and eval("false") returns false. Keep in mind the performance implications when using eval() though.
*/

// Example 11

var myBool = eval (yourString);
// Examples:

alert (eval ("true") == true); // TRUE
alert (eval ("true") == false); // FALSE
alert (eval ("1") == true); // TRUE
alert (eval ("1") == false); // FALSE
alert (eval ("false") == true); // FALSE;
alert (eval ("false") == false); // TRUE
alert (eval ("0") == true); // FALSE
alert (eval ("0") == false); // TRUE
alert (eval ("") == undefined); // TRUE
alert (eval () == undefined); // TRUE

// Example 12

var myBool = Boolean.parse("true");
// or

var myBool = Boolean("true");
// or

var myBool = !!"true";

// Here is the thing with using Boolean("true"). It is a little misleading because Boolean("false") and Boolean('wtf") evaluate to true as well.
