<?php

/**
 * @file
 * Contains php functions.
 */

/**
 * Edit message.
 *
 * $row - massive containing an information selected from DataBase,
 * by function select_all_WHERE_id_GETid in sql_connection.php.
 * $curr_page - value of the variable $_GET[page].
 */
function send_edit_message() {
  global $row;
  if ($_GET['edit'] == 1) {
    select_all_WHERE_id_GETid();
    print $row[2];
    $_SESSION['edit'] = 1;
    $_SESSION['id2'] = $_GET[id];
    $_SESSION['page2'] = $_GET[page];
  }
}
/**
 * Button 'Cancel' for edit message.
 *
 * 'Cancel' button cancels editing and returns to the current page.
 */
function button_cancel_edit() {
  if ($_GET['edit'] == 1) {
    $cancel = "<a href='login.php?page={$_GET[page]}&edit=0'> cancel </a> ";
    print $cancel;
  }
}
/**
 * Variables for main.php.
 *
 * The variables are used for the function user_message_window
 * in the function.php file and function pagination in the main.php file.
 */
function peremennie_main() {
  // Select from share table.
  global $sql;
  global $result;
  select_all_by_id_desc ();
  $num_rows = mysql_num_rows($result);
  $k = $num_rows;
  global $curr_page;
  global $curr_page;
  global $all;
  global $lim;
  global $prev;
  global $curr_link;
  global $curr_css;
  global $link2;
  global $pages;
  global $login;
  global $ses_id_mes;
  $curr_page = $_GET[page];
  $pages = ceil($k / 10);
  //Number of posts in the category.
  $all = $k;
  //Number of posts placed on one page.
  $lim = 10;
  //Number of displayed links before and after the current page number.
  $prev = 3;
  //The current page number (we get from the URL).
  $curr_link = $curr_page;
  //Css-style for linking to the "current (active)" page.
  $curr_css = "current";
  //Part of the URL used to create links to other pages.
  $link2 = "main.php";
}
/**
 * Variables for main.php.
 *
 * The variables are used for the function user_message_window
 * in the function.php file and function pagination in the login.php file.
 */
function peremennie_login() {
  // Select from share table.
  global $sql;
  global $result;
  select_all_by_id_desc();
  $num_rows = mysql_num_rows($result);
  $k = $num_rows;
  global $curr_page;
  global $curr_page;
  global $all;
  global $lim;
  global $prev;
  global $curr_link;
  global $curr_css;
  global $link2;
  global $pages;
  global $login;
  global $ses_id_mes;
  $curr_page = $_GET[page];
  $pages = ceil($k/10);
  // Number of posts in the category.
  $all = $k;
  // Number of posts placed on one page.
  $lim = 10;
  // Number of displayed links before and after the current page number.
  $prev = 3;
  // The current page number (we get from the URL).
  $curr_link = $curr_page;
  // Css-style for linking to the "current (active)" page.
  $curr_css = "current";
  // Part of the URL used to create links to other pages.
  $link2 = "login.php";
}
/**
 * Window where all messages are displayed.
 *
 * $sql and $result - variables from the function select_all_by_id_desc
 * from file sql_connection.php.
 * $ses_id_mes - array created in function 'file3' in functions.php file.
 */
function user_message_window() {
  global $sql;
  global $result;
  select_all_by_id_desc();
  $num_rows = mysql_num_rows($result);
  $k = $num_rows;
  $ses_id = $_SESSION['id'];
  $login = $_SESSION['user'];
  global $login;
  file3();
  $curr_page = $_GET[page];
  global $curr_page;
  // If number of rows < 10, then display all rows.
  if ($k < 10) {
    while ($row = mysql_fetch_array($result)) {
      print $row['date'] . " " . $row['name'] . "<br>";
      print $row['message'] . "<br>";
      print "<br><hr>\n";
    }
  }
  else {
    // If number of rows > 10, then display 10 rows.
    if ($curr_page == 1) {
      $gt = 0;
    }
    else {
      // If page not first, then calculate.
      $gt = ($curr_page * 10) - 10;
    }
    global $ses_id_mes;
    // Select 10 rows from the database starting by $gt number.
    select_all_by_id_desc_LIMIT_10($gt);
    while ($row = mysql_fetch_array($result)) {
      // Display 10 rows starting by $gt number.
      print $row['date'] . " " . $row['name'] . "<br>";
      // Adding the Delete and Edit buttons if the user is logged in.
      for ($i = 0; $i <= count($ses_id_mes); $i++) {
        if ($ses_id_mes[$i] == $row['id']) {
          print "<a href='delete.php?id={$row['id']}&page={$curr_page}'>Удалить\n</a>";
          print "<a href='login.php?id={$row['id']}&page={$curr_page}&edit=1'>Редактировать</a>";
        }
      }
      print "<br />";
      print $row['message'] . "<br>";
      print "<br><hr>\n";
    }
  }
}
/**
 * Processing of session messages.
 *
 * Create a file with the name of the login, in which the message id,
 * separated by the | symbol, will be stored. Then we extract this
 * line into the array $ses_id_mes.
 */
