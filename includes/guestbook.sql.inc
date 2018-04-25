<?php

/**
 * @file
 * Contains sql connection functions.
 */

/**
 * Mysql connection constants.
 */
define('GUESTBOOK_SQL_HOST', '');
define('GUESTBOOK_SQL_USER', 'root');
define('GUESTBOOK_SQL_PASS', '');
define('GUESTBOOK_SQL_DBNAME', 'guestbook');

/**
 * Connects to db.
 *
 * @return \mysqli|null
 */
function guestbook_sql_connect() {
  static $link = NULL;

  if (!$link) {
    $link = mysqli_connect(GUESTBOOK_SQL_HOST, GUESTBOOK_SQL_USER, GUESTBOOK_SQL_PASS, GUESTBOOK_SQL_DBNAME);
    if (!$link) {
      die('Connection failed: ' . mysqli_error($link));
    }
    mysqli_select_db($link, GUESTBOOK_SQL_DBNAME);
  }
  return $link;
}

/**
 * Returns messages.
 *
 * @param string $order
 * @return array
 */
function guestbook_get_messages($order = 'DESC') {
  if (!isset($_GET['guestbook_curr_link'])) {
    $_GET['guestbook_curr_link'] = 1;
  }
  $all = guestbook_get_page();
  $pages = ceil($all / 10);
  if ($_GET['guestbook_curr_link'] == 1) {
    $gt = 0;
  }
  else {
    if ($_GET['guestbook_curr_link'] <= $pages) {
      $gt = ($_GET['guestbook_curr_link'] * 10) - 10;
    }
    else {
      $gt = 0;
    }
  }
  $limit = ' LIMIT ' . $gt . ', 10';
  $query = mysqli_query(guestbook_sql_connect(), 'SELECT * FROM `share` ORDER BY id ' . $order . $limit);
  return $query->fetch_all();
}

/**
 * Returns user name.
 *
 * @param integer $uid
 * @return string.
 */
function guestbook_get_name($uid) {
  $query = mysqli_prepare(guestbook_sql_connect(), 'SELECT * FROM `users` WHERE id = ?');
  $query->bind_param('i', $uid);
  $query->execute();
  $result = $query->get_result();
  $result = $result->fetch_assoc();
  return $result['name'];
}

/**
 * Returns user data.
 *
 * @param $login
 * @param $passw
 * @return array
 */
function guestbook_get_user_data($login, $passw) {
  $passw = sha1($passw);
  $query = mysqli_prepare(guestbook_sql_connect(), 'SELECT * FROM `users` WHERE login = ? AND password = ?');
  $query->bind_param('ss', $login, $passw);
  $query->execute();
  $result = $query->get_result();
  return $result->fetch_assoc();
}

/**
 * Returns num rows.
 *
 * @return int.
 */
function guestbook_get_page() {
  $query = mysqli_query(guestbook_sql_connect(), 'SELECT * FROM `share`');
  return $query->num_rows;
}

/**
 * Saves messages.
 *
 * This function is used for creation new messages and edit already existed.
 *
 * @param bool $new
 */
function guestbook_save_message($new = TRUE) {
  if (isset($_GET['edit']) && $_GET['edit'] == 1) {
    $new = FALSE;
  }
  if ($new == TRUE) {
    if (isset($_POST['Guestbook_user_message_window']) && $_POST['Guestbook_user_message_window'] != NULL) {
      $message = $_POST['Guestbook_user_message_window'];
      $uid = $_SESSION['user']['id'];
      $time = Time();
      $session_cookie = $_SESSION['user']['session_cookie'];
      $query = mysqli_prepare(guestbook_sql_connect(), 'INSERT INTO `share`(id, date, message, uid, session_cookie) VALUES(\'NULL\', ?, ?, ?, ?)');
      $query->bind_param('issi', $time, $message, $uid, $session_cookie);
      $query->execute();
      if (isset($_SESSION['guestbook_curr_link'])) {
        $gcl = $_SESSION['guestbook_curr_link'];
        guestbook_goto($gcl);
      }
      guestbook_goto();
    }
    guestbook_goto();
  }
  if ($new == FALSE) {
    if (isset($_POST['Guestbook_user_message_window']) && isset($_GET['id'])) {
      $message = $_POST['Guestbook_user_message_window'];
      $uid = $_SESSION['user']['id'];
      $id = $_GET['id'];
      $query = mysqli_prepare(guestbook_sql_connect(), 'UPDATE `share` SET message = ? , uid = ? WHERE id = ? ');
      $query->bind_param('ssi', $message, $uid, $id);
      $query->execute();
      if (isset($_GET['guestbook_curr_link'])) {
        header('Location: /?guestbook_curr_link=' . $_GET['guestbook_curr_link']);
      }
      else {
        guestbook_goto();
      }

    }
  }
}

/**
 * Deletes messages.
 *
 * @param bool $new
 */
function guestbook_delete_message() {
  if (isset($_GET['edit']) && $_GET['edit'] == 2) {
    if (isset($_GET['id'])) {
      $id = $_GET['id'];
    }
    else {
      header('Location: /');
    }
    $query = mysqli_prepare(guestbook_sql_connect(), 'DELETE FROM `share` WHERE id= ? ');
    $query->bind_param('i', $id);
    $query->execute();
    if (isset($_GET['guestbook_curr_link'])) {
      header('Location: /?guestbook_curr_link=' . $_GET['guestbook_curr_link']);
    }
    else {
      header('Location: /');
    }
  }
}
