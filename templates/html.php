<?php
/**
 * @file
 * Includes main html.
 */
?>

<?php // Get all necessary theme variables. ?>
<?php guestbook_preprocess_theme($vars); ?>

<html>
  <head>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="../css/guestbook.css" />
    <title>Guest book</title>
  </head>

  <body>
  <h1>Guest Book</h1>

  <?php // Show login block if user is not logged, otherwise - "logout" and name. ?>
  <?php if ($vars['is_logged']): ?>
    <div class="blockloguot">
      <form name="logOut" action="/logout" method="POST">
        <label><?php print $vars['name']; ?></label>
        <input type="submit" name="signout" value="Sign out" />
      </form>
    </div>
  <?php else: ?>
    <div class="blocklogin">
      <form name="logIn" action="/login" method="POST">
        <div>
          <label>Login</label>
          <input type="text" name="login" placeholder="login"/>
        </div>
        <div>
          <label>Password</label>
          <input type="password" name="password" placeholder="password"/>
        </div>
        <input class="submit" type="submit" name="signin" abc="asdasd" value="Sign In"/>
      </form>
    </div>
  <?php endif; ?>

  <div class="guestbook-messages">
    <?php if ($vars['messages']): ?>
      <?php foreach ($vars['messages'] as $message): ?>
        <div><?php print $message[2]; ?></div>
      <?php endforeach; ?>
    <?php endif; ?>
  </div>

  </body>
</html>