function file3() {
  global $ses_id_mes;
  $login = $_SESSION['user'];
  $sesid = $_SESSION['id'];
  $fle2 = $login. ".txt";
  // Open the file in write mode. The carriage is at the end of the text.
  $fpp = fopen("$fle2", 'a');
  $mytext2 = "|";
  // Write to the file separator '|'.
  fwrite($fpp, $mytext2);
  // Write the message id.
  fwrite($fpp, $sesid);
  // Reads entire file into a string.
  $file = file_get_contents($fle2);
  // Splits a string using a separator '|' and writes array to $ses_id_mes.
  $ses_id_mes = explode("|", $file);
  // Removes duplicate values from $ses_id_mes array.
  $ses_id_mes = array_unique($ses_id_mes);
  // Sort $ses_id_mes array by key.
  ksort($ses_id_mes);
  // Return all the values of $ses_id_mes array.
  $ses_id_mes = array_values($ses_id_mes);
  fclose($fpp);
}
/**
 * Pagination.
 *
 * Create buttons to switch pages.
 * $all, $lim, $prev, $curr_link, $curr_css, $link2 are contained in a function
 * peremennie_main or peremennie_login in functions.php file.
 *
 * @param $all
 *  Integer. Number of posts in the category.
 * @param $lim
 *  Integer. Number of displayed links before and after the current page number.
 * @param $prev
 *  Integer. Number of displayed links before and after the current page number.
 * @param $curr_link
 *  Integer. The current page number (we get from the URL): $_GET[page].
 * @param $curr_css
 *  A string containing a css-style for linking to the "current (active)" page.
 * @param $link2
 *  A string containing a part of the URL used to create links to other pages.
 */
function pagination($all, $lim, $prev, $curr_link, $curr_css, $link2){
  // Check that the displayed first and last pages do not exceed the numbering limits.
  $first = $curr_link - $prev;
  if ($first < 1) {
    $first = 1;
  }
  $last = $curr_link + $prev;
  if ($last > ceil($all / $lim)) {
    $last = ceil($all / $lim);
  }
  // first page
  $pervaya = "<a href='{$link2}?page=1'>-первая-</a> ";
  print $pervaya;
  if ($curr_link <= 1) {
    $i_pred = $curr_link = 1;
  }
  else {
    $i_pred = $curr_link - 1;
    $pred = "<a href='{$link2}?page={$i_pred}'> -предыдущая- </a> ";
    print $pred;
  }
  $y = 1;
  if ($first > 1) print "<a href='{$link2}?page={$y}'>1</a> ";
  // if there are a lot of pages, then the ellipsis.
  $y = $first - 1;
  if ($first > 10) {
    print "<a href='{$link2}?page={$y}'>...</a> ";
  }
  else {
    for($i = 2; $i < $first; $i++) {
      print "<a href='{$link2}?page={$i}'>$i</a> ";
    }
  }
  // Display the specified range: current page + - $ prev.
  for($i = $first; $i < $last + 1; $i++) {
    //If the current page is displayed, then it is assigned a special style css
    if($i == $curr_link) {
      ?>
      <span><?php print $i; ?></span>
      <?php
    }
    else {
      $alink = "<a href='{$link2}?page={$i}'>$i</a> ";
      print $alink;
    }
  }
  $y = $last + 1;
  //Ellipsis.
  if ($last < ceil($all / $lim) && ceil($all / $lim) - $last > 2) print "<a href='{$link2}?page={$y}'>...</a> ";
    //Show last page.
    $e = ceil($all / $lim);
  if ($last < ceil($all / $lim)) {
    print "<a href='{$link2}?page={$e}'>$e</a>";
  }
  $pos_page = ceil($all/$lim);
  if ($curr_link >= $pos_page) {
    $i_sled=$curr_link;
  }
  else {
    $i_sled = $curr_link + 1;
    $sled = "<a href='{$link2}?page={$i_sled}'> -следующая- </a> ";
    print $sled;
  }
  $pos_page = ceil($all/$lim);
  $posled = "<a href='{$link2}?page={$pos_page}'> -последняя- </a> ";
  print $posled;
}
