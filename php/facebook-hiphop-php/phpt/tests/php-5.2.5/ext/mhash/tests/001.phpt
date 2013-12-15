--TEST--
mhash() test
--INI--
magic_quotes_runtime=0
--SKIPIF--
<?php
	include "skip.inc";
?>
--FILE--
<?php

$supported_hash_al = array(
"MHASH_MD5"       => "-�ۑ�N����S*̓j",
"MHASH_SHA1"      => "/�A�Z����I{�;�ہ*}�",
"MHASH_HAVAL256"  => "�U���d'5�ǐ�ƕ���;���� �u����",
"MHASH_HAVAL192"  => "L�7�H0	*��p�Ɉ����",
"MHASH_HAVAL224"  => "SbхgR�,�����r����^�&�&K��",
"MHASH_HAVAL160"  => "Ƴo�uWi����\"q�{��",
"MHASH_RIPEMD160" => "lGCZ��YķƯF4�\x0C>XX=",
"MHASH_GOST"      => "\x0A%Rν�|��QG�U�C)5��,���-�",
"MHASH_TIGER"     => "���y�:�g~ ������0T�\9",
"MHASH_CRC32"     => "��",
"MHASH_CRC32B"    => "��Z�"
);

$data = "This is the test of the mhash extension...";

foreach ($supported_hash_al as $hash=>$wanted) {
	$result = mhash(constant($hash), $data);
	if ($result==$wanted) {
		echo "$hash\nok\n";
	} else {
		echo "$hash: ";
		var_dump($wanted);
		echo "$hash: ";
		var_dump($result);
	}
	echo "\n";
}
?>
--EXPECT--
MHASH_MD5
ok

MHASH_SHA1
ok

MHASH_HAVAL256
ok

MHASH_HAVAL192
ok

MHASH_HAVAL224
ok

MHASH_HAVAL160
ok

MHASH_RIPEMD160
ok

MHASH_GOST
ok

MHASH_TIGER
ok

MHASH_CRC32
ok

MHASH_CRC32B
ok
