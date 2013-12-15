// Item -----------------------------------------------------------------------
function Item(gallery) {
  //log('Making new Item. ' + this);
  this.image = document.createElement('img');
  this.image_class = 'image';
  this.thumb = document.createElement('img');
  this.loading_icon = document.createElement('img');
  this.thumb_attempts_ = 0;
  this.gallery_ = gallery;
  this.gallery_.addListener(this.galleryListener_.bind(this));
  this.listeners_ = [];
  this.width = 0;
  this.height = 0;
      
  this.loaded = false;
  this.resized_loaded = false;
  this.file_ = null;

  this.id = null;
  this.name = null;
  this.type = null;
  this.queue_delete_ = false;
  
  this.caption = '';
  this.friendly_time = '';
  
  // #131, #161
  this.viewer = null; // main element containing the audio/video player, text viewer, to be displayed instead of this.image
  this.viewer_type = null; // 'audio', 'video', 'audio_flash', 'text_viewer', ...
  this.viewer_loaded = false;
  
  // hide edit filename pencil
  this.hide_edit_timeout = null;
}

Item.EVENT_LOADED = 0;
Item.EVENT_UPLOADED = 1;
Item.EVENT_DELETED = 2;
Item.EVENT_RELOAD = 3;
Item.EVENT_RESIZED_LOADED = 4;
Item.EVENT_VIDEO_START_LOADING = 5;
Item.EVENT_VIDEO_LOADED = 6; // video display loaded

Item.DEFAULT_CAPTION_TEXT = 'Add Caption';
Item.FILENAME_MAXLENGTH = 100; // not including extension

Item.ERROR_FILENAME_REQUIRED = 'Filename required';
Item.ERROR_FILENAME_MAXLENGTH = 'Up to ' + Item.FILENAME_MAXLENGTH + ' characters only'

Item.PLAYER_SKIN_CONF = {
buttonOverColor: '#000000',
backgroundColor: '#CCCCCC',
backgroundGradient: 'none',
sliderColor: '#FFFFFF',
sliderBorder: '1px solid #808080',
volumeSliderColor: '#FFFFFF',
volumeBorder: '1px solid #808080',
timeColor: '#000000',
durationColor: '#737373'
};

Item.unknownFileCount = 1;

Item.generateUnknownFileName = function() {
  return 'file ' + Item.unknownFileCount++;
}

// List of icons available.
Item.ICONS = ['7z', 'ai', 'aif', 'aifc', 'aiff', 'asf', 'asp', 'asr', 'asm',
  'au', 'avi', 'cab', 'dgn', 'dmg', 'doc', 'docx', 'drw', 'dwg', 'dxf', 'fla', 'flv', 
  'ft10', 'html', 'iam', 'iges', 'igs', 'indd', 'iso', 'ipt', 'jsf', 'jsp', 'lzh', 'm4p', 
  'm4u', 'mdb', 'mid', 'mov', 'mp2', 'mp3', 'mp4', 'mpa', 'mpe',
  'mpeg', 'mpg', 'mxp', 'par', 'pdf', 'php', 'ppt', 'pptx', 'prt', 'psd', 'rar', 'rtf', 'skp',
  'stp', 'swf', 'txt', 'vwx', 'wav', 'xls', 'xlsx', 'xpr', 'xps', 'zip'];
Item.GENERIC_ICON = 'generic_archive';

// Return the src of the icon if the file is not an image.
Item.prototype.getIcon = function (default_icon) {
  var ext = '' + extension(this.name);
  ext = ext.toLowerCase();
  
  if (typeof default_icon == 'undefined') {
    default_icon = false;
  }
  if (typeof new_ext != 'undefined' && new_ext.length > 0) {
    ext = new_ext;
  }
  
  // priority pdf
  if (streql_in(ext, 'pdf') && !default_icon) {
    var pdf_icon_src = this.getPDFPreviewSRC();
	if (pdf_icon_src) {
	  this.image_class = 'image';
	  return pdf_icon_src;
	}
  }
  
  this.image_class = 'file_icon';
  if ($.inArray(ext, Item.ICONS) >= 0) {
    return Gallery.URL_FILE_ICONS + ext + '.png';
  } else {
    // use the generic icon
    return Gallery.URL_FILE_ICONS + Item.GENERIC_ICON + '.png';
  }
};

Item.getSmallIcon = function(name) {
  var ext = name.substr(1);
  //var ext = '' + extension(name);
  if ($.inArray(ext, Item.ICONS) >= 0) {
    return Gallery.URL_FILE_ICONS_SMALL + ext + '.png';
  } else {
    // use the generic icon
    return Gallery.URL_FILE_ICONS_SMALL + Item.GENERIC_ICON + '.png';
  }
};

// Returns view/download link and resized image src url
Item.prototype.getLink = function(thumb) {
  var valid_image = Gallery.isValidImage(this);
  var base = '';
  var ext = extension(this.name);
  var id_pref = Gallery.URL_ITEM;
  if (typeof thumb !== 'undefined' && thumb) {
    id_pref = Gallery.URL_THUMB;
  }
  if (is_debug()) {
    if (ext) {
      //return window.location.protocol + '//' + window.location.host + id_pref + this.id + '.' + ext;
	  return id_pref + this.id + '.' + ext;
	} else {
	  return id_pref + this.id;
	}
  } else {
    if (valid_image) {
	  //base = Gallery.URL_IMAGE_DOWNLOAD;
	  base = Gallery.URL_FILE_DOWNLOAD;
	} else {
	  base = Gallery.URL_FILE_DOWNLOAD;
	}
	if (ext) {
	  return  base + id_pref + this.id + '.' + extension(this.name);
	} else {
	  return base + id_pref + this.id;
	}
  }
}

