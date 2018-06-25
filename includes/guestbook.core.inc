<?php

/**
 * @file
 * Includes core functions for guestbook.
 */

/**
 * Returns the equivalent of Apache's $_SERVER['REQUEST_URI'] variable.
 *
 * Because $_SERVER['REQUEST_URI'] is only available on Apache, we generate an
 * equivalent using other environment variables.
 */
function guestbook_request_uri() {
  if (isset($_SERVER['REQUEST_URI'])) {
    $uri = $_SERVER['REQUEST_URI'];
  }
  else {
    $uri = $_SERVER['SCRIPT_NAME'];
    if (isset($_SERVER['argv'])) {
      $uri = $_SERVER['SCRIPT_NAME'] . '?' . $_SERVER['argv'][0];
    }
    elseif (isset($_SERVER['QUERY_STRING'])) {
      $uri = $_SERVER['SCRIPT_NAME'] . '?' . $_SERVER['QUERY_STRING'];
    }
  }
  // Prevent multiple slashes to avoid cross site requests via the Form API.
  $uri = preg_split("[/]", $uri);
  $uri[1] = '/' . $uri[1];
  return $uri;
}

/**
 * Redirect callback.
 *
 * @param string $path
 * @param int $http_response_code
 */
function guestbook_goto($path = '', $http_response_code = 302) {
  $path = empty($path) ? '/' : $path;
  header('Location: ' . $path, TRUE, $http_response_code);
  exit();
}

/**
 * Returns whether a session has been started.
 */
function guestbook_session_started($set = NULL) {
  static $session_started = FALSE;
  if (isset($set)) {
    $session_started = $set;
  }
  return $session_started && session_id();
}

/**
 * Starts a session.
 */
function guestbook_session_start() {
  if (!guestbook_session_started()) {
    // Save current session data before starting it, as PHP will destroy it.
    $session_data = isset($_SESSION) ? $_SESSION : NULL;

    session_start();
    guestbook_session_started(TRUE);

    // Restore session data.
    if (!empty($session_data)) {
      $_SESSION += $session_data;
    }
  }
}

/**
 * Starts a session time.
 */
function guestbook_session_time() {
  if (guestbook_user_is_logged() && !isset($_SESSION['time'])) {
    $_SESSION['time'] = time();
  }
}

/**
 * Verifies is user is already logged.
 *
 * @return bool
 */
function guestbook_user_is_logged() {
  $is_logged = FALSE;
  if (isset($_SESSION['user']['id'], $_SESSION['user']['login']) && session_id() == $_SESSION['user']['sid']) {
    $is_logged = TRUE;
  }
  return $is_logged;
}

/**
 * Mapping.
 *
 * @return array
 */
function guestbook_menu_mapping() {
  return array(
    // @todo Create callback.
    '/login' => array(
      'page callback' => 'guestbook_login',
    ),
    '/logout' => array(
      'page callback' => 'guestbook_logout',
    ),
    '/message/delete' => array(
      'page callback' => 'guestbook_delete_message',
    ),
    '/message/save' => array(
      'page callback' => 'guestbook_save_message',
    ),
   );
}

/**
 * Menu handler.
 */
function guestbook_menu_handler() {
  $uri = guestbook_request_uri();
  // Get info from header.
  if (isset($uri[1])) {
    $key = $uri[1];
  }
  if (isset($uri[2])) {
    $key = $uri[1] . '/' . $uri[2];
  }
  $mapping = guestbook_menu_mapping();
  if (isset($mapping[$key])) {
    $menu = $mapping[$key];
    if (function_exists($menu['page callback'])) {
      $menu['page callback']($uri);
    }
  }
}

/**
 * User login callback.
 */
function guestbook_login() {
  global $user;
  guestbook_setcookie();
  // Verify that user is exist and verified successfully.
  $user_data = guestbook_get_user_data($_POST['login'], $_POST['password']);
  if (empty($user_data)) {
    guestbook_goto();
  }
  // Do not store password in session.
  unset($user_data['password']);
  $user = $user_data + array(
      'time'=> time(),
      'sid' => session_id(),
      'session_cookie' => rand() + rand(),
      );
  $_SESSION['user'] = $user;
  guestbook_goto();
}

/**
 * User logout callback.
 */
function guestbook_logout() {
    guestbook_unsetcookie();
    guestbook_goto();
}

/**
 * Set a cookie.
 */
function guestbook_setcookie() {
  setcookie('guestbook_cookie_id', session_id());
}

/**
 * Unset a cookie.
 */
function guestbook_unsetcookie() {
  unset ($_SESSION['user']);
  unset ($_SESSION['time']);
  setcookie('guestbook_cookie_id');
}

/**
 * Pagination.
 *
 * Create array of pages.
 * Used $_GET['page'] as current page.
 *
 * @param int $lim
 *  Number of displayed messages on the page.
 * @param int $prev
 *  Number of displayed pages before and after the current page number.
 * @param int $total_rows
 *  Number of rows in the database.
 * @return array.
 */
function pagination($lim = 10, $prev = 2, $total_rows = 1){
  $pages = array();
  // Count the total number of pages.
  $total_pages = (int) ceil($total_rows / $lim);
  if (guestbook_get_page()) {
    $total_pages = (int) ceil(guestbook_get_page() / $lim);
  }
  // Set the current page number.
  if (!isset($_GET['page']) || ($_GET['page'] > $total_pages) || ($_GET['page'] < 1)) {
    $_GET['page'] = 1;
  }
  $page = (int) $_GET['page'];
  // Calculating the range of pages.
  if ($page + $prev >= $total_pages) {
      $page_last = $total_pages;
  }
  else if ($page + $prev <= ($prev *2) + 1) {
    $page_last = ($prev *2) + 1;
  }
  else {
    $page_last = $page + $prev;
  }
  if ($page - $prev <= 1) {
    $page_first = 1;
  }
  else {
    $page_first = $page_last - ($prev *2);
  }
  $range = $page_last - $page_first;
  // Creating an array of pages.
  for ($i = 0; $i <= $range; $i++) {
      $pages[$i] = (int) $page_first + $i;
  }
  $pages['page'] = $page;
  $pages['first'] = 1;
  if ($page - 1 >= 1) {
    $pages['previous'] = $page - 1;
  }
  else {
    $pages['previous'] = 'do not show if string';
  }
  if ($page < $total_pages) {
    $pages['next'] = $page + 1;
  }
  else {
    $pages['next'] = 'do not show if string';
  }
  $pages['last'] = $total_pages;
  // Output page array.
  return $pages;
}
