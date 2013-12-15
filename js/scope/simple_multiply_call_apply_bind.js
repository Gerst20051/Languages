var first_object = {
	num: 42
};

var second_object = {
	num: 24
};

function multiply(mult) {
	return this.num * mult;
}

multiply.call(first_object, 5); // returns 42 * 5
multiply.call(second_object, 5); // returns 24 * 5
// OR
multiply.apply(first_object, [5]); // returns 42 * 5
multiply.apply(second_object, [5]); // returns 24 * 5


// BIND BUTTON CLICK


Function.prototype.bind = function(obj) {
	var method = this,
	temp = function() {
		return method.apply(obj, arguments);
	};

	return temp;
} 

var first_multiply = multiply.bind(first_object);
first_multiply(5); // returns 42 * 5

var second_multiply = multiply.bind(second_object);
second_multiply(5); // returns 24 * 5

function addhandler() {
	var deep_thought = new BigComputer(42),
	the_button = document.getElementById('thebutton');
	the_button.onclick = deep_thought.ask_question.bind(deep_thought);
}