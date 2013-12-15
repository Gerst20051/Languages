<?php
//CREATE TABLE `admin` (
//  `id` TINYINT NOT NULL AUTO_INCREMENT, 
//  `name` VARCHAR(16) NOT NULL, 
//  `password` VARCHAR(16) NOT NULL,
//  PRIMARY KEY (`id`),
//  UNIQUE (`name`)
//);

function IsAdminLogin() {
	$res = mysql_query("SELECT `name` FROM `admin` WHERE `name`= '".addslashes($_SESSION['user'])."' AND `password`= '".md5($_SESSION['pass'])."' LIMIT 1;");
	//if ((ADMIN_USER == $_SESSION['user']) && (ADMIN_PASS == $_SESSION['pass']))
	if (@mysql_fetch_assoc($res))
		$GLOBALS['admin'] = TRUE;
	else
		$GLOBALS['admin'] = FALSE;
}



function da($array) {
	echo LNtoBR(print_r($array, 1));
}

function ShortW($str, $nrw) {
	$tmp = explode(' ', $str);
	for ($i = 0; $i < $nrw; $i++) {
		$ret.= $tmp[$i] . ' ';
	}
	if ($ret != '') return ($ret . ' ...');
}



function sqlesc($text) {
	$text = stripslashes($text);
	$text = str_replace(
		Array("\\", "'"),
		Array("\\\\", "\'"),
		$text
	);
	return $text;
}


function DateFieldSel($name, $value = Array(), $limits = Array()) {
	global $v;
	
	if (count($limits) == 0) {
		$limits[0] = 1;
		$limits[1] = 31;
		$limits[2] = 1;
		$limits[3] = 12;
		$limits[4] = date('Y', time());
		$limits[5] = date('Y', time()) + 3;
	}
	
	if ($v["{$name}_day"]) $value[0] = $v["{$name}_day"];
	if ($v["{$name}_month"]) $value[1] = $v["{$name}_month"];
	if ($v["{$name}_year"]) $value[2] = $v["{$name}_year"];
	
	$t = "<select name=\"{$name}_day\" style=\"width: 45px\">";
	for ($i = $limits[0]; $i <= $limits[1]; $i++) $t.= "<option value=\"{$i}\"" . (($i == $value[0])?' selected':'') . ">$i</option>";
	$t.= "</select>";
	
	$t.= "<select name=\"{$name}_month\" style=\"width: 45px\">";
	for ($i = $limits[2]; $i <= $limits[3]; $i++) $t.= "<option value=\"{$i}\"" . (($i == $value[1])?' selected':'') . ">$i</option>";
	$t.= "</select>";

	$t.= "<select name=\"{$name}_year\" style=\"width: 60px\">";
	for ($i = $limits[4]; $i <= $limits[5]; $i++) $t.= "<option value=\"{$i}\"" . (($i == $value[2])?' selected':'') . ">$i</option>";
	$t.= "</select>";
	
	return $t;
}



function htmlesc($text) {
 	$text = stripslashes(htmlspecialchars($text, -1, 'ISO-8859-15'));
	return $text;
}
function urlesc($text) {
 	$text = urlencode(stripslashes($text));
	return $text;
}

function Short($str, $len, $a = 1) {
	if (strlen($str) > $len) {
		if ($a) return '<a title="' . $str . '">' . substr($str, 0, $len-2) . ' ...</a>';
		else return substr($str, 0, $len-2) . ' ...';
	} else return $str;
}

function DateMyPhp($date, $format = 'd.m.Y') {
	if ($date == '0000-00-00 00:00:00') return '&nbsp;';
	$tmp = explode(' ', $date);
	$tmp0 = explode('-', $tmp[0]);
	$tmp1 = explode(':', $tmp[1]);
	return @date($format, mktime($tmp1[0], $tmp1[1], $tmp1[2], $tmp0[1], $tmp0[2], $tmp0[0]));
}

function NumRows($q) {
	if (strpos('GROUP BY', $q) === FALSE) {
		$q = preg_replace("/SELECT(.+?)FROM/", 'SELECT COUNT(*) FROM', $q);
		$res = @mysql_query($q) or die("error: table not found");
		$row = mysql_fetch_row($res);
		mysql_free_result($res);
		return $row[0];
	} else {
		$res = mysql_query($q);
		$nr = mysql_num_rows($res);
		mysql_free_result($res);
		return $nr;
	}
}



