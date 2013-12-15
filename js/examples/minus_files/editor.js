// Editor ----------------------------------------------------------------

function Editor(gallery, reader_url, expanded_menu_initial) { 
  this.gallery_ = gallery;
  //this.reader_url = reader_url;
  this.reader_url = gallery.reader_url;
  this.gallery_.addListener(this.galleryListener.bind(this));
  this.item_nodes_ = [];
  this.expanded_menu_initial = expanded_menu_initial;
  
  this.node_ = createElement('div', 'dm-editor', document.body);
  
  //this.node_wrapper_ = createElement('div', 'dm-editor-wrapper', document.body);
  //this.node_ = createElement('div', 'dm-editor', this.node_wrapper_);
  
  //if(window.location.pathname!='/' && window.location.pathname !='/pages/galleries') {
  if(window.location.pathname!='/' && window.location.pathname !='/mine') {
    $(this.node_).show();
  } else {
    $(this.node_).hide();
  }
  
  this.node_dark_ = createElement('div', 'dm-editor-dark', document.body);
  $(this.node_dark_).height($(this.node_).height());
  
  var node_dark_hover_in = function() {
    if (!this.gallery_.LIGHT_MODE) {
      $(this.node_dark_).height($(this.node_).height());
	  $(this.node_dark_).addClass('dm-editor-dark-zero');
	}
  }.bind(this);
  var node_dark_hover_out = function() {
    $(this.node_dark_).height($(this.node_).height());
	var visible_edit = Editor.visibleEdit();
	if (!this.gallery_.LIGHT_MODE && !visible_edit) {
	  $(this.node_dark_).removeClass('dm-editor-dark-zero');
	}
  }.bind(this);
  
  $(this.node_dark_).hover(node_dark_hover_in,
  function() {
    if (!this.gallery_.LIGHT_MODE) {
      $(this.node_dark_).height($(this.node_).height());
	}
  });
  //node_dark_hover_out);
  $(this.node_).hover(node_dark_hover_in, node_dark_hover_out);
  
  //this.node_.onresize = function() {
    //window.console.log('----a resize dm-editor');
    //alert('hello');
    //$(this.node_dark_).height($(this).height());
  //};
  
  if(this.gallery_.IS_EDITOR) {
    this.ajax_counter = 0; // counts number of current active ajax
  
    // timeout variables
    this.hide_edit_timeout = null; // for urls
    this.hide_title_edit_timeout = null; // for gallery title
	
	// ajax variables
	this.validateUrlLastAjax = null; // stores the last ajax object calling validateURLAPI_()
 
	this.SHOW_LINKS = true;
	this.node_title_ = createElement('div', 'title', this.node_);
	this.node_edit_button =  createElement('div', 'edit', this.node_title_);
	this.node_name_ = createElement('div', 'listname', this.node_title_);
	this.node_name_edit_ = createElement('div', 'listname_edit', this.node_title_);
	
	var pencil_img = $('<img>', {'class': 'inline_edit_btn pencil_light_mode', 'inline_input_id': 'listname_edit', 'src': '/smedia/minus/lightpencil.png'});
	var pencil_img_dmode = $('<img>', {'class': 'inline_edit_btn pencil_dark_mode', 'inline_input_id': 'listname_edit', 'src': '/smedia/minus/lightpencil-darkmode.png'});
	
	$(this.node_edit_button).append(pencil_img);
	$(this.node_edit_button).append(pencil_img_dmode);
	
	$(this.node_edit_button).click(function(e) {
	  this.nameClicked_(this);
	  this.layoutPencil();
	}.bind(this));
	
	var loading_div = $('<div>', {'class': 'loading', 'style': 'display:none;'});
	//var loading_img = $('<img>', {'class': 'loading_img', 'src': '/smedia/minus/images/ajax-loader-bar.gif'});
	var loading_img = $('<img>', {'class': 'loading_img', 'src': Gallery.URL_AJAX_LOADER_SMALL});
	loading_div.append(loading_img);
	
	$(this.node_name_edit_).hide();
	
	//var input_name = $('<input/>', {'id': 'listname_edit', 'class': 'inline_edit'});
	var input_name = $('<textarea/>', {'id': 'listname_edit', 'class': 'inline_edit', 'spellcheck': false});
	
	input_name.noNewLines();
	
	var this_editor = this;
	if (this.gallery_.name && this.gallery_.name.length > 0 && this_editor.gallery_.name != 'Untitled') {
	  var gallery_name_decoded = htmlDecode(this.gallery_.name);
	  //input_name.val(this.gallery_.name).change();
	  input_name.val(gallery_name_decoded).change();
	} else {
	  input_name.val('').change();
	}
	
	var update_ml_func = function (text_box) {
	  var value = $(text_box).val().replace(/\n/g, ' ');
	  $(text_box).val(value);
	  var name = $(text_box).val();
	  gallery.setName(name);
	};
	
	input_name.keypress(function(event) {
	  // update name when enter is pressed
	  if (event.which == '13') {
	    update_ml_func(this);
	  }
	});
	
	input_name.blur(function(event) {
	  if (this_editor.gallery_.inline_edit_flag) {
        update_ml_func(this);
      } else if (this_editor.gallery_.esc_key_flag) {
	   if (this_editor.gallery_.name && this_editor.gallery_.name.length > 0 && this_editor.gallery_.name != 'Untitled') {
          $(this).val(this_editor.gallery_.name);
        } else {
          input_name.val('');
        }
        $(this_editor.node_name_).show();
        $(this_editor.node_name_edit_).hide();
        valid_input($(this));
	  }
	});
	
	$(this.node_name_edit_)
	  .append(input_name)
	  .append(loading_div);
	
	input_name.autogrow({
	  maxHeight: 56, 
	  callback: function() {
	    this.layoutPencil();
	  }.bind(this)
	});
	
	this.updateName_(true);
	
    var delete_button = createElement('div', 'delete', this.node_title_);  
	$(delete_button).hover(
	  function() {
		$(this).addClass('delete_hover');
	  },
	  function() {
		$(this).removeClass('delete_hover');
	  }
	).attr('title','Delete this gallery.');
	addEventListenerWrapper(delete_button, 'click', this.titleDeleteClicked_.bind(this));

  }
  else {
  	this.SHOW_LINKS = false;
	this.node_title_ = createElement('div', 'title', this.node_);
	this.node_name_ = createElement('div', 'listname', this.node_title_);
	this.updateName_(true);
  } 
 
  this.node_username_ = createElement('div', 'gallery_creator', this.node_title_);
  this.node_gallery_info_ = createElement('div', 'gallery_info', this.node_title_);
  if (this.gallery_.getCreator().length > 0) {
    this.node_username_.innerHTML = 'by <a href="/u/' + this.gallery_.getCreator() + '">' + this.gallery_.getCreator() + '</a>';
  }
  
  this.node_body_ = createElement('div', 'body', this.node_);
  this.node_filelist_ = createElement('div', 'filelist', this.node_body_);

  this.node_insert_ = createElement('div', 'insert-indicator', this.node_);

  // Sometimes we have to do things (such as get a gallery id and key)
  // before we can send the file.
  this.manual_submit_queue_ = [];

  this.node_status_ = createElement('div', 'dm-editor-status ', document.body);
  
  addEventListenerWrapper(document.body, 'dragover', this.dragOver_.bind(this));
  addEventListenerWrapper(document.body, 'dragenter', this.dragEnter_.bind(this));
  addEventListenerWrapper(document.body, 'dragleave', this.dragLeave_.bind(this));

  this.hide_drag_indicator = null;
  
  if(gallery.IS_EDITOR) {
    addEventListenerWrapper(document.body, 'drop', this.fileDropped.bind(this)); 
  }

  this.dragging_ = false;
  this.mousemovehandler_ = this.mousemove_.bind(this);
  this.mouseuphandler_ = this.mouseup_.bind(this);

  addEventListenerWrapper(document.body, 'mousemove', this.mousemovehandler_);
  
  // If we're loading into a gallery that already has data in it, prepare
  // our UI.
  this.reorder();
  
  if (this.gallery_.id) {
    //this.showAccessURL_(Gallery.URL_VIEWER_BASE + this.gallery_.id);
	this.showAccessURL_(this.gallery_.id);
	this.showGalleryInfo_();
	this.setupURLEdit();
    this.layoutPencil();
  }
}

Editor.NODE_STATUS_BOTTOM = 35;
Editor.URL_IMG_PUBLIC = '/smedia/minus/images/public.png';
Editor.URL_IMG_PRIVATE = '/smedia/minus/images/private.png';
Editor.PUBLISHED_MSG = 'The gallery is now published to your Minus profile.';

// return true if textarea/input text is visible in gallery menu (ignores the radio buttons)
Editor.visibleEdit = function() {
  var visible_edit = $('.dm-editor').find('textarea:visible').length + $('.dm-editor').find('input[type=text]:visible').length;
  if (visible_edit > 0) {
    return true;
  }
  return false;
}

Editor.prototype.getAjaxCounter = function() {
  return this.ajax_counter;
}

Editor.prototype.incAjaxCounter = function() {
  this.ajax_counter++;
  this.checkAjaxCounter();
}

Editor.prototype.decAjaxCounter = function() {
  this.ajax_counter--;
  this.checkAjaxCounter();
}

