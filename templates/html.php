<?php
/**
 * @file
 * Includes main html.
 */
?>

<?php require_once GUESTBOOK_ROOT . '/includes/sql_connection.php'; ?>
<?php
$a = guestbook_get_messages();
?>

<html>
  <head>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="../css/gbstyle.css" />
    <title>Guest book</title>
  </head>

  <body>
  <h1>Guest Book</h1>
  <?php if ($is_logged): ?>
    <div class="blockloguot">
      <form name="logOut" action="../templates/logout.php" method="post">
        <label><?php print $name; ?></label>
        <input type="submit" name="signOut" value="Sign out" />
      </form>
    </div>
  <?php else: ?>
    <div class="blocklogin">
      <form name="logIn" action="../templates/connection.php" method="post">
        <div>
          <label>Login</label>
          <input type="text" name="MemberLogin" placeholder="login"/>
        </div>
        <div>
          <label>Password</label>
          <input type="password" name="MemberPassword" placeholder="password"/>
        </div>
        <input class="submit" type="submit" name="signIn" value="Sign In"/>
      </form>
    </div>
  <?php endif; ?>

  </body>
</html>
