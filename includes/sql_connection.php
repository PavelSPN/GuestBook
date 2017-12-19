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
 * @param $link
 *   Link to db connection.
 */
function guestbook_sql_connect(&$link) {
  $link = mysqli_connect(GUESTBOOK_SQL_HOST, GUESTBOOK_SQL_USER, GUESTBOOK_SQL_PASS, GUESTBOOK_SQL_DBNAME);
  if (!$link) {
    die('Connection failed: ' . mysqli_error($link));
  }
  mysqli_select_db($link, GUESTBOOK_SQL_DBNAME);
}

/**
 * Returns messages.
 *
 * @param string $order
 * @return bool
 */
function guestbook_get_messages($order = 'DESC') {
  guestbook_sql_connect($link);
  $query = mysqli_prepare($link, 'SELECT * FROM {share} ORDER BY id :order');
  $query->bind_param(':order', $order);
  $result = $query->execute();
  return $result;
}

/**
 * Select all from 'share' by id from the end.
 */
function select_all_by_id_desc() {
  global $sql;
  global $result;
	$sql = ("SELECT * FROM `share` ORDER BY `id`  DESC");
  $result = mysql_query($sql) or die("Ошибка " . mysql_error($link));
}
/**
 * Select all from 'share' by id from the end. LIMIT 10.
 *
 * @param $gt
 *   Integer. Line to start the selection.
 */
function select_all_by_id_desc_LIMIT_10($gt) {
	global $sql;
	global $result;
	$sql = ("SELECT * FROM `share` ORDER BY id DESC LIMIT $gt,10 ");
  $result = mysql_query($sql) or die("Ошибка " . mysql_error($link));
}
/**
 * Select all from 'share' where id = $_GET[id].
 */
function select_all_WHERE_id_GETid() {
  global $row;
	$id = $_GET[id];
	$id = mysql_real_escape_string($id);
	$row = mysql_fetch_row(mysql_query("SELECT * FROM `share` WHERE `id`=$id"));
}
/**
 * Update table where id = $_SESSION[id2].
 *
 * $post, $name - variables using in the file send.php.
 * $post - user message(string).
 * $name - name and surname of the user from the database according
 * to the id of the user.
 */
function sql_update() {
  global $sql;
  global $post;
  global $name;
	$post2 = $post;
	$name2 = $name;
	$id2 = $_SESSION[id2];
	$name2 = mysql_real_escape_string($name2);
	$post2 = mysql_real_escape_string($post2);
	$id2 = mysql_real_escape_string($id2);
	$sql = "UPDATE share SET  date = CURRENT_DATE(),message = '$post2' ,name = '$name2' WHERE id = $id2";
  mysql_query($sql)or die("Ошибка " . mysql_error($link));
  $_SESSION['edit'] = 0;
  print "<html><head>
<meta HTTP-EQUIV='Refresh' CONTENT='0; URL=login.php?page={$_SESSION[page2]}'>
</head></html>";
  exit;
}
/**
 * Insert message into table.
 *
 * $post, $name - variables using in the file send.php.
 * $post - user message(string).
 * $name - name and surname of the user from the database according
 * to the id of the user.
 */
function INSERT_INTO_share() {
  global $post;
  global $name;
	$post2 = $post;
	$name2 = $name;
	$name2 = mysql_real_escape_string($name2);
	$post2 = mysql_real_escape_string($post2);
  $sql = "INSERT INTO share(id, date, message, name) VALUES('NULL',CURRENT_DATE(),'$post2','$name2')";
  mysql_query($sql) or die("Ошибка " . mysql_error($link));
  $id = mysql_insert_id();
  $_SESSION['id'] = $id;
  print "<html><head>
<meta HTTP-EQUIV='Refresh' CONTENT='0; URL=login.php?page=1'>
</head></html>";
}
