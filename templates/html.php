<meta charset="utf-8">
<link rel="stylesheet" type="text/css" href="../css/gbstyle.css" />
<title>Guest book</title>
<?php

/**
 * @file
 * Contains html functions.
 */

/**
 * LogOut.
 *
 * Button for logout with the name and surname of the user.
 * $name - $_SESSION['name'] from connection.php file.
 */
function logout() {
  GLOBAL $name
  ?>
  <div class="blockloguot">
  <form name="logOut" action="../templates/logout.php" method="post">
  <label>
  <?php
  print $name;
  ?>
  </label>
  <input type="submit" name="signOut" value="Sign out" />
  </form>
  </div>
  <?php
}
/**
 * LogIn.
 *
 * Login and password form.
 */
function login() {
  ?>
  <div class="blocklogin">
  <form name="logIn" action="../templates/connection.php" method="post">
  <label>Login</label>
  <input type="text" name="MemberLogin" placeholder="login" /><br /><br />
  <label>Password</label>
  <input type="password" name="MemberPassword" placeholder="password" /><br /><br />
  <input type="submit" name="signIn" value="Sign In" />
  </form>
  </div>
<?php
}