// Returns the individual image view link (empty string when item is not an image)
Item.prototype.getImageViewLink = function() {
  if (Gallery.isValidImage(this)) {
    //return window.location.protocol + '//' + 'min.us' + Gallery.URL_THUMB + this.id;
	return window.location.protocol + '//' + 'min.us' + '/l' + this.id;
  }
  return '';
}

Item.prototype.generateViewer_ = function() {
  // use flash player
  var viewer_funcs = [this.getMusicPlayer.bind(this, true), this.getVideoPlayer.bind(this), this.getTextReader.bind(this)];
  //var viewer = null;
  // TODO add video and text here
  //this.viewer = this.getMusicPlayer();
  for (var i in viewer_funcs) {
    this.viewer = viewer_funcs[i]();
	if (this.viewer !== null) {
	  break;
	}
  }
}

Item.prototype.updateViewer_ = function(item_node) {
  if (this.viewer_type == 'text_viewer') {
    this.updateTextReader(item_node);
  } else if (this.viewer_type == 'video') {
    this.updateVideoPlayer(item_node);
  }
}

// call after adding the element to body
Item.prototype.viewerAdded_ = function() {
  if (this.viewer_type == 'audio_flash') {
    this.getFlashMusicPlayer();
  }
}

// Return audio tag if mp3/ogg
Item.prototype.getMusicPlayer = function(flash) {
  if (typeof flash !== 'undefined' && flash) {
    return this.getFlashMusicContainer();
  }
  
  var ext = extension(this.name);
  if ((ext == 'mp3' || ext == 'ogg') && this.id !== null) {
    //var audio = $('<audio>', {
    //  'controls': 'controls'	
    //});
	var audio = document.createElement('audio');
	audio.setAttribute('controls', 'controls');
	//audio.setAttribute('loop', 'loop');
	//audio.setAttribute('preload', 'auto');
	
	if (!!audio.canPlayType) {
	  var can_play_mp3 = !!audio.canPlayType && "" != audio.canPlayType('audio/mpeg');
      var can_play_ogg = !!audio.canPlayType && "" != audio.canPlayType('audio/ogg; codecs="vorbis"');
	  if ((ext == 'mp3' && can_play_mp3) || (ext == 'ogg' && can_play_ogg)) {
        add_source(audio, '/i' + this.id + '.' + ext);
		// TODO add fallback format? flash player?
		// Add events
		// play (pause other players that are playing??)
		
		if (BrowserDetect.browser == 'Firefox') {
		  $(audio)
		    .css('height', '100px')
			.css('margin-top', '-50px');
		}
		
		$(audio).bind('play', function() {
		  this.blur();
		  //window.console.log('----a play');
		  $('audio').each(function() {
		    if (this.currentTime > 0 && this != audio) {
			  this.pause();
			}
		  });
		});
		
		/*
		$(audio).bind('ended', function() {
		  this.currentTime = 0;
		  this.pause();
		});
		*/

		$(audio).keypress(function(e) {
		  switch(e.keyCode) {
		    case 34: 
		    case 39:
		    case 40:
			  this.blur();
			  e.preventDefault();
			  //this.pause();
			  return false;
			  break;
			case 33:
		    case 37:
		    case 38:
			  this.blur();
			  e.preventDefault();
			  //this.pause();
			  return false;
			  break;
		  }
		});
		
		this.viewer_type = 'audio';
		return audio;
		//return null;
	  }
	}
  }
  return null;
}

Item.prototype.getFlashMusicContainer = function() {
  var ext = extension(this.name) + '';
  if (streql_in(ext, 'mp3') && this.id != null) {
    //var div = $('<div>', {'id': 'audio-' + this.id, 'href': this.getLink()});
	var div = document.createElement('div');
	div.setAttribute('id', 'audio-' + this.id);
	div.setAttribute('class', 'audio_flash');
	div.setAttribute('href', this.getLink());
	$(div)
	  .css('width', '350px')
	  .css('height', '30px')
	  .css('margin-left', '-175px')
	  .css('margin-top', '-15px')
	  .css('top', '50%')
	  .css('left', '50%')
	  .css('position', 'absolute');
	
	this.viewer_type = 'audio_flash';
	return div;
	//return null;
  }
  return null;
}

Item.prototype.getFlashMusicPlayer = function() {
  var ext = extension(this.name) + '';
  var this_item = this;
  var controls_opts = Item.PLAYER_SKIN_CONF;
  controls_opts.fullscreen = false;
  controls_opts.height = 30;
  controls_opts.autoHide = false;
  try {
    if (ext == 'mp3' && this.id !== null) {
      $f('audio-'+this.id, {src: Gallery.URL_MUSIC_PLAYER, wmode: 'opaque'}, {
	    plugins: {
		  controls: controls_opts
	    },
	    clip: {
	      autoPlay: false,
		  onBeforeBegin: function() {
			$('.audio_flash').each(function() {
		      if (this_item.id !== $(this).attr('id')) {
			    $f($(this).attr('id')).pause();
			  }
			});
		  }
	    }
	  });
    }
  } catch (e) {}
}