Editor.prototype.checkAjaxCounter = function() {
  window.console.log('checkAjaxCounter');
  if (this.ajax_counter == 0) {
	$('#edit_url_icon_div').show();
  } else {
    $('#edit_url_icon_div').hide();
  }
}

Editor.prototype.layoutPencil = function() {
  if (this.node_filelist_ === null || this.node_filelist_ === undefined) return;
  if (this.node_access_url === null || this.node_access_url_ === undefined) return;
  
  var file_div = $(this.node_filelist_).find('.file.selected');
  if (file_div !== null && file_div.length > 0) {
    var edit_div = file_div.find('.edit');
    var title_height = $(this.node_title_).height();
    var file_div_top = $(file_div).position().top;
    var filelist_div_ptop = parseFloat($(this.node_filelist_).css('padding-top'));
    var file_div_ptop = parseFloat($(this.node_filelist_).css('padding-top'));
    edit_div.css('top', file_div_top + file_div_ptop - 2);
  
    if (file_div_top <= 0 || (file_div_top > 0 && (file_div_top - (title_height + filelist_div_ptop) <= 0))) {
      edit_div.hide();
    }
    else if (file_div_top > 0 && ((file_div_top + file_div.height())> $(this.node_access_url_).position().top)) {
      edit_div.hide();
    }
    else {
      if (this.gallery_.getSelected().id) {
        edit_div.show();
	  }
    }
  }
}

Editor.updateFileInputWidth = function(text_box, text_node_span) {
  var text_width = $(text_node_span).width();
  if (text_width <= 150) {
    $(text_box).css('width',text_width+5);
  } else {
    $(text_box).css('width', 155);
  }
}

Editor.prototype.isLoggedIn = function() {
  if (this.gallery_.USER.length > 0) {
    return true;
  }
  return false;
}

Editor.prototype.setupURLEdit = function() {
  if (this.gallery_.IS_EDITOR) {
	  // url edits
	  var this_editor = this;
	  var logged_in = this.isLoggedIn();
	  var this_access_url = $(this.node_access_url_);
	  var text_box = this_access_url.find('.url_text_edit input');
	  
	  // #92
	  /*
      this_access_url.find('.url_label, .url_text').hover(function() {
	    $('.dm-editor .inline_edit_btn').parent().hide();
	    this_access_url.find('.edit').hide();
        //$(this).find('.edit').show();
		$(this).parent().find('.url_label .edit').show();
		clearTimeout(this_editor.hide_edit_timeout);
      }, function() {
	    this_editor.hide_edit_timeout = setTimeout(function(){$(this).find('.edit').hide()}.bind(this), 1000);
      });
	  */
	  
	  $(this.node_filelist_).scroll(this_editor.layoutPencil.bind(this_editor));
	  
	  this_access_url.find('.url_label .edit').click(function(e) {
	    if (logged_in) {
	      var wrapper = $(this).parent().parent();
		  if (!wrapper.find('.url_text_loading').is(':visible')) {
		    wrapper.find('.url_text').hide();
		    var text_edit = wrapper.find('.url_text_edit');
		    text_edit.show();
		    text_edit.find('input').select();
		    //$(this).hide();
		  }
		} else {
		  var error_msg = $('#error_cant_edit_url');
		  if (error_msg.length == 0) {
		    this_editor.gallery_.callListeners_({
		      type : Gallery.EVENT_ERRORS,
              errors : [],
			  inner_html: Gallery.ERROR_CANT_EDIT_URL
		    });
		  }
		}
	  });
	  
	  text_box.blur(function() {
	    var wrapper = $(this).parent().parent();
		var url_type = $(this).attr('gallery_url');
		var esc_func = function() {
		 valid_input($(this));
		 if (url_type == 'share') {
		    $(this).val(this_editor.gallery_.reader_url);
		  } else if (url_type == 'edit') {
		    $(this).val(this_editor.gallery_.id);
		  }
		  wrapper.find('.url_text').show();
		  wrapper.find('.url_text_edit').hide();
		  wrapper.find('.url_label .edit').show();
		}.bind(this);
		
		if (this_editor.gallery_.inline_edit_flag) {
          if (url_type == 'edit' && this_editor.gallery_.id ==  $(this).val()) {
		    esc_func();
		  } else if (url_type == 'share' && this_editor.gallery_.reader_url == $(this).val()) {
		    esc_func();
		  } else {
		    if (is_valid_input(text_box)) {
		      var res = this_editor.validateUpdateURL_($(this), wrapper, url_type);
		      if (!res) { esc_func(); }
		    } else {
              esc_func();
		    }
		  }
		} else if (this_editor.gallery_.esc_key_flag) {
		  esc_func();
		}
	  });
	  
	  text_box.keypress(function(e) {
	    var wrapper = $(this).parent().parent();
		// type of the url: edit/share
		// can be used in ajax
		var url_type = $(this).attr('gallery_url');
		var this_text_box = $(this);
		
	    if (e.which == '13') {
		  // #87
		  //this_editor.updateURL_($(this), wrapper, url_type);
		  this_editor.validateUpdateURL_($(this), wrapper, url_type);
		  e.preventDefault();
		  e.stopPropagation();
		  $(this).blur();
		  return false; // IE Fix
		}
/*		else if (e.which == '32') {
		  e.preventDefault();
		  e.stopPropagation();
		  return false; // prevent typing spaces
		}*/
	  });
	  
	  text_box.keyup(function(e) {
	    if (this_editor.validateUrlLastAjax !== null) {
          this_editor.validateUrlLastAjax.abort();
        }
	    var wrapper = $(this).parent().parent();
		var url_type = $(this).attr('gallery_url');
		if (e.which != '13') {
		  // front-end validation
		  var res = this_editor.validateURL_($(this).val());
		  if (!res.result) {
		    invalid_input($(this), res.msg);
		  } else {
		    // if successful, call api to validate
		    this_editor.validateURLAPI_($(this));
		  }
		}
	  });
    }
}

Editor.prototype.validateURL_ = function(url) {
  var new_url = $.trim(url);
  if (new_url.length == 0) {
	return {result: false, msg: Gallery.ERROR_URL_REQUIRED};
  }
  if (new_url.length > Gallery.URL_MAXLENGTH) {
    return {result: false, msg: Gallery.ERROR_URL_MAXLENGTH};
  }
  // no spaces allowed
  if (url.indexOf(' ') != -1) {
	return {result: false, msg: Gallery.ERROR_URL_SPACES_NOT_ALLOWED};
  }
  return {result: true};
}

Editor.prototype.updateURL_ = function(text_box, wrapper, url_type) {
  var this_editor = this;
  var error_func = function() {
    text_box.attr('disabled', false);
    wrapper.find('.url_text').show();
    wrapper.find('.url_text_edit').hide();
	wrapper.find('.url_text_loading').hide();
    if (url_type == 'edit') {
      text_box.val(this_editor.gallery_.id);
    } else if (url_type == 'share') {
      text_box.val(this_editor.reader_url);
	  
	  this_editor.gallery_.callListeners_({
	    type: Gallery.EVENT_END_AJAX
      });
	  //this_editor.decAjaxCounter();
    }
	if (url_type == 'share') {
	  this_editor.gallery_.callListeners_({
	    type: Gallery.EVENT_END_AJAX
      });
	} else {
	  this_editor.showHideGalleryOverlay();
	}
  };
  
  var success_func = function(text, text_status, xml_req) {
    if (text.length === 0) {
      text_box.attr('disabled', false);
      wrapper.find('.url_text_loading').hide();
      var view_text = wrapper.find('.url_text');
	  var new_url = text_box.val();
	  var new_link = window.location.protocol + '//min.us' + Gallery.URL_VIEWER_BASE + new_url;
	  var new_zip_link = Gallery.URL_VIEWER_BASE + new_url + Gallery.URL_ZIP;
	
	  view_text.find('a')
	    .attr('href', Gallery.URL_VIEWER_BASE + new_url)
	    .text(new_link);
	  if (url_type == 'share') {
	    wrapper.find('.sharelink').attr('href', Gallery.URL_VIEWER_BASE + new_url);
	  }
      view_text.show();
	  wrapper.find('.url_label .edit').show();
	
      // update url gallery.id / this.reader_url
      if (url_type == 'edit') {
        this_editor.gallery_.id = new_url;
		// #114/#116 change the browser url
		if (window.history.replaceState) {
		  window.history.replaceState({}, '', Gallery.URL_VIEWER_BASE + new_url);
		} else {
		  // redirect
		  if (this_editor.gallery_.loading_queue_.length === 0) {
		    var all_items_uploaded = true;
		    for (var i = 0, item; item = this_editor.gallery_.items_[i]; i++) {
              if (!item.id) {
			    all_items_uploaded = false;
			    break;
              }
            }
			if (all_items_uploaded) {
		      window.location.href = Gallery.URL_VIEWER_BASE + new_url;
			}
          }
		}
      } else if (url_type == 'share') {
        this_editor.reader_url = text_box.val();
        this_editor.gallery_.reader_url = text_box.val();
	    // update the download zip link
	    $('#zip_url').attr('href', new_zip_link);
	    // update social icon links
	    $('#twitter_url').attr('href', Gallery.URL_TWITTER + new_link);
	    $('#reddit_url').attr('href', Gallery.URL_REDDIT + new_link);
	    $('#digg_url').attr('href', Gallery.URL_DIGG + new_link);
	    $('#facebook_url').attr('href', Gallery.URL_FACEBOOK + new_link);
	    $('#stumble_url').attr('href', Gallery.URL_STUMBLE + new_link);
	    $('#email_url').attr('href', Gallery.URL_EMAIL + new_link);
		
		this_editor.gallery_.callListeners_({
	      type: Gallery.EVENT_END_AJAX
        });
		//this_editor.decAjaxCounter();
      }

	  this_editor.gallery_.redirect = true;
      // update all gallery links and forum codes
      this_editor.gallery_.callListeners_({
        type : Gallery.EVENT_URL_UPDATE
      });
    } else {
	  invalid_input(text_box, text);
	}
	if (url_type == 'share') {
	  this_editor.gallery_.callListeners_({
	    type: Gallery.EVENT_END_AJAX
      });
	} else {
	  this_editor.showHideGalleryOverlay();
	}
  };
  
  var url_api = '';
  
  if (url_type == 'share') {
    url_api = Gallery.URL_CHANGE_SHARE_URL;
  } else if (url_type == 'edit') {
    url_api = Gallery.URL_CHANGE_EDIT_URL;
  } else {
    return false;
  }
  
  $.ajax({
    url: url_api,
	data: $.param({url: $(text_box).val(), edit_url: this_editor.gallery_.id}),
    type: 'POST',
	dataType: 'text',
	beforeSend: function() {
      text_box.attr('disabled', true);
	  wrapper.find('.url_text_edit').hide();
	  wrapper.find('.url_text_loading').show();
	  if (url_type == 'share') {
	    this_editor.incAjaxCounter();
	  }
    },
    success: success_func,
    error: error_func
  });
  
  return true;
}

