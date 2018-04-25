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
  $uri = ltrim($uri, '/');
  $uri = preg_split("[/]", $uri);
  $uri2 = $uri[0];
  // redirect $_GET
  if (isset ($uri[1])) {
      $_GET['edit'] = $uri[1];
  }
  if (isset ($uri[2])) {
    $_GET['id'] = $uri[2];
  }
  if (isset ($uri[3])) {
    $_GET['guestbook_curr_link'] = $uri[3];
  }
  return $uri2;
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
    // @todo Create callback for login.
    'login' => array(
      'page callback' => 'guestbook_login',
    ),
    // @todo Create callback for logout.
    'logout' => array(
      'page callback' => 'guestbook_logout',
    // @todo Create callback for delete message.
    ),
    'delete_message' => array(
      'page callback' => 'guestbook_delete_message',
    // @todo Create callback for save message.
    ),
    'guestbook_sm' => array(
      'page callback' => 'guestbook_save_message',
    ),
  );
}

/**
 * Menu handler.
 */
function guestbook_menu_handler() {
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
  guestbook_setcookie();
  // Verify that user is exist and verified successfully.
  $user_data = guestbook_get_user_data($_POST['login'], $_POST['password']);
  if (empty($user_data)) {
    guestbook_goto('/?guestbook_curr_link=1');
  }
  // Do not store password in session.
  //unset($user_data['password']);
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
  global $user;
  // Verify that we have all necessary POST data.
  if (isset($_POST['signout'])) {
    unset ($_SESSION['user']);
    unset ($_SESSION['time']);
    guestbook_unsetcookie();
    guestbook_goto();
   }
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
  setcookie('guestbook_cookie_id');
}

/**
 * Pagination.
 *
 * Create buttons to switch pages.
 * Used $_GET['guestbook_curr_link'] as current page.
 *
 * @param $lim
 *  Integer. Number of displayed links before and after the current page number.
 * @param $prev
 *  Integer. Number of displayed links before and after the current page number.
 * @param $link2
 *  A string containing a part of the URL used to create links to other pages.
 */
function pagination($lim = 10, $prev = 3, $link2 = '/'){
  // Check that the displayed first and last pages do not exceed the numbering limits.
  $all = guestbook_get_page();
  if (!isset($_GET['guestbook_curr_link'])) {
    $_GET['guestbook_curr_link'] = 1;
  }
  if ($_GET['guestbook_curr_link'] > ceil($all / $lim)) {
    $_GET['guestbook_curr_link'] = 1;
  }
  $curr_link = $_GET['guestbook_curr_link'];
  $first = $curr_link - $prev;
  if ($first <= 1) {
    $first = 1;
  }
  $last = $curr_link + $prev;
  if ($last > ceil($all / $lim)) {
    $last = ceil($all / $lim);
  }
  // first page
  $pervaya = "<a href='{$link2}?guestbook_curr_link=1'>-первая-</a> ";
  print $pervaya;
  if ($curr_link <= 1) {
    $i_pred = $curr_link = 1;
  }
  else {
    $i_pred = $curr_link - 1;
    $pred = "<a href='{$link2}?guestbook_curr_link={$i_pred}'> -предыдущая- </a> ";
    print $pred;
  }
  $y = $first;
  if ($first > 10) {
    print "<a href='{$link2}?guestbook_curr_link={$y}'>...</a> ";
  }
  else {
    for($i = 2; $i < $first; $i++) {
      print "<a href='{$link2}?guestbook_curr_link={$i}'>$i</a> ";
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
      $alink = "<a href='{$link2}?guestbook_curr_link={$i}'>$i</a> ";
      print $alink;
    }
  }
  $y = $last + 1;
  //Ellipsis.
  if ($last < ceil($all / $lim) && ceil($all / $lim) - $last > 2) print "<a href='{$link2}?guestbook_curr_link={$y}'>...</a> ";
  //Show last page.
  $e = ceil($all / $lim);
  if ($last < ceil($all / $lim)) {
    print "<a href='{$link2}?guestbook_curr_link={$e}'>$e</a>";
  }
  $pos_page = ceil($all/$lim);
  if ($curr_link >= $pos_page) {
    $i_sled=$curr_link;
  }
  else {
    $i_sled = $curr_link + 1;
    $sled = "<a href='{$link2}?guestbook_curr_link={$i_sled}'> -следующая- </a> ";
    print $sled;
  }
  $pos_page = ceil($all/$lim);
  $posled = "<a href='{$link2}?guestbook_curr_link={$pos_page}'> -последняя- </a> ";
  print $posled;
}
