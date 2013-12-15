BEGIN {
	arr_len = split( "Mary lamb freezer", my_array, / / );
	for (word in my_array) {
		copy_array[word] = my_array[word];
	}
	for (word in copy_array) {
		print copy_array[word];
	}
}