Editor.prototype.validateURLAPI_ = function(text_box) {
  if (this.validateUrlLastAjax !== null) {
    this.validateUrlLastAjax.abort();
  }
  var this_editor = this;
  var url_api = '';
  var url_type = $(text_box).attr('gallery_url');
  if (url_type == 'share') {
    url_api = Gallery.URL_VALIDATE_SHARE_URL;
  } else if (url_type == 'edit') {
    url_api = Gallery.URL_VALIDATE_EDIT_URL;
  } else {
    return false;
  }
  this.validateUrlLastAjax = $.ajax({
    url: url_api,
	type: 'POST',
	dataType: 'text',
	data: $.param({url: $(text_box).val(), edit_url: this_editor.gallery_.id}),
	beforeSend: function() {
	},
	success: function(text, text_status, xml_req) {
	  if (text.length === 0) {
	    valid_input(text_box);
	  } else {
	    invalid_input(text_box, text);
	  }
	},
	error: function() {
	  valid_input(text_box);
	}
  });
}

Editor.prototype.validateUpdateURL_ = function(text_box, wrapper, url_type) {
  if (this.validateUrlLastAjax !== null && this.validateUrlLastAjax.abort) {
    this.validateUrlLastAjax.abort();
  }
  // validate using javascript first
  var new_url = $(text_box).val();
  var valid_url = this.validateURL_(new_url);
  
  if (!valid_url.result) {
    //invalid_input($(text_box));
	//$(text_box).showErrorMsg({msg: valid_url.msg, additional_left: -20});
	invalid_input($(text_box), valid_url.msg);
	return false;
  } else {
    valid_input($(text_box));
  }
  
  var this_editor = this;
  var url_api = '';
  
  if (url_type == 'share') {
    url_api = Gallery.URL_VALIDATE_SHARE_URL;
  } else if (url_type == 'edit') {
    url_api = Gallery.URL_VALIDATE_EDIT_URL;
  } else {
    return false;
  }
  
  //window.console.log('----a validateUpdateURL: ' );
  // #87
  // then validate using api
  $.ajax({
    url: url_api,
    type: 'POST',
	dataType: 'text',
	data: $.param({url: new_url, edit_url: this_editor.gallery_.id}),
	beforeSend: function() {
	  text_box.attr('disabled', true);
	  wrapper.find('.url_text_edit').hide();
	  wrapper.find('.url_text_loading').show();
	  if (url_type == 'share') {
	    this_editor.incAjaxCounter();
	  }
	},
	success: function(text, text_status, xml_req) {
	  text_box.attr('disabled', false);
	  if (text.length === 0) {
	    this_editor.updateURL_(text_box, wrapper, url_type);
	  } else {
	    wrapper.find('.url_text_edit').show();
		wrapper.find('.url_text_loading').hide();
	    invalid_input(text_box, text);
		text_box.focus();
	  }
	  /*
	  if (url_type == 'share') {
	    this_editor.gallery_.callListeners_({
	      type: Gallery.EVENT_END_AJAX
        });
	  } else {
	    this_editor.showHideGalleryOverlay();
	  }
	  */
	},
	error: function(xml_req, text_status, error_thrown) {
	  text_box.attr('disabled', false);
	  //if (url_type == 'share') {
	  //  this_editor.decAjaxCounter();
	  //}
	  wrapper.find('.url_text').show();
      wrapper.find('.url_text_edit').hide();
	  wrapper.find('.url_text_loading').hide();
      if (url_type == 'edit') {
        text_box.val(this_editor.gallery_.id);
      } else if (url_type == 'share') {
        text_box.val(this_editor.reader_url);
      }
	  if (url_type == 'share') {
	    this_editor.gallery_.callListeners_({
	      type: Gallery.EVENT_END_AJAX
        });
	  } else {
	    this_editor.showHideGalleryOverlay();
	  }
	}
  });
  
  return true;
}

Editor.prototype.manualFileChanged = function(form, filenode) {
  log('Editor.prototype.manualFileChanged. filenode: ' + $(form).html());
  if (this.gallery_.id) {
    form.submit();
    form.reset();
  } else {
    this.gallery_.get_id();
    this.manual_submit_queue_.push(form);
  }
}

// static
Editor.cancelEvent = function(e) {
  if (!e) var e = window.event;
  e.cancelBubble = true;
  if (e.stopPropagation) e.stopPropagation();
  if (e.preventDefault) e.preventDefault();
}

Editor.prototype.setStatus = function(text) {
  if (text) {
    $(this.node_status_).css('opacity','1').css('bottom', Editor.NODE_STATUS_BOTTOM).html(text).show();
  } else {
    $(this.node_status_).css('opacity','1').css('bottom', Editor.NODE_STATUS_BOTTOM).hide();
  }
}

Editor.prototype.updateName_ = function(encoded) {
  if (typeof encoded == 'undefined') {
    encoded = false;
  }
  if (this.gallery_.name && this.gallery_.name.length > 0 && this.gallery_.name != 'Untitled') {
    var truncated_title = this.gallery_.name;
    if (!encoded) {
	  if (truncated_title.length > 60) {
	    truncated_title = truncated_title.substr(0, 60) + '...';
	  }
	} else {
	  var title_decoded = htmlDecode(this.gallery_.name);
	  if (title_decoded.length > 60) {
	    truncated_title = title_decoded.substr(0, 60) + '...';
	  } else {
	    truncated_title = title_decoded;
	  }
	}
    var new_title_span = $('<span>').text(truncated_title);
	//var new_title_span = $('<span>').html(truncated_title);
	$('div.listname').empty().html(new_title_span);
  } else {
    if (this.gallery_.IS_EDITOR) {
	  var add_title = $('<span>', {'text': 'Add Title', 'class': 'title_add inline_edit_btn', 'inline_input_id': 'listname_edit'});
	  $('div.listname').empty().append(add_title);
	  $(add_title).click(function(){this.nameClicked_(this);}.bind(this));
	} else {
	  $('div.listname').html('<span>Untitled</span>');
	}
  }

  if(this.gallery_.IS_EDITOR) {
	// #92
	//var edit_div = $('<div>', {'class': 'edit', 'style': 'display:none;'});
	
	/*
	var edit_div = $('<div>', {'class': 'edit'});
	var pencil_img = $('<img>', {'class': 'inline_edit_btn', 'inline_input_id': 'listname_edit', 'src': '/smedia/minus/lightpencil.png'});
    
	edit_div.append(pencil_img);
	
	$('div.listname')
	  .append(edit_div);
	
	$('div.listname').find('.edit').click(function(e) {
	    this.nameClicked_(this);
	  }.bind(this));
	*/
  }
}

