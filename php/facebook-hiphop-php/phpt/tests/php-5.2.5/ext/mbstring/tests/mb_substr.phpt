--TEST--
mb_substr()
--SKIPIF--
<?php extension_loaded('mbstring') or die('skip mbstring not available'); ?>
--INI--
output_handler=
mbstring.func_overload=0
--FILE--
<?php
// TODO: Add more encodings
ini_set('include_path','.');
include_once('common.inc');

// EUC-JP
$euc_jp = '0123����ʸ��������ܸ�Ǥ���EUC-JP��ȤäƤ��ޤ������ܸ�����ݽ�����';

print  "1: ". mb_substr($euc_jp,  10,  10,'EUC-JP') . "\n";
print  "2: ". mb_substr($euc_jp,   0, 100,'EUC-JP') . "\n";

$str = mb_substr($euc_jp, 100, 10,'EUC-JP');
// Note: returns last character
($str === "") ? print "3 OK\n" : print "NG: $str\n";

$str = mb_substr($euc_jp, -100, 10,'EUC-JP');
($str !== "") ? print "4 OK: $str\n" : print "NG: $str\n";

?>

--EXPECT--
1: ���ܸ�Ǥ���EUC-
2: 0123����ʸ��������ܸ�Ǥ���EUC-JP��ȤäƤ��ޤ������ܸ�����ݽ�����
3 OK
4 OK: 0123����ʸ�����


