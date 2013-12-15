<?php
session_start();

include ("db.inc.php");

/* ---------------------------------------------------- */
/* ------------ >>>  Table of Contents  <<< ----------- */
/* ---------------------------------------------------- */
/* GET  ID
/* GET  ACTION
/* GET  DATA
/* GET  SUBDATA

/* ---------------------------------------------------- */

if (isset($_GET['id'])) {
$id = trim($_GET['id']);
$username = $_SESSION['username'];
$userid = $_SESSION['user_id'];

if (isset($_GET['action'])) $action = trim($_GET['action']);
if (isset($_GET['data'])) $data = trim($_GET['data']);
if (isset($_GET['subdata'])) $subdata = trim($_GET['subdata']);

switch ($id) {

}
}
?>