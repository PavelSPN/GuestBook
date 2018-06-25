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
  if (!isset($_GET['page'])) {
    $_GET['page'] = 1;
  }
  $all = guestbook_get_page();
  $pages = ceil($all / 10);
  if ($_GET['page'] == 1) {
    $gt = 0;
  }
  else {
    if ($_GET['page'] <= $pages) {
      $gt = ($_GET['page'] * 10) - 10;
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
 * @param int $uid
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
 * Returns user info by id.
 *
 * @param int $id
 * @return array.
 */
function guestbook_get_info($id) {
  $query = mysqli_prepare(guestbook_sql_connect(), 'SELECT * FROM `share` WHERE id = ?');
  $query->bind_param('i', $id);
  $query->execute();
  $result = $query->get_result();
  $result = $result->fetch_assoc();
  return $result;
}

/**
 * Returns user data.
 *
 * @param string $login
 * @param string $passw
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
  $query = mysqli_query(guestbook_sql_connect(), "SELECT COUNT(*) as count FROM `share`");
  return $query->fetch_row()[0];
}

/**
 * Saves messages.
 *
 * This function is used for creation new messages and edit already existed.
 *
 * @param bool $new
 */
function guestbook_save_message($uri) {
  // Get info from header.
  if (isset($uri[3])) {
    $id = $uri[3];
  }
  if (isset($uri[4])) {
    $page = $uri[4];
  }
  // Check user is logged.
  $user_is_logged = guestbook_user_is_logged();
  if ($user_is_logged === TRUE) {
    // Check id.
    if (isset($id)) {
      // Get info by id.
      $title = guestbook_get_info($id);
      if ($title['id']) {
        $new = FALSE;
      }
      else {
        // Show warning message if 'id' does not exist in database.
        try {
          throw new Exception("Сообщение не существует. Вернуться на главную страницу: " . "<a href='/'> Главная </a> ");
        }catch (Exception $e) {
          echo $e->getMessage();
        }
      }
    }
    else {
      $new = TRUE;
    }
    // Save message if new = true.
    if ($new == TRUE) {
      if (isset($_POST['message']) && $_POST['message'] != NULL) {
        $message = $_POST['message'];
        $uid = $_SESSION['user']['id'];
        $time = Time();
        $session_cookie = $_SESSION['user']['session_cookie'];
        $query = mysqli_prepare(guestbook_sql_connect(), 'INSERT INTO `share`(id, date, message, uid, session_cookie) VALUES(\'NULL\', ?, ?, ?, ?)');
        $query->bind_param('issi', $time, $message, $uid, $session_cookie);
        $query->execute();
        // Checking number of page in header.
        if (isset($page)) {
          $path = '/' . $page;
          // Redirect to page.
          guestbook_goto($path);
        }
        guestbook_goto();
      }
      guestbook_goto();
    }
    // Edit message if new = false.
    else {
      // Check message is not empty.
      if (isset($_POST['message'])) {
        $message = $_POST['message'];
        // Get user id.
        $uid = $_SESSION['user']['id'];
        // Prepare and execute message editing.
        $query = mysqli_prepare(guestbook_sql_connect(), 'UPDATE `share` SET message = ? , uid = ? WHERE id = ? ');
        $query->bind_param('ssi', $message, $uid, $id);
        $query->execute();
        // Checking number of page in header.
        if (isset($page)) {
          $path = '/' . $page;
          // Redirect to page.
          guestbook_goto($path);
        }
        else {
          guestbook_goto();
        }
      }
      guestbook_goto();
    }
  }
  // Redirect to main page if user is not logged.
  guestbook_goto();
}

/**
 * Deletes messages.
 *
 * @param array $uri
 */
function guestbook_delete_message($uri) {
  // Get info from header.
  if (isset($uri[3])) {
    $id = $uri[3];
  }
  if (isset($uri[4])) {
    $page = $uri[4];
  }
  // Check user is logged.
  $user_is_logged = guestbook_user_is_logged();
  if ($user_is_logged === TRUE) {
    // Check id.
    if (isset($id)) {
      // Get info by id.
      $title = guestbook_get_info($id);
      // Show warning message if 'id' does not exist in database.
      try {
        if (!$title['id']) {
          throw new Exception("Сообщение не существует. Вернуться на главную страницу: " . "<a href='/'> Главная </a> ");
        }
      }catch (Exception $e) {
        echo $e->getMessage();
      }
      // Checking cookies, that would not delete someone else's message.
      if ($_SESSION['user']['session_cookie'] == $title['session_cookie']) {
        // Deleting message.
        $query = mysqli_prepare(guestbook_sql_connect(), 'DELETE FROM `share` WHERE id= ? ');
        $query->bind_param('i', $id);
        $query->execute();
      }
    }
    // Checking number of page in header.
    if (isset($page)) {
      $path = '/' . $page;
      // Redirect to page.
      guestbook_goto($path);
    }
    else {
      guestbook_goto();
    }
  }
  // Redirect to main page if user is not logged.
  guestbook_goto();
}

