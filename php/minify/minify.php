<?php
require_once('minify/min/lib/minify.php');

// Create new Minify objects.
$minifyCSS = new Minify(TYPE_CSS);
$minifyJS = new Minify(TYPE_JS);

// Specify the files to be minified.
// Full URLs are allowed as long as they point
// to the same server running Minify.
$minifyCSS->addFile(array(
'/styles/fonts.css',
'/styles/nifty.css',
'/styles/public.css',
'/styles/calendar.css',
'/styles/navigation.css'
));

$minifyJS->addFile(array(
'/scripts/jquery.js',
'/scripts/nifty.js',
'/scripts/public.js'
));
?>

<html>

<head>
<style type="text/css"><?php
echo $minifyCSS->combine();
?></style>
<script type="text/javascript"><?php
echo $minifyJS->combine();
?></script>
</head>

<body>
</body>

</html>