// Gallery -------------------------------------------------------------------
/**
 * This contains all the data for the current set of items. It's responsible
 * for syncing the item data up to the cloud.
 */
function Gallery (id, key, name, reader_url, is_public, creator, created, views) {
  this.name = name ? name : 'Untitled';
  this.description = '';

  //window.console.log('Gallery. reader_url: '+reader_url);
  this.reader_url = reader_url;
  
  this.items_ = [];
  this.loading_queue_ = [];
  
  this.uploading_queue_ = []; // file/image ajax upload queue

  this.selected_ = null;
  this.listeners_ = [];

  // A public identifier unique to this gallery.
  this.id = id;
  this.id_ext = null; // TODO

  // A private key used to allow modification of this gallery.
  this.key = key;
  //window.console.log('Gallery being created.');
  
  window.onbeforeunload = this.closeWarning.bind(this);
  
  // Redirect after save if true
  this.redirect = true;
  
  this.inline_edit_flag = false;
  this.esc_key_flag = false;
  
  //this.LIGHT_MODE = true; // light/dark background
  // TODO move SHOW_LINKS from presenter.js here...
  
  this.SHOW_INFO = false;
  
  this.is_public = is_public || false;
  this.creator = creator || '';
  this.created = created;
  this.views = views;
}

Gallery.prototype.getCreator = function() {
  if (this.creator.length != 0) {
    return this.creator;
  } else if (this.USER.length != 0 && this.INITIAL_SCREEN) {
    return this.USER;
  }
  return '';
}

Gallery.prototype.useDarkMode = function() {
  var class_name = 'dark';
  if ($('.dm-editor').is(':visible')) {
    $('.dm-editor-dark')
	  .height($('.dm-editor').height());
  } else {
	$('.dm-editor-dark').hide();
  }
  $('html').addClass(class_name);
  $('body').addClass(class_name);
  $('#option_background')
    .attr('option_description', Menu.DARK_MODE_TEXT)
	.optionDescription();
}

Gallery.prototype.useLightMode = function() {
  var class_name = 'dark';
  $('html').removeClass(class_name);
  $('body').removeClass(class_name);
  $('#option_background')
    .attr('option_description', Menu.LIGHT_MODE_TEXT)
	.optionDescription();
}

Gallery.prototype.switchColor = function() {
  var url_api = '';
  if (this.LIGHT_MODE) {
    url_api = Gallery.URL_DARK_MODE;
    this.LIGHT_MODE = false;
    this.useDarkMode();
  } else {
    url_api = Gallery.URL_LIGHT_MODE;
    this.LIGHT_MODE = true;
	this.useLightMode();
  }
  // Submit
  $.ajax({
    url: url_api,
    type: 'POST'
  });
}

Gallery.prototype.closeWarning = function() {
  var close_msg = 'Your files are still uploading, are you sure you wish to leave this page?';
  if (FILES_IN_QUEUE > 0) {
    return close_msg;
  }
  for (var i = 0, item; item = this.items_[i]; i++) {
    if (!item.id) {
	  return close_msg;
	  break;
	}
  }
}

// static
Gallery.isImage = function(file) {
  switch (file.type) {
    case 'image/jpeg':
    case 'image/png':
    case 'image/gif':
    case 'image/bmp':
    case 'image/jpg':
      return true;
    default: 
      return false;
  }
}

Gallery.EVENT_NEWITEM = 1;
Gallery.EVENT_ITEMLOADED = 2;
Gallery.EVENT_SELECTION = 3;
Gallery.EVENT_ALLITEMSLOADED = 4;
Gallery.EVENT_REORDER = 5;
Gallery.EVENT_ITEMSDROPPED = 6;
Gallery.EVENT_SAVEDID = 7;
Gallery.EVENT_SAVED = 8;
Gallery.EVENT_ERRORS = 9;
Gallery.EVENT_NAMECHANGED = 10;
Gallery.EVENT_DELETING = 11;
Gallery.EVENT_DELETED = 12;
Gallery.EVENT_DELETEFAILED = 13;

// Couldn't find this, but this constant is being used... added as 14
Gallery.EVENT_DELETION = 14;

//Added
Gallery.EVENT_NAMECHANGING = 15;
Gallery.EVENT_NAMECHANGEDFAILED = 16;
Gallery.EVENT_NEW_AJAX = 17;
Gallery.EVENT_END_AJAX = 18;
Gallery.EVENT_GALLERY_HISTORY_CLEARED = 19;
Gallery.EVENT_RESIZED_IMAGE_LOADED = 20;

