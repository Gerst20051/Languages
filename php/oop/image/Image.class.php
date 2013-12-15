<?php
class Image {
	
	// class atributes (variables)
	protected $image;
	protected $width;
	protected $height;
	protected $mimetype;
	
	function __construct($filename) {
		
		// read the image file to a binary buffer
		$fp = fopen($filename, 'rb') or die("Image '$filename' not found!");
		$buf = '';
		while(!feof($fp))
			$buf .= fgets($fp, 4096);
		
		// create image and assign it to our variable
		$this->image = imagecreatefromstring($buf);
		
		// extract image information
		$info = getimagesize($filename);
		$this->width = $info[0];
		$this->height = $info[1];
		$this->mimetype = $info['mime'];
	}
	
	public function display() {
		header("Content-type: {$this->mimetype}");
		switch($this->mimetype) {
			case 'image/jpeg': imagejpeg($this->image); break;
			case 'image/png': imagepng($this->image); break;
			case 'image/gif': imagegif($this->image); break;
		}
		//exit;
	}
}

// Thumbnail subclass
class Thumbnail extends Image {
	
	function __construct($image, $width, $height) {
		parent::__construct($image);
		
		$thumb = imagecreatetruecolor($width, $height);
		imagecopyresampled($thumb, $this->image, 0, 0, 0, 0, $width, $height, $this->width, $this->height);
		$this->image = $thumb;
	}
}
?>