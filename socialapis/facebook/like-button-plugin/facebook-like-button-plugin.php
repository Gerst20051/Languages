<?php
/**
 * Plugin Name: Facebook Like Button Plugin
 * Plugin URI: http://martinj.net/wordpress-plugins/facebook-like-button
 * Description: The new Facebook like button.
 * Version: 1.3.1
 * Author: Martin Jonsson
 * Author URI: http://martinj.net
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

function facebook_like_button_plugin_sc_standard($atts) {
	return facebook_like_button_plugin_shortcode('standard', $atts);
}

function facebook_like_button_plugin_sc_count($atts) {
	return facebook_like_button_plugin_shortcode('button_count', $atts);
}

function facebook_like_button_plugin_shortcode($layout, $atts) {
	global $post;
	$options = unserialize(get_option('facebook_like_button_plugin_options'));
	$atts['layout'] = $layout;
	$options = shortcode_atts(facebook_like_button_plugin_defaults($options), $atts);
	
	if ($options['output_type'] == 'xfbml')	
		return facebook_like_button_plugin_create_xfbml($options, get_permalink($post->id));	
		
	return facebook_like_button_plugin_create_iframe($options, get_permalink($post->id));	
}

function facebook_like_button_plugin_create_xfbml($options, $url) {
	return
		'<fb:like 
		href="'.$url.'" 
		layout="'.$options['layout'].'" 
		show_faces="'.($options['show_faces'] ? 'true' : 'false').'" 
		width="'.$options['width'].'" 
		'.($options['height'] ? 'height="'.$options['height'].'"': '').'
		action="'.$options['action'].'" 
		colorscheme="'.$options['colorscheme'].'" 
		style="'.$options['iframe_style'].'"
		class="fb_edge_widget_with_comment fb_iframe_widget"></fb:like>';
}

function facebook_like_button_plugin_create_iframe($options, $url) {
	$url = urlencode($url);
	return 
		'<iframe src="http://www.facebook.com/plugins/like.php?href='.$url.
		'&amp;layout=' .$options['layout']. 
		'&amp;'. ($options['show_faces'] ? 'show_faces=true' : '') .
		'&amp;width=' . $options['width'] .
		'&amp;action=' . $options['action'] .
		(strlen($options['font']) > 0 ? '&amp;font=' . $options['font'] : '') .
		'&amp;colorscheme=' . $options['colorscheme'] .
		'" scrolling="no" frameborder="0" allowTransparency="true" style="border:none; overflow:hidden; width:'.$options['width'].'px;'.($options['height'] ? 'height:'.$options['height'].'px;': '').($options['iframe_style'] ? $options['iframe_style'] : '').'"></iframe>';	
}

function facebook_like_button_plugin_output($out = '') {
	global $post;

	$custom_fields = get_post_custom($post->ID);
	if (isset($custom_fields['facebook_like_button']) && in_array('suppress', $custom_fields['facebook_like_button']))
		return $out;

	$options = unserialize(get_option('facebook_like_button_plugin_options'));
	$options = facebook_like_button_plugin_defaults($options);			

	if (!$options['show_on_pages'] && is_page()) return $out;
	if (!$options['show_on_home'] && is_home()) return $out;
	if (!$options['show_on_posts'] && is_single()) return $out;
	if (!$options['show_on_archive'] && is_archive()) return $out;
	
	if (!$options['show_on_posts'] && !$options['show_on_pages'] && !$options['show_on_home'] && !$options['show_on_archive']) return $out;

	
	if ($options['output_type'] == 'xfbml') {
		$iframe = facebook_like_button_plugin_create_xfbml($options, get_permalink($post->id));
	} else {
		$iframe = facebook_like_button_plugin_create_iframe($options, get_permalink($post->id));		
	}
	
	if ($options['position'] == 'top') {
		$out = $iframe . $out;
	} else {
		$out .= $iframe;
	}
		
	return $out;
}

function facebook_like_button_plugin_wp_footer() {
	$options = unserialize(get_option('facebook_like_button_plugin_options'));
	$options = facebook_like_button_plugin_defaults($options);
	if ($options['output_type'] != 'xfbml') return;
	
	echo 
	'<div id="fb-root"></div>' . 
	"
	<script>
	  window.fbAsyncInit = function() {
	    FB.init({
	      appId  : '".$options['fb_app_id']."',
	      status : true,
	      cookie : true,
	      xfbml  : true
	    });
	  };

	  (function() {
	    var e = document.createElement('script');
	    e.src = document.location.protocol + '//connect.facebook.net/".$options['locale']."/all.js';
	    e.async = true;
	    document.getElementById('fb-root').appendChild(e);
	  }());
	</script>	
	";
}

function facebook_like_button_plugin_wp_head() {
	global $post;
	if (is_home()) return;
	
	$options = unserialize(get_option('facebook_like_button_plugin_options'));
	if ($options['fb_admins']) {
		echo '<meta property="fb:admins" content="'.$options['fb_admins'].'"/>';		
	}
	if ($options['fb_app_id']) {
		echo '<meta property="fb:app_id" content="'.$options['fb_app_id'].'"/>';		
	}
	
	echo '<meta property="og:site_name" content="'.get_bloginfo('name').'"/>';
	echo '<meta property="og:title" content="'.$post->post_title.'" />';

	if (function_exists('has_post_thumbnail') && has_post_thumbnail($post->ID)) {
		if ($thumb_id = get_post_meta($post->ID, '_thumbnail_id', true)) {
			$thumb_meta = wp_get_attachment_image_src($thumb_id);
			echo '<meta property="og:image" content="'.$thumb_meta[0].'"/>';			
		}
	}	
}

function facebook_like_button_plugin_defaults($options) {
	if (!isset($options['show_on_posts'])) $options['show_on_posts'] = true;	
	if (!isset($options['show_on_pages'])) $options['show_on_pages'] = false;	
	if (!isset($options['show_on_home'])) $options['show_on_home'] = false;	
	if (!isset($options['show_on_archive'])) $options['show_on_archive'] = false;
	
	if (!isset($options['show_faces'])) $options['show_faces'] = true;
	if (!isset($options['output_type'])) $options['output_type'] = 'iframe';
	if (!isset($options['locale'])) $options['locale'] = 'en_US';
	if (!$options['layout']) $options['layout'] = 'standard';
	if (!$options['width']) $options['width'] = '450';
	if (!$options['action']) $options['action'] = 'like';
	if (!$options['colorscheme']) $options['colorscheme'] = 'light';
	if (!$options['position']) $options['position'] = 'bottom';
	if (!isset($options['iframe_style'])) $options['iframe_style'] = 'margin-top:5px;';		
	
	return $options;
}

function facebook_like_button_plugin_options() {
	if ($_POST["fb_like_button_submit"]) {
		
		$submitted_options = array();		
		$submitted_options['show_on_posts'] = stripslashes($_POST["show_on_posts"]);
		$submitted_options['show_on_pages'] = stripslashes($_POST["show_on_pages"]);		
		$submitted_options['show_on_home'] = stripslashes($_POST["show_on_home"]);
		$submitted_options['show_on_archive'] = stripslashes($_POST["show_on_archive"]);
		$submitted_options['fb_admins'] = stripslashes($_POST["fb_admins"]);
		$submitted_options['fb_app_id'] = stripslashes($_POST["fb_app_id"]);
		$submitted_options['output_type'] = stripslashes($_POST["output_type"]);
		$submitted_options['locale'] = stripslashes($_POST["locale"]);
		
		$submitted_options['layout'] = stripslashes($_POST["layout"]);
		$submitted_options['show_faces'] = isset($_POST["show_faces"]) ? true : false;
		$submitted_options['width'] = stripslashes($_POST["width"]);
		$submitted_options['height'] = stripslashes($_POST["height"]);		
		$submitted_options['action'] = stripslashes($_POST["action"]);
		$submitted_options['font'] = stripslashes($_POST["font"]);
		$submitted_options['colorscheme'] = stripslashes($_POST["colorscheme"]);
		$submitted_options['position'] = stripslashes($_POST["position"]);
		$submitted_options['iframe_style'] = stripslashes($_POST["iframe_style"]);
				
		update_option('facebook_like_button_plugin_options', serialize($submitted_options));
	}
	
	$options = unserialize(get_option('facebook_like_button_plugin_options'));
	$options = facebook_like_button_plugin_defaults($options);
	
	echo '
		<div>
		<form method="post">
		<div class="wrap">
			<h2>Facebook Like Button Plugin</h2>
			<h3 class="title">Configuration</h3>				
			<dl>
				<dt>
					Show on Home
				</dt>
					<dd>
						<input name="show_on_home" id="param_show_on_home" value="true" '.($options['show_on_home'] ? 'checked="1"' : '').' type="checkbox"><label 	for="param_show_on_home">Show in post listing on Home Page?</label>					
					</dd>
			
				<dt>
					Show on Pages
				</dt>
					<dd>
						<input name="show_on_pages" id="param_show_on_pages" value="true" '.($options['show_on_pages'] ? 'checked="1"' : '').' type="checkbox"><label for="param_show_on_pages">Show the button on pages?</label>					
					</dd>
				<dt>
					Show on Posts
				</dt>
					<dd>
						<input name="show_on_posts" id="param_show_on_posts" value="true" '.($options['show_on_posts'] ? 'checked="1"' : '').' type="checkbox"><label 	for="param_show_on_posts">Show in posts?</label>					
					</dd>
				<dt>
					Show on Archive
				</dt>
					<dd>
						<input name="show_on_archive" id="param_show_on_archive" value="true" '.($options['show_on_archive'] ? 'checked="1"' : '').' type="checkbox"><label 	for="param_show_on_posts">Show on archive pages (Category, Tag, Author and Date based pages)?</label>					
					</dd>
				<dt>
					Position
				</dt>
					<dd>
						<select name="position">
							<option value="bottom" '. ($options['position'] == 'bottom' ? 'selected="1"' : '') . '>bottom</option>
							<option value="top" '. ($options['position'] == 'top' ? 'selected="1"' : '') . '>top</option>
						</select>
						<span>Where to display the button in relation to Post/Page.</span>
					</dd>
			</dl>
			
			<h3 class="title">Customization</h3>				
			<dl>
				<dt>
					<label for="param_layout">Layout Style</label>
				</dt>
					<dd>
						<select name="layout" id="param_layout">
							<option value="standard" '. ($options['layout'] == 'standard' ? 'selected="1"' : '') . '>standard</option>
							<option value="button_count" '. ($options['layout'] == 'button_count' ? 'selected="1"' : '') . '>button_count</option>
						</select>
						<span>determines the size and amount of social context next to the button</span>
					</dd>
				<dt>
					Show Faces
				</dt>
					<dd>
						<input name="show_faces" id="param_show_faces" value="true" '.($options['show_faces'] ? 'checked="1"' : '').' type="checkbox"><label for="param_show_faces">Show profile pictures below the button.</label>
					</dd>
				<dt>
					<label for="param_width">Width</label>
				</dt>
					<dd>
						<input name="width" id="param_width" value="'.$options['width'].'" class="small-text" type="text">
						<span>the width of the plugin, in pixels</span>
					</dd>
				<dt>
					<label for="param_height">Height</label>
				</dt>
					<dd>
						<input name="height" id="param_height" value="'.$options['height'].'" class="small-text" type="text">
						<span>the height in pixels. If you want to show faces don\'t make this to small, otherwise 30 is a good height.</span>
					</dd>
				<dt>
					<label for="param_action">Verb to display</label>
				</dt>
					<dd>
						<select name="action" id="param_action">
							<option value="like" '. ($options['action'] == 'like' ? 'selected="1"' : '') . '>like</option>
							<option value="recommend" '. ($options['action'] == 'recommend' ? 'selected="1"' : '') . '>recommend</option>
						</select>
						<span>The verb to display in the button. Currently only \'like\' and \'recommend\' are supported.</span>
					</dd>
				<dt>
					<label for="param_font">Font</label> 		
				</dt>
					<dd>
						<select name="font" id="param_font">
							<option '. (!$options['font'] ? 'selected="1"' : '') . '></option>
							<option value="arial" '. ($options['font'] == 'arial' ? 'selected="1"' : '') . '>arial</option>
							<option value="lucida grande" '. ($options['font'] == 'lucida grande' ? 'selected="1"' : '') . '>lucida grande</option>
							<option value="segoe ui" '. ($options['font'] == 'segoe ui' ? 'selected="1"' : '') . '>segoe ui</option>
							<option value="tahoma" '. ($options['font'] == 'tahoma' ? 'selected="1"' : '') . '>tahoma</option>
							<option value="trebuchet ms" '. ($options['font'] == 'trebuchet ms' ? 'selected="1"' : '') . '>trebuchet ms</option>
							<option value="verdana" '. ($options['font'] == 'verdana' ? 'selected="1"' : '') . '>verdana</option>
						</select>
						<span>the font of the plugin</span>
					</dd>
				<dt>
					<label for="param_colorscheme">Color Scheme</label>
				</dt>
					<dd>
						<select name="colorscheme" id="param_colorscheme">
							<option value="light" '. ($options['colorscheme'] == 'light' ? 'selected="1"' : '') . '>light</option>
							<option value="dark" '. ($options['colorscheme'] == 'dark' ? 'selected="1"' : '') . '>dark</option>
						</select>
						<span>The color scheme of the plugin.</span>
					</dd>
				<dt>
					<label for="param_iframe_style">CSS style</label>
				</dt>
					<dd>
						<input name="iframe_style" id="param_iframe_style" value="'.$options['iframe_style'].'" class="regular-text" type="text">
						<span>Extra css styling.</span>
					</dd>					
			</dl>
			
			<h3 class="title">Advanced Settings</h3>				
			<dl>
				<dt>
					<label for="param_output_type">Output Type</label>
				</dt>
					<dd>
						<select name="output_type" id="param_output_type">
							<option value="iframe" '. ($options['output_type'] == 'iframe' ? 'selected="1"' : '') . '>iframe</option>
							<option value="xfbml" '. ($options['output_type'] == 'xfbml' ? 'selected="1"' : '') . '>XFBML</option>
						</select>
						<span></span>
					</dd>
				<dt>
					<label for="param_locale">Language</label>
				</dt>
					<dd>
						<select name="locale" id="param_locale">
						<option value="ca_ES" '. ($options['locale'] == 'ca_ES' ? 'selected="1"' : '') . '>Catalan</option>
						<option value="cs_CZ" '. ($options['locale'] == 'cs_CZ' ? 'selected="1"' : '') . '>Czech</option>
						<option value="cy_GB" '. ($options['locale'] == 'cy_GB' ? 'selected="1"' : '') . '>Welsh</option>
						<option value="da_DK" '. ($options['locale'] == 'da_DK' ? 'selected="1"' : '') . '>Danish</option>
						<option value="de_DE" '. ($options['locale'] == 'de_DE' ? 'selected="1"' : '') . '>German</option>
						<option value="eu_ES" '. ($options['locale'] == 'eu_ES' ? 'selected="1"' : '') . '>Basque</option>
						<option value="en_PI" '. ($options['locale'] == 'en_PI' ? 'selected="1"' : '') . '>English (Pirate)</option>
						<option value="en_UD" '. ($options['locale'] == 'en_UD' ? 'selected="1"' : '') . '>English (Upside Down)</option>
						<option value="ck_US" '. ($options['locale'] == 'ck_US' ? 'selected="1"' : '') . '>Cherokee</option>
						<option value="en_US" '. ($options['locale'] == 'en_US' ? 'selected="1"' : '') . '>English (US)</option>
						<option value="es_LA" '. ($options['locale'] == 'es_LA' ? 'selected="1"' : '') . '>Spanish</option>
						<option value="es_CL" '. ($options['locale'] == 'es_CL' ? 'selected="1"' : '') . '>Spanish (Chile)</option>
						<option value="es_CO" '. ($options['locale'] == 'es_CO' ? 'selected="1"' : '') . '>Spanish (Colombia)</option>
						<option value="es_ES" '. ($options['locale'] == 'es_ES' ? 'selected="1"' : '') . '>Spanish (Spain)</option>
						<option value="es_MX" '. ($options['locale'] == 'es_MX' ? 'selected="1"' : '') . '>Spanish (Mexico)</option>
						<option value="es_VE" '. ($options['locale'] == 'es_VE' ? 'selected="1"' : '') . '>Spanish (Venezuela)</option>
						<option value="fb_FI" '. ($options['locale'] == 'fb_FI' ? 'selected="1"' : '') . '>Finnish (test)</option>
						<option value="fi_FI" '. ($options['locale'] == 'fi_FI' ? 'selected="1"' : '') . '>Finnish</option>
						<option value="fr_FR" '. ($options['locale'] == 'fr_FR' ? 'selected="1"' : '') . '>French (France)</option>
						<option value="gl_ES" '. ($options['locale'] == 'gl_ES' ? 'selected="1"' : '') . '>Galician</option>
						<option value="hu_HU" '. ($options['locale'] == 'hu_HU' ? 'selected="1"' : '') . '>Hungarian</option>
						<option value="it_IT" '. ($options['locale'] == 'it_IT' ? 'selected="1"' : '') . '>Italian</option>
						<option value="ja_JP" '. ($options['locale'] == 'ja_JP' ? 'selected="1"' : '') . '>Japanese</option>
						<option value="ko_KR" '. ($options['locale'] == 'ko_KR' ? 'selected="1"' : '') . '>Korean</option>
						<option value="nb_NO" '. ($options['locale'] == 'nb_NO' ? 'selected="1"' : '') . '>Norwegian (bokmal)</option>
						<option value="nn_NO" '. ($options['locale'] == 'nn_NO' ? 'selected="1"' : '') . '>Norwegian (nynorsk)</option>
						<option value="nl_NL" '. ($options['locale'] == 'nl_NL' ? 'selected="1"' : '') . '>Dutch</option>
						<option value="pl_PL" '. ($options['locale'] == 'pl_PL' ? 'selected="1"' : '') . '>Polish</option>
						<option value="pt_BR" '. ($options['locale'] == 'pt_BR' ? 'selected="1"' : '') . '>Portuguese (Brazil)</option>
						<option value="pt_PT" '. ($options['locale'] == 'pt_PT' ? 'selected="1"' : '') . '>Portuguese (Portugal)</option>
						<option value="ro_RO" '. ($options['locale'] == 'ro_RO' ? 'selected="1"' : '') . '>Romanian</option>
						<option value="ru_RU" '. ($options['locale'] == 'ru_RU' ? 'selected="1"' : '') . '>Russian</option>
						<option value="sk_SK" '. ($options['locale'] == 'sk_SK' ? 'selected="1"' : '') . '>Slovak</option>
						<option value="sl_SI" '. ($options['locale'] == 'sl_SI' ? 'selected="1"' : '') . '>Slovenian</option>
						<option value="sv_SE" '. ($options['locale'] == 'sv_SE' ? 'selected="1"' : '') . '>Swedish</option>
						<option value="th_TH" '. ($options['locale'] == 'th_TH' ? 'selected="1"' : '') . '>Thai</option>
						<option value="tr_TR" '. ($options['locale'] == 'tr_TR' ? 'selected="1"' : '') . '>Turkish</option>
						<option value="ku_TR" '. ($options['locale'] == 'ku_TR' ? 'selected="1"' : '') . '>Kurdish</option>
						<option value="zh_CN" '. ($options['locale'] == 'zh_CN' ? 'selected="1"' : '') . '>Simplified Chinese (China)</option>
						<option value="zh_HK" '. ($options['locale'] == 'zh_HK' ? 'selected="1"' : '') . '>Traditional Chinese (Hong Kong)</option>
						<option value="zh_TW" '. ($options['locale'] == 'zh_TW' ? 'selected="1"' : '') . '>Traditional Chinese (Taiwan)</option>
						<option value="fb_LT" '. ($options['locale'] == 'fb_LT' ? 'selected="1"' : '') . '>Leet Speak</option>
						<option value="af_ZA" '. ($options['locale'] == 'af_ZA' ? 'selected="1"' : '') . '>Afrikaans</option>
						<option value="sq_AL" '. ($options['locale'] == 'sq_AL' ? 'selected="1"' : '') . '>Albanian</option>
						<option value="hy_AM" '. ($options['locale'] == 'hy_AM' ? 'selected="1"' : '') . '>Armenian</option>
						<option value="az_AZ" '. ($options['locale'] == 'az_AZ' ? 'selected="1"' : '') . '>Azeri</option>
						<option value="be_BY" '. ($options['locale'] == 'be_BY' ? 'selected="1"' : '') . '>Belarusian</option>
						<option value="bn_IN" '. ($options['locale'] == 'bn_IN' ? 'selected="1"' : '') . '>Bengali</option>
						<option value="bs_BA" '. ($options['locale'] == 'bs_BA' ? 'selected="1"' : '') . '>Bosnian</option>
						<option value="bg_BG" '. ($options['locale'] == 'bg_BG' ? 'selected="1"' : '') . '>Bulgarian</option>
						<option value="hr_HR" '. ($options['locale'] == 'hr_HR' ? 'selected="1"' : '') . '>Croatian</option>
						<option value="nl_BE" '. ($options['locale'] == 'nl_BE' ? 'selected="1"' : '') . '>Dutch (België)</option>
						<option value="en_GB" '. ($options['locale'] == 'en_GB' ? 'selected="1"' : '') . '>English (UK)</option>
						<option value="eo_EO" '. ($options['locale'] == 'eo_EO' ? 'selected="1"' : '') . '>Esperanto</option>
						<option value="et_EE" '. ($options['locale'] == 'et_EE' ? 'selected="1"' : '') . '>Estonian</option>
						<option value="fo_FO" '. ($options['locale'] == 'fo_FO' ? 'selected="1"' : '') . '>Faroese</option>
						<option value="fr_CA" '. ($options['locale'] == 'fr_CA' ? 'selected="1"' : '') . '>French (Canada)</option>
						<option value="ka_GE" '. ($options['locale'] == 'ka_GE' ? 'selected="1"' : '') . '>Georgian</option>
						<option value="el_GR" '. ($options['locale'] == 'el_GR' ? 'selected="1"' : '') . '>Greek</option>
						<option value="gu_IN" '. ($options['locale'] == 'gu_IN' ? 'selected="1"' : '') . '>Gujarati</option>
						<option value="hi_IN" '. ($options['locale'] == 'hi_IN' ? 'selected="1"' : '') . '>Hindi</option>
						<option value="is_IS" '. ($options['locale'] == 'is_IS' ? 'selected="1"' : '') . '>Icelandic</option>
						<option value="id_ID" '. ($options['locale'] == 'id_ID' ? 'selected="1"' : '') . '>Indonesian</option>
						<option value="ga_IE" '. ($options['locale'] == 'ga_IE' ? 'selected="1"' : '') . '>Irish</option>
						<option value="jv_ID" '. ($options['locale'] == 'jv_ID' ? 'selected="1"' : '') . '>Javanese</option>
						<option value="kn_IN" '. ($options['locale'] == 'kn_IN' ? 'selected="1"' : '') . '>Kannada</option>
						<option value="kk_KZ" '. ($options['locale'] == 'kk_KZ' ? 'selected="1"' : '') . '>Kazakh</option>
						<option value="la_VA" '. ($options['locale'] == 'la_VA' ? 'selected="1"' : '') . '>Latin</option>
						<option value="lv_LV" '. ($options['locale'] == 'lv_LV' ? 'selected="1"' : '') . '>Latvian</option>
						<option value="li_NL" '. ($options['locale'] == 'li_NL' ? 'selected="1"' : '') . '>Limburgish</option>
						<option value="lt_LT" '. ($options['locale'] == 'lt_LT' ? 'selected="1"' : '') . '>Lithuanian</option>
						<option value="mk_MK" '. ($options['locale'] == 'mk_MK' ? 'selected="1"' : '') . '>Macedonian</option>
						<option value="mg_MG" '. ($options['locale'] == 'mg_MG' ? 'selected="1"' : '') . '>Malagasy</option>
						<option value="ms_MY" '. ($options['locale'] == 'ms_MY' ? 'selected="1"' : '') . '>Malay</option>
						<option value="mt_MT" '. ($options['locale'] == 'mt_MT' ? 'selected="1"' : '') . '>Maltese</option>
						<option value="mr_IN" '. ($options['locale'] == 'mr_IN' ? 'selected="1"' : '') . '>Marathi</option>
						<option value="mn_MN" '. ($options['locale'] == 'mn_MN' ? 'selected="1"' : '') . '>Mongolian</option>
						<option value="ne_NP" '. ($options['locale'] == 'ne_NP' ? 'selected="1"' : '') . '>Nepali</option>
						<option value="pa_IN" '. ($options['locale'] == 'pa_IN' ? 'selected="1"' : '') . '>Punjabi</option>
						<option value="rm_CH" '. ($options['locale'] == 'rm_CH' ? 'selected="1"' : '') . '>Romansh</option>
						<option value="sa_IN" '. ($options['locale'] == 'sa_IN' ? 'selected="1"' : '') . '>Sanskrit</option>
						<option value="sr_RS" '. ($options['locale'] == 'sr_RS' ? 'selected="1"' : '') . '>Serbian</option>
						<option value="so_SO" '. ($options['locale'] == 'so_SO' ? 'selected="1"' : '') . '>Somali</option>
						<option value="sw_KE" '. ($options['locale'] == 'sw_KE' ? 'selected="1"' : '') . '>Swahili</option>
						<option value="tl_PH" '. ($options['locale'] == 'tl_PH' ? 'selected="1"' : '') . '>Filipino</option>
						<option value="ta_IN" '. ($options['locale'] == 'ta_IN' ? 'selected="1"' : '') . '>Tamil</option>
						<option value="tt_RU" '. ($options['locale'] == 'tt_RU' ? 'selected="1"' : '') . '>Tatar</option>
						<option value="te_IN" '. ($options['locale'] == 'te_IN' ? 'selected="1"' : '') . '>Telugu</option>
						<option value="ml_IN" '. ($options['locale'] == 'ml_IN' ? 'selected="1"' : '') . '>Malayalam</option>
						<option value="uk_UA" '. ($options['locale'] == 'uk_UA' ? 'selected="1"' : '') . '>Ukrainian</option>
						<option value="uz_UZ" '. ($options['locale'] == 'uz_UZ' ? 'selected="1"' : '') . '>Uzbek</option>
						<option value="vi_VN" '. ($options['locale'] == 'vi_VN' ? 'selected="1"' : '') . '>Vietnamese</option>
						<option value="xh_ZA" '. ($options['locale'] == 'xh_ZA' ? 'selected="1"' : '') . '>Xhosa</option>
						<option value="zu_ZA" '. ($options['locale'] == 'zu_ZA' ? 'selected="1"' : '') . '>Zulu</option>
						<option value="km_KH" '. ($options['locale'] == 'km_KH' ? 'selected="1"' : '') . '>Khmer</option>
						<option value="tg_TJ" '. ($options['locale'] == 'tg_TJ' ? 'selected="1"' : '') . '>Tajik</option>
						<option value="ar_AR" '. ($options['locale'] == 'ar_AR' ? 'selected="1"' : '') . '>Arabic</option>
						<option value="he_IL" '. ($options['locale'] == 'he_IL' ? 'selected="1"' : '') . '>Hebrew</option>
						<option value="ur_PK" '. ($options['locale'] == 'ur_PK' ? 'selected="1"' : '') . '>Urdu</option>
						<option value="fa_IR" '. ($options['locale'] == 'fa_IR' ? 'selected="1"' : '') . '>Persian</option>
						<option value="sy_SY" '. ($options['locale'] == 'sy_SY' ? 'selected="1"' : '') . '>Syriac</option>
						<option value="yi_DE" '. ($options['locale'] == 'yi_DE' ? 'selected="1"' : '') . '>Yiddish</option>
						<option value="gn_PY" '. ($options['locale'] == 'gn_PY' ? 'selected="1"' : '') . '>Guaraní</option>
						<option value="qu_PE" '. ($options['locale'] == 'qu_PE' ? 'selected="1"' : '') . '>Quechua</option>
						<option value="ay_BO" '. ($options['locale'] == 'ay_BO' ? 'selected="1"' : '') . '>Aymara</option>
						<option value="se_NO" '. ($options['locale'] == 'se_NO' ? 'selected="1"' : '') . '>Northern Sámi</option>
						<option value="ps_AF" '. ($options['locale'] == 'ps_AF' ? 'selected="1"' : '') . '>Pashto</option>
						<option value="tl_ST" '. ($options['locale'] == 'tl_ST' ? 'selected="1"' : '') . '>Klingon</option>						
						</select>
						<span>only affects XFBML output</span>
					</dd>
				<dt>
					<label for="param_fb_admins">Facebook User Admins</label>
				</dt>
					<dd>
						<input name="fb_admins" id="param_fb_admins" value="'.$options['fb_admins'].'" class="regular-text" type="text">
						<span>Comma-separated list of the user IDs or usernames of the Facebook accounts who own the page. More <a href="http://developers.facebook.com/docs/opengraph">info on administering</a>, check the "Administering your page" section</span>
						<p><a target="_new" href="http://www.facebook.com/pages/manage/">Pages You Admin</a></p>
					</dd>										
				<dt>
						<label for="param_fb_app_id">Facebook App ID (You should add this if you use XFBML)</label>
				</dt>
					<dd>
						<input name="fb_app_id" id="param_fb_app_id" value="'.$options['fb_app_id'].'" class="regular-text" type="text">
						<span>Facebook Platform application ID that administers the page instead of Facebook User Admins. If you don\'t have one you can create one <a href="http://www.facebook.com/developers/apps.php">here</a></span>
					</dd>													
			</dl>
			
			<p class="submit">
				<input type="submit" name="fb_like_button_submit" value="Update Options &raquo;" />
			</p>
		</div>
		</form>
		</div>	
	';
}

function facebook_like_button_plugin_html_namespaces($args = '') {
	return 'xmlns:og="http://opengraphprotocol.org/schema/" xmlns:fb="http://www.facebook.com/2008/fbml" ' . $args;
}

function add_facebook_like_button_plugin_submenu() {
    add_submenu_page('options-general.php', 'Facebook Like Button', 'Facebook Like Button', 10, __FILE__, 'facebook_like_button_plugin_options'); 
}

function facebook_like_button_plugin_filter_plugin_actions($links) {
	$new_links = array();	
	$new_links[] = '<a href="options-general.php?page=facebook-like-button-plugin/facebook-like-button-plugin.php">' . __('Settings', 'facebook-like-button-plugin') . '</a>';	
	return array_merge($new_links, $links);
}

add_action('plugin_action_links_' . plugin_basename(__FILE__), 'facebook_like_button_plugin_filter_plugin_actions');
add_action('admin_menu', 'add_facebook_like_button_plugin_submenu');
add_action('the_content', 'facebook_like_button_plugin_output', 99);
add_action('wp_head', 'facebook_like_button_plugin_wp_head');
add_action('wp_footer', 'facebook_like_button_plugin_wp_footer');

add_shortcode('fb_like_button_standard', 'facebook_like_button_plugin_sc_standard');
add_shortcode('fb_like_button_count', 'facebook_like_button_plugin_sc_count');

add_filter('language_attributes', 'facebook_like_button_plugin_html_namespaces', 99);
?>