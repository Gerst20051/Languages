<?php
/*
 * EasyWebFetch - Fetch a page by opening socket connection, no dependencies
 *
 * PHP version 5
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author 	  Nashruddin Amin <me@nashruddin.com>
 * @copyright Nashruddin Amin 2008
 * @license	  GNU General Public License 3.0
 * @package   EasyWebFetch
 * @version   1.1
 */

class EasyWebFetch 
{
	private	$_request_url;
	private $_host;
	private $_path;
	private $_query;
	private $_fragment;
	private $_headers_only;
	private $_portnum		= 80;
	private $_user_agent	= "SimpleHttpClient/1.0";
	private $_req_timeout	= 30;
	private $_maxredirs		= 5;

	private $_use_proxy		= false;
	private $_proxy_host;
	private $_proxy_port;
	private $_proxy_user;
	private $_proxy_pass;
	
	private $_status;
	private $_resp_headers;
	private $_resp_body;

	private $_is_error;
	private $_errmsg;
	
	/**
	 * class constructor
	 */
	public function __construct()
	{
		$this->_resp_headers = array();
		$this->_resp_body	 = "";
	}

	/**
	 * get the requested page
	 *
	 * @param string  $url			URL of the requested page
	 * @param boolean $headers_only	true to return headers only, 
	 *								false to return headers and body
	 *
	 * @return 	boolean	true on success, false on failure
	 */
	public function get($url = '', $headers_only = false)
	{
		$this->_request_url	 = $url;
		$this->_headers_only = $headers_only;
		
		$redir = 0;

		while(($redir++) <= $this->_maxredirs) {
			$this->parseUrl($this->_request_url);
			
			if (($response = $this->makeRequest()) == false) {
				return(false);
			}
			
			/* split head and body */
			$neck = strpos($response, "\r\n\r\n");
			$head = substr($response, 0, $neck);
			$body = substr($response, $neck+2);

			/* read response headers */
			$this->_resp_headers = $this->parseHeaders($head);
			
			/* check for redirects */
			if ($this->getStatus() == 301 || $this->getStatus() == 302) {
				$follow = $this->_resp_headers['location'];
				$this->_request_url = $this->setFullPath($follow, $this->_request_url);
				continue;
			} else {
				/* no redirects, start reading response body */
				break;
			}
		}
		
		/* read the body part */
		if ($this->_resp_headers['transfer_encoding'] == 'chunked') {
			$this->_resp_body = $this->joinChunks($body);
		} else {
			$this->_resp_body = $body;
		}
		
		return(true);
	}

	/**
	 * build HTTP header and perform HTTP request
	 *
	 * @return 	mixed	HTTP response on success, false on failure
	 */
	private function makeRequest()
	{
		$method   	= ($this->_headers_only == true) ? "HEAD" : "GET";
		$proxy_auth = base64_encode("$this->_proxy_user:$this->_proxy_pass");
		$response 	= "";
		
		if ($this->_use_proxy) {
			$headers = "$method $this->_request_url HTTP/1.1\r\n"
					 . "Host: $this->_host\r\n"
					 . "Proxy-Authorization: Basic $proxy_auth\r\n"
					 . "User-Agent: $this->_user_agent\r\n"
					 . "Connection: Close\r\n\r\n";
			$fp = fsockopen($this->_proxy_host, $this->_proxy_port, $errno, $errmsg, $this->_req_timeout);
		} else {
			$headers = "$method $this->_path$this->_query$this->_fragment HTTP/1.1\r\n"
					 . "Host: $this->_host\r\n"
					 . "User-Agent: $this->_user_agent\r\n"
					 . "Connection: Close\r\n\r\n";
			$fp = fsockopen($this->_host, $this->_portnum, $errno, $errmsg, $this->_req_timeout);
		}		
		
		if (!$fp) {
			$this->_is_error = true;
			$this->_errmsg   = "Unknown error";
			return(false);
		}		
		fwrite($fp, $headers);
		
		while(!feof($fp)) {
			$response .= fgets($fp, 4096);
		}		
		fclose($fp);

		return($response);
	}

	/**
	 * parse the requested URL to its host, path, query and fragment
	 *
	 * @return void
	 */
	private function parseUrl($url)
	{
		$this->_host	 = parse_url($url, PHP_URL_HOST);
		$this->_path	 = parse_url($url, PHP_URL_PATH);
		$this->_query	 = parse_url($url, PHP_URL_QUERY);
		$this->_fragment = parse_url($url, PHP_URL_FRAGMENT);
		
		if (empty($this->_path)) {
			$this->_path = '/';
		}
	}

