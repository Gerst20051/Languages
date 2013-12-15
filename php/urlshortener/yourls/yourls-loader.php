<?php
// Handle inexistant root favicon requests and exit
if ( '/favicon.ico' == $_SERVER['REQUEST_URI'] ) {
	header('Content-Type: image/gif');
	echo base64_decode("R0lGODlhEAAQAJECAAAAzFZWzP///wAAACH5BAEAAAIALAAAAAAQABAAAAIplI+py+0PUQAgSGoNQFt0LWTVOE6GuX1H6onTVHaW2tEHnJ1YxPc+UwAAOw==");
	exit;
}

// Start YOURLS
require_once( dirname(__FILE__).'/includes/load-yourls.php' );

// Get request in YOURLS base (eg in 'http://site.com/yourls/abcd' get 'abdc')
$scheme = ( isset($_SERVER["HTTPS"]) ? 'https' : 'http' );
$request = str_replace( YOURLS_SITE.'/', '', $scheme . '://' . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'] );

/**
 * TODO: think about doing something with $_SERVER['QUERY_STRING']?
 * Like sho.rt/keyword?something
 * if so:
 *     - deal with logout requests early
 *     - handle differently short urls (sho.rt/keyword?something) and prefix-n-shorten (sho.rt/http://bleh/?s=1)
 */

// Make valid regexp pattern from authorized charset in keywords
$pattern = yourls_make_regexp_pattern( yourls_get_shorturl_charset() );

// Now load required template and exit

yourls_do_action( 'pre_load_template', $request );

// At this point, $request is not sanitized. Sanitize in loaded template.

// Redirection:
if( preg_match( "@^([$pattern]+)/?$@", $request, $matches ) ) {
	$keyword   = isset( $matches[1] ) ? $matches[1] : '';
	$keyword = yourls_sanitize_keyword( $keyword );
	yourls_do_action( 'load_template_go', $keyword );
	include( YOURLS_ABSPATH.'/yourls-go.php' );
	exit;
}

// Stats:
if( preg_match( "@^([$pattern]+)\+(all)?/?$@", $request, $matches ) ) {
	$keyword   = isset( $matches[1] ) ? $matches[1] : '';
	$keyword = yourls_sanitize_keyword( $keyword );
	$aggregate = isset( $matches[2] ) ? (bool)$matches[2] && yourls_allow_duplicate_longurls() : false;
	yourls_do_action( 'load_template_infos', $keyword );
	include( YOURLS_ABSPATH.'/yourls-infos.php' );
	exit;
}

// Prefix-n-Shorten sends to bookmarklet (doesn't work on Windows)
if( preg_match( "@^[a-zA-Z]+://.+@", $request, $matches ) ) {
	$url = yourls_sanitize_url( $matches[0] );
	yourls_do_action( 'load_template_redirect_admin', $url );
	yourls_redirect( yourls_admin_url('index.php').'?u='.rawurlencode( $url ), 302 );
	exit;
}

// Past this point this is a request the loader could not understand
yourls_do_action( 'loader_failed', $request );
yourls_redirect( YOURLS_SITE, 302 );
exit;