function Navig($q, $per_page, $align = 0, $var = '') {
  $v = $_REQUEST;

	$nr_page = $v['nr_page' . $var];
  $token['nrmax' . $var] = $nrmax = NumRows($q);
  $nrmaxpage = ceil($nrmax/$per_page);
  $link = $_SERVER['PHP_SELF'] . '?' . str_replace('&nr_page' . $var . '=' . $nr_page, '', $_SERVER['QUERY_STRING']) . '&nr_page' . $var . '=';
  if (($nr_page <= 1) || (!is_numeric($nr_page))) $nr_page = 1;
  elseif ($nr_page >= $nrmaxpage) $nr_page = $nrmaxpage;
  $GLOBALS['start_i' . $var] = $start = ($nr_page-1) * $per_page;
  
  $row['browse_left'] = '&lt;&lt;';
  $row['browse_right'] = '&gt;&gt;';
  
  if ($nr_page > 1) {
    $row['browse_left'] = '<a href="' . $link . ($nr_page-1) . '" class="a">&lt;&lt;</a>';
  }
  if ($nr_page < $nrmaxpage) {
    $row['browse_right'] = '<a href="' . $link . ($nr_page+1) . '" class="a">&gt;&gt;</a>';
  }
  $row['nr_page'] = $nr_page;
  $row['nrmax_pages'] = $nrmaxpage;

	$row['align'] = 'center';
	if ($align == 1) $row['align'] = 'right';
	elseif ($align == -1) $row['align'] = 'left';
  
	$token['browse_page'] = "Browsing page {$row['nr_page']} of {$row['nrmax_pages']}";
	
	if ($_GET[type] != 'latest') {
	$GLOBALS['navig' . $var] = '
		<div class="small_text" align="' . $row['align'] . '">
			' . $row['browse_left'] . '
			&nbsp;&nbsp;
			' . $token['browse_page'] . '
			&nbsp;&nbsp;
			' . $row['browse_right'] . '
		</div>
	';
	}
	
	$v['nr_page' . $var] = $nr_page;
  return 'LIMIT ' . $start . ', ' . $per_page;
  
}



function Redirect ($url, $sec = 0, $display = TRUE) {
	if ((!$sec) && ($display) && (!headers_sent())) {
		header('Location: ' . $url);
    exit();
	}

  $url = '<META HTTP-EQUIV="Refresh" CONTENT="' . $sec . ';URL=' . $url . '">';
	if ($display) {
		echo $url;
		exit();
	}
	return $url;
}

function LNtoBR($str) {
	$str = str_replace("\r\n", "\n", $str);
	$str = str_replace("\n", "\r", $str);
	$str = str_replace("\r", "<br />\r\n", $str);
	$str = str_replace("  ", "&nbsp;&nbsp;", $str);
	return $str;
}

function BRtoLN($str) {
	$str = str_replace(Array('<br>', '<br/>', '<br />', '&nbsp;'), Array("\r\n", "\r\n", "\r\n", ' '), $str);
	return $str;
}

function __($text, $array) {
	$text = preg_replace("/\{(\w+)\}/e", "\$array[strtolower('$1')]", $text);
	return $text;
}

function SiteMenu() {
	global $rewrite;

	unset($c);
	$menu = 'A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z';
	$menu = explode('|', $menu);

	foreach ($menu as $key => $val)
		
	if ($rewrite=="off") { $menu[$key] = '<td><a href="display.php?action=search&by=' . $val . '" class="menu_link">' . $val . '</a></td>'; }
		else { $menu[$key] = '<td><a href="' . $val . '" class="menu_link">' . $val . '</a></td>'; }
	
	$c = '
		<table width="90%" cellpadding="0" cellspacing="0" border="0">
			<tr>
				' . implode($menu, '<td style="padding: 0px 2px 0px 2px;">-</td>') . '
						</tr>
						<tr>
				<td colspan="50" align="center">
<br /><br />
<div align="center"><script type="text/javascript"><!--
google_ad_client = "pub-1006241150471906";
google_ad_width = 728;
google_ad_height = 90;
google_ad_format = "728x90_as";
google_ad_channel ="";
google_color_border = "EB694A";
google_color_bg = "BCD2E9";
google_color_link = "005387";
google_color_url = "000000";
google_color_text = "000000";
//--></script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script></div>
					<form name="form" method="get" action="display.php">
					<input type="hidden" name="action" value="search" />
					<table align="center" cellpadding="0" cellspacing="0" border="0">
						<tr>
							<td>Search</td>
							<td width="10"></td>
							<td><input type="text" name="word" value="' . $_GET['word'] . '" class="search_field" /></td>
							<td width="10"></td>
							<td><input type="radio" name="type" value="latest"' . (($_GET['type'] != 'full')?' checked':'') . ' /></td>
							<td>beginning of word</td>
							<td width="10"></td>
							<td><input type="radio" name="type" value="full"' . (($_GET['type'] == 'full')?' checked':'') . ' /></td>
							<td>full word and in description</td>
							<td width="10"></td>
							<td><input type="submit" value="  Go  " /></td>
						</tr>
					</table>
					</form>
				</td>
			</tr>
		</table>
	';

	if ($_GET[type] == 'latest') { $c .= '<h1>Last 10 definitions</h1>'; }
		else { $c .= '<br><a href="display.php?action=search&type=latest"><u>New Definitions</u></a>'; }

	if ($GLOBALS['admin']) {
		$GLOBALS['adminContent'].= '<div align="center"><b><a href="admin.php?action=edit&from=' . $GLOBALS['from'] . '">Add new word definition</a> | <a href="admin.php?action=change">Change pass</a> | <a href="admin.php?action=rewrite">Mod Rewrite (On/Off)</a> | <a href="admin.php?action=logout">Admin logout</a></b></div><br />';
	}

	$GLOBALS['content'] = $c;
}

?>