Gallery.REORDER_BEFORE = 21;
Gallery.REORDER_AFTER = 22;

Gallery.EVENT_ITEM_RENAME = 30;
Gallery.EVENT_URL_UPDATE = 31;
Gallery.EVENT_ITEM_CAPTION_UPDATE = 32;
Gallery.EVENT_ITEM_UPLOADED = 33;
Gallery.EVENT_VIDEO_LOADED = 34; // video player
Gallery.EVENT_ITEM_VIEWER_LOADED = 35; // used by pdf preview,

Gallery.URL_GETID = '/api/CreateGallery_Web/';
Gallery.URL_FILE_UPLOAD = '/api/UploadItem_Web/';
Gallery.URL_FILE_DELETE = '/api/deleteitem/';

Gallery.URL_SAVE = '/api/SaveGallery_Web/';
Gallery.URL_ITEM = '/i'; // no trailing slash
Gallery.URL_THUMB = '/j'; // also used for preview of other files
Gallery.URL_SMALL = '/k'; // smallest size
Gallery.URL_VIEWER_BASE = '/m'; // no trailing slash
//Gallery.URL_VIEWER_BASE = '/'; // no trailing slash; NOTE: removed 'm'
Gallery.URL_DELETE_GALLERY = '/api/deletegallery/';

Gallery.URL_MANUAL_UPLOAD = '/api/manualupload/';
Gallery.URL_ZIP = '/gallery.zip';

Gallery.URL_RENAME_ITEM = '/api/ChangeItemName/';
Gallery.URL_CHANGE_ITEM_CAPTION = '/api/ChangeItemCaption/';
Gallery.URL_CHANGE_SHARE_URL = '/api/ChangeShareUrl/';
Gallery.URL_CHANGE_EDIT_URL = '/api/ChangeEditUrl/';
Gallery.URL_VALIDATE_SHARE_URL = '/api/ValidateShareUrl/';
Gallery.URL_VALIDATE_EDIT_URL = '/api/ValidateEditUrl/';
Gallery.URL_VALIDATE_ITEM_NAME = '/api/ValidateItemName/';

Gallery.URL_DELETE_FROM_HISTORY = '/api/clear_gallery_from_history/';
Gallery.URL_DELETE_FROM_TIMELINE = '/api/delete_from_timeline/';
Gallery.URL_SET_PUBLIC = '/api/set_public/';
Gallery.URL_SET_PRIVATE = '/api/set_private/';

Gallery.URL_LIGHT_MODE = '/api/TurnOnLights';
Gallery.URL_DARK_MODE = '/api/TurnOffLights';

Gallery.URL_FILE_ICONS = '/smedia/minus/images/file_icons/';
Gallery.URL_FILE_ICONS_SMALL = '/smedia/minus/images/file_icons/small/';
Gallery.URL_AJAX_LOADER_SMALL = '/smedia/minus/images/ajax-loader-bar-small.gif';
Gallery.URL_IMG_LOADING_ICON = '/smedia/minus/images/ajax-loader-circle-gray.gif';

Gallery.URL_IMAGE_DOWNLOAD = 'http://i.min.us';
Gallery.URL_FILE_DOWNLOAD = 'http://k.min.us';

// Social icon links
Gallery.URL_TWITTER = 'http://twitter.com/home?status=';
Gallery.URL_REDDIT = 'http://reddit.com/submit?url=';
Gallery.URL_DIGG = 'http://www.digg.com/submit?phase=2&url=';
Gallery.URL_FACEBOOK = 'http://www.facebook.com/sharer.php?u=';
Gallery.URL_STUMBLE = 'http://www.stumbleupon.com/submit?url=';
Gallery.URL_EMAIL = 'mailto:email@address.com?subject=';

//Gallery.URL_MUSIC_PLAYER = '/smedia/flowplayer/swf/flowplayer-3.2.5.swf';
Gallery.URL_MUSIC_PLAYER = '/smedia/flowplayer/swf/flowplayer.commercial-3.2.6.swf';

Gallery.URL_MAXLENGTH = 40; // not including m prefix

Gallery.VALID_IMAGES = ['image/jpeg', 'image/png', 'image/gif', 'image/bmp', 'image/jpg'];
Gallery.VALID_FILETYPES = ['application/pdf', 'application/zip'];

// will use this instead of VALID_FILETYPES
Gallery.VALID_IMG_EXT = ['jpeg', 'jpg', 'png', 'gif', 'bmp'];

