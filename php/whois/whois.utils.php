<?php
/*
Whois.php        PHP classes to conduct whois queries

Copyright (C)1999,2005 easyDNS Technologies Inc. & Mark Jeftovic

Maintained by David Saez (david@ols.es)

For the most recent version of this package visit:

http://www.phpwhois.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/* utils.whois	1.0 Colin Viebrock <cmv@easydns.com> 1999/12/06 */
/* 		1.1 Mark Jeftovic <markjr@easydns.com> 1999/12/15 */

class utils extends Whois {
	/*
	Not updated anymore
	var $REGISTRARS = array(
		'ac' => 'http://www.nic.ac/cgi-bin/whois',
		'ad' => 'http://www.nic.ad/',
		'ar' => 'http://www.nic.ar/consultas/consdom.htm',
		'as' => 'http://www.nic.as/ASWhois.html',
		'at' => 'http://whois.aco.net/',
		'au' => 'http://www.aunic.net/namestatus.html',
		'be' => 'http://www.DNS.BE/domain-info/info.html',
		'br' => 'http://registro.fapesp.br/cgi-bin/nicbr/domainsearch',
		'ca' => 'http://www.cdnnet.ca/',
		'ch' => 'http://www.switch.ch/domain/search_form.html',
		'ci' => 'http://whois.nic.ci',
		'cl' => 'http://sunsite.dcc.uchile.cl/cgi-bin/dom-CL',
		'cr' => 'http://www.nic.cr/consulta-dns.html',
		'cx' => 'http://whois.nic.cx/',
		'cz' => 'gopher://gopher.eunet.cz:70/00/cz-info/cz.list',
		'de' => 'http://www.nic.de/servlet/Whois',
		'dk' => 'http://www.dk.net/nic/resIPno.html',
		'es' => 'http://www.nic.es/whois/',
		'fi' => 'http://www.thk.fi/englanti/internet/n2579.htm',
		'fm' => 'http://www.dot.fm/search.html',
		'fr' => 'http://www.nic.fr/info/whois.html',
		'ge' => 'http://georgia.net.ge/domain/',
		'gi' => 'http://www.gibnet.gi/nic/domains.html',
		'gr' => 'http://pythia.ics.forth.gr/cgi-bin/webwhois',
		'hm' => 'http://www.nic.hm/whois.html',
		'hr' => 'http://ds.carnet.hr/whois/diggere.html',
		'ie' => 'http://www.ucd.ie/hostmaster/dom-list.html',
		'im' => 'http://www.nic.im/exist.html',
		'io' => 'http://www.io.io/whois.html',
		'is' => 'http://www.isnet.is/en/sites.html',
		'it' => 'http://www.nis.garr.it/cgi-bin/HY/rc?/who/conf',
		'jp' => 'http://www.nic.ad.jp/cgi-bin/whois_gate',
		'kr' => 'http://www.krnic.net/cgi-bin/whois',
		'kz' => 'http://www.domain.kz/search',
		'li' => 'http://www.switch.ch/domain/search_form.html',
		'lk' => 'http://www.nic.lk/cgi-bin/whois',
		'lt' => 'http://vingis.sc-uni.ktu.lt/cgi-bin/whoisnoc?',
		'lu' => 'http://www.domain-registration.lu/whois.html',
		'mm' => 'http://www.nic.mm/whois.html',
		'mt' => 'http://www.um.edu.mt/nic/dir/',
		'mx' => 'http://www.nic.mx:81/cgi/db/dom',
		'nl' => 'http://www.domain-registry.nl/NLwhois.html',
		'nu' => 'http://www.nunames.nu',
		'nz' => 'http://servius.waikato.ac.nz/isocnz/nz-domain/DNSquery.html',
		'pa' => 'http://www.nic.pa/indice2.html',
		'pe' => 'http://ekeko.rcp.net.pe/rcp/PE-NIC/busqueda.html',
		'pk' => 'http://www.pknic.net.pk/pknic/assigned.htm',
		'pt' => 'http://www.fccn.pt/dns/pt.html',
		'ru' => 'http://www.ripn.net/nic/NICHomePage.html',
		'se' => 'http://www.sunet.se/domreg/',
		'sg' => 'http://www.nic.net.sg/sgnametk.htm',
		'sk' => 'http://www.eunet.sk/sk-nic/',
		'st' => 'http://www.nic.sh/cgi-bin/whois',
		'th' => 'http://www.thnic.net/whois.html',
		'tm' => 'http://www.nic.tm/cgi-bin/search',
		'to' => 'http://www.tonic.to/newname.htm',
		'tw' => 'http://www.twnic.net/ipdomain.html',
		'uk' => 'http://www.pipex.net/~guyd/rwhois.html',
		'us' => 'http://www.isi.edu:80/in-notes/usdnr/rwhois.html',
		've' => 'http://www.nic.ve/nicwho01.html',
		'yu' => 'http://ubbg.etf.bg.ac.yu/yu-tld/domains.html',
		'za' => 'http://co.za/whois.shtml'
		);

	function getRegistrar($cc) {
		$cc = trim(strtolower($cc));
		$temp = $this->REGISTRARS;
		if ($temp[$cc]) {
			return $temp[$cc];
		} else {
			return false;
		}
	}
	*/

