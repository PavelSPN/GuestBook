<?php

/**
 * @file
 * Connection, verification of login and password.
 */

session_start();
include_once "../includes/sql_connection.php";
$login = $_POST[MemberLogin];
$password = $_POST[MemberPassword];
$login2 = $login;
$login2 = mysql_real_escape_string($login2);
$sql = "SELECT id FROM `users` where login='$login2'";
$result = mysql_query($sql) or die("Ошибка " . mysql_error($link));

$row = mysql_fetch_array($result);
if ($row[id]) {
  $a = 1;
}
$sql2 = "SELECT password FROM `users` where id='$row[id]'";
$result2 = mysql_query($sql2) or die("Ошибка " . mysql_error($link));
$row2 = mysql_fetch_array($result2);
$sql3 = "SELECT name FROM `users` where id='$row[id]'";
$result3 = mysql_query($sql3) or die("Ошибка " . mysql_error($link));
$row3 = mysql_fetch_array($result3);
if ($row2[password] == $password) {
  $b = 1;
}
else {
  $b = 2;
}
if ($a == $b) {
  $_SESSION['user'] = $login;
  $_SESSION['name'] = $row3[name];
  $fpp = fopen("$login.txt", 'w');
  fclose($fpp);
  print "<HTML><HEAD>
  <META HTTP-EQUIV='Refresh' CONTENT='0; URL=login.php?page=1'>
  </HEAD></HTML>";
}
else {
  print "Данные введены не корректно<br />"."Попробовать снова ";
}
?>
<a href="../index.php">Main page</a>
<?php
mysql_close($link);