// added pptx, and odt for generic icon testing
Gallery.VALID_FILE_EXT = ['7z', 'ai', 'aif', 'aifc', 'aiff', 'asf', 'asp', 'asr',
  'au', 'avi', 'cab', 'dmg', 'doc', 'docx', 'fla', 'flv', 
  'ft10', 'html', 'indd', 'iso', 'jsf', 'jsp', 'lzh', 'm4p', 
  'm4u', 'mdb', 'mid', 'mov', 'mp2', 'mp3', 'mpa', 'mpe',
  'mpeg', 'mpg', 'mxp', 'odt', 'pdf', 'php', 'ppt', 'pptx', 'psd', 'rar', 'rtf',
  'swf', 'txt', 'wav', 'xls', 'xlsx', 'zip'];
  
// files that text can be displayed
//Gallery.VIEW_READER_TXT = ['asp', 'css', 'js', 'jsp', 'html', 'php', 'py', 'txt'];
Gallery.VIEW_READER_TXT = ['txt',
'as', 'mx',
'ada', 'ads', 'adb',
'asm',
'asp',
'au3',
'sh', 'bsh',
'bat', 'cmd', 'nt',
'c',
'h', 'hpp', 'hxx', 'cpp', 'cxx', 'cc',
'cs',
'cmake',
'cbl', 'cbd', 'cdb', 'cdc', 'cob',
'css',
'diff', 'patch',
'f', 'for', 'f90', 'f95', 'f2k',
'hs', 'lhs', 'las',
'html', 'htm', 'shtml', 'xhtml',
'ini', 'inf', 'reg', 'url',
'iss',
'java', 
'js',
'jsp',
'lsp', 'lisp',
'lua',
'mak',
'pas', 'inc',
'pl', 'pm', 'plx',
'php',
'py', 'pyw',
'ps',
'r',
'rb', 'rbw',
'scm', 'smd', 'ss',
'st',
'sql',
'tex',
'vb', 'vbs',
'xml', 'xsml', 'xsl', 'xsd', 'kml', 'wsdl',
'yml'];
  
Gallery.FILES_LIMIT = 50;
Gallery.MAX_FILE_SIZE = 25000000;
//Gallery.MAX_FILE_SIZE = 10000000;
//Gallery.MAX_FILE_SIZE = 50000000;
Gallery.MAX_FILE_SIZE_PLUPLOAD = '50mb';
Gallery.LARGE_IMAGE_SIZE = 5000000;
Gallery.LARGE_UPLOAD_SIZE = 7000000;
Gallery.ERROR_MAX_LIMIT_REACHED = '<h2>Sorry,</h2>' + '<p>the max limit per gallery is ' + Gallery.FILES_LIMIT + ' items.</p>';
Gallery.ERROR_INVALID_FILE = 'File type not supported';
Gallery.ERROR_INVALID_SIZE = 'Exceeds 25 megabytes';
//Gallery.ERROR_INVALID_SIZE = 'Exceeds 10 megabytes';
//Gallery.ERROR_INVALID_SIZE = 'Exceeds 50 megabytes';
Gallery.ERROR_INVALID_IMAGE = '<p id="error_image">The image you have uploaded is not in the correct format and cannot be properly rendered. Please try another image.</p>';
Gallery.ERROR_CANT_EDIT_URL = '<p id="error_cant_edit_url">Sign up for Minus to customize your URL and enjoy many more features!</p>';

Gallery.ERROR_URL_REQUIRED = 'URL required';
Gallery.ERROR_URL_SPACES_NOT_ALLOWED = 'Spaces are not allowed';
Gallery.ERROR_URL_MAXLENGTH = 'Up to ' + Gallery.URL_MAXLENGTH + ' characters only';

Gallery.isValidImage = function(item) {
  var ext = '' + extension(item.name);
  
  // check first if the extension is .bin
  if (streql_in(ext, 'bin')) return false;
  
  // if type has value check it first
  if (item.type) {
    if ($.inArray(item.type, Gallery.VALID_IMAGES) >= 0) {
	  return true;
	}
  }
  // fallback, check the extension
  if ($.inArray(ext.toLowerCase(), Gallery.VALID_IMG_EXT) >= 0) {
    return true;
  }
  return false;
}
// use only for drag n drop
Gallery.isLargeImage = function(item) {
  if (Gallery.isValidImage(item)) {
    if (item.file_) {
      if (item.file_.size >= Gallery.LARGE_IMAGE_SIZE) {
        return true;
      }
	}
  }
  return false;
}