	// showObject() and debugObject()
	// - debug code to show an object or array

	function showObject(&$obj)
		{
		$r = $this->debugObject($obj);
		return "<PRE>$r</PRE>\n";
		}

	function debugObject($obj,$indent=0)
		{
		if (is_Array($obj))
			{
			$return = '';
			foreach($obj as $k => $v)
				{
				$return .= str_repeat('&nbsp;',$indent);
				$return .= $k."->$v\n";
				$return .= $this->debugObject($v,$indent+1);
			}
			return $return;
		}
	}             

	function ns_rr_defined($query) {
		return checkdnsrr($query,'NS');
	}       

	// get nice HTML output
	
	function showHTML($result, $link_myself=true, $params='query=$0&amp;output=nice') {
		
		// adds links fort HTML output
		
		$email_regex = "/([-_\w\.]+)(@)([-_\w\.]+)\b/i";
		$html_regex = "/(?:^|\b)((((http|https|ftp):\/\/)|(www\.))([\w\.]+)([,:%#&\/?~=\w+\.-]+))(?:\b|$)/is";
		$ip_regex = "/\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b/i";
		
		$out = '';
		$lempty = true;
		
		foreach($result['rawdata'] as $line)
			{
			$line = trim($line);

			if ($line == '')
				{
				if ($lempty) continue;
				else $lempty = true;
				}
			else
				$lempty = false;
				
			$out .= $line."\n";
			}

		if ($lempty) $out = trim($out);

		$out = strip_tags($out);
		$out = preg_replace ($email_regex, '<a href="mailto:$0">$0</a>', $out); 
		$out = preg_replace_callback ($html_regex, 'href_replace', $out); 
		
		if ($link_myself)
			{
			if ($params[0] == '/')
				$link = $params;
			else
				$link = $_SERVER['PHP_SELF'].'?'.$params;
			
			$out = preg_replace ($ip_regex, '<a href="'.$link.'">$0</a>', $out); 			
			
			if (isset($result['regrinfo']['domain']['nserver']))
				{
				$nserver = $result['regrinfo']['domain']['nserver'];
				}
			else
				$nserver = false;
				
			if (isset($result['regrinfo']['network']['nserver']))
				{
				$nserver = $result['regrinfo']['network']['nserver'];
				}
				
			if (is_array($nserver))
				{
				reset($nserver); 
				while (list($host, $ip) = each($nserver))
					{
					$url = '<a href="'. str_replace('$0',$ip,$link)."\">$host</a>";
					$out = str_replace($host, $url, $out);
					$out = str_replace(strtoupper($host), $url, $out);
					}
				}
			}
		
		// Add bold field names
		
		$out = preg_replace ("/(?m)^([-\s\.&;'\w\t\(\)\/]+:\s*)/", '<b>$1</b>', $out);
		
		// Add italics for disclaimer
		
		$out = preg_replace ("/(?m)^(%.*)/", '<i>$0</i>', $out);
			
		return str_replace("\n","<br/>\n",$out);
	}
}

function href_replace($matches)
{
if (substr($matches[0],0,4)=='www.')
	{
	$web=$matches[0];
	$url='http://'.$web;
	}
else
	{
	$web=$matches[0];
	$url=$web;
	}
	
return '<a href="'.$url.'" target="_blank">'.$web.'</a>';
}
?>
