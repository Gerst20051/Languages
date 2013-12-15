<?php
class Mail {
var $sendto = array();
var $acc = array();
var $abcc = array();
var $aattach = array();
var $xheaders = array();
var $priorities = array('1 (Highest)', '2 (High)', '3 (Normal)', '4 (Low)', '5 (Lowest)');
var $charset = "us-ascii";
var $ctencoding = "7bit";
var $receipt = 0;

function Mail(){
	$this->autoCheck(true);
	$this->boundary = "--".md5(uniqid("myboundary"));
}

function autoCheck($bool){
	if ($bool) $this->checkAddress = true;
	else $this->checkAddress = false;
}

function Subject($subject){
	$this->xheaders['Subject'] = strtr($subject, "\r\n", "  ");
}

function From($from){
	if (!is_string($from)){
		echo "Class Mail: error, From is not a string";
		exit;
	}
	$this->xheaders['From'] = $from;
}

function ReplyTo($address){
	if (!is_string($address)) return false;
	$this->xheaders["Reply-To"] = $address;
}

function Receipt(){
	$this->receipt = 1;
}

function To($to){
	if (is_array($to)) $this->sendto= $to;
	else $this->sendto[] = $to;
	if ($this->checkAddress == true) $this->CheckAdresses($this->sendto);
}

function Cc($cc){
	if (is_array($cc)) $this->acc= $cc;
	else $this->acc[]= $cc;
	if ($this->checkAddress == true) $this->CheckAdresses($this->acc);
}

function Bcc($bcc){
	if (is_array($bcc)) $this->abcc = $bcc;
	else $this->abcc[] = $bcc;
	if ($this->checkAddress == true) $this->CheckAdresses($this->abcc);
}

function Body($body, $charset=""){
	$this->body = $body;
	if ($charset != "") {
		$this->charset = strtolower($charset);
		if ($this->charset != "us-ascii") $this->ctencoding = "8bit";
	}
}

function Organization($org){
	if (trim($org != "")) $this->xheaders['Organization'] = $org;
}

function Priority($priority){
	if (!intval($priority)) return false;
	if (!isset($this->priorities[$priority-1])) return false;
	$this->xheaders["X-Priority"] = $this->priorities[$priority-1];
	return true;
}

function Attach($filename, $filetype="", $disposition="inline"){
	if ($filetype == "") $filetype = "application/x-unknown-content-type";
	$this->aattach[] = $filename;
	$this->actype[] = $filetype;
	$this->adispo[] = $disposition;
}

function BuildMail(){
	$this->headers = "";
	if (count($this->acc) > 0) $this->xheaders['CC'] = implode(", ", $this->acc);
	if (count($this->abcc) > 0) $this->xheaders['BCC'] = implode(", ", $this->abcc);
	if ($this->receipt) {
		if (isset($this->xheaders["Reply-To"])) $this->xheaders["Disposition-Notification-To"] = $this->xheaders["Reply-To"];
		else $this->xheaders["Disposition-Notification-To"] = $this->xheaders['From'];
	}
	if ($this->charset != "") {
		$this->xheaders["Mime-Version"] = "1.0";
		$this->xheaders["Content-Type"] = "text/plain; charset=$this->charset";
		$this->xheaders["Content-Transfer-Encoding"] = $this->ctencoding;
	}
	$this->xheaders["X-Mailer"] = "Php/libMailv1.3";
	if (count($this->aattach) > 0) $this->_build_attachement();
	else $this->fullBody = $this->body;
	reset($this->xheaders);
	while (list($hdr, $value) = each($this->xheaders)) {
		if ($hdr != "Subject") $this->headers .= "$hdr: $value\n";
	}
}

function Send(){
	$this->BuildMail();
	$this->strTo = implode(", ", $this->sendto);
	$res = @mail($this->strTo, $this->xheaders['Subject'], $this->fullBody, $this->headers);
}

function Get(){
	$this->BuildMail();
	$mail = "To: ".$this->strTo."\n";
	$mail .= $this->headers."\n";
	$mail .= $this->fullBody;
	return $mail;
}

function ValidEmail($address){
	if (ereg(".*<(.+)>", $address, $regs)) $address = $regs[1];
 	if (ereg("^[^@  ]+@([a-zA-Z0-9\-]+\.)+([a-zA-Z0-9\-]{2}|net|com|gov|mil|org|edu|int)\$", $address)) return true;
 	else return false;
}

function CheckAdresses($aad){
	for ($i = 0; $i < count($aad); $i++) {
		if (!$this->ValidEmail($aad[$i])) {
			echo "Class Mail, method Mail : invalid address $aad[$i]";
			exit;
		}
	}
}

function _build_attachement(){
	$this->xheaders["Content-Type"] = "multipart/mixed;\n boundary=\"$this->boundary\"";
	$this->fullBody = "This is a multi-part message in MIME format.\n--$this->boundary\n";
	$this->fullBody .= "Content-Type: text/plain; charset=$this->charset\nContent-Transfer-Encoding: $this->ctencoding\n\n".$this->body."\n";
	$sep = chr(13).chr(10);
	$ata = array();
	$k = 0;
	for ($i = 0; $i < count($this->aattach); $i++) {
		$filename = $this->aattach[$i];
		$basename = basename($filename);
		$ctype = $this->actype[$i];
		$disposition = $this->adispo[$i];
		
		if (!file_exists($filename)) {
			echo "Class Mail, method attach : file $filename can't be found"; exit;
		}
		$subhdr = "--$this->boundary\nContent-type: $ctype;\n name=\"$basename\"\nContent-Transfer-Encoding: base64\nContent-Disposition: $disposition;\n  filename=\"$basename\"\n";
		$ata[$k++] = $subhdr;
		$linesz = filesize($filename)+1;
		$fp = fopen($filename, 'r');
		$ata[$k++] = chunk_split(base64_encode(fread($fp, $linesz)));
		fclose($fp);
	}
	$this->fullBody .= implode($sep, $ata);
}
}
?>