// new static method for checking if the filetype is valid
Gallery.isValidFile = function(file) {
  //window.console.log('----a file.type: ' + file.type);
  //window.console.log('----a file.size: ' + file.size);

  return true; // everything is allowed
  
  // if type has value check it first
  if (file.type) {
    if ($.inArray(file.type, Gallery.VALID_IMAGES.concat(Gallery.VALID_FILETYPES)) >= 0) {
	  return true;
	}
  }
  
  // fallback, check the extension
  var ext = '' + extension(file.name);
  if ($.inArray(ext.toLowerCase(), Gallery.VALID_FILE_EXT.concat(Gallery.VALID_IMG_EXT)) >= 0) {
    return true;
  }
  return false;
}

Gallery.isTextViewable = function(file) {
  var ext = '' + extension(file.name);
  if ($.inArray(ext.toLowerCase(), Gallery.VIEW_READER_TXT) >= 0) { 
    return true;
  } else {
    return false;
  }
}

Gallery.isFileEmpty = function(file) {
  if (file.size === 0) {
    return true;
  }
  return false;
}

Gallery.updateAccess = function(edit_id, access, ajax_opts) {
  var url = '';
  var ajax_opts = ajax_opts || {};
  if (access == 'public') {
    url = Gallery.URL_SET_PUBLIC;
  } else if (access == 'private') {
    url = Gallery.URL_SET_PRIVATE;
  }

  /*
  $.ajax({
    url: url + edit_id,
    type: 'GET',
	cache: false,
  });
  */
  ajax_opts.url = url + edit_id;
  ajax_opts.type = 'GET';
  ajax_opts.cache = false;
  $.ajax(ajax_opts);
}

Gallery.prototype.addListener = function(listener) {
  this.listeners_.push(listener);
}

Gallery.prototype.callListeners_ = function(e) {
  for (var i = 0, func; func = this.listeners_[i]; i++) {
    func(e);
  }
}

Gallery.prototype.getItems = function() {
  return this.items_;
}

Gallery.prototype.findItemNumber = function(item) {
  for (var i = 0; i < this.items_.length; i++) {
    if (this.items_[i] == item) {
	  return i+1;
	}
  }
  return '';
}

/*
Gallery.prototype.addUploadQueue = function(item, options) {
  this.uploading_queue_.push({item: item, options: options});
  if (this.uploading_queue_.length === 1) {
    this.processUploadQueue();
  }
}
*/

Gallery.prototype.processUploadQueue = function(item) {
  //window.console.log('----a Gallery.prototype.processUploadQueue: ');
  this.uploading_queue_.push(item);
  if (this.uploading_queue_.length === 1) {
    this.sendUploadQueue(item);
  }
}

Gallery.prototype.sendUploadQueue = function(item) {
  //window.console.log('----a Gallery.prototype.sendUploadQueue');
  var req = XMLHttpRequest();
  
  var load_func = function() {
	item.saveFileComplete_.bind(item)();
	this.uploading_queue_.shift();
	if (this.uploading_queue_.length > 0) this.sendUploadQueue(this.uploading_queue_[0]);
  }.bind(this);
  var error_func = function() {
    item.saveFileError_.bind(item)();
    this.uploading_queue_.shift();
	this.sendUploadQueue(this.uploading_queue_[0]);
	if (this.uploading_queue_.length > 0) this.sendUploadQueue(this.uploading_queue_[0]);
  }.bind(this);
	
  req.upload.addEventListener("progress", item.saveFileProgress_.bind(item), false);
  req.upload.addEventListener("load", load_func, false);
  req.upload.addEventListener("error", error_func, false);
  req.onreadystatechange = item.saveFileResult_.bind(item, req);

  req.open("POST", Gallery.URL_FILE_UPLOAD + 
      '?editor_id=' + item.gallery_.id +
      '&key=' + item.gallery_.key +
      '&filename=' + encodeURIComponent(item.name)
      , true);
	  
  if (typeof FormData != 'undefined') {
    var form_data = new FormData();
    form_data.append('file', item.file_);
    req.send(form_data);
  } else {
    // Firefox3
    var reader = new FileReader();
    reader.onerror = function(e) {window.console.log("File read error");}
    reader.onload = function(e) {
      window.console.log("File data loaded, uploading...");
      req.sendAsBinary(e.target.result);
    }
    reader.readAsBinaryString(item.file_);   
  }
}

