<?php

/**
 * @file
 * Connect files with functions for login and displaying messages.
 */

include_once "../templates/html.php";
include_once "../includes/functions.php";
include_once "../includes/sql_connection.php";

// Log In form.
login();
// User message window.
?>
<div class="blockmessagemain">
<?php
user_message_window(peremennie_main());
// Buttons for switching to another page.
?>
</div>
<div class="pages">
<?php
pagination($all, $lim, $prev, $curr_link, $curr_css, $link2);
?>
</div>
<?php
