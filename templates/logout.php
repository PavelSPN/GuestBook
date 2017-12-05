<?php

/**
 * @file
 * Logout and session destroy.
 */

session_start();
$fle = $_SESSION['user'] . ".txt";
unlink($fle);
fclose($fpp);
session_destroy();
print "<HTML><HEAD> 
<META HTTP-EQUIV='Refresh' CONTENT='0; URL=../index.php'> 
</HEAD></HTML>";
