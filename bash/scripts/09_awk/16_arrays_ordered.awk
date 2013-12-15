BEGIN {
	my_array[0] = "Partridge";
	my_array[1] = "pear";
	my_array[2] = "tree";
	my_array[13] = "Cassidy";

	min = 0; max = 0;
	for ( my_index in my_array ) {
		if (my_index+0 < min) min = my_index;
		if (my_index+0 > max) max = my_index;
	}
	for (i=min; i<= max; i++) {
		if (i in my_array) {
			print i "=" my_array[i];
		}
		if (!(i in my_array)) {
			print i " is unset.";
		}
	}
}