Item.prototype.getVideoPlayer = function() {
  var check_videos = {
    'webm': 'video/webm; codecs="vp8, vorbis"',
	'ogv': 'video/ogg; codecs="theora, vorbis"',
	'mp4': 'video/mp4; codecs="avc1.42E01E, mp4a.40.2"'
  };
  var ext = extension(this.name) + '';
  var this_item = this;
  if (this.id !== null) {
    var video_cont = document.createElement('div');
    var video = document.createElement('video');
	video.setAttribute('controls', 'controls');
    for (var video_ext in check_videos) {  
	  if (!!video.canPlayType && streql_in(ext, video_ext)) {
	    if (video.canPlayType(check_videos[video_ext]) != "") {
		  if (ext == 'mp4') { 
		    add_source(video, '/i' + this.id + '.' + ext, 'video/mp4');
		  } else {
	        add_source(video, '/i' + this.id + '.' + ext);
		  }
		  this.viewer_type = 'video';
		  
		  $(video).bind('loadeddata', function() {
		    var video_height = $(video).height();
		    var video_width = $(video).width();

		    $(this)
			  .css('margin-top', -video_height/2)
			  .css('margin-left', -video_width/2);
			  
			  this_item.callListeners_({
			    type: Item.EVENT_VIDEO_LOADED
			  });
		  });
		  
		  $(video).bind('loadstart', function() {
		    this_item.callListeners_({
			  type: Item.EVENT_VIDEO_START_LOADING
			});
		  });
		  
		  $(video).bind('loadedmetadata', function() {
			$(video).unbind('progress');
			$(video)
			  .css('height', '')
			  .css('width', '');
		    this_item.callListeners_({
			  type: Item.EVENT_VIDEO_START_LOADING
			});
		  });
		  
		  var presenter_node = $('div.presentation');
		  var max_width = presenter_node.outerWidth() - Presenter.getWidthSpaceValue();
	      var max_height = presenter_node.outerHeight() - Presenter.MIN_EDGE_SPACING * 2;

		  $(video)
			.css('height', '240px')
			.css('width', '320px')
			.css('max-height', max_height + 'px')
			.css('max-width', max_width + 'px');
	      return video;
		  
		  //return video_cont;
		}
	  }
	}
  }
  return null;
}

Item.prototype.updateVideoPlayer = function(item_node) {
  var presenter_node = $('div.presentation');
  var max_width = presenter_node.outerWidth() - Presenter.getWidthSpaceValue();
  var max_height = presenter_node.outerHeight() - Presenter.MIN_EDGE_SPACING * 2;
  var min_width = 320;
  var min_height = 240;
  
  if (max_width < min_width || max_height < min_height) {
    max_height = min_height;
    max_width = min_width;
  }
  
  var viewer_to_update = $(this.viewer);
  if (typeof item_node !== 'undefined' && item_node.viewer) {
    viewer_to_update = item_node.viewer; // priority
  }
  
  $(viewer_to_update)
	.css('max-width', max_width + 'px')
    .css('max-height', max_height + 'px');
}

Item.prototype.getTextReader = function() {
  var ext = extension(this.name) + '';
  if (Gallery.isTextViewable(this) && this.id) {
    var div = document.createElement('div');
	div.setAttribute('id', 'text-' + this.id);
	var pre = document.createElement('pre');
	pre.innerHTML = 'Loading text...';
	div.appendChild(pre);
	div.className = 'text_viewer';
	$(div)
	  .css('height', '300px')
	  .css('width', '500px')
	  .css('margin-left', '-250px')
	  .css('margin-top', '-150px')
	  .css('top', '50%')
	  .css('left', '50%')
	  .css('position', 'absolute');
	this.viewer_type = 'text_viewer';
	
	$.ajax({
	  url: Gallery.URL_THUMB + this.id + '.' + extension(this.name),
	  type: 'GET',
	  dataType: 'text',
	  success: function(text, text_status, xml_req) {
	    $(pre).text(text);
	  }
	});
	
	return div;
  }
  return null;
}

Item.prototype.updateTextReader = function(item_node) {
  if (this.viewer_type == 'text_viewer') {
    //if ($(this.viewer).is(':visible')) {
	  var presenter_node = $('div.presentation');
	  var max_width = presenter_node.outerWidth() - Presenter.getWidthSpaceValue();
	  var max_height = presenter_node.outerHeight() - Presenter.MIN_EDGE_SPACING * 2;
	  var min_width = 450;
	  var min_height = 300;
	  var height = 0;
	  var width = 0;
	  
	  if (max_width < min_width || max_height < min_height) {
	    height = min_height;
		width = min_width;
	  } else {
	    width = max_width;
		height = max_height;
	  }
	  
	  var viewer_to_update = $(this.viewer);
	  if (typeof item_node !== 'undefined' && item_node.viewer) {
	    viewer_to_update = item_node.viewer; // priority
	  }
      $(viewer_to_update)
	    .css('height', height + 'px')
		.css('width', width + 'px')
		.css('margin-top', (-height/2) + 'px')
		.css('margin-left', (-width/2) + 'px');
	
	//}
  }
}

Item.prototype.getPDFPreviewSRC = function() {
  var ext = extension(this.name) + '';
  if (streql_in(ext, 'pdf') && this.id) {
    var src = '';
	if (!is_debug()) {
	  src += Gallery.URL_IMAGE_DOWNLOAD;
	}
	src += Gallery.URL_THUMB + this.id + '.jpg';
	return src;
  }
  return null;
}

/*
Item.prototype.getPDFPreview = function() {
  var ext = extension(this.name) + '';
  var this_item = this;
  if (streql_in(ext, 'pdf') && this.id) {
    var src = '';
	if (!is_debug()) {
	  src += Gallery.URL_IMAGE_DOWNLOAD;
	}
	src += Gallery.URL_THUMB + this.id + '.jpg';
	var img = document.createElement('img');
	img.src = src;
	img.className = 'pdf_preview';
	addEventListenerWrapper(img, 'load', function() {
	  this.viewer_loaded = true;
	  this_item.gallery_.callListeners_({
	    type: Gallery.EVENT_ITEM_VIEWER_LOADED
	  });
	});
	addEventListenerWrapper(img, 'error', function() {
	  //this.src = this_item.getIcon(); // get the pdf icon
	});
	this.viewer_type = 'pdf_preview';
    return img;
  }
  return null;
}
*/

