/*
function Animal(name) {
this.name = name;
};

Animal.prototype.talk = function() {
return "";
};

function Cat(name) {
Animal.call(this, name);
};

Cat.prototype = new Animal();

Cat.prototype.talk = function() {
return "Meow!";
};

function Dog(name) {
Animal.call(this, name);
};

Dog.prototype = new Animal();

Dog.prototype.talk = function() {
return "Arf! Arf!";
};

var animals = [
new Cat("Missy"),
new Cat("Mr. Paul"),
new Dog("Lassie")
];

document.write(animals[1].name + ": " + new Cat("Mr. Missy").talk + " " + animals[2].talk + "<br />");

//
// Missy: Meow!
// Mr. Paul: Meow!
// Lassie: Arf! Arf!

for (var animal in animals) {
document.write(animal.name + ": " + animal.talk + "<br/>");
}
*/

/* get selected text
var text = "";
function getActiveText(e) { 
text = (document.all) ? document.selection.createRange().text : document.getSelection();
if (text != "") {
alert(text);
}
return true;
}

document.onmouseup = getActiveText;
if (!document.all) document.captureEvents(Event.MOUSEUP);
*/

// Download file
$("#dl").click(function() {
var aSrc="/content/learning/ide/buildSource.php?file=template.html&script="+escape($("#source").text());    

$('#pushzip').html("<iframe src='"+aSrc+"'></iframe>");
});

if($.browser.opera) {
$('#pushzip').css({display:'block'});
$('#canvasbox').css({marginTop:'30px'});
$('#source').css({position:'relative!important',top:'15px'});
}