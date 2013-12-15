function preg_grep (pattern, input, flags) {
	// *	 example 1: var arr = [1, 4, 4.5, 3, 'a', 4.4];
	// *	 example 1: preg_grep("/^(\\d+)?\\.\\d+$/", arr);
	// *	 returns 1: {2: 4.5, 5: 4.4}
	
	var p = '', retObj = {};
	var invert = (flags === 1 || flags === 'PREG_GREP_INVERT');

	if (typeof pattern === 'string') {
		pattern = eval(pattern);
	}

	if (invert) {
		for (p in input) {
			if (input[p].search(pattern) === -1) {
				retObj[p] = input[p];
			}
		}
	} else {
		for (p in input) {
			if (input[p].search(pattern) !== -1) {
				retObj[p] = input[p];
			}
		}
	}

	return retObj;
}

function searchArr(pattern, input) {	
	var p = '', retObj = {};
	if (typeof pattern === 'string') {
		pattern = eval(pattern);
	}
	for (p in input) {
		if (input[p].search(pattern) !== -1) {
			retObj[p] = input[p];
		}
	}
	return retObj;
}