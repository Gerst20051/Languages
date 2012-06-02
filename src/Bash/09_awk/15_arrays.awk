BEGIN {
	my_array[0] = "Partridge";
	my_array[1] = "pear";
	my_array[2] = "tree";
	my_array["David"] = "Cassidy";

	for ( my_index in my_array ) {
		print my_index "=" my_array[my_index];
	}
}
