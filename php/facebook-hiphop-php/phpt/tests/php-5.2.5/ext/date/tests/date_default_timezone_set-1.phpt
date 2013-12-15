--TEST--
date_default_timezone_set() function [1]
--INI--
date.timezone=
--FILE--
<?php
///	putenv("TZ=");
	$date1 = strtotime("2005-01-12 08:00:00");
	$date2 = strtotime("2005-07-12 08:00:00");
	date_default_timezone_set("America/Indiana/Knox");
	$date3 = strtotime("2005-01-12 08:00:00");
	$date4 = strtotime("2005-07-12 08:00:00");

	echo date_default_timezone_get(), "\n";
	echo date(DATE_ISO8601, $date1), "\n";
	echo date(DATE_ISO8601, $date2), "\n";
	echo date(DATE_ISO8601, $date3), "\n";
	echo date(DATE_ISO8601, $date4), "\n";
?>
--EXPECTF--
Strict Standards: strtotime(): It is not safe to rely on the system's timezone settings. Please use the date.timezone setting, the TZ environment variable or the date_default_timezone_set() function. In case you used any of those methods and you are still getting this warning, you most likely misspelled the timezone identifier. We selected 'UTC' for 'UTC/0.0/no DST' instead in %sdate_default_timezone_set-1.php on line 3

Strict Standards: strtotime(): It is not safe to rely on the system's timezone settings. Please use the date.timezone setting, the TZ environment variable or the date_default_timezone_set() function. In case you used any of those methods and you are still getting this warning, you most likely misspelled the timezone identifier. We selected 'UTC' for 'UTC/0.0/no DST' instead in %sdate_default_timezone_set-1.php on line 4
America/Indiana/Knox
2005-01-12T03:00:00-0500
2005-07-12T03:00:00-0500
2005-01-12T08:00:00-0500
2005-07-12T08:00:00-0500
