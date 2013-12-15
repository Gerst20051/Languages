--TEST--
mhash_keygen_s2k() test
--SKIPIF--
<?php
	include "skip.inc";
?>
--FILE--
<?php

$supported_hash_al = array(
"MHASH_MD5"       => "��\x15N��2��4z�P���F�\x06栔\x0CTy�zcg�h���t^W\x09��-��s���A7Y��:��\x10w�ݲx�d�q�S���^҃��&U�,���: �aǙ��z�\x06\x1C�S�\x01",
"MHASH_SHA1"      => "�1\\p\x06\x1D\x07E]S��\x0B\x08�\x0Ca�f\\\x1A��\x01�\x10�T#\$��2���\x06��xc\x0A�=\x16��\"IJ\x0E�'�NΚԽ�n���\x08J3�\x15<+H\x13\x1D0�[\x00��[����Y�\x12q�ء\x19��K�mI",
"MHASH_HAVAL256"  => "�G\x00����N�Pw�\x0CH6W�ل�+����,���\x1C\x0DgdZ��U��\x07)oAi�[Na����d�T�\x17\x1F���2�Z����U�46}IYb,���C�\x17�\x1Au����{�[x",
"MHASH_HAVAL224"  => "\\J�=�Z�\x08�\x08���y�8ha�\x0A���p��g|y��ܫq���\x19\x1E\x06���)�˼l��ǌ�G\x0CB@kw�º�Q���v52O�P�-0�5\x00L	m`G-2�I",
"MHASH_HAVAL192"  => "\"��q&\x02<�.�A\x07�/\x1E�2�d\x17����\x1C2�Xv\x0B�p�ű�%�\x03\x16(�C;�+\x08p�z8��\x12\x15\x13N�\x08�u��d�d)qه��v_���MIN�����xLt����Ǉ",
"MHASH_HAVAL160"  => "����\x07����dn�6GX��m��\x06\x1E\x00���η#���5�M��\"m�i�\x1D�1\$\x0E�|�X<O)Y;�=�ʰ�y.��`\"�`\x02��ٴB�	ԯ��ҵ�k>G����\x19�\x17~�\x0D~",
"MHASH_RIPEMD160" => "���F��x�\x0D�5��\x02\x0a\x1D[\x19�gDXgw��ܡD�&�b�����j��B�����7����]�G��[\x14G`^\x1B��\x08K��\x11\x17�3C-AB�%�\x04K\x03??�K������",
"MHASH_GOST"      => "�D�i�~�C�=wƂ�\x17�B��\x15}��s�M�\x01�d|\x01�4h�Y�\"\x0A�I*b@ф�xc@sިy���N%d��\x09��FD\x0a\x12\x12P�\x0F��}E��~�:pO��\x0D�pw�R{\x19M�",
"MHASH_TIGER"     => "g��{��\nG��&/3d��#7`�2U�BQ/�y)����X#k'h����n��/��E�������p*M1 V��K�y�O`�	���M����6��|\"�",
"MHASH_CRC32"     => "H@�&_�Y��oQ���1[Fq\"�\x08���f��V�8\x08�E��E<����M�:Ⱥ��r��k*�H��u��/}���h3iW��J?vui��=�W\"���",
"MHASH_CRC32B"    => "�l�e�>}�*�]�Fכ6\x13�bC�ˏ.	I�\x0F�j�Υ0��\x0D\x1F<��.Qy疾��ti��xb�\$}�\x0B��ȴ�	�ϬȌD�DK�\x13�1uL��6��`��ɏ�|Ec�\x04�",
);

foreach ($supported_hash_al as $hash=>$wanted) {
	$passwd = str_repeat($hash, 10);
	$salt = str_repeat($hash, 2);
	$result = mhash_keygen_s2k(constant($hash), $passwd, $salt, 100);
	if (!strcmp($result, $wanted)) {
		echo "$hash\nok\n";
	} else {
		echo "$hash: ";
		var_dump(bin2hex($wanted));
		echo "$hash: ";
		var_dump(bin2hex($result));
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

MHASH_HAVAL224
ok

MHASH_HAVAL192
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
