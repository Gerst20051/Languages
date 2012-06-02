function join(input_array, separator) {
	string = "";
	first = 1;
	for (i in input_array) {
		if (first) first = 0;
		else string = string separator;
		string = string input_array[i];
	}
	return string;
}
BEGIN {
	arr_len = split( "foo bar baz", my_array, / /);

	for (word in my_array) {
		print my_array[word];
	}

	print join(my_array, " ");
}