Editor.prototype.galleryListener = function(e) {
  if (e.type == Gallery.EVENT_NEWITEM) {
    this.setStatus('40pxing...');
    this.reorder();
	this.layoutPencil();
  } else if (e.type == Gallery.EVENT_SELECTION) {
    this.layout();
	this.moveScrollbar();
	this.layoutPencil();
  } else if (e.type == Gallery.EVENT_REORDER || 
             e.type == Gallery.EVENT_DELETION) {
    this.reorder();
	this.layoutPencil();
  } else if (e.type == Gallery.EVENT_ITEMLOADED) {
    this.layoutPencil();
  } else if (e.type == Gallery.EVENT_ALLITEMSLOADED) {
    $(this.node_).show();
	$('div.filelist div.file').hover(
		function() {
		  $(this).find('.name').css('color','#000');
		},
		function() {
		  $(this).find('.name').css('color','');
		}
	);
	if(this.gallery_.IS_EDITOR) {  
	  //$('div.filelist div.file ').css('cursor','move');
	}	
	else {
	  $('div.filelist div.file').css('cursor','pointer');
	}
	this.layoutPencil();
  } else if (e.type == Gallery.EVENT_NAMECHANGED) {
    this.updateName_();
	this.layoutPencil();
  } else if (e.type == Gallery.EVENT_NAMECHANGEDFAILED || 
             e.type === Gallery.EVENT_NAMECHANGING) {
    this.layoutPencil();
  } else if (e.type == Gallery.EVENT_SAVED) {
    this.setStatus();
  } else if (e.type == Gallery.EVENT_DELETING) {
    this.setStatus("Deleting...");
  } else if (e.type == Gallery.EVENT_DELETED) {
    this.setStatus("Deleted, returning home...");
    this.node_.style.top = -100 + 'px';
    this.node_.style.opacity = 0;
    setTimeout(function() {window.location.href = '/'}, 150);
  } else if (e.type == Gallery.EVENT_DELETEFAILED) {
    alert("There was an error while deleting.");
  }  
  else if (e.type == Gallery.EVENT_SAVEDID) {
    // If we have pending manual uploads, send them.
    if (this.manual_submit_queue_.length) {
      for (var i = 0, form; form = this.manual_submit_queue_[i]; i++) {
        form.submit();
        form.reset();
      }
      this.manual_submit_queue_ = [];
    }

    if (window.history.pushState)
      window.history.pushState({}, "", Gallery.URL_VIEWER_BASE + e.id);

    //this.showAccessURL_(Gallery.URL_VIEWER_BASE + e.id);
	this.showAccessURL_(e.id);
	this.showGalleryInfo_();
	this.setupURLEdit();
  } else if (e.type == Gallery.EVENT_ERRORS) {
    if (e.inner_html !== undefined) {
	  this.showErrors_(this.gallery_, e.errors, e.inner_html);
	} else {
      this.showErrors_(this.gallery_, e.errors);
	}
  } else if (e.type == Gallery.EVENT_NEW_AJAX) {
    //window.console.log('---a Gallery.EVENT_NEW_AJAX');
	this.incAjaxCounter();
  } else if (e.type == Gallery.EVENT_END_AJAX) {
	//window.console.log('----a Gallery.EVENT_END_AJAX');
	this.showHideGalleryOverlay();
	this.decAjaxCounter();
  }
}

Editor.prototype.showHideGalleryOverlay = function() {
  //window.console.log('----a show hide gallery overlay');
  //var visible_edit = $(this.node_).find('textarea:visible').length + $(this.node_).find('input[type=text]:visible').length;
  var visible_edit = Editor.visibleEdit();
  if (!this.gallery_.LIGHT_MODE && !visible_edit) {
    $(this.node_dark_).height($(this.node_).height());
    $(this.node_dark_).removeClass('dm-editor-dark-zero');
  }
}


// Move file list scrollbar to display the current selected item
Editor.prototype.moveScrollbar = function() {
  var file;
  var filelist = $(this.node_filelist_);
  var filelist_ptop = parseFloat(filelist.css('padding-top'));
  var filelist_mtop = parseFloat(filelist.css('margin-top'));
  var title = $(this.node_title_);
  var title_top = title.position().top;
  var title_height = title.height();
  
  var selected = this.gallery_.getSelected();  
  for (var i = 0, item_node; item_node = this.item_nodes_[i]; i++) {
    if (item_node.item == selected) {
      file = $(item_node);
	  break;
    }
  }
  
  var filelist_height = $(filelist).height();
  var file_offset = $(file).offset();
  var file_position = $(file).position();
  
  if (file_position !== null) {
	if (file_position.top > (filelist_height + title.height())) {
	  filelist.scrollTop(filelist.scrollTop() + Math.abs(filelist_height - file_position.top) - title.height() + 10);
	} else if (file_position.top < title.height()) {
	  filelist.scrollTop(filelist.scrollTop() - Math.abs(file_position.top - title.height() - filelist_ptop - filelist_mtop));
    }
  }
}

Editor.prototype.createFile_ = function(item) {
  item.addListener(this.itemListener.bind(this));
  var file = createElement('div', 'file', this.node_filelist_);

  // $(this.node_filelist_).append('<div class="clear">&nbsp</div>');
  file.text_node = createElement('div', 'name', file);
  $(file.text_node).html('<span style="color:#aaa;">Loading...</span>');
  file.item = item;
  var editor = this;
  $(file).click(function(e) {
    editor.gallery_.setSelected(file.item);
    save_event('click_item', null, file.item.id);
  });
  
  $(file).attr('itemid',item.id);  
  
  if(this.gallery_.IS_EDITOR) {
    //var loading_img = $('<img/>').attr('src', '/smedia/minus/images/ajax-loader-bar.gif');
	var loading_img = $('<img/>').attr('src', Gallery.URL_AJAX_LOADER_SMALL);
    file.edit_button = createElement('div', 'edit', file);
    var pencil_img = $('<img/>').attr('src', '/smedia/minus/lightpencil.png').attr('class', 'inline_edit_btn pencil_light_mode').attr('inline_input_id', 'input_inline-' + item.id);
	var pencil_img_dmode = $('<img/>').attr('src', '/smedia/minus/lightpencil-darkmode.png').attr('class', 'inline_edit_btn pencil_dark_mode').attr('inline_input_id', 'input_inline-' + item.id);
    $(file.edit_button).hide().append(pencil_img);
	$(file.edit_button).append(pencil_img_dmode);
  
    var this_editor = this;
    var name_edit = createElement('div', 'name_edit', file);
	//var name_edit_text = $('<input/>', {'type': 'text', 'id': 'input_inline-' + item.id, 'class': 'filename_edit inline_edit', 'maxlength': ''+Item.FILENAME_MAXLENGTH});
	var name_edit_text = $('<input/>', {'type': 'text', 'id': 'input_inline-' + item.id, 'class': 'filename_edit inline_edit'});
	file.text_node_loading = createElement('div', 'loading_edit', file);
	
    var delete_button = createElement('div', 'delete', file);

	$(file.text_node_loading).hide();
	$(file.text_node_loading).append(loading_img);
	
	$(name_edit).append(name_edit_text).hide();
	$(name_edit_text).enableSelection();

	$(name_edit_text).keypress(function(e) {
	  if (e.which == '13') {
        this_editor.validateUpdateItemName_(file, item, name_edit, this);
		return false; // IE fix
	  }
	});
	
	$(name_edit_text).keyup(function(e) {
	  if (e.which != '13') {
	    var new_filename = $(this).val();
        var res = this_editor.validateItemName_(new_filename);
        if (!res.result) {
          invalid_input($(this), res.msg);
        } else {
		  // validate using api
		  this_editor.validateItemNameAPI_(item, $(this));
        }
	  }
	});
	
	$(name_edit_text).blur(function() {
	  var esc_func = function() {
	    valid_input($(this));
	    $(this).val(filename_noext(item.name)).change();
		$(name_edit).hide();
		$(file.text_node).show();
		if (item == this_editor.gallery_.getSelected()) {
		  $(file.edit_button).show();
		}
	  }.bind(this);
	  
	  if (this_editor.gallery_.inline_edit_flag) {
	    if (is_valid_input(name_edit_text)) {
	      var res = this_editor.validateUpdateItemName_(file, item, name_edit, this);
		  if (!res) { esc_func(); }
		} else {
		  esc_func();
		}
	  } else if (this_editor.gallery_.esc_key_flag) {
	    esc_func();
	  }
	});
	
	$(name_edit_text).change(function() {
	  if ($(this).hasClass('error')) {
	    valid_input($(this));
	  }
	});
	
	$(name_edit_text).autoGrowInput({
	  comfortZone: 12,
	  minWidth: 10,
	  maxWidth: 155,
	  callback: function() {$(name_edit_text).showErrorMsg();}
	});
	
    //pencil_img.click(function(event) {
	$(file.edit_button).click(function(event) {
	  if (!$(file.text_node_loading).is(':visible')) {
	    $(file.text_node).hide();
	    $(name_edit).show();
	    $(name_edit_text).select();
	    clearTimeout(item.hide_edit_timeout);
      }
    });

	$(delete_button).hover(
	  function() {
		$(this).addClass('delete_hover');
	  },
	  function() {
		$(this).removeClass('delete_hover');
	  }
	).attr('title', 'Delete this item from the gallery.');
	$(delete_button).click(function(e) {
		editor.gallery_.deleteItem(file.item);
		e.stopPropagation();
		return false;
	});
  }
  $(file).append(
	"<div class='wait'></div>"
  );
  $(file).find('.delete').hide();
  $(file).find('.wait').show();
  if(this.gallery_.IS_EDITOR) {  
	$(file).css('cursor','move');
  }
  return file;  
}

Editor.prototype.validateItemName_ = function(name) {
  var new_name = $.trim(name);
  // empty names not allowed
  // filenames more than the limit not allowed
  if (new_name.length == 0) {
	return {result: false, msg: Item.ERROR_FILENAME_REQUIRED};
  } else if (new_name.length > Item.FILENAME_MAXLENGTH) {
    return {result: false, msg: Item.ERROR_FILENAME_MAXLENGTH};
  }
  return {result: true};
}

