<?php

/**
 * @file
 * Edit and sending message.
 */

session_start();
$login = $_SESSION['user'];
$name = $_SESSION['name'];

// Connection to the database.
include_once "../includes/sql_connection.php";

// Check for special characters in the message.
$post = $_POST[user_message];
$post = ltrim($post);
$post = rtrim($post);
$post = htmlspecialchars((stripslashes($post)), ENT_QUOTES);
$post = str_replace("/", "", $post);
$post = str_replace("<", "", $post);
$post = str_replace(">", "", $post);

// Sending the message.
if ($_SESSION[edit] == 1) {
  sql_update();
}
if (strlen($post) !== 0) {
  INSERT_INTO_share();
}
else {
  print "<HTML><HEAD>
  <META HTTP-EQUIV='Refresh' CONTENT='0; URL=../templates/login.php?page=1'>
  </HEAD></HTML>";
}