// TODO comment out this
Gallery.prototype.setImageSRC = function(item) {
  if (item === undefined && this.items_.length > 0) {
    item = this.items_[0];
  }
  if (item) {
    var valid_image = Gallery.isValidImage(item);
    var image_url;
  
    if (valid_image) {
      //image_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_ITEM + item.id + '.' + extension(item.name);
	  image_url = Gallery.URL_ITEM + item.id + '.' + extension(item.name);
    } else {
      //image_url = window.location.protocol + '//' + 'min.us' + item.getIcon();
	  image_url = item.getIcon();
    }
  
    $('link[rel=image_src]').attr('href', image_url);
  } else {
    $('link[rel=image_src]').attr('href', '');
  }
}

Gallery.prototype.setSelected = function(item, call_listeners) { // added optional argument call_listeners
  this.selected_ = item;
  if (call_listeners === undefined) {
    call_listeners = true;
  }
  if (call_listeners) {
    this.callListeners_({
      type : Gallery.EVENT_SELECTION,
      data : item
    });
  }
  var hash_num = this.findItemNumber(item);
  if (hash_num != '') {
    window.location.hash = '#' + hash_num;
  }
  _gaq.push(['_trackEvent', 'gallery', 'selected_new_item']);
}

Gallery.prototype.selectNext = function() {
  // -1 is to prevent overflow errors - if the selection is at the end,
  // there's no point going further (wrapping dooooom).
  for (var i = 0; i < this.items_.length - 1; i++) {
    if (this.items_[i] == this.selected_) {
      this.setSelected(this.items_[i + 1]);
	  window.location.hash = '#' + (i + 2);
      return;
    }
  }
  this.setSelected(this.items_[0]); // set to the first item
}

Gallery.prototype.selectPrev = function() {
  // i = 1 is to prevent overflow errors.
  for (var i = 1; i < this.items_.length; i++) {
    if (this.items_[i] == this.selected_) {
      this.setSelected(this.items_[i - 1]);
	  window.location.hash = '#' + i;
      return;
    }
  }
  //this.setSelected(this.items_[0]);
  var last = this.getItems().length;
  this.setSelected(this.items_[last-1]); // set to the last item
}

Gallery.prototype.getSelected = function() {
  if (!this.selected_) {
    this.selected_ = this.items_[0];
	//if (this.selected_) {
    //  this.setImageSRC(this.items_[0]);
	//}
  }
  return this.selected_;
}

Gallery.prototype.setName = function(name) {
  var curr_name = this.name;
  var success_func = function() {
    this.callListeners_({
	  type: Gallery.EVENT_END_AJAX
	});
    this.name = name;
    var listname_edit = $('div.listname_edit');
	listname_edit.hide();
	listname_edit.find('#listname_edit').show();
	listname_edit.find('.loading').hide();
	$('div.listname').show();
	if ($.trim(name).length > 0) {
	  document.title = name + ' - Minus';
	} else {
	  document.title = 'Untitled - Minus';
	}
	this.callListeners_({
      type : Gallery.EVENT_NAMECHANGED,
      data : name
    });
  }.bind(this);
  
  // show/hide divs
  var error_func = function() {
    this.callListeners_({
	  type: Gallery.EVENT_END_AJAX
	});
    var listname_edit = $('div.listname_edit');
	listname_edit.hide();
	listname_edit.find('#listname_edit').show();
	listname_edit.find('.loading').hide();
	$('div.listname').show();
	this.name = curr_name;
	listname_edit.find('#listname_edit').val(curr_name); // set back to original name
	this.callListeners_({
	  type: Gallery.EVENT_NAMECHANGEDFAILED
	});
  }.bind(this);
  
  // TODO pass arguments
  var options = {
    beforeSend: function() {
	  var text_box = $('div.listname_edit').find('#listname_edit');
	  var loading_div = $('div.listname_edit').find('.loading');
	  text_box.hide();
      loading_div.show();
	  this.callListeners_({
	    type: Gallery.EVENT_NAMECHANGING
	  });
	  this.callListeners_({
	    type: Gallery.EVENT_NEW_AJAX
	  });
	}.bind(this),
	success: success_func,
	error: error_func
  };
  this.name = name;
  this.save(options);
  _gaq.push(['_trackEvent', 'gallery', 'set_name']);
}