Item.blur = function(img, radius) {
  // From http://www.java2s.com/Code/Java/Advanced-Graphics/FastBlurDemo.htm.
  // We actually keep the unpremultiplied values as it gives a nice border
  // effect on the images.
  var windowSize = radius * 2 + 1;
  var radiusPlusOne = radius + 1;
  var width = img.width;
  var height = img.height;
  var sumAlpha;
  var sumRed;
  var sumGreen;
  var sumBlue;
  var nextPixelIndex, previousPixelIndex;

  var sumLookupTable = [];
  for (var i = 0; i < 256 * windowSize; i++) {
    sumLookupTable[i] = i / windowSize;
  }

  var indexLookupTable = [];
  if (radius < width) {
    for (var i = 0; i < radiusPlusOne; i++) {
      indexLookupTable[i] = i;
    }
  } else {
    for (var i = 0; i < width; i++) {
      indexLookupTable[i] = i;
    }
    for (var i = width; i < radiusPlusOne; i++) {
      indexLookupTable[i] = width - 1;
    }
  }

  var srcIndex = 0;
  var dstIndex;
  var srcPixels = img.data;
  var dstPixels = [];

  for (var u = 0; u < 4; u++) {
    for (var y = 0; y < height; y++) {
      sumAlpha = sumRed = sumGreen = sumBlue = 0;
      dstIndex = y;

      var index = srcIndex * 4;
      sumAlpha += radiusPlusOne * (srcPixels[index + 3]);
      sumRed   += radiusPlusOne * (srcPixels[index + 0]);
      sumGreen += radiusPlusOne * (srcPixels[index + 1]);
      sumBlue  += radiusPlusOne * (srcPixels[index + 2]);

      for (var i = 1; i <= radius; i++) {
        var index = (srcIndex + indexLookupTable[i]) * 4;
        sumAlpha += srcPixels[index + 3];
        sumRed   += srcPixels[index + 0];
        sumGreen += srcPixels[index + 1];
        sumBlue  += srcPixels[index + 2];
      }

      for  (var x = 0; x < width; x++) {
        dstPixels[dstIndex * 4 + 3] = sumLookupTable[sumAlpha];
        dstPixels[dstIndex * 4 + 0] = sumLookupTable[sumRed];
        dstPixels[dstIndex * 4 + 1] = sumLookupTable[sumGreen];
        dstPixels[dstIndex * 4 + 2] = sumLookupTable[sumBlue];

        dstIndex += height;

        var nextPixelIndex = x + radiusPlusOne;
        if (nextPixelIndex >= width) {
          nextPixelIndex = width - 1;
        }

        var previousPixelIndex = x - radius;
        if (previousPixelIndex < 0) {
          previousPixelIndex = 0;
        }

        var nextPixelIndex = (srcIndex + nextPixelIndex) * 4;
        var previousPixelIndex = (srcIndex + previousPixelIndex) * 4;

        sumAlpha += srcPixels[nextPixelIndex + 3];
        sumAlpha -= srcPixels[previousPixelIndex + 3];

        sumRed += srcPixels[nextPixelIndex + 0];
        sumRed -= srcPixels[previousPixelIndex + 0];

        sumGreen += srcPixels[nextPixelIndex + 1];
        sumGreen -= srcPixels[previousPixelIndex + 1];

        sumBlue += srcPixels[nextPixelIndex + 2];
        sumBlue -= srcPixels[previousPixelIndex + 2];
      }
      srcIndex = y * width;
    }

    for (var i = 0; i < img.data.length; i++) {
      srcPixels[i] = dstPixels[i];
    }
  }
  return img;
}

//Item.blurImage = function(image, amount, width, height) {
Item.blurImage = function(image, amount, width, height, thumb) {
  // Firefox 3 fails at putImageData - I suspect the blur 
  // algorithm may be to blame, however (clamping dstPixels
  // causes all sorts of tomfoolery).
  /*
  if (isValidBrowser() && BrowserDetect.version >= 4) {
    // Create the canvas.
    var canvas = document.createElement('canvas');

    canvas.width = width;
    canvas.height = height;
    var ctx = canvas.getContext("2d");

    // Place the image.
    var scale = min((width - amount * 4) / image.width, (height - amount * 4) / image.height);
    scale = (scale < 1) ? scale : 1;
    var scaled_width = image.width * scale;
    var scaled_height = image.height * scale;
    ctx.drawImage(image,
        canvas.width / 2 - scaled_width / 2,
        canvas.height / 2 - scaled_height / 2,
        scaled_width, scaled_height);

    // Blur the image.
    //var img = ctx.getImageData(0, 0, canvas.width, canvas.height);
    //img = Item.blur(img, amount);
    //ctx.putImageData(img, 0, 0);

    // Export the image.
    var new_image = new Image();
    new_image.src = canvas.toDataURL();
    canvas = null;
    return img;		
  }
  window.console.log("Blurring failed");
  */
  var img = new Image();
  img.src = image.src;

  if (image.width > image.height) {
    height = parseInt(height * (image.height / image.width));
  } else {
    width = parseInt(width * (image.width / image.height));
  }
  
  //if (image.width == image.height && image.width < width) {
  if (image.width < width && image.height < height) {
    width = image.width;
	height = image.height;
  }

  img.width = width;
  img.height = height;
  
  //window.console.log('----a blurimage, height: ' + height);
  //window.console.log('----a blurimage, width: ' + width);
  //window.console.log('----a blurimage, image.height: ' + image.height);
  //window.console.log('----a blurimage, image.width: ' + image.width);
  
  if (typeof thumb != 'undefined' && thumb != null) {
    //window.console.log('----a thumb not null / undefined');
    thumb.src = image.src;
	thumb.width = width;
	thumb.height = height;
	thumb.style.width = width + 'px';
	thumb.style.height = height + 'px';
	thumb.style.top = -parseInt(height / 2) + 'px';
    thumb.style.left = -parseInt(width / 2) + 'px';
  }
  
  return img;
  //return image;
}

