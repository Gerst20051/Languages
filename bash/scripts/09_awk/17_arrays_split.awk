BEGIN {
	arr_len = split( "Mary lamb freezer", my_array, / / );
}
{
	for (word in my_array) {
		print my_array[word];
		if (match($0, my_array[word])) {
			print $0;
		}
	}
}