Gallery.prototype.deleteItem = function(to_delete) {
  var new_items = [];
  for (var i = 0, item; item = this.items_[i]; i++) {
    if (item != to_delete)
      new_items.push(item);
  }
  this.items_ = new_items;
  to_delete.deleteItem();
  this.save();
  if (this.selected_ == to_delete)
    this.selectPrev();

  _gaq.push(['_trackEvent', 'gallery', 'delete_item']);
  
  this.callListeners_({
    type : Gallery.EVENT_DELETION
  });
}

Gallery.prototype.itemsFromJSON = function(items) {
  window.console.log("Importing item IDs...");
  for (var i = 0, data; data = items[i]; i++) {
    var item = new Item(this);
    
    item.setID(data.id);
    item.setName(data.name);
    log('set dimensions. filesize: '+data.filesize);
    item.width = data.width;
    item.height = data.height;
    item.filesize = data.filesize;
	item.caption = data.caption;
	item.friendly_time = data.friendly_time;
    this.items_.push(item);
    item.addListener(this.itemListener.bind(this));
    this.loading_queue_.push(item);
    this.callListeners_({
      type : Gallery.EVENT_NEWITEM,
      data : item
    });
  }
  if (this.loading_queue_.length) {
    this.processQueue_();
  } else {
    setTimeout(this.callAllItemsLoaded_.bind(this), 50);
  }
}

Gallery.prototype.newLocalFiles = function(files, dropitem, order) {
  if(this.name.length>0) document.title = this.name + ' - min.us';
  else document.title = 'Untitled - min.us';
  
  // Try to get a fresh ID and key from the server.
  this.getID_();

  this.callListeners_({
    type : Gallery.EVENT_ITEMSDROPPED
  });

  var errors = [];
  var error_inner_html = '';
  var new_items = [];
  
  // Verify that they're images and add the files.
  for (var i = 0, file; file = files[i]; i++) {
  
    // Check if the limit is already reached. Add the error.
    if (new_items.length + this.items_.length >= Gallery.FILES_LIMIT && !error_inner_html) {
	  error_inner_html += Gallery.ERROR_MAX_LIMIT_REACHED;
	}
	
	if (Gallery.isValidFile(file) && !Gallery.isFileEmpty(file) && file.size < Gallery.MAX_FILE_SIZE) {
	  if (new_items.length + this.items_.length < Gallery.FILES_LIMIT) {
	    var item = new Item(this);
        item.setFile(file);
        log('Gallery.prototype.newLocalFiles. Name: '+item.name +'. id: '+item.id);
        new_items.push(item);
        item.addListener(this.itemListener.bind(this));
	  }
    } else {
      var error = '';
	  
	  if (error_inner_html === '') {
	    if (!Gallery.isValidFile(file) || Gallery.isFileEmpty(file)) {
          //error = "file type not supported";
		  error = Gallery.ERROR_INVALID_FILE;
	    } else if (file.size >= Gallery.MAX_FILE_SIZE) {
          //error = "exceeds 7 megabytes";
		  error = Gallery.ERROR_INVALID_SIZE;
	    }
		if ($.inArray(error, errors) === -1) {
	      errors.push(error);
	    }
	  }
      //errors.push(file.name + ' (' + error + ')');
      //if (!Gallery.isImage(file))
      //  error = "not an image";
    }
  }

  // If the files have been dropped into the list, 
  // insert them properly.
  if (dropitem && order) {
    window.console.log('finding position');
    var position = 0;
    for (var i = 0, item; item = this.items_[i]; i++) {
      if (item == dropitem) {
        position = (order == Gallery.REORDER_AFTER) ? i + 1 : i;
        break;
      }
    }
    window.console.log('inserting at position ' + i);
    for (var i = 0, item; item = new_items[i]; i++) {
      this.items_.splice(position + i, 0, item);
    }
  } else {
    this.items_ = this.items_.concat(new_items);
  }

  // Add the items to the loading queue, and let people
  // know they've been created.
  for (var i = 0, item; item = new_items[i]; i++) {
    log('Gallery.prototype.newLocalFiles. Name: '+item.name+'. ID: '+item.id);
    this.loading_queue_.push(item);
    this.callListeners_({
      type : Gallery.EVENT_NEWITEM,
      data : item
    });
	this.callListeners_({
	  type: Gallery.EVENT_NEW_AJAX
	});
  }
  if (this.loading_queue_.length)
    this.processQueue_();

  if (errors.length || error_inner_html.length) {
    this.callListeners_({
      type : Gallery.EVENT_ERRORS,
      errors : errors,
	  inner_html: error_inner_html
    });
  }
}

