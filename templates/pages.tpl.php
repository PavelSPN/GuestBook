<?php

/**
 * @file
 * Includes pages block.
 */

?>
<div class="pages">
  <ul class="pagination">
    <?php $pages = $variables['pages_array']; ?>
    <li><a href="?page=<?php print $pages['first']; ?>"> &laquoпервая</a></li>
    <?php if (is_int ($pages['previous'])): ?>
      <li><?php print "<a href='?page={$pages['previous']}'> &laquoпредыдущая</a> "; ?></li>
    <?php endif; ?>
    <?php foreach ($pages as $key => $value): ?>
      <?php if (is_int ($key)): ?>
        <?php if ($value == $pages['page']): ?>
          <li class="active"><?php print "<a href='?page={$value}'>$value</a> "; ?></li>
        <?php else: ?>
          <li><?php print "<a href='?page={$value}'>$value</a> "; ?></li>
        <?php endif; ?>
      <?php endif; ?>
    <?php endforeach; ?>
    <?php if (is_int ($pages['next'])): ?>
      <li><?php print "<a href='?page={$pages['next']}'> следующая&raquo</a> "; ?></li>
    <?php endif; ?>
    <li><?php print "<a href='?page={$pages['last']}'> последняя&raquo</a> "; ?></li>
  </ul>
    <div align="center"> Павел Сержанин </div>
</div>