<?php
define('YOURLS_API', true);
require_once( dirname(__FILE__).'/includes/load-yourls.php' );
yourls_maybe_require_auth();

$action = ( isset( $_REQUEST['action'] ) ? $_REQUEST['action'] : null );

yourls_do_action( 'api', $action );
	
switch( $action ) {

	// Shorten a URL
	case 'shorturl':
		$url = ( isset( $_REQUEST['url'] ) ? $_REQUEST['url'] : '' );
		$keyword = ( isset( $_REQUEST['keyword'] ) ? $_REQUEST['keyword'] : '' );
		$title = ( isset( $_REQUEST['title'] ) ? $_REQUEST['title'] : '' );
		$return = yourls_add_new_link( $url, $keyword, $title );
		$return['simple'] = ( isset( $return['shorturl'] ) ? $return['shorturl'] : '' ); // This one will be used in case output mode is 'simple'
		unset( $return['html'] ); // in API mode, no need for our internal HTML output
		break;
	
	// Global stats
	case 'stats':
		$filter = ( isset( $_REQUEST['filter'] ) ? $_REQUEST['filter'] : '' );
		$limit = ( isset( $_REQUEST['limit'] ) ? $_REQUEST['limit'] : '' );
		$return = yourls_api_stats( $filter, $limit );
		break;
	
	// Stats for a shorturl
	case 'url-stats':
		$shorturl = ( isset( $_REQUEST['shorturl'] ) ? $_REQUEST['shorturl'] : '' );
		$return = yourls_api_url_stats( $shorturl );
		break;

	// Expand a short link
	case 'expand':
		$shorturl = ( isset( $_REQUEST['shorturl'] ) ? $_REQUEST['shorturl'] : '' );
		$return = yourls_api_expand( $shorturl );
		break;
	
	// Missing or incorrect action parameter
	default:
		$return = array(
			'errorCode' => 400,
			'message'   => 'Unknown or missing "action" parameter',
			'simple'    => 'Unknown or missing "action" parameter',
		);
		

}

$format = ( isset( $_REQUEST['format'] ) ? $_REQUEST['format'] : 'xml' );

yourls_api_output( $format, $return );

die();