BEGIN {
	my_array["purple"] = "Partridge";
	my_array["mountain"] = "pear";
	my_array["majesties"] = "tree";
	my_array["fruited"] = "Cassidy";

	mykey = "fruited";
	delete my_array["mountain"];
	delete my_array[mykey];

	for (i in my_array) {
		print i "=" my_array[i];
	}
}
