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
  $vars['messages'] = guestbook_get_messages(); //записывает все сообщения из книги.
  $vars['is_logged'] = guestbook_user_is_logged(); //записывается true если пользователь залогинен.
  $vars['name'] = isset($_SESSION['user']['name']) ? $_SESSION['user']['name'] : NULL; //краткая запись if.
  //если isset($_SESSION['user']['name']) существует, то записать значение $_SESSION['user']['name'] иначе NULL.
}