// for keyup
Editor.prototype.validateItemNameAPI_ = function() {
  var lastAjax = null;
  return function(item, text_box, cancel_only) {
    if (lastAjax !== null && lastAjax.abort) {
	  lastAjax.abort();
	}
	if (cancel_only === undefined || cancel_only !== true) {
	  lastAjax = $.ajax({
	    url: Gallery.URL_VALIDATE_ITEM_NAME,
	    type: 'POST',
	    dataType: 'text',
	    data: $.param({edit_url: this.gallery_.id, id: item.id, filename: text_box.val()}),
	    success: function(text, text_status, xml_req) {
	      if ($(text_box).is(':visible')) {
	        if (text.length === 0) {
		      valid_input(text_box);
		    } else {
		      invalid_input(text_box, text);
		    }
          }
		},
	    error: function() {
		  valid_input(text_box);
	    }
	  });
	}
  };
}();

Editor.prototype.validateUpdateItemName_ = function(file, item, text_div, text_box) {
  var new_filename = $(text_box).val();
  // front end validation
  var fe_val = this.validateItemName_(new_filename);
  if (fe_val.result) {
    this.validateItemNameAPI_(null, null, true);
    // back end validation (if successful, call update )
	$.ajax({
	  url: Gallery.URL_VALIDATE_ITEM_NAME,
	  type: 'POST',
	  data: $.param({edit_url: this.gallery_.id, id: item.id, filename: new_filename}),
	  dataType: 'text',
	  beforeSend: function() {
	    $(text_box).attr('disabled', true);
	    $(text_div).hide();
	    $(file.text_node_loading).show();
	    this.incAjaxCounter();
	  }.bind(this),
	  success: function(text, text_status, xml_req) {
		$(text_box).attr('disabled', false);
	    if (text.length === 0) {
	      this.updateItemName_(file, item, text_div, text_box);
		} else {
		  $(text_div).show();
		  $(file.text_node_loading).hide();
		  invalid_input(text_box, text);
		}
		//this.gallery_.callListeners_({
	    //  type: Gallery.EVENT_END_AJAX
        //});
	  }.bind(this),
	  error: function() {
	    // error in validation, hide input box and show display div
		$(text_div).hide();
		$(text_box).val(filename_noext(item.name)).change();
		$(file.text_node).show();
		this.gallery_.callListeners_({
	      type: Gallery.EVENT_END_AJAX
        });
	  }.bind(this)
	});
	return true;
  } else {
    invalid_input(text_box, fe_val.msg);
    return false;
  }
}

Editor.prototype.updateItemName_ = function(file, item, text_div, text_box) {
  var gallery = this.gallery_;
  var this_editor = this;
  var val = $(text_box).val();
  var ext = extension(item.name);

  var new_filename = val;
  if (extension(item.name)) {
    new_filename += '.' + extension(item.name);
  }
  
  var success_func = function(text, text_status, xml_req) {
    if (text.length === 0) {
      $(text_box).attr('disabled', false);
	  $(file.text_node_loading).hide();
      item.name = new_filename;
	  var item_filename_noext = filename_noext(item.name);
      var filename_html = '<span>' + htmlEncode(item_filename_noext) + '</span>';
	  if (extension(item.name)) {
        filename_html += '.' + extension(item.name);
      }
      $(file.text_node).html(filename_html);

      // Update also the picture info
      gallery.callListeners_({
        type : Gallery.EVENT_ITEM_RENAME,
        item: item
      });
      $(file.text_node).show();
	  if (item == gallery.getSelected()) {
        $(file.edit_button).show();
	  }
      $(text_box).val(item_filename_noext);
	  Editor.updateFileInputWidth(text_box, $(file.text_node).find('span'));
	} else {
	  invalid_input(text_box, text);
	}
	this_editor.gallery_.callListeners_({
	  type: Gallery.EVENT_END_AJAX
    });
  };
  
  $.ajax({
    url: Gallery.URL_RENAME_ITEM,
    type: 'POST',
	dataType: 'text',
	data: $.param({id: item.id, filename: $(text_box).val(), edit_url: gallery.id}),
    beforeSend: function () {
	  $(text_box).attr('disabled', true);
	  $(text_div).hide();
	  $(file.text_node_loading).show();
    },
    success: success_func,
    error: function(XMLHttpRequest, textStatus, errorThrown) {
	  // hide input box and show display div
      $(file.text_node_loading).hide();
	  $(text_box).attr('disabled', false);
      $(text_box).val(filename_noext(item.name)).change();
      $(text_div).hide();
      $(file.text_node).show();
	  this_editor.gallery_.callListeners_({
	    type: Gallery.EVENT_END_AJAX
      });
    }
  });
  return true;
}

Editor.prototype.getFileForItem_ = function(item) {
  for (var i = 0, file; file = this.item_nodes_[i]; i++) {
    if (file.item == item)
      return file;
  }
  return this.createFile_(item);
}

Editor.prototype.reorder = function() {
  this.setStatus('Loading...');
  var items = this.gallery_.getItems();
  if (!items) { return }

  // If there has been a deletion.
  for (var i = 0, node; node = this.item_nodes_[i]; i++) {
    node.processed = false;
  }

  var new_item_nodes = [];
  for (var i = 0, item; item = items[i]; i++) {
    var node = this.getFileForItem_(item)
    this.node_filelist_.appendChild(node);
    node.processed = true;
    new_item_nodes.push(node);    
    stripped_hash=window.location.hash.substring(1);
    if(stripped_hash.length>0 && stripped_hash=='i'+item.id) {    
        this.gallery_.setSelected(item);        
    }
  }

  // Find untouched nodes-they must have been deleted.
  if (new_item_nodes.length != this.item_nodes_.length) {
    for (var i = 0, node; node = this.item_nodes_[i]; i++) {
      if (node.processed == false) {
        this.node_filelist_.removeChild(node);
        delete node;
      }
    }
  }

  this.item_nodes_ = new_item_nodes;
  this.layout();
  gallery = this.gallery_;
  if(gallery.IS_EDITOR) {
    $(".dm-editor div.filelist").sortable({
      axis: 'y',
      revert: true,
      deactivate: function(event, ui) {         
          gallery.sortable_changed();
      }
    });
//	.disableSelection(); // input text can't be selected when editing filename
  }
}

Editor.prototype.itemListener = function(e) {
  var item = e.data;
  var this_editor = this;
  if (e.type == Item.EVENT_LOADED) {
    log('Editor.prototype.itemListener.'); 
    for (var i = 0, file; file = this.item_nodes_[i]; i++) {
      // log('Editor.prototype.itemListener. file.item: ' + $(file.item).html() + '. Item: '+$(item).html());   
      if (file.item == item) {
        //setText(file.text_node, item.name);
		var edit_div = $(file).find('.name_edit');
		var filename_decoded = htmlDecode(item.name); // from backend so it is encoded.
		var item_filename_noext = filename_noext(filename_decoded);
		var filename_html = '<span>' + htmlEncode(item_filename_noext) + '</span>';
		if (extension(item.name)) {
		  filename_html += '.' + extension(item.name);
		}
		$(file.text_node).html(filename_html);
		var text_box = $(file).find('.filename_edit');
        text_box.val(item_filename_noext);
		Editor.updateFileInputWidth(text_box, $(file.text_node).find('span'));
		
		if (this.gallery_.IS_EDITOR) {
		  $(file).hover(function() {
            //$('div.file .edit').hide();
			if (item.id) {
			  $('.dm-editor .filelist .inline_edit_btn').parent().hide();
			  if (this_editor.gallery_.getSelected().id) {
			    $('.dm-editor .filelist .file.selected .edit').show();
			  }
              $(this).find('.edit').show().css('top', ($(this).offset().top-7));
			}
            clearTimeout(item.hide_edit_timeout);
          }, function() {
		    var hide_pencil_func = function() {
			  if (item != this_editor.gallery_.getSelected()) {
			    $(this).find('.edit').hide();
			  }
			}.bind(this);
			item.hide_edit_timeout = setTimeout(hide_pencil_func, 1000);
            //item.hide_edit_timeout = setTimeout(function() {$(this).find('.edit').hide();}.bind(this), 1000);
          });
        }
		
		if (extension(item.name) && $(edit_div).find('span').length === 0) {
		  $(edit_div).append($('<span>', {text: '.' + extension(item.name)}));
		}
        log('Editor.prototype.itemListener. Picture loaded. ID available: '+item.id+'. Name: '+item.name+". Width: "+item.width);
		var filename_decoded = htmlDecode(item.name);
        $(file).attr('itemname', item.name).attr('itemid', item.id);
		$(file).find('input.filename_edit').val(filename_noext(filename_decoded));
		$(file).find('.name_edit span').text('.' + extension(filename_decoded));
        $(file).find('.delete').show();
        $(file).find('.wait').hide();
      }
    }
  }
  else if (e.type == Item.EVENT_RELOAD) {
    for (var i = 0, file; file = this.item_nodes_[i]; i++) {
      // log('Editor.prototype.itemListener. file.item: ' + $(file.item).html() + '. Item: '+$(item).html());   
      if (file.item == item) {
        $(file).find('.delete').hide();
        $(file).find('.wait').show();        
      }
    }
  }
}

