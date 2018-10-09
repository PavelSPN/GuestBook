<?php

/**
 * @file
 * Includes main html.
 */

// Get all necessary theme variables.
guestbook_preprocess_theme($vars);
?>

<html>
  <head>
    <meta charset="utf-8" />
    <link rel="stylesheet" type="text/css" href="/css/guestbook.css" />

      <title>Guest book</title>
  </head>

  <body>
    <?php print $vars['header']; ?>
    <?php print $vars['login']; ?>
    <?php print $vars['messages_area']; ?>
  </body>

  <footer>
    <?php print $vars['pages_bar']; ?>
  </footer>
</html>