/**
 * When using local files, sometimes things get hairy if we try to do
 * too many in parallel.
 */
Gallery.prototype.processQueue_ = function() {
  this.loading_queue_.splice(0, 1)[0].load();
}

Gallery.prototype.callAllItemsLoaded_ = function() {
  this.callListeners_({
    type : Gallery.EVENT_ALLITEMSLOADED
  });
}

/**
 * Called when an item is loaded.
 */
Gallery.prototype.itemListener = function(e) {
  if (e.type == Item.EVENT_LOADED) {
    if (this.loading_queue_.length == 0) {
      this.callAllItemsLoaded_();
    } else {
      this.callListeners_({
        type : Gallery.EVENT_ITEMLOADED,
        data : e.data
      });
      this.processQueue_();
    }
  } else if (e.type == Item.EVENT_UPLOADED) {
    // When an item is finished uploading decrease number of ajax counter:
	this.callListeners_({
	  type: Gallery.EVENT_END_AJAX
	});
	this.callListeners_({
	  type: Gallery.EVENT_ITEM_UPLOADED,
	  data: e.data
	});
    // This will succeed when all items have been uploaded.
    this.save();
  } else if (e.type == Item.EVENT_RESIZED_LOADED) {
    this.callListeners_({
	  type: Gallery.EVENT_RESIZED_IMAGE_LOADED,
	  data: e.data
	});
  } else if (e.type == Item.EVENT_VIDEO_LOADED || e.type == Item.EVENT_VIDEO_START_LOADING) {
    this.callListeners_({
	  type: Gallery.EVENT_VIDEO_LOADED
	});
  }
}

function find_item_by_id(id, items) {
   for(i=0; i<items.length; i++) {
      if(id==items[i].id) {
        return items[i];
      }
   }
   return false;
}

Gallery.prototype.sortable_changed = function() {
  _gaq.push(['_trackEvent', 'gallery', 'changed_item_order']);

  var newitems = [];
  var item;
  current_items = this.items_;
  $('div.dm-editor div.filelist').children().each(function(){
        window.console.log('Gallery.prototype.sortable_changed. Iterating items. Item: '+$(this).html());  
        item = find_item_by_id($(this).attr('itemid'), current_items);
        if(item) newitems.push(item);        
    }
  );
  this.items_ = newitems;  
  for(var i=0;i<newitems.length;i++) {
    window.console.log('Gallery.prototype.sortable_changed. ID: '+newitems[i].id + '. Name: ' + newitems[i].name + '. Length: '+newitems.length);
  }
  this.callListeners_({
    type : Gallery.EVENT_REORDER,
    data : null
  });
  this.save();
}

Gallery.prototype.reorder = function(newitem, relative, ordering) {
  var newitems = [];
  for (var i = 0, item; item = this.items_[i]; i++) {
    if (item == newitem) continue;
    if (item == relative) {
      if (ordering == Gallery.REORDER_BEFORE) {
        newitems.push(newitem);
        newitems.push(relative);
      } else {
        newitems.push(relative);
        newitems.push(newitem);
      }
    } else {
      newitems.push(item);
    }
  }
  this.items_ = newitems;

  this.callListeners_({
    type : Gallery.EVENT_REORDER,
    data : newitem
  });

  this.save();
}

// SAVING AND LOADING
Gallery.fromJSON = function(data) {
  var gallery = new Gallery(data.id, data.key, data.name, data.reader_url, data.is_public, data.creator, data.created, data.views);
  gallery.itemsFromJSON(data.items); // #130
  return gallery;
}

Gallery.prototype.save = function(options) {
  var callbacks = {
    beforeSend: function() {},
	success: function() {},
	error: function() {}
  };
  if (options !== undefined) {
    $.extend(callbacks, options);
  }
  if (this.id && !this.key) {
    window.console.log("No permission to save this gallery");
    return;
  }
  // Check to see if we have the id and key, which are required to
  // save any more details.
  if (!this.id && !this.key) {
    window.console.log("No id or key details");
    this.getID_();
    return;
  }
  // Check that all our items have ids; if they haven't,
  // abort. When they get their IDs, their EVENT_UPLOADED will
  // trigger another save attempt.
  var item_ids = [];
  for (var i = 0, item; item = this.items_[i]; i++) {
    if (!item.id) {
      window.console.log("Not all items have been uploaded");
      return;
    }
    item_ids.push(item.id);
  }
  $('.dm-editor div.filelist').sortable({disabled: false});
  $('.dm-editor div.file').css('cursor', 'move');
  
  $('#share_url_icon_div').show();
  var data = JSON.stringify({
    name : this.name,
	id : this.id,
    key : this.key,
	items : item_ids // #130
  });

  callbacks.beforeSend();
  window.console.log("Submitting save: " + data);
  var req = new XMLHttpRequest();
  req.open('POST', Gallery.URL_SAVE, true);
  req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  req.onreadystatechange = this.saveListener_.bind(this, req, options);
  req.send(data);
}