Editor.prototype.layout = function() {
  var items = this.gallery_.getItems();
  var selected = this.gallery_.getSelected();  
  for (var i = 0, file; file = this.item_nodes_[i]; i++) {
    $(file).addClass('file'); //.addClass('ui-state-default')
    if (file.item == selected) {
      $(file).addClass('selected');
	  //this.layoutPencil();
	  //$(file.edit_button).show();
    } else {
      $(file).removeClass('selected');
	  $(file.edit_button).hide();
    }
  }
  resize_editor_window();
}

Editor.prototype.titleClicked_ = function(e) {
  if (this.node_.className == 'dm-editor') {
    this.node_.className = 'dm-editor collapsed';
  } else {
    this.node_.className = 'dm-editor';
  }
}

Editor.prototype.titleDeleteClicked_ = function(file) {
  if (confirm("Are you sure you want to delete this gallery?")) {
    this.gallery_.deleteGallery();
  }
  if (BrowserDetect.browser != "Explorer") {  
  //  window.event.stopPropagation();
  //  window.event.preventDefault();
  }
}

Editor.prototype.nameClicked_ = function(e) {
  $(this.node_name_).hide();
  if (this.node_name_edit_) {
    $(this.node_name_edit_).show();
	$('#listname_edit').select();
  }
}

Editor.prototype.mousemove_ = function(e) {
  this.highlightDrop_(e);
}

Editor.prototype.dragEnter_ = function(e) {
  this.dragging_ = true;
  Editor.cancelEvent(e);
}

Editor.prototype.dragLeave_ = function(e) {
  this.dragging_ = false;
  this.hide_drag_indicator = setTimeout("$('#drag_indicator').hide()", 200);
}

Editor.prototype.dragOver_ = function(e) {
  if (this.gallery_.IS_EDITOR && isValidBrowser()) {
    $('#drag_indicator').show();
	clearTimeout(this.hide_drag_indicator);
  }
  this.dragging_ = true;
  this.highlightDrop_(e);
  Editor.cancelEvent(e);
}

Editor.prototype.highlightDrop_ = function(e) {
  if (!this.dragging_) return;

  var drop = this.getDropPoint_(e);
  if (drop) {
    this.node_insert_.style.display = 'block';
    this.node_insert_.style.top = (drop.file.offsetTop + ((drop.after) ? drop.file.offsetHeight : 0)) + 'px';
  } else {
    this.node_insert_.style.display = 'none';
  }
}

Editor.prototype.getDropPoint_ = function(e) {
  for (var i = 0, file; file = this.item_nodes_[i]; i++) {
    if (e.target == file) {
      return {
        file : file,
        item : file.item,
        after : (e.offsetY > file.offsetHeight / 2)
      }
    }
  }
  return false;
}

Editor.prototype.mouseup_ = function(e) {
  window.console.log('Editor.prototype.mouseup_. Dragging: ' + this.dragging_);
  if (!this.dragging_) { return; }
  if (this.dragging_ == e.target) { 
    this.cancelDrag_();
    return;
  }

  var drop = this.getDropPoint_(e);
  if (drop)
    this.gallery_.reorder(this.dragging_.item, drop.item, drop.after ? Gallery.REORDER_AFTER : Gallery.REORDER_BEFORE);

  this.cancelDrag_();
}

Editor.prototype.cancelDrag_ = function() {
  window.console.log('Editor.prototype.cancelDrag_');
  this.dragging_ = false;
  this.node_insert_.style.display = 'none';
  document.body.removeEventListener('mouseup', this.mouseuphandler_, false);
}

Editor.prototype.fileDropped = function(e) {
  if (!isValidBrowser()) return;

  // disable sortable
  $('.dm-editor div.filelist').sortable("disable");
  
  $('#drag_indicator').hide();
  $('#share_url_icon_div').hide(); // #152 hide the share url edit icon when a file is uploading
  this.dragging_ = false;

  var data = e.dataTransfer;
  if (!data || !data.files.length) return;

  var drop = this.getDropPoint_(e);
  if (!drop) {
    this.gallery_.newLocalFiles(data.files);
  } else {
    this.gallery_.newLocalFiles(data.files, drop.item, drop.after ? Gallery.REORDER_AFTER : Gallery.REORDER_BEFORE);
  }

  // Show the dm-editor after files drop
  $(this.node_).show();
  $(this.node_dark_).height($(this.node_).height());
  $(this.node_dark_).show();
  
  e.stopPropagation();
  e.preventDefault();
  window.console.log('Editor.prototype.fileDropped');
  this.cancelDrag_();
  $('div.filelist div.file').css('cursor', 'default');
  return false;
}

