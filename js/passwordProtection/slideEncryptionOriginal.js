var char_set = '$%^NOZ1&PQR(./~`"CDEFG!@STUVWZghij}:<pHB*#8uvwx>?[]\',ef34590qrklmnoIJ)_+{st67 abcdAyzKLM2Y';

function encrypt_data() {
	var input = window.input.input_field.value;
	var output = "";
	var char_code;

	//get algorithm from pull down select
	var algorithm = window.input.shift_select.selectedIndex;
	algorithm++;

	var alpha_length = char_set.length - algorithm;
	var space;

	//begin for loop to cycle through input
	for (loop=0; loop<input.length; loop++) {

	//if conditional detects unknown characters
	if (char_set.indexOf(input.charAt(loop)) == -1) {
	alert("Program Error: Unknown Character!");
	}

	//search char_set string for character and set char_code variable...
	char_code = char_set.indexOf(input.charAt(loop));

	//actual text encoding algorithm goes here
	if (char_code + algorithm > char_set.length)
	{
	space = char_set.length - char_code;
	char_code = algorithm - space;
	}
	else
	{
	char_code += algorithm;
	}

	//set output variable in accordance to char_set
	output += char_set.charAt(char_code);
	}
	//dump contents of output variable into textarea
	window.input.input_field.value = output;
}

function decrypt_data() {
	var input = window.input.input_field.value;
	var output = "";
	var char_code;

	//get algorithm from pull down select
	var algorithm = window.input.shift_select.selectedIndex;
	algorithm++;

	var alpha_length = char_set.length - algorithm;
	var space;

	//begin for loop to cycle through input
	for (loop=0; loop<input.length; loop++) {

	//if conditional detects unknown characters
	if (char_set.indexOf(input.charAt(loop)) == -1) {
	alert("Program Error: Unknown Character!");
	}

	//search char_set string for character and set char_code variable...
	char_code = char_set.indexOf(input.charAt(loop));

	//oppisite of encrypt algorithm goes here
	if (char_code - algorithm < 0) {
	space = algorithm - char_code;
	char_code = char_set.length - space;
	} else {
	char_code -= algorithm;
	}

	//set output variable in accordance to char_set
	output += char_set.charAt(char_code);
	}
	//dump contents of output variable into textarea
	window.input.input_field.value = output;
}
</script>
</HEAD>
<BODY>
<form name="input">
<TEXTAREA name="input_field" style="WIDTH: 95%; HEIGHT: 300px">
</TEXTAREA>
<center>
<b>Key: </b>
<select name="shift_select">
<option>1
<option>2
<option>3
<option>4
<option selected>5
<option>6
<option>7
<option>8
<option>9
</select>
<input type="button" name="encrypt" value="EnCrypt" onclick="encrypt_data();">
<input type="button" name="decrypt" value="DeCrypt" onclick="decrypt_data();">
</center>
</form>