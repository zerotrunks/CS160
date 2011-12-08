<?php
$username="cssjsuor_160s1g2";
$password="theateam";
$database="cssjsuor_160s1g2";
$dblocal="localhost";

mysql_connect($dblocal,$username,$password);
@mysql_select_db($database) or die("Unable to select database");
?>