Item.prototype.galleryListener_ = function(e) {
  if (e.type == Gallery.EVENT_SAVEDID) {
    if (this.queue_delete_) {
      this.deleteItem();
    } else {
      this.saveFile_();
    }
  }
}

Item.prototype.addListener = function(listener) {
  this.listeners_.push(listener);
}

Item.prototype.callListeners_ = function(e) {
  for (var i = 0, func; func = this.listeners_[i]; i++) {
    func(e);
  }
}

Item.prototype.setFile = function(file) {
  this.file_ = file;
  this.name = this.file_.name;
  this.type = this.file_.type;

  if (this.gallery_.id) {
    this.saveFile_();
  }
  // Otherwise saveFile will be triggered when Gallery 
  // broadcasts EVENT_SAVEDID.
}

Item.prototype.setID = function(id) {
  this.id = id;
}

Item.prototype.setName = function(name) {
  this.name = name;
}

Item.prototype.updateCaption = function(display_div, edit_div, text_box) {
  var picture_info = $(display_div).parent();
  var picture_link = picture_info.next();
  var caption_loading = $(edit_div).find('.caption_loading');
  var success_func = function() {
    this.gallery_.callListeners_({
	  type: Gallery.EVENT_END_AJAX
	});
    $(text_box).attr('disabled', false);
	$(display_div).show();
    $(edit_div).hide();
	$(text_box).show();
	caption_loading.hide();
	
	// TODO get sanitized caption?
	this.caption = $(text_box).val();
	var text_box_caption = $(text_box).val();
	// Updates the display caption div
	if (text_box_caption.length > 0) {
      $(display_div).find('span.caption_text').text(text_box_caption).show();
      $(display_div).find('span.caption_add').hide();
      $(display_div).find('img').show();
    } else {
      $(display_div).find('span.caption_add').show();
      $(display_div).find('span.caption_text').hide();
      $(display_div).find('img').hide();
	}
	
	this.gallery_.callListeners_({
	  type: Gallery.EVENT_ITEM_CAPTION_UPDATE
	});
  }.bind(this);
  
  $.ajax({
    url: Gallery.URL_CHANGE_ITEM_CAPTION,
    type: 'POST',
	dataType: 'json',
	data: $.param({id: this.id, caption: $(text_box).val(), edit_url: this.gallery_.id}),
	beforeSend: function() {
	  $(text_box).attr('disabled', true);
	  $(text_box).hide();
	  //$(text_box).blur(); // Opera fix
	  var picture_info_mt = parseFloat(picture_info.css('margin-top'));
	  var picture_link_mt = parseFloat(picture_link.css('margin-top'));
	  caption_loading.show();
	  picture_info.css('margin-top', picture_info_mt+$(text_box).height()-caption_loading.height());
	  /*
	  if (BrowserDetect.browser != "Opera") {
	    picture_link.css('margin-top', picture_link_mt-$(text_box).height()+caption_loading.height());
	  } else {
	    picture_link.css('margin-top', picture_link_mt-15);
	  }
	  */
	  this.gallery_.callListeners_({
	    type: Gallery.EVENT_NEW_AJAX
	  });
	}.bind(this),
	success: success_func,
	error: function() {
	  this.gallery_.callListeners_({
	    type: Gallery.EVENT_END_AJAX
	  });
      $(text_box).attr('disabled', false);
	  $(this).blur();
	  $(display_div).show();
      $(edit_div).hide();
	  $(text_box).val(this.caption).change();
	  caption_loading.hide();
	  valid_input($(text_box));
	  $(text_box).show();
	}.bind(this)
  });
}

Item.prototype.getTextInfo = function() {
  var text = '';
  var divider = '&nbsp · &nbsp ';
  var views = '### Views';
  var bandwidth = '### Bandwidth';
  var submitted = 'Submitted ' + this.friendly_time + ' ago';
  var item_name_trunc = this.name;
  if (item_name_trunc.length > 20) {
    item_name_trunc = item_name_trunc.substr(0, 17) + '...';
  }
  if (Gallery.isValidImage(this)) {
	text = item_name_trunc + '&nbsp · &nbsp ' + this.width + ' by ' + this.height + divider + this.filesize;
  } else {
	text = item_name_trunc + divider + this.filesize;
  }
  //text +=  divider + views + '<br/>' + bandwidth + divider + submitted;
  //text +=  divider + views + divider + bandwidth + divider + submitted;
  text +=  divider + submitted;
  return text;
}

