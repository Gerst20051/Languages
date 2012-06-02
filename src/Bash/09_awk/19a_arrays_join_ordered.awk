
function count_elements(input_array)
{
    counter=0;
    for (word in input_array) {
        counter++;
    }
    return counter;
}

function join(input_array, separator) {
        string = "";        first = 1;

        # Note: this preserves order, but does not
        # work with nonnumeric or sparse arrays.

        for (i=1; i<=count_elements(input_array); i++) {
                if (first) first = 0;
                else string = string separator;
                string = string input_array[i];
        }

	# This works on Mac OS X, but is not fully portable
	# because of bugs in GNU Awk's implementation of length().
        # for (i=1; i<=length(input_array); i++) {
                # if (first) first = 0;
                # else string = string separator;
                # string = string input_array[i];
        # }
        return string;
}
BEGIN {
        arr_len = split( "foo bar baz", my_array, / /);

        for (word in my_array) {
                print my_array[word];
        }

        print join(my_array, " ");
}
