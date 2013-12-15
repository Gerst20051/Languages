--TEST--
mb_ereg()
--SKIPIF--
<?php
extension_loaded('mbstring') or die('skip mbstring not available');
function_exists('mb_ereg') or die("skip mb_ereg() is not available in this build");
?>
--INI--
output_handler=
--FILE--
<?php
	mb_regex_set_options( '' );

	$encs = array( 'EUC-JP', 'Shift_JIS', 'SJIS', 'UTF-8' );

	function test_ereg( $test_enc, $pat, $str, $in_enc = 'EUC-JP' ) {
		mb_regex_encoding( $test_enc );
		$pat = mb_convert_encoding( $pat, $test_enc, $in_enc );
		$str = mb_convert_encoding( $str, $test_enc, $in_enc );

		printf( "(%d)%s\n", mb_ereg( $pat, $str, $reg ), ( is_array( $reg )? mb_convert_encoding( implode( ' ', $reg ), $in_enc, $test_enc ) : '' ) );
	} 
	function do_tests( $enc ) {
		test_ereg( $enc, 'abc ([a-z]+) ([a-z]+) ([a-z]+)$', "abc def ghi jkl" );
		$pat = '([��-��]+) ([ ��-��]+)([��-��]+) ([��-��]+)$'; 
		test_ereg( $enc, $pat, '���� ������ ������ ����' );
		test_ereg( $enc, $pat, '�������� ������ ���� ���' );
	}

	foreach( $encs as $enc ) {
		do_tests( $enc );
	}
?>

--EXPECT--
(15)abc def ghi jkl def ghi jkl
(27)���� ������ ������ ���� ���� ������ �� ���� ����
(27)�������� ������ ���� ��� �������� ������  ���� ���
(15)abc def ghi jkl def ghi jkl
(27)���� ������ ������ ���� ���� ������ �� ���� ����
(27)�������� ������ ���� ��� �������� ������  ���� ���
(15)abc def ghi jkl def ghi jkl
(27)���� ������ ������ ���� ���� ������ �� ���� ����
(27)�������� ������ ���� ��� �������� ������  ���� ���
(15)abc def ghi jkl def ghi jkl
(39)���� ������ ������ ���� ���� ������ �� ���� ����
(39)�������� ������ ���� ��� �������� ������  ���� ���
