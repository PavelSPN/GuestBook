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
define('GUESTBOOK_SQL_PASS', 'root');
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
 * @return bool
 */
function guestbook_get_messages($order = 'DESC') {
  $query = mysqli_query(guestbook_sql_connect(), 'SELECT * FROM `share` ORDER BY id ' . $order);
  return $query->fetch_all();
}

/**
 * Returns user data.
 *
 * @param $login
 * @param $passw
 * @return bool
 */
function guestbook_get_user_data($login, $passw) {
  $query = mysqli_prepare(guestbook_sql_connect(), 'SELECT * FROM `users` WHERE login = ? AND password = ?');
  $query->bind_param('ss', $login, $passw);
  $query->execute();
  $result = $query->get_result();
  return $result->fetch_assoc();
}

/**
 * Saves messages.
 *
 * This function is used for creation new messages and edit already existed.
 *
 * @param bool $new
 */
function guestbook_save_message($new = TRUE) {

}

/**
 * Deletes messages.
 *
 * @param bool $new
 */
function guestbook_delete_message($new = TRUE) {

}
