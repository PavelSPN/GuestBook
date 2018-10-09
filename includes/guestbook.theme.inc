<?php

/**
 * @file
 * Includes preprocess functions for theme of guestbook.
 */

/**
 * Preprocess function for main html file.
 *
 * These variables will be passed to main html file.
 *
 * @param $vars
 */
function guestbook_preprocess_theme(&$vars) {
  $vars['messages'] = guestbook_get_messages(); // Get messages from database.
  $vars['is_logged'] = guestbook_user_is_logged(); // True if user is logged.
  $vars['name'] = isset($_SESSION['user']['name']) ? $_SESSION['user']['name'] : NULL; // Get logged user name.
  $vars['pages_array'] = guestbook_pagination();  // Get pages array.
  $vars['header'] = guestbook_render_template('/templates/header.phtml', $vars); // Include block "header" to html file.
  $vars['login'] = guestbook_render_template('templates/login.phtml', $vars); // Include block "login" to html file.
  $vars['messages_area'] = guestbook_render_template('templates/messages.phtml', $vars); // Include block "messages" to html file.
  $vars['pages_bar'] = guestbook_render_template('/templates/pages.tpl.php', $vars); // Include block "pages" to html file.
}