Item.prototype.load = function() {
  window.console.log("Item.prototype.load...");
  $('div.presentation').css('z-index','10');
  
  if (this.id) {
    var url = Gallery.URL_ITEM + this.id+'/'+this.name;
    window.console.log("Loading item from: " + url);
    $('div.dm-editor-status').html('Loading '+this.name+'...').show();
	
    this.image = new Image();

	// use an icon if the file is not an image
	if (!Gallery.isValidImage(this)) {
	  addEventListenerWrapper(this.loading_icon, 'error', this.loadImageComplete_.bind(this));
	  addEventListenerWrapper(this.loading_icon, 'load', this.loadImageComplete_.bind(this));
	  this.loading_icon.src = Gallery.URL_IMG_LOADING_ICON;
	
	  // 'delaying', just in case the resized image is already loaded, cached
	  setTimeout(function() {
	    addEventListenerWrapper(this.image, 'load', this.loadResizedImageComplete_.bind(this));
		addEventListenerWrapper(this.image, 'error', function() {
	      this.image.src = this.getIcon(true);
	    }.bind(this));
	    this.image.src = this.getIcon();
	  }.bind(this), 100);
	} else {
	  // #184, still create the view even if image failed to load or the image is invalid
	  /*
	  if (BrowserDetect.browser == 'Explorer' && BrowserDetect.version == 7) { // TODO debugging IE7, remove later
	    var hashdebug = function() { window.location.hash = '#hashdebug-' + this.name + Math.floor(Math.random()*10000000);}.bind(this);
  		// bug here
		//addEventListenerWrapper(this.loading_icon, 'load', this.loadImageComplete_.bind(this));
	    //this.loading_icon.src = '/j'+this.id+'.'+extension(this.name); // no bug here (unless using same image as Gallery.URL_IMG_LOADING_ICON)
		//this.loading_icon.src = Gallery.URL_IMG_LOADING_ICON; // bug here definitely 
  
		//addEventListenerWrapper(this.image, 'load', hashdebug);
		//this.image.src = '/j'+this.id+'.'+extension(this.name);
		
		//this.loading_icon.src = '/j'+this.id+'.'+extension(this.name);
		//this.loadImageComplete_.bind(this)();
			
	  } else {*/
	    addEventListenerWrapper(this.loading_icon, 'error', this.loadImageComplete_.bind(this));
	    addEventListenerWrapper(this.loading_icon, 'load', this.loadImageComplete_.bind(this));
	    this.loading_icon.src = Gallery.URL_IMG_LOADING_ICON;

	
		  // 'delaying', just in case the resized image is already loaded, cached
		  setTimeout(function() {
			addEventListenerWrapper(this.image, 'load', this.loadResizedImageComplete_.bind(this));
			//this.image.src = '/j'+this.id+'.'+extension(this.name);
			this.image.src = this.getLink(true);
		  }.bind(this), 100);
	  //}
	}
  } else if (this.file_) {
    this.name = this.file_.name;
    this.type = this.file_.type;

	if (!Gallery.isValidImage(this)) {
	  this.image = new Image();
	  addEventListenerWrapper(this.image, 'load', function() {
	    this.resized_loaded = true;
	    this.loadImageComplete_();
	  }.bind(this));
	  addEventListenerWrapper(this.image, 'error', function() {
	    this.image.src = this.getIcon(true);
	  }.bind(this));
      this.image.src = this.getIcon();
    } else {
	  if (Gallery.isLargeImage(this)) {
	    this.image = new Image();
		addEventListenerWrapper(this.loading_icon, 'error', this.loadImageComplete_.bind(this));
	    addEventListenerWrapper(this.loading_icon, 'load', this.loadImageComplete_.bind(this));
	    this.loading_icon.src = Gallery.URL_IMG_LOADING_ICON;
	
	  } else {
	    // Load the file.
        var reader = new FileReader();
        window.console.log("Item.prototype.load. Name: "+this.name+". id: "+this.id);
        reader.readAsDataURL(this.file_);
        reader.onloadend = this.loadLocalFileComplete_.bind(this);
	  }
	}
  } else {
    throw new Error("Item load: no data specified.");
  }
}

Item.prototype.loadLocalFileComplete_ = function(e) {
  this.resized_loaded = true;
  this.image.src = e.target.result;
  //window.console.log("Item.prototype.loadLocalFileComplete_: Name: "+this.name +". ID: "+this.id);
  // Need to give the image time to 'soak' before using it,
  // HACK: otherwise calamity (blankness) strikes.
  setTimeout(this.loadImageComplete_.bind(this), 1);
}

Item.prototype.loadImageComplete_ = function(e) {
  window.console.log("Item.prototype.loadImageComplete_: Name: "+this.name +". ID: "+this.id);
  this.generateViewer_();
  this.generateThumb_();
  
  /*
  if (BrowserDetect.browser == 'Explorer' && BrowserDetect.version == 7) { // TODO debugging IE7, remove later
	window.location.hash = '#' + 'item-loadImageComplete_-testingie7-' + Math.floor(Math.random()*10000000) + '-' + this.name;
  }
  */
}

Item.prototype.generateThumb_ = function() {
  window.console.log("Item.prototype.generateThumb_");
  //window.console.log('this.image.src ' + this.image.src);
  
  // Note: This is broken with CDN in chrome.
  this.thumb = Item.blurImage(this.image, 7, 240, 240);

  // Similar to loadFileComplete, we need to give the image time to
  // do whatever it does before going off and using it, otherwise it
  // will give strange results for image.width etc.
  /*
  if (this.thumb.src.length < 600 && this.thumb_attempts_ < 3) {
    window.console.log("Failed thumbnailing attempt");
    this.thumb_attempts_++;
    setTimeout(this.generateThumb_.bind(this), 1);
  } else {
  */
  
  this.loaded = true;
  window.console.log("Thumbnail generation complete");
  setTimeout(this.callListeners_.bind(this, {
    type : Item.EVENT_LOADED,
    data : this
  }), 1);
  window.console.log("Item.prototype.generateThumb_: complete");
}

Item.prototype.loadResizedImageComplete_ = function() {
  this.resized_loaded = true;
//  window.console.log('----a loadResizedImageComplete');
  this.callListeners_({
    type : Item.EVENT_RESIZED_LOADED,
    data : this
  });
}

