<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'frm_http.inc.php';

if (!isset($_GET['t'])) redirect('forums.php');
require_once 'hd.inc.php';

$topicid = $_GET['t'];
$limit = 10;

showTopic($topicid, TRUE);

require_once 'ft.inc.php';
?>