Editor.prototype.showAccessURL_ = function(url) {
  if (!this.node_access_url_)   
  {
      var relative_full_url = Gallery.URL_VIEWER_BASE + url;
	  var full_url = window.location.protocol + '//' + 'min.us' + Gallery.URL_VIEWER_BASE + url;
	  var relative_full_reader_url = Gallery.URL_VIEWER_BASE + this.gallery_.reader_url;
      var full_reader_url = window.location.protocol + '//' + 'min.us' +  Gallery.URL_VIEWER_BASE + this.gallery_.reader_url;
	  var zip_url = '';
	  var this_gallery = this.gallery_;
	  if (is_debug()) {
	    zip_url = Gallery.URL_VIEWER_BASE + this.gallery_.reader_url + Gallery.URL_ZIP;
	  } else {
	    zip_url = full_reader_url + Gallery.URL_ZIP;
	  }
      //zip_inner_html = '<p class="download-gallery"> <a style="color:#0044CC;" href="' + zip_url + '" id="zip_url">Download all as zip</a>.</p>';
	  // Create download. 
	  // this.node_download_gallery = createElement('div', 'download-gallery-msg', this.node_body_);                   
	  //this.node_download_gallery.innerHTML = zip_inner_html; 
      // Add "Share this URL".
      this.node_access_url_ = createElement('div', 'access-url', this.node_body_);
	  //this.node_access_url_.innerHTML = zip_inner_html; 
      if(this.gallery_.IS_EDITOR) {  
        if (isValidBrowser()) {
		    this.node_access_url_.innerHTML += '<div class="is_editor" style="clear:both;"></div>';
            this.node_access_url_.innerHTML +=
			  '<div style="display:block;float:left;margin-top:2px;padding:2px 0 0px 0;margin-bottom:0px;width:45px;text-align:right;height:18px;">Upload </div><div style="display:block;white-space:nowrap;float:right;text-align:left;width:169px;margin:2px 2px 0 0px;padding:2px 0 0px 3px;cursor:default;">Drag onto this page or <span style="color:#0044CC;cursor:pointer;" id="upload_more_link">select</span>.</div>';
			  //'<div style="display:block;float:left;margin-top:8px;margin-bottom:7px;width:207px;padding:9px 10px 5px 10px;border-top:solid 1px #ccc;">Drop picture files anywhere on this page to upload new picture to gallery.</div>';
        } 
        else {
           this.node_access_url_.innerHTML +=
		    '<div class="is_editor" style="display:block;float:left;margin-top:0px;margin-bottom:0px;width:45px;text-align:right;padding:0px 0 0px 0;height:20px;">Upload </div><div style="display:block;float:right;padding:0px 0 0px 0;width:172px;margin-top:0px;margin-bottom:0px;"><span style="color:#0044CC;cursor:pointer;padding:4px 0 0px 0px;" id="upload_more_link">Browse...</span></div>';            
        }
		this.node_access_url_.innerHTML += '<div class="download_link_row" style="clear:both;"></div>'            
  	    this.node_access_url_.innerHTML += '<div class="download_link_row" style="display:block;float:left;margin-top:7px;margin-bottom:0px;width:45px;text-align:right;"><img src="/smedia/minus/images/save.png" style="padding:2px 0 0 0;"/> </div><div class="download_link_row" style="display:block;float:right;width:172px;margin-top:7px;margin-bottom:0px;padding:2px 0 0 0;"><a  style="color:#0044CC;" href="'+zip_url+'" id="zip_url">Download all as a zip</a>.</div>';

		this.node_access_url_.innerHTML += '<div style="clear:both;"></div>'            
		
		var edit_url = '<div class="url_wrapper">';
		edit_url += '<div class="url_label" style="display:block;float:left;margin-top:10px;margin-bottom:0px;width:45px;text-align:right;padding:2px 0 0px 0;"><div class="edit" id="edit_url_icon_div"><img class="inline_edit_btn pencil_light_mode" inline_input_id="input_url_access_edit" src="/smedia/minus/lightpencil.png" /><img class="inline_edit_btn pencil_dark_mode" inline_input_id="input_url_access_edit" src="/smedia/minus/lightpencil-darkmode.png" /></div>Edit </div>';
		edit_url += '<div class="url_text"><a target="_blank" href="' + relative_full_url + '">' + full_url + '</a></div>';
		edit_url += '<div class="url_text_edit" style="display:none;">' + window.location.protocol + '//min.us' + Gallery.URL_VIEWER_BASE + '<input id="input_url_access_edit" class="inline_edit" type="text" value="' + url + '" gallery_url="edit"/></div>';
		edit_url += '<div class="url_text_loading" style="display:none;"><img src="' + Gallery.URL_AJAX_LOADER_SMALL + '" /></div>';
		edit_url += '</div>';
		
		this.node_access_url_.innerHTML += edit_url;
  	    this.node_access_url_.innerHTML += '<div style="clear:both;"></div>';
        
		var share_url = '<div class="url_wrapper">';
		//share_url += '<div class="url_label" style="display:block;float:left;margin-top:10px;margin-bottom:5px;width:45px;text-align:right;padding:2px 0 0px 0;height:39px;"><div class="edit" id="share_url_icon_div"><img class="inline_edit_btn pencil_light_mode" inline_input_id="input_url_access_share" src="/smedia/minus/lightpencil.png" /><img class="inline_edit_btn pencil_dark_mode" inline_input_id="input_url_access_share" src="/smedia/minus/lightpencil-darkmode.png" /></div>Share </div>';
		share_url += '<div class="url_label" style="display:block;float:left;margin-top:10px;margin-bottom:5px;width:45px;text-align:right;padding:2px 0 0px 0;"><div class="edit" id="share_url_icon_div"><img class="inline_edit_btn pencil_light_mode" inline_input_id="input_url_access_share" src="/smedia/minus/lightpencil.png" /><img class="inline_edit_btn pencil_dark_mode" inline_input_id="input_url_access_share" src="/smedia/minus/lightpencil-darkmode.png" /></div>Share </div>';
		share_url += '<div class="url_text"><div class="yellow"><a  target="_blank" href="' + relative_full_reader_url + '" id="viewer_url" >' + full_reader_url + '</a></div></div>';
		share_url += '<div class="url_text_edit" style="display:none;">' + window.location.protocol + '//min.us' + Gallery.URL_VIEWER_BASE + '<input id="input_url_access_share" class="inline_edit" type="text" value="' + this.gallery_.reader_url + '" gallery_url="share" /></div>';
		share_url += '<div class="url_text_loading" style="display:none;"><img src="' + Gallery.URL_AJAX_LOADER_SMALL + '" /></div>';
		share_url += '<div style="clear:both;"></div>'  
		share_url += '</div>';

		this.node_access_url_.innerHTML += share_url;
		this.node_access_url_.innerHTML += '<div style="clear:both;"></div>';
      }
      else {
  		this.node_access_url_.innerHTML += '<div class="download_link_row" style="clear:both;"></div>'            
  	    this.node_access_url_.innerHTML += '<div class="download_link_row" style="display:block;float:left;margin-top:3px;margin-bottom:0px;width:45px;text-align:right;"><img src="/smedia/minus/images/save.png"/> </div><div class="download_link_row" style="display:block;float:right;width:172px;margin-top:3px;margin-bottom:0px;"><a  target="_blank" style="color:#0044CC;" href="'+zip_url+'">Download all as a zip</a>.</div>';
  	    this.node_access_url_.innerHTML += '<div style="clear:both;"></div>'            
		this.node_access_url_.innerHTML += '<div class="url_label" style="display:block;float:left;margin-top:10px;margin-bottom:5px;width:45px;text-align:right;padding:2px 0 0px 0;height:39px;"><div class="edit" style="display:none;"><img class="inline_edit_btn" inline_input_id="input_url_access_share" src="/smedia/minus/lightpencil.png" /></div>Share </div>';
		this.node_access_url_.innerHTML += '<div class="url_text"><div class="yellow"><a  target="_blank" href="' + full_reader_url + '" id="viewer_url" >' + full_reader_url + '</a></div></div>';
      }
      //this.node_access_url_.innerHTML += "<div style='clear:both;'></div>"
      var socialNode = ''; //'<div style="display:block;float:left;margin-top:5px;margin-bottom:7px;width:45px;text-align:right;"><b>&nbsp</b></div>';
      socialNode += '';
      var socialDiv = '<div style="display:block;float:right;width:172px;height:27px;margin-top:5px;margin-bottom:0px;"><table style="width: 100%" cellpadding="0" cellspacing="0"><tbody><tr>';
      socialDiv += '<td style="text-align: left"><a class="social_link_narrow" target="_blank" title="Tweet It" href="' + Gallery.URL_TWITTER + full_reader_url + '" id="twitter_url"><img src="/smedia/minus/twitterb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '<td style="text-align: left; padding-left: 7px;"><a class="social_link" target="_blank" title="Submit to Reddit" href="' + Gallery.URL_REDDIT + full_reader_url + '" id="reddit_url"><img src="/smedia/minus/redditb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '<td style="text-align: center"><a class="social_link" target="_blank" title="Submit to Digg" href="' + Gallery.URL_DIGG + full_reader_url + '" id="digg_url"><img src="/smedia/minus/diggb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '<td style="text-align: right; padding-right: 5px;"><a class="social_link" target="_blank" title="Share on Facebook" href="' + Gallery.URL_FACEBOOK + full_reader_url + '" id="facebook_url"><img src="/smedia/minus/facebookb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '<td style="text-align: right; padding-right: 5px;"><a class="social_link" target="_blank" title="Submit to Stumble Upon" href="' + Gallery.URL_STUMBLE + full_reader_url + '" id="stumble_url"><img src="/smedia/minus/stumbleuponb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '<td style="text-align: right"><a class="social_link_narrow" title="Email It" href="' + Gallery.URL_EMAIL + full_reader_url + '" id="email_url"><img src="/smedia/minus/emailb.png" style="border: none;" class="has_faded_version" /></a></td>';
      socialDiv += '</tr></tbody></table></div>';

      socialNode += socialDiv;
      this.node_access_url_.innerHTML += socialNode;
	  this.node_access_url_.innerHTML += '<div style="clear:both;"></div>';
	  
	  if (this.gallery_.IS_EDITOR && this.gallery_.getCreator() == this.gallery_.USER && this.gallery_.USER != '') {
	    var accessDiv = $('<div>', {'class': 'gallery_access'});
		var accessCont = $('<div>', {'class': 'access_cont'});
		var publicDiv = $('<div>', {'class': 'gallery_access_public', 'text': 'Published '});
		var privateDiv = $('<div>', {'class': 'gallery_access_private', 'text': 'Unpublished '});
		var publicLinkCont = $('<span>', {'class': 'other_txt', 'text': '('});
		var privateLinkCont = $('<span>', {'class': 'other_txt', 'text': '('});
		var setPublicLink = $('<span>', {'id': 'gaccess_setpublic', 'class': 'gaccess_link', 'text': 'Publish'});
		var setPrivateLink = $('<span>', {'id': 'gaccess_setprivate', 'class': 'gaccess_link', 'text': 'Unpublish'});
		var accessClear = $('<div>', {'style': 'clear:both;'});
		
		publicLinkCont
		  .append(setPublicLink)
		  .append(')');
		privateLinkCont
		  .append(setPrivateLink)
		  .append(')');
		publicDiv
		  .append(privateLinkCont)
		privateDiv
		  .append(publicLinkCont)
		
		if (this_gallery.is_public) {
		  privateDiv.hide();
		} else {
		  publicDiv.hide();
		}
		
		accessCont
		  .append(publicDiv)
		  .append(privateDiv);
		
		accessDiv.append(accessCont).append(accessClear);

		$(this.node_body_).append(accessDiv);
		
		setPrivateLink.click(function(e) {
		  privateDiv.show();
		  publicDiv.hide();
		  Gallery.updateAccess(this_gallery.id, 'private');
		});
		
		setPublicLink.click(function(e) {
		  var ajax_opts = {
		    success: function() {
		      show_confirm(Editor.PUBLISHED_MSG);
			}
		  };
		  privateDiv.hide();
		  publicDiv.show();
		  Gallery.updateAccess(this_gallery.id, 'public', ajax_opts);
		});
		
	  } else if (this.gallery_.IS_EDITOR) {
	    var accessDiv = $('<div>', {'class': 'gallery_access'});
		var accessCont = $('<div>', {'class': 'access_cont'});
		var accessClear = $('<div>', {'style': 'clear:both;'});
		if (this_gallery.is_public) {
		  accessCont.text('Published');
		} else {
		  accessCont.text('Unpublished');
		}
		accessDiv.append(accessCont).append(accessClear);
		$(this.node_body_).append(accessDiv);
	  }
	  
      $('.has_faded_version').mouseover(function(){
        var oldSrc = $(this).attr('src');
        $(this).attr('src', oldSrc.substring(0, oldSrc.length - 5) + '.png');
      });

      $('.has_faded_version').mouseout(function(){
        var oldSrc = $(this).attr('src');
        $(this).attr('src', oldSrc.substring(0, oldSrc.length - 4) + 'b.png');
      });

	  $('.social_link').tooltip({position: 'bottom center'});

      $('.social_link_narrow').tooltip({tipClass: 'tooltip_narrow', position: 'bottom center'});

      $('.social_link_taller').tooltip({tipClass: 'tooltip_taller', position: 'bottom center'});

	  $('div.access-url .viewer_url').disableSelection();
	  //$('div.filelist').disableSelection(); // input text can't be selected when editing filename
      
	  $('div.dm-editor').css('z-index', '10000');
	  
	  fixup_editboxes(this.gallery_.IS_EDITOR, this.expanded_menu_initial );

	  $(this.node_access_url_).find('#upload_more_link').click(function() {
	    // check if the number of items in the gallery is already 50.
		if (this_gallery.getItems().length == Gallery.FILES_LIMIT) {
		  this_gallery.callListeners_({
		    type : Gallery.EVENT_ERRORS,
            errors : [],
	        inner_html: Gallery.ERROR_MAX_LIMIT_REACHED
		  });
		} else {
          $('#upload_more_dialog').show();
          $("#pluploader_more").pluploadQueue({
            // General settings
            runtimes : 'html5, flash, html4', 
            url : '/api/plupload/',
            unique_names : false,
            multipart: true,
			max_file_size: Gallery.MAX_FILE_SIZE_PLUPLOAD,
            flash_swf_url : '/smedia/plupload/js/plupload.flash.swf',
            preinit: function(uploader) {
              uploader.bind('QueueChanged', function(up) {
              FILES_IN_QUEUE = uploader.files.length;
              });
              uploader.bind('FileUploaded', function(up, f, resp) {
                FILES_IN_QUEUE = FILES_IN_QUEUE - 1;
                if(FILES_IN_QUEUE===0) {
				  window.location.href='/api/upload_done'
                }
                else {
                }
              });
              uploader.bind('UploadFile', function(up, f) {				
              });
              uploader.bind('Init', function(up, params) {
              });	
              uploader.bind('PostInit', function(up) {
                $("#pluploader_more").find('.plupload_container').removeAttr('title');
              });
			}
		  });
        }
	  });
	this.checkAjaxCounter(); // check if edit url pencil needs to be hidden
	// check also if share url pencil needs to be hidden
	for (var i = 0, item; item = this.gallery_.items_[i]; i++) {
	  if (!item.id) {
	    $('#share_url_icon_div').hide();
		break;
	  }
	}
	$(this.node_dark_).height($(this.node_).height());
    $(this.node_dark_).show();
  }
}

