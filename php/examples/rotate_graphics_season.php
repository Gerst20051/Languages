<?php
function current_season() {
       // Locate the icons
       $icons = array(
               "spring" => "images/spring.png",
               "summer" => "images/summer.png",
               "autumn" => "images/autumn.png",
               "winter" => "images/winter.png"
       );

       // What is today's date - number
       $day = date("z");

       //  Days of spring
       $spring_starts = date("z", strtotime("March 21"));
       $spring_ends   = date("z", strtotime("June 20"));

       //  Days of summer
       $summer_starts = date("z", strtotime("June 21"));
       $summer_ends   = date("z", strtotime("September 22"));

       //  Days of autumn
       $autumn_starts = date("z", strtotime("September 23"));
       $autumn_ends   = date("z", strtotime("December 20"));

       //  If $day is between the days of spring, summer, autumn, and winter
       if( $day >= $spring_starts && $day <= $spring_ends ) :
               $season = "spring";
       elseif( $day >= $summer_starts && $day <= $summer_ends ) :
               $season = "summer";
       elseif( $day >= $autumn_starts && $day <= $autumn_ends ) :
               $season = "autumn";
       else :
               $season = "winter";
       endif;

       $image_path = $icons[$season];

       echo $image_path;
}
?>
<img src="<?php current_season() ?>" alt="" />
<?php
$day = date(“z”);
if( $day < 79) $season = "winter";
elseif( $day < 171) $season = "spring";
elseif( $day < 265) $season = "summer";
elseif( $day < 354) $season = "autumn";
else $season = "winter";
?>