Gallery.prototype.saveListener_ = function(req, options) {
  if (req.readyState == 4 && req.status == 200) {
    window.console.log(' Gallery Saved: ' + this.id + ' this.reader_url: '+this.reader_url);
    this.callListeners_({
      type : Gallery.EVENT_SAVED
    });
    
    window.console.log("Gallery.prototype.saveListener_. " + window.location.pathname + '. INITIAL_SCREEN: ' + this.INITIAL_SCREEN);
    
    //if((this.redirect && (window.location.pathname=='/' || window.location.pathname=='/pages/galleries'))) {
	if((this.redirect && (window.location.pathname=='/' || window.location.pathname=='/mine'))) {
        //window.console.log("Gallery.prototype.saveListener_. Redirecting. ");    
        window.location.href='/m'+this.id;
    }
	
	if (this.redirect && window.location.pathname != Gallery.URL_VIEWER_BASE + this.id && this.loading_queue_.length == 0) {
	  var all_items_uploaded = true;
	  for (var i = 0, item; item = this.items_[i]; i++) {
        if (!item.id) {
		  all_items_uploaded = false;
          break;
        }
      }
	  if (all_items_uploaded) {
	    window.location.href = Gallery.URL_VIEWER_BASE + this.id;
	  }
	}
	
	if (options !== undefined && options.success !== undefined) {
	  options.success();
	}
  } else if (req.readyState == 4 && req.status != 200) {
    if (options !== undefined && options.error !== undefined) {
     options.error();
	}
  }
}

Gallery.prototype.getID_ = function() {
  if (this.id) return;

  var req = new XMLHttpRequest();
  req.open('GET', Gallery.URL_GETID, true);
  
  req.onreadystatechange = this.getIDListener_.bind(this, req);
  req.send(null);
}

Gallery.prototype.getIDListener_ = function(req) {
  if (req.readyState == 4 && req.status == 200) {
    var data = JSON.parse(req.responseText);
    //if (data && data.editor_id && data.key) {
	
	// #66
	if (data) {
      this.id = data.editor_id;
	  //this.id = data.editor_id_ext;
	  
      //this.key = data.key;
	  this.key = 'ok'; // temp, TODO remove 'key' property
      this.reader_url = data.reader_id;
      this.callListeners_({
        type : Gallery.EVENT_SAVEDID,
        id : this.id,
        key : 'ok'
      });
	  
	  /*
	  if (this.USER && this.USER.length > 0) {
	    show_confirm('The gallery is now published to your Minus profile.');
	  }*/
    } else {
      window.console.log('Save error: ' + req.responseText);
    }
  }
}

Gallery.prototype.deleteGallery = function(e) {
  if (this.id && !this.key) {
    window.console.log("No permission to delete this gallery");
    return;
  }
  this.callListeners_({
    type : Gallery.EVENT_DELETING
  });
  
  this.callListeners_({
	type: Gallery.EVENT_NEW_AJAX
  });
  
  // #66
  var req = new XMLHttpRequest();
  req.open('GET', Gallery.URL_DELETE_GALLERY + '?id=' + this.id + '&key=' + this.key, true);
  //req.open('GET', Gallery.URL_DELETE_GALLERY + '?id=' + this.id + '&id_ext=' + this.id + '&key=' + this.key, true);
  req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  req.onreadystatechange = this.deleteListener_.bind(this, req);
  req.send();
}

Gallery.prototype.deleteListener_ = function(req) {
  if (req.readyState == 4 && req.status == 200) {
    this.callListeners_({
	  type: Gallery.EVENT_END_AJAX
    });
    window.console.log('response:' + req.responseText);
    var data = JSON.parse(req.responseText);
    if (data.success) {
      this.callListeners_({type : Gallery.EVENT_DELETED});
    } else {
      this.callListeners_({type : Gallery.EVENT_DELETEFAILED});
    }
  } else if (req.readyState == 4) {
    this.callListeners_({
	  type: Gallery.EVENT_END_AJAX
    });
  }
}