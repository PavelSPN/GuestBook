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
  return '/' . ltrim($uri, '/');
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
 * @return array
 */
function guestbook_menu_mapping() {
  return array(
    '/login' => array(
      'page callback' => 'guestbook_login',
    ),
    // @todo Create callback for logout.
    '/logout' => array(
      'page callback' => 'guestbook_logout',
    ),
  );
}

/**
 * Menu handler.
 */
function guestbook_menu_hanlder() {
  $mapping = guestbook_menu_mapping();
  if (isset($mapping[guestbook_request_uri()])) {
    $menu = $mapping[guestbook_request_uri()];
    if (function_exists($menu['page callback'])) {
      $menu['page callback']();
    }
  }
}

/**
 * User login callback.
 */
function guestbook_login() {
  global $user;

  // Verify that we have all necessary POST data.
  if (!isset($_POST['login'], $_POST['password'])) {
    return;
  }

  // Verify that user is exist and verified successfully.
  $user_data = guestbook_get_user_data($_POST['login'], $_POST['password']);
  if (empty($user_data)) {
    return;
  }
  // Do not store password in session.
  unset($user_data['password']);

  $user = $user_data + array(
    'time'=> time(),
    'sid' => session_id(),
    'session_cookie' => '',
  );

  $_SESSION['user'] = $user;
  guestbook_goto();
}
