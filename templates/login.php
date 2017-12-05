<?php

/**
 * @file
 * Connect files with functions for login, displaying messages and sending.
 */

session_start();
$login = $_SESSION['user'];
$name = $_SESSION['name'];

if ($_SESSION['user'] == NULL) {
  print "<HTML><HEAD>
  <META HTTP-EQUIV='Refresh' CONTENT='0; URL=index.php'>
  </HEAD></HTML>";
  session_destroy();
  exit;
}

include_once "../templates/html.php";
include_once "../includes/functions.php";
include_once "../includes/sql_connection.php";

// Button Log out.
logout();
// User message window.
?>
<div class="blockmessagemain">
<?php
user_message_window(peremennie_login());
?>
</div> 
<?php
// Buttons for switching to another page.
?>
<div class="pages">
<?php
pagination($all, $lim, $prev, $curr_link, $curr_css, $link2);
?>
</div>
<?php
// User input window.
?>
<div class="blockmessagesend">
  <form name="message_send" action="../templates/send.php"  method="post" >
  <textarea rows="5" cols="190" name="user_message">
<?php
send_edit_message();
?>
  </textarea><br /><br />
  <input type="submit" name="save" value="Save" />
  </form>
<?php
button_cancel_edit();
?>
</div>
<?php