// SAVING
Item.prototype.saveFile_ = function() {
  if (!this.file_ || this.id) return;
  
  //if (this.file_.size >= Gallery.LARGE_UPLOAD_SIZE) {
  //  window.console.log('----a this.file_.size: ' + this.file_.size);
  //  window.console.log('----a add to upload queue');
    //this.gallery_.processUploadQueue(this); // add to file upload queue
  //  return;
  //}
  window.console.log("Uploading '" + this.name + "' ...");
  _gaq.push(['_trackEvent', 'item', 'picture_upload']);

  var req = new XMLHttpRequest();
  req.upload.addEventListener("progress", this.saveFileProgress_.bind(this), false);
  req.upload.addEventListener("load", this.saveFileComplete_.bind(this), false);
  req.upload.addEventListener("error", this.saveFileError_.bind(this), false);
  req.onreadystatechange = this.saveFileResult_.bind(this, req);
  
  req.open("POST", Gallery.URL_FILE_UPLOAD + 
      '?editor_id=' + this.gallery_.id +
//	  '&id_ext=' + this.gallery_.id +
      '&key=' + this.gallery_.key +
      '&filename=' + encodeURIComponent(this.name)
      , true);
	  
  if (typeof FormData != 'undefined') {
    var form_data = new FormData();
    form_data.append('file', this.file_);
    req.send(form_data);
  } else {
    // Firefox3
    var reader = new FileReader();
    reader.onerror = function(e) {window.console.log("File read error");}
    reader.onload = function(e) {
      window.console.log("File data loaded, uploading...");
      req.sendAsBinary(e.target.result);
    }
    reader.readAsBinaryString(this.file_);   
  }
  
}

Item.prototype.saveFileProgress_ = function(e) {
  if (!e.lengthComputable) return;
  var percent = Math.round((e.loaded * 100) / e.total);
  //window.console.log('Item.prototype.saveFileProgress_. Name: '+this.name+'. id: '+this.id+'. percent: '+percent+'. ');
  
  // #142
  var upload_progress_status = $('div.picture_detail[filename="'+this.name+'"] div.direct_image_link');
  upload_progress_status
//	.css('position', 'absolute')
//	.css('min-width', '200px')
//	.css('left', '50%')
    .show()
	.html('Uploaded&nbsp;'+percent + '% &nbsp; &nbsp; <img src="/smedia/minus/images/ajax-loader-bar.gif"/>');

  //upload_progress_status
  //  .css('margin-left', -upload_progress_status.width()/2);

  $('div.filelist div.file[itemname="'+this.name+'"] .delete').hide();
  $('div.filelist div.file[itemname="'+this.name+'"] .wait').show();
  this.callListeners_({
    type : Item.EVENT_RELOAD,
    data : this
  });
  this.callListeners_({
    type : Item.EVENT_PROGRESS,
    data : percent
  });
  
}

Item.prototype.saveFileComplete_ = function(e) {

  // This removes a race condition. 
  //$().delay(50, function() {
    window.console.log("File upload complete. Name: "+this.name + ". id: "+this.id);
    this.callListeners_({
      type : Item.EVENT_PROGRESS,
      data : 100
    });
  //});
}

Item.prototype.saveFileError_ = function(e) {
  window.console.log("File save error");
}

Item.prototype.processAfterUpload_ = function() {
  window.console.log('Item.prototype.saveFileResult_. Matched picture_links: ' + $('div.picture_detail[filename="'+this.name+'"]').length);
  
  if (Gallery.isValidImage(this) && this.width == 0 && this.height == 0) {
    // Select all div containing filename == this.name, and rename them (adding .bin extension)
	var item_divs = $('div[filename="'+this.name+'"]');
	this.name += '.bin'; // add the bin extension
	item_divs.attr('filename', this.name);
  }
  
  this.callListeners_({
    type : Item.EVENT_UPLOADED,
    data : this.id
  });
  this.callListeners_({
    type : Item.EVENT_LOADED,
    data : this
  });
  
  if (this.id) {
    //$('div.picture_detail[filename="'+this.name+'"] a.direct_image_link').hide();
	// #142
	$('div.picture_detail[filename="'+this.name+'"] div.direct_image_link').hide();
		
    var ajax_loader = '<div class="picture_detail"></div>';
    var this_gallery = this.gallery_;

	var show_links_func = function() {
	  var parent_div = $(this).parent().parent();
      var links = $(parent_div).find('.links_container');
	  var info = $(parent_div).find('.item_info');
	  info.hide();
	  this_gallery.SHOW_INFO = false;
      if (links.is(':hidden')) {
        this_gallery.SHOW_LINKS = true;
        links.slideDown(100);
      } else {
        this_gallery.SHOW_LINKS = false;
        links.slideUp(100);
      }
	};
	var show_info_func = function() {
	  var parent_div = $(this).parent().parent();
	  var links = $(parent_div).find('.links_container');
	  var info = $(parent_div).find('.item_info');
	  links.hide();
	  this_gallery.SHOW_LINKS  = false;
	  if (info.is(':hidden')) {
        this_gallery.SHOW_INFO = true;
        info.slideDown(100);
      } else {
        this_gallery.SHOW_INFO = false;
        info.slideUp(100);
      }
	}

    var html_links = Presenter.createShareLinks(ajax_loader, this, this.gallery_);
    var picture_info_inside = Presenter.createItemInfo(this); // #78
	
    var picture_link = $('div.picture_link[filename="'+this.name+'"]');
    var picture_info = $('div.picture_info[filename="'+this.name+'"]');
    var picture_link_margin_left = parseFloat(picture_link.css('margin-left'));
    var share_buttons_margin_left = -picture_link_margin_left-(Presenter.SHARE_BUTTONS_WIDTH/2);
    html_links.css('margin-left', share_buttons_margin_left);
	
    //picture_link.empty().append(picture_info_inside).append(html_links); // #78
	picture_link.empty().append(html_links).append(picture_info_inside); // #185
	if (this_gallery.SHOW_INFO) {
	  $(picture_info_inside).show();
	} else {
	  $(picture_info_inside).hide();
	}
    
	var links_container = picture_link.find('.links_container');
    var links_container_width = links_container.outerWidth();
    var new_margin = (-picture_link_margin_left-share_buttons_margin_left)-(links_container_width/2);

    var item_name_trunc = this.name;
    Presenter.createCaption(this, picture_info.find('.caption'), picture_info.find('.caption_edit'));
	
    if (this.name.length > 20) {
      item_name_trunc = this.name.substr(0, 17) + '...';
    }

    // reset the links_container margin-left
    picture_link.find('.links_container').css('margin-left', new_margin);
    picture_link.find('.share_button').bind('click', show_links_func);
	picture_link.find('.info_button').bind('click', show_info_func);

    log('Item.prototype.saveFileResult_: Setting dimensions: '+this.width + ' by '+this.height +'&nbsp - &nbsp '+this.filesize+'');
    if (Gallery.isValidImage(this)) {
      $('div.picture_info[filename="'+this.name+'"] span.dimensions')
        .html(item_name_trunc + '&nbsp · &nbsp ' + this.width + ' by '+this.height +'&nbsp · &nbsp '+this.filesize);
    } else {
      $('div.picture_info[filename="'+this.name+'"] span.dimensions')
        .html(item_name_trunc + '&nbsp · &nbsp ' +this.filesize);
    }
    $('div.filelist div.file[itemname="'+this.name+'"] .delete').show();
    $('div.filelist div.file[itemname="'+this.name+'"] .wait').hide();  
  }
  //else {
  //}
  
  window.console.log('Saved: ' + this.id + ". Name: "+this.name + ". File: "+this.file_.name);
  if (this.loaded == false) {
    this.callListeners_({
      type : Item.EVENT_RELOAD,
      data : this
    });
    this.load();      
  }
}