Editor.prototype.showGalleryInfo_ = function() {  
  this.node_gallery_info_.innerHTML = (this.gallery_.views ? this.gallery_.views : '0 Views') + ' &#8226; Created ' + (this.gallery_.created ? this.gallery_.created : '0 seconds') + ' ago';	
}

function fixup_editboxes(is_editor, expanded_menu_initial ) {
    if (!is_editor){
        if ($('.dm-editor .title').size() == 1){
            $('.title .delete').css('right: 40px');
            $('.dm-editor .title').append("<div class='show_filelist_container'><a href='#show_filelist' class='show_filelist' title='More...'><img src='/smedia/minus/images/minimize.png' /></a></div>");
            $('.dm-editor .title').append("<div class='hide_filelist_container'><a href='#hide_filelist' class='hide_filelist' title='Hide'><img src='/smedia/minus/images/maximize.png' /></a></div>");
        
            $('.dm-editor .title .show_filelist_container .show_filelist').click(function(){
                $(this).parent().siblings('.hide_filelist_container').show();
                $(this).parent().parent().siblings('.body').children('.filelist').show();
                $(this).parent().hide();
				$('.download_link_row').show();	
				$('.dm-editor .access-url').css('padding-top','8');
				save_event('maxim');
                return false;
            });
            $('.dm-editor .title .hide_filelist_container .hide_filelist').click(function(){
                $(this).parent().siblings('.show_filelist_container').show();
                $(this).parent().parent().siblings('.body').children('.filelist').hide();
                $(this).parent().hide();
				//$('.download_link_row').hide();	
				//$('.dm-editor .access-url').css('padding-top','0');
				save_event('minim');
                return false;
            });
        } else{
            $('.dm-editor .body .filelist').before("<div class='show_filelist_container'><a href='#show_filelist' class='show_filelist' title='More...'><img src='/smedia/minus/images/minimize.png' /></a></div>");
            $('.dm-editor .body .filelist').before("<div class='hide_filelist_container'><a href='#hide_filelist' class='hide_filelist' title='Hide'><img src='/smedia/minus/images/maximize.png' /></a></div>");			
            $('.dm-editor .body .show_filelist_container .show_filelist').click(function(){
                $(this).parent().siblings('.hide_filelist_container').show();
                $(this).parent().siblings('.filelist').show();
                $(this).parent().hide();
				$('.download_link_row').show();	
				$('.dm-editor .access-url').css('padding-top','8');
                return false;
            });
            $('.dm-editor .body .hide_filelist_container .hide_filelist').click(function(){
                $(this).parent().siblings('.show_filelist_container').show();
                $(this).parent().siblings('.filelist').hide();
                $(this).parent().hide();
				//$('.download_link_row').hide();								
				//$('.dm-editor .access-url').css('padding-top','0');
                return false;
            });
            //$('.dm-editor .body .hide_filelist_container .hide_filelist').click();
        }
		if(expanded_menu_initial=='True') {		  
		  $('.dm-editor .title .show_filelist_container .show_filelist').click();
		} else {
		  $('.dm-editor .title .hide_filelist_container .hide_filelist').click();
		}
    }
	
	// #79 change back to a links edit/share url view
	/*
    var viewer_url = $('.dm-editor .body .access-url div #viewer_url').html();
    $('.dm-editor .body .access-url div #viewer_url').parent().html("<input spellcheck='false' type='text' value='"+viewer_url+"' id='viewer_url'  />");
    $('.dm-editor .body .access-url div #viewer_url').click(function(){
		$('.dm-editor .body .access-url div #editor_url').blur();
		var editor_url_val = $('.dm-editor .body .access-url div #editor_url').val();
		$('.dm-editor .body .access-url div #editor_url').val('');
		$('.dm-editor .body .access-url div #editor_url').val(editor_url_val);
    });
    var editor_url = $('.dm-editor .body .access-url div a[title="Keep and use this URL to edit gallery."]').html();
    $('.dm-editor .body .access-url div a[title="Keep and use this URL to edit gallery."]').parent()
		.html("<input spellcheck='false' type='text' value='"+editor_url+"' id='editor_url' class='editor_url_box' " +
		"/>");
    var editor_url_textbox = $('.dm-editor .body .access-url div #editor_url');
	var highlight_func = function() {
	  $(this).select();
	  //return false;
	};
	var deselect_func = function() {
	  //$(this).blur();
	};
	editor_url_textbox.click(function(){
    	$('.dm-editor .body .access-url div #viewer_url');
		var viewer_url_val = $('.dm-editor .body .access-url div #viewer_url').val();
		$('.dm-editor .body .access-url div #viewer_url').val('');
		$('.dm-editor .body .access-url div #viewer_url').val(viewer_url_val);
		$(this).select();
		return false;
    });
	editor_url_textbox.hover(highlight_func, deselect_func);
    $('.dm-editor .body .access-url div #editor_url').blur(function(){
        //$(this).css('border', '1px solid #b7b7b7');
    });
    
	var viewer_url_textbox = $('.dm-editor .body .access-url div #viewer_url');
	viewer_url_textbox.click(function() {
		$(this).select();
		return false;
	});
	viewer_url_textbox.hover(highlight_func, deselect_func);
  */
}

Editor.prototype.showErrors_ = function(gallery, errors, inner_html) {
  window.console.log("File error.");
  var error_div = $('.dm-editor-error');
  if (error_div.length > 0) {
    error_div.find('input').before(inner_html);
	error_div.css('margin-top', -error_div.height()/2);
  } else {
    var node = createElement('div', 'dm-editor-error', document.body);
    if (inner_html === undefined || !inner_html) {
      //node.innerHTML += '<h2>Excuse us,</h2>';
      node.innerHTML += '<h2>Please try uploading again.</h2>';
      //node.innerHTML += '<p>but we were unable to upload the following files:</p>';
	  node.innerHTML += '<p>We were unable to upload your files.</p>';
    } else {
      node.innerHTML += inner_html;
    }
  
    /*
    node.innerHTML += '<ul>';
    for (var i = 0, error; error = errors[i]; i++) {
      node.innerHTML += '<li>' + error + '</li>';
    }
    node.innerHTML += '</ul>';
    */
  
    // remove the file list.
    if (errors.length > 0) {
      var files = createElement('div', 'inside', node);
      files.innerHTML += '<ul>';
      for (var i = 0, error; error = errors[i]; i++) {
        files.innerHTML += '<li>' + error + '</li>';
      }
      files.innerHTML += '</ul>';
    }
  
    //var close = createElement('input', '', node);
    //close.type = 'button';
    //close.value = 'Continue';
  
    var close = createElement('input', 'continue_btn');
    close.setAttribute('type', 'button');
    close.setAttribute('value', 'Continue');
  

  	if (BrowserDetect.browser == 'Explorer' && BrowserDetect.version === 7) {
	  var new_div = createElement('div', 'continue_div');
	  new_div.appendChild(close);
	  node.appendChild(new_div);
	} else {
      node.appendChild(close);
	}
	
    node.style.marginTop = (-node.offsetHeight/2) + 'px';
  
    addEventListenerWrapper(close, 'click', function(e) {
      document.body.removeChild(node);
      gallery.redirect = true;
      gallery.save();
    });
	
  }
}

