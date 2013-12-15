<?php
function linkify( $text ) {
  $text = preg_replace('/(?!<\S)(\w+:\/\/[^<>\s]+\w)(?!\S)/i', '<a href="$1" target="_blank">$1</a>', $text);
  $text = preg_replace('/(?!<\S)#(\w+\w)(?!\S)/i', '<a href="http://twitter.com/search?q=#$1" target="_blank">#$1</a>', $text);
  $text = preg_replace('/(?!<\S)@(\w+\w)(?!\S)/i', '@<a href="http://twitter.com/$1" target="_blank">$1</a>', $text);
  return $text;
}
?>
<li><?php echo linkify($status->text) . '<br />' . $time_display; ?></li>