	/**
	 * get the full path of the page to redirect. if the requested page is
	 * http://www.example.com and it redirects to redirpage.html, then the
	 * new request is http://www.example.com/redirpage.html
	 *
	 * @param string $loc			new location from the HTTP response headers
	 * @param string $parent_url	the parent's URL
	 *
	 * @return string  full path of the page to redirect
	 */
	private function setFullPath($loc, $parent_url)
	{
		$parent_url = preg_replace("/\/[^\/]*$/", "", $parent_url);
		
		if (strpos($loc, 'http://') !== false) {
			return($loc);
		}		
		if (strpos($loc, '../') === false) {
			return("$parent_url/$loc");
		}
		
		while (strpos($loc, '../') !== false) {
			$loc   		= preg_replace("/^\.\.\//", "", $loc);
			$parent_url = preg_replace("/\/[^\/]+$/", "", $parent_url);
		}

		return("$parent_url/$loc");	
	}
	
	/**
	 * parse HTTP response headers to array
	 *
	 * @param string $string HTTP response headers
	 *
	 * @return array 
	 */
	private function parseHeaders($string)
	{
		$string  = trim($string);
		$headers = array();

		$lines = explode("\r\n", $string);

		$headers['http_status'] = $lines[0];

		/* read HTTP _status in first line */
		preg_match('/HTTP\/(\\d\\.\\d)\\s*(\\d+)\\s*(.*)/', $lines[0], $m);
		$this->_status = $m[2];

		array_splice($lines, 0, 1); /* remove first line */

		foreach ($lines as $line) {
			list($key, $val) = explode(': ', $line);

			$key = str_replace("-", "_", $key);
			$key = strtolower($key);
			$val = trim($val);

			$headers[$key] = $val;	
		}
		return($headers);
	}

	/**
	 * join parts of the HTTP response body with chunked transfer-encoding
	 *
	 * @param string $chunks HTTP response body
	 * 
	 * @return string full body
	 */
	private function joinChunks($chunks)
	{
		preg_match("/\r\n([0-9a-z]+)(;?.*)\r\n/", $chunks, $match);
		$size = hexdec($match[1]);
		
		$body = "";
		
		while($size > 0) {	
			/* remove line with chunk size */
			$chunks = preg_replace("/\r\n.+\r\n/m", "", $chunks, 1); 
			
			$part 	= substr($chunks, 0, $size);
			$chunks = substr($chunks, $size);
			
			$body .= $part;
			
			/* get next chunk size */
			preg_match("/\r\n([0-9a-z]+)(;?.*)\r\n/", $chunks, $match);
			$size = hexdec($match[1]);
		}	
		return($body);
	}

	/**
	 * set the requested URL
	 *
	 * @param string $url URL of the requested page
	 */
	public function setRequestUrl($url)
	{
		$this->_request_url = $url;
	}
	
	/**
	 * set to return headers only
	 *
	 * @param boolean $headers_only true to return headers only,
	 *								false to return headers and body
	 */
	public function returnHeadersOnly($headers_only)
	{
		$this->_headers_only = $headers_only;
	}
	
	/**
	 * set proxy host and port
	 *
	 * @param string $hostport proxy host and proxy port in format proxy_host:proxy_port
	 */
	public function setProxyHost($hostport)
	{
		list($this->_proxy_host, $this->_proxy_port) = explode(':', $hostport);
		$this->_use_proxy = true;
	}
	
	/**
	 * set proxy user and password
	 *
	 * @param string $userpass proxy user and password in format proxy_user:proxy_password
	 */
	public function setProxyUser($userpass)
	{
		list($this->_proxy_user, $this->_proxy_pass) = explode(':', $userpass);
	}
		
	/**
	 * get the HTTP response status (200, 404, etc)
	 *
	 * @return string
	 */
	public function getStatus()
	{
		return($this->_status);
	}

	/**
	 * get the requested URL
	 *
	 * @return string 
	 */
	public function getRequestUrl()
	{
		return($this->_request_url);
	}
	
	/**
	 * set maximum redirects
	 *
	 * @param int $maxredirs
	 */
	public function setMaxRedirs($maxredirs)
	{
		$this->_maxredirs = $maxredirs;
	}
	
	/**
	 * get HTTP response headers
	 *
	 * @return array 
	 */
	public function getHeaders()
	{
		return($this->_resp_headers);
	}

	/**
	 * get the HTTP response body, usually in HTML
	 *
	 * @return string
	 */
	public function getContents()
	{
		return($this->_resp_body);
	}
	
	/**
	 * get error message
	 *
	 * @return string
	 */
	public function getErrorMessage()
	{
		return($this->_errmsg);
	}
	
	/**
	 * print debug information
	 */
	private function debug($text)
	{
		print "$text\n";
	}
}