Item.prototype.saveFileResult_ = function(req) {
  if (req.readyState == 4 && req.status == 200) {
    var data = JSON.parse(req.responseText);
	var valid_image = Gallery.isValidImage(this);
	var large_image = Gallery.isLargeImage(this);
	
	var resized_after_load_func = function() {
	  this_item.loadResizedImageComplete_();
	  if (large_image) {
	    this_item.processAfterUpload_();
	  } else {
	    //Item.blurImage(this_item.image, 7, 240, 240, this_item.thumb);
	  }
	};
	
    if (data && data.id) {
      window.console.log('Item.prototype.saveFileResult_. responseText: '+req.responseText + '. data.id: '+ data.id + '. width: '+data.width);    
      this.id = data.id;
	  var this_item = this;
	  if (valid_image) {
        this.width = data.width;
        this.height = data.height;
		
		// used the resized image instead of the local file
		addEventListenerWrapper(this.image, 'load', resized_after_load_func);
		addEventListenerWrapper(this.image, 'error', resized_after_load_func);
		if (this.width > 0 && this.height > 0) { // valid image
		  this.image.src = this.getLink(true);
		} else { // invalid image, use icon for .bin
		  // override the filename, append .bin
		  // this.name += '.bin';
		  this.image.src = this.getIcon(true, 'bin');
		}
	  } else {
		var new_src = this.getIcon();
		var old_src = this.image.src;
		if (old_src != new_src) { // for pdf preview
		  addEventListenerWrapper(this.image, 'load', resized_after_load_func);
		  addEventListenerWrapper(this.image, 'error', function() {
		    resized_after_load_func();
		  });
		  this.image.src = new_src;
		} else {
	      this.width = this.image.width;
		  this.height = this.image.height;
		}
	  }
      this.filesize = data.filesize;
	  this.friendly_time = data.friendly_time;
	  
	  if (!large_image) {
	    this.processAfterUpload_();
	  }
    } else {
      window.console.log('Save error: ' + req.responseText);
    }
  }
  
  // ajax failed
  if (req.readyState == 4 && req.status != 200 && req.status > 0) {
    if (Gallery.isValidImage(this)) {
      var error_div = $('.dm-editor-error');
      if (error_div.length == 0 || error_div.find('#error_image').length == 0) {
        this.gallery_.callListeners_({
          type: Gallery.EVENT_ERRORS,
          errors: [],
          inner_html: Gallery.ERROR_INVALID_IMAGE
	  });
	  }
      this.gallery_.redirect = false;
	  // #184 don't delete from list, show it.
	  this.id = -1; // assign fake id so it won't show message the it is still uploading when page is refreshed or closed
      //this.gallery_.deleteItem(this);
    }
  }

  // We tried to delete ourselves while we were uploading.
  // time to delete us.
  if (this.queue_delete_) {
    this.deleteItem();
  }
}

// Delete
Item.prototype.deleteItem = function() {
  window.console.log("Deleting '" + this.name + "' ...");
  if (!this.id) {
    // We don't have an id yet, so mark ourselves as deletable so that we
    // can delete us when we do have an id (we might be in the middle of
    // saving, for example).
    this.queue_delete_ = true;
    return;
  }

  this.gallery_.callListeners_({
	type: Gallery.EVENT_NEW_AJAX
  });
  
  var req = new XMLHttpRequest();
  // #130
  req.open("GET", Gallery.URL_FILE_DELETE + 
      '?id=' + this.gallery_.id +
      '&key=' + this.gallery_.key +
	  '&item_id=' + this.id +
      '&filename=' + this.name
      , true);
  req.onreadystatechange = this.deleteResult_.bind(this, req);
  req.send();
}

Item.prototype.deleteResult_ = function(req) {
  if (req.readyState == 4 && req.status == 200) {
    this.gallery_.callListeners_({
	  type: Gallery.EVENT_END_AJAX
    });
    window.console.log("Deleted.");
    this.callListeners_({
      type : Item.EVENT_DELETED,
      data : this
    });
  } else if (req.readyState == 4) {
    this.gallery_.callListeners_({
	  type: Gallery.EVENT_END_AJAX
    });
  }
}

