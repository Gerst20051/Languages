CREATE DATABASE `netai` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
 CREATE  TABLE  `netai`.`empty` (  `id` int( 10  )  unsigned NOT  NULL  AUTO_INCREMENT ,
 PRIMARY  KEY (  `id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`empty` SELECT * FROM `misc`.`empty`;

DROP TABLE `misc`.`empty`;
 CREATE  TABLE  `netai`.`hnscontacts` (  `user_id` int( 10  )  unsigned NOT  NULL ,
 `contacts` text NOT  NULL ,
 PRIMARY  KEY (  `user_id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`hnscontacts` SELECT * FROM `misc`.`hnscontacts`;

DROP TABLE `misc`.`hnscontacts`;
 CREATE  TABLE  `netai`.`hnsdesktop` (  `user_id` int( 10  )  unsigned NOT  NULL ,
 `notepad` mediumtext NOT  NULL ,
 `theme_id` int( 2  )  NOT  NULL DEFAULT  '1',
 `bg_color` varchar( 20  )  NOT  NULL ,
 `wallpaper_file` varchar( 50  )  NOT  NULL ,
 `wallpaper_position` varchar( 20  )  NOT  NULL ,
 `wallpaper_repeat` varchar( 20  )  NOT  NULL ,
 `font_color` varchar( 20  )  NOT  NULL ,
 `transparency` int( 1  )  NOT  NULL DEFAULT  '1',
 `screensaver` int( 1  )  NOT  NULL DEFAULT  '1',
 `screensaver_time` int( 3  )  NOT  NULL ,
 `yt_playlist` text NOT  NULL ,
 `music_song` varchar( 255  )  NOT  NULL ,
 `browser_home` varchar( 100  )  NOT  NULL ,
 `browser_favorites` varchar( 255  )  NOT  NULL ,
 `browser_history` varchar( 255  )  NOT  NULL ,
 `players` varchar( 255  )  NOT  NULL ,
 `twttr_playlist` text NOT  NULL ,
 `launcher_autorun` varchar( 255  )  NOT  NULL ,
 `launcher_thumbs` varchar( 255  )  NOT  NULL ,
 `launcher_startmenuapps` varchar( 255  )  NOT  NULL ,
 `launcher_startmenutools` varchar( 255  )  NOT  NULL ,
 `launcher_quickstart` varchar( 255  )  NOT  NULL ,
 `launcher_tray` varchar( 255  )  NOT  NULL ,
 `app_documents` varchar( 100  )  NOT  NULL ,
 `app_preferences` varchar( 100  )  NOT  NULL ,
 `app_notepad` varchar( 100  )  NOT  NULL ,
 `app_flash_name` varchar( 100  )  NOT  NULL ,
 `app_ytinstant` varchar( 100  )  NOT  NULL ,
 `app_piano` varchar( 100  )  NOT  NULL ,
 `app_about_hnsdesktop` varchar( 100  )  NOT  NULL ,
 `app_feedback` varchar( 100  )  NOT  NULL ,
 `app_tic_tac_toe` varchar( 100  )  NOT  NULL ,
 `app_friends` varchar( 100  )  NOT  NULL ,
 `app_goom_radio` varchar( 100  )  NOT  NULL ,
 `app_search` varchar( 100  )  NOT  NULL ,
 `app_chat` varchar( 100  )  NOT  NULL ,
 `app_music` varchar( 100  )  NOT  NULL ,
 `app_web_browser` varchar( 100  )  NOT  NULL ,
 `app_calendar` varchar( 100  )  NOT  NULL ,
 `app_app_explorer` varchar( 100  )  NOT  NULL ,
 `app_calculator` varchar( 100  )  NOT  NULL ,
 `app_twitter` varchar( 100  )  NOT  NULL ,
 UNIQUE  KEY  `uid` (  `user_id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`hnsdesktop` SELECT * FROM `misc`.`hnsdesktop`;

DROP TABLE `misc`.`hnsdesktop`;
 CREATE  TABLE  `netai`.`info` (  `user_id` int( 10  )  NOT  NULL ,
 `firstname` varchar( 20  )  COLLATE latin1_general_ci NOT  NULL ,
 `middlename` varchar( 20  )  COLLATE latin1_general_ci NOT  NULL ,
 `lastname` varchar( 20  )  COLLATE latin1_general_ci NOT  NULL ,
 `email` varchar( 50  )  COLLATE latin1_general_ci NOT  NULL ,
 `gender` enum(  'Male',  'Female'  )  COLLATE latin1_general_ci NOT  NULL ,
 `birth_month` int( 2  )  NOT  NULL ,
 `birth_day` int( 2  )  NOT  NULL ,
 `birth_year` int( 4  )  NOT  NULL ,
 `hometown` varchar( 50  )  COLLATE latin1_general_ci NOT  NULL ,
 `current_city` varchar( 50  )  COLLATE latin1_general_ci NOT  NULL ,
 `images` text COLLATE latin1_general_ci NOT  NULL ,
 PRIMARY  KEY (  `user_id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1 COLLATE  = latin1_general_ci;

INSERT INTO `netai`.`info` SELECT * FROM `misc`.`info`;

DROP TABLE `misc`.`info`;
 CREATE  TABLE  `netai`.`login` (  `user_id` int( 10  )  unsigned NOT  NULL  AUTO_INCREMENT ,
 `username` varchar( 20  )  NOT  NULL ,
 `pass` varchar( 41  )  NOT  NULL ,
 `access_level` tinyint( 1  )  NOT  NULL DEFAULT  '1',
 `last_login` date NOT  NULL ,
 `date_joined` date NOT  NULL ,
 `logins` int( 11  )  NOT  NULL ,
 PRIMARY  KEY (  `user_id`  ) ,
 UNIQUE  KEY  `username` (  `username`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`login` SELECT * FROM `misc`.`login`;

DROP TABLE `misc`.`login`;
 CREATE  TABLE  `netai`.`passportparking` (  `id` int( 10  )  unsigned NOT  NULL  AUTO_INCREMENT ,
 `input` varchar( 255  )  NOT  NULL ,
 `output` varchar( 255  )  NOT  NULL ,
 PRIMARY  KEY (  `id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`passportparking` SELECT * FROM `misc`.`passportparking`;

DROP TABLE `misc`.`passportparking`;
 CREATE  TABLE  `netai`.`voting` (  `id` int( 10  )  unsigned NOT  NULL  AUTO_INCREMENT ,
 `title` varchar( 255  )  NOT  NULL ,
 `data` text NOT  NULL ,
 PRIMARY  KEY (  `id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`voting` SELECT * FROM `misc`.`voting`;

DROP TABLE `misc`.`voting`;
 CREATE  TABLE  `netai`.`websurprise` (  `id` int( 10  )  unsigned NOT  NULL  AUTO_INCREMENT ,
 `timestamp` int( 15  )  NOT  NULL ,
 `website` varchar( 255  )  NOT  NULL ,
 PRIMARY  KEY (  `id`  )  ) ENGINE  =  MyISAM  DEFAULT CHARSET  = latin1;

INSERT INTO `netai`.`websurprise` SELECT * FROM `misc`.`websurprise`;

DROP TABLE `misc`.`websurprise`;
DROP DATABASE `misc`;