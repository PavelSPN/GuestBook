<?php

/**
 * @file
 * Delete message.
 */

include_once "../includes/sql_connection.php";
$sql = "DELETE FROM `share` WHERE id='$_GET[id]';";
mysql_query($sql) or die (mysql_error());
print "<html><head>
<meta HTTP-EQUIV='Refresh' CONTENT='0; URL=login.php?page=$_GET[page]'>
</head></html>";
