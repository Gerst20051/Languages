// Presenter ------------------------------------------------------------------
function Presenter(gallery, parent_node) {
	// fixup feedback button
	$('.frby').css('top', $(window).height()/2-60);
	$(window).resize(function(){
		$('.frby').css('top', $(window).height()/2-60);
	});	
	
  this.gallery_ = gallery;
  this.gallery_.addListener(this.galleryListener.bind(this));

  // Contains the state if the share button is clicked or not.
  //if(this.gallery_.IS_EDITOR) {
//	this.gallery_.SHOW_LINKS = true;
  //}
  //else {
	this.gallery_.SHOW_LINKS = false;
  //}

  this.node_ = createElement('div', 'presentation', parent_node);
  this.scene_ = createElement('div', 'scene', this.node_);
  
  // FF didn't like addEventListener, also brain dead.
  if (window.onresize) {
    var temp_func = window.onresize;
    window.onresize = function() {
	  temp_func();
	  this.layout();
	}.bind(this);
  } else {
    window.onresize = this.layout.bind(this);
  }
  
  $(document).keydown(function(e) {
	  var is_special_key = false;
	  switch (e.keyCode) {
		  case 9:
			  is_special_key=true;
			  break;
          case 27:
		      this_presenter.gallery_.esc_key_flag = true;
			  $('.inline_edit:visible').each(function() {
			    $(this).blur();
			  });
			  if (!this_presenter.gallery_.LIGHT_MODE) {
				$('.dm-editor-dark').height($('.dm-editor').height());
	            $('.dm-editor-dark').removeClass('dm-editor-dark-zero');
	          }
			  this_presenter.gallery_.esc_key_flag = false;
		      break;
		  case 34: 
		  case 39:
		  case 40:
		    if (!$(e.target).is(':input')) {
			  is_special_key=true;
			  gallery.selectNext();
			  break;
			}
		  case 33:
		  case 37:
		  case 38:
		    if (!$(e.target).is(':input')) {
		      is_special_key=true;
			  gallery.selectPrev();
			}
			break;
	  };
	  if(is_special_key && gallery) {
		save_event('keypress', gallery.id);		
		// return false;
	  }
    }
  );
  
  this.item_nodes_ = [];
  
  if (this.gallery_.IS_EDITOR) {
    var this_presenter = this;
    $(function() {
	  $(document).click(function(e) {
		this_presenter.gallery_.inline_edit_flag = true;
		if ($(e.target).attr('class') != 'inline_edit_btn') {
		  $('.inline_edit:visible').each(function() {
            if (!$(this).is(':focus')) {
              $(this).blur();
            }
          });
        } else {
          var inline_id = $(e.target).attr('inline_input_id');
          if (inline_id) {
            $('.inline_edit:visible').each(function() {
              if ($(this).attr('id') != inline_id) {
                $(this).blur();
              }
            });
          }
        }
		if (!this_presenter.gallery_.LIGHT_MODE) {
		  //var visible_edit = $('.dm-editor').find('textarea:visible').length + $('.dm-editor').find('input[type=text]:visible').length;
		  //if (visible_edit > 0) {
		  var visible_edit = Editor.visibleEdit();
		  if (visible_edit) {
			$('.dm-editor-dark').height($(this.node_).height());
			$('.dm-editor-dark').addClass('dm-editor-dark-zero');
          }
		}
        this_presenter.gallery_.inline_edit_flag = false;
      });
      $('.inline_edit').click(function(e) {
        e.stopPropagation();
        return false;
      });
    });
  }
}

Presenter.SPACING = 270;
Presenter.SELECTED_OVERLAP = 20;
Presenter.SMALL_WIDTH = 240;
Presenter.SMALL_HEIGHT = 240;
//Presenter.MIN_EDGE_SPACING = 75;
//Presenter.MIN_EDGE_SPACING = 110;
//Presenter.MIN_EDGE_SPACING = 125;
//Presenter.MIN_EDGE_SPACING = 145;
Presenter.MIN_EDGE_SPACING = 135;
Presenter.SMALL_IMAGE_SPACE = 125;

/*Presenter.ARROW_SPACE = 400;*/

Presenter.ARROW_SPACE = 55;
Presenter.GAL_MENU_WIDTH = 240;

// Width of the share links div
Presenter.SHARE_LINKS_WIDTH = 520;
// Width of the share buttons div
Presenter.SHARE_BUTTONS_WIDTH = 300;
//Presenter.PICTURE_INFO_WIDTH = 375;
Presenter.PICTURE_INFO_WIDTH = 500;
Presenter.UPLOAD_STATUS_WIDTH = 300;

// preload ajax loader gif
Presenter.PROGRESS_BAR = $('<img/>', {'src': '/smedia/minus/images/ajax-loader-bar.gif'});
Presenter.PROGRESS_BAR_SMALL = $('<img/>', {'src': '/smedia/minus/images/ajax-loader-bar-small.gif'});
Presenter.LOADER_ICON = $('<img/>', {'src': '/smedia/minus/images/ajax-loader-mellow-big.gif'});
Presenter.ARROW_ICON = $('<img/>', {'src': '/smedia/minus/images/arrows.png'});

Presenter.scale = function(node, scale) {
  //if (BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 8) {
  if ((BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 9) || BrowserDetect.browser == "Opera") {
    // Don't resize the thumbnail again
    if (node.natural_width && node.className !== 'thumb') {
      // window.console.log('Presenter.scale. Scale: '+scale+". Full_width: "+node.natural_width);
      node.width = parseInt(node.natural_width * scale);
      node.height = parseInt(node.natural_height * scale);
	  
      node.style.left = (-node.width / 2) + 'px'
      node.style.top = (-node.height / 2) + 'px';
    }
    return;
  }
  
  //window.console.log('Presenter.scale. Scale: '+scale+"Full_width: "+node.width);
  node.style.webkitTransform = 'scale('+scale+')';
  node.style.MozTransform = 'scale('+scale+')';
}

Presenter.opacity = function(node, opacity) {
  //if (BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 8) {
  if (BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 9) {
    // haxhaxhax
    var blurString = 'alpha(opacity=' + parseInt(opacity * 100) + ')';
    if (node.className == 'thumb') {
      //node.style.filter = blurString + ' progid:DXImageTransform.Microsoft.Blur(pixelradius=7)';
	  node.style.filter = blurString + ' progid:DXImageTransform.Microsoft.Blur(pixelradius=1)';
    } else {
      node.style.filter = blurString;
    }
    return;
  }
  node.style.opacity = opacity;
  node.style.pointerEvents = (opacity == 0) ? 'none' : 'auto';
}

Presenter.htmlCode = function(href, img_src) {
  return '<a target="_blank" href="' + href + '"><img src="' + img_src + '" border="0"/></a>'
}

// Create share links for the image
Presenter.createShareLinks = function(ajax_loader, item, gallery, item_nodes) {
  var valid_image = Gallery.isValidImage(item);
  var ext = extension(item.name) + '';
  if (valid_image && streql_in(ext, 'bin')) {
    valid_image = false;
  }
  var item_number = gallery.findItemNumber(item);
  var divider = '&nbsp; · &nbsp;';

  var full_reader_url = '';
  if (gallery.getItems().length > 1) {
    full_reader_url = window.location.protocol + '//' + 'min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url + '#' + item_number;
  } else {
    full_reader_url = window.location.protocol + '//' + 'min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url;
  }
  
  var image_url = '';
  var direct_image_url = '';
  var image_code_url = '';
  var image_small_code_url = '';
  
  if (valid_image) {
    // #186
    //image_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_ITEM + item.id + '.' + extension(item.name);
	image_url = item.getImageViewLink(); // image view page
	direct_image_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_ITEM + item.id + '.' + extension(item.name);
	image_code_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_THUMB + item.id + '.' + extension(item.name);
	image_small_code_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_SMALL + item.id + '.jpg';
  } else {
    image_url = window.location.protocol + '//min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url + '#' + item_number;
	image_code_url = window.location.protocol + '//' + 'min.us' + item.getIcon();
  }
  
  //var forum_code = '[URL=' + full_reader_url + '][IMG]' + image_code_url + '[/IMG][/URL]';
  //var html_code = Presenter.htmlCode(full_reader_url, image_code_url);
  var forum_code = '[URL=' + full_reader_url + '][IMG]' + image_small_code_url + '[/IMG][/URL]';
  var html_code = Presenter.htmlCode(full_reader_url, image_small_code_url);
  
  // When a text box is clicked, highlight its content.
  var highlight_func = function(e) {
	$(this).select();
	e.preventDefault();	
	e.stopImmediatePropagation();
	return false;
  };
  
  var deselect_func = function() {
	//$(this).blur();
  };
  
//  var input_style = {'class': 'viewer_links', 'spellcheck': false, 'style': 'display: block; float: left; width: 172px; margin-top: 5px; margin-bottom: 0px;padding:0 2px 0px 2px;'};
//  var label_style = {'style': 'display: block; float: left; margin-top: 5px; margin-bottom: 0px; width: 75px; padding: 2px 5px 0px 0px; text-align:right;'};
  
  var input_style = {'class': 'viewer_links', 'spellcheck': false, 'style': 'display: block; float: left; width: 172px; margin-top: 3px; margin-bottom: 3px;padding:0 2px 0px 2px;'};
  var label_style = {'style': 'display: block; float: left; margin-top: 3px; margin-bottom: 3px; width: 75px; padding: 2px 5px 0px 0px; text-align:right;'};
  
  // share button
  var info_button = $('<span>', {'class': 'info_button'}).text('Info');
  //var share_button = $('<button/>', {'class': 'share_button'}).html('<span class="share_button_content">Share</span>');
  var share_button = $('<span>', {'class': 'share_button'}).text('Share');

  var link_container_margin_left = (item.thumb.width/2)-((item.thumb.width/2)-(Presenter.SHARE_BUTTONS_WIDTH/2))-260;
  var links_container_display = 'none';
  if(gallery.SHOW_LINKS) {
    links_container_display = 'block';
  }
  
  var links_container_inside = $('<div/>', {'class': 'links_container_inside'});
  //var links_container = $('<div/>', {'class': 'links_container', 'style': 'display:' + links_container_display + '; width:520px; margin-left: ' + link_container_margin_left + 'px; padding:4px 12px 0 5px;'});
  var links_container = $('<div/>', {'class': 'links_container', 'style': 'display:' + links_container_display + '; width:540px; margin-left: ' + link_container_margin_left + 'px;'});
  
  if (valid_image) {
    var gallery_link_textbox = $('<input/>', input_style).val(full_reader_url).attr('name', 'gallery_link');
    var image_link_textbox = $('<input/>', input_style).val(image_url).attr('name', 'image_link');
	//var image_link_textbox = $('<input/>', input_style).val(direct_image_url).attr('name', 'image_link');
    var forum_code_textbox = $('<input/>', input_style).val(forum_code).attr('name', 'forum_code');
    var html_code_textbox = $('<input/>', input_style).val(html_code).attr('name', 'html_code');
	var direct_link_textbox = $('<input/>', input_style).val(direct_image_url).attr('name', 'direct_link');
	direct_link_textbox
	  .addClass('direct_link_box')
	  .css('margin-top', '5px');
	
    gallery_link_textbox.click(highlight_func);
    gallery_link_textbox.hover(highlight_func, deselect_func);
    image_link_textbox.click(highlight_func);
    image_link_textbox.hover(highlight_func, deselect_func);
    forum_code_textbox.click(highlight_func);
    forum_code_textbox.hover(highlight_func, deselect_func);
    html_code_textbox.click(highlight_func);
    html_code_textbox.hover(highlight_func, deselect_func);
	direct_link_textbox.click(highlight_func);
	direct_link_textbox.hover(highlight_func, deselect_func);
  
    var html_gallery_link = $('<div/>', label_style).html('Gallery Link');
    var html_image_link = $('<div/>', label_style).html('Image Link');
    var html_forum_code = $('<div/>', label_style).html('Forum Code');
    var html_html_code = $('<div/>', label_style).html('HTML Code');
	var html_direct_link = $('<div/>', label_style).html('Direct Link').addClass('direct_link_label').css('margin-top', '5px');
	
    //links_container
	links_container_inside
      .append($('<div/>', {'style': 'float: left;'}).append(html_gallery_link).append(gallery_link_textbox))
      .append($('<div/>', {'style': 'float: right;'}).append(html_image_link).append(image_link_textbox))
      .append($('<div/>', {'style': 'float: left;'}).append(html_forum_code).append(forum_code_textbox))
      .append($('<div/>', {'style': 'float: right;'}).append(html_html_code).append(html_code_textbox))
	  .append($('<div/>', {'style': 'float: left;'}).append(html_direct_link).append(direct_link_textbox));
  } else {
    var html_link = $('<div/>', label_style).html('Link');
	var link_textbox = $('<input/>', input_style).val(full_reader_url).attr('name', 'link');
    html_link.css('width', '35px');
	link_textbox.click(highlight_func);
	link_textbox.hover(highlight_func, deselect_func);
	
    //links_container
	links_container_inside
	  .append($('<div/>', {'style': 'float: left;'}).append(html_link).append(link_textbox))
	  .css('height', '24px');
	  
	links_container
	  .css('width', '240px');
  }
	
  var html_links = $('<div/>')
    .addClass('share_div')
    .css('width', Presenter.SHARE_BUTTONS_WIDTH)
	.css('margin-left', (item.thumb.width/2)-(Presenter.SHARE_BUTTONS_WIDTH/2))
	.append(info_button)
	.append(divider)
    .append(share_button)
	.append(divider);

  if (!valid_image) {
    //var download_href = 'http://k.min.us/i' + item.id + '.' + extension(item.name);
	var download_href = item.getLink();
	var download_button = $('<span>', {'class': 'download_button', 'href': download_href}).text('Download');
	html_links.append(download_button);
  } else {
    //var view_href = 'http://i.min.us/i' + item.id + '.' + extension(item.name);
	//var view_href = item.getLink();
	var view_href = '/l' + item.id;
	var view_button = $('<span>', {'class': 'view_button', 'href': view_href}).text('View');
	html_links.append(view_button);
  }
  
  // onclick handler for share
  html_links.find('.download_button').click(function() {
    if (BrowserDetect.browser == 'Explorer') {
	  var download_link = $('<a>', {'href': download_href, 'target': '_blank', 'text': 'temp', 'style': 'position:absolute; display:block'});
	  download_link[0].click();
	} else {
	  window.open($(this).attr('href'));
	}
	return false;
  });
  html_links.find('.view_button').click(function() {
	window.open($(this).attr('href'));
	return false;
  });
  
  links_container
    .append(links_container_inside);
  html_links
	.append(ajax_loader)
	.append(links_container);

  return html_links;
}

Presenter.createCaption = function(item, caption_div, caption_edit_div) {
  caption_div.html('');
  if (caption_edit_div !== undefined) {
    caption_edit_div.html('');
  }
  
  var pencil_div = $('<div>', {'class': 'edit'});
  var pencil_img = $('<img>', {'class': 'edit inline_edit_btn pencil_light_mode', 'inline_input_id': 'caption-' + item.id}).attr('src', '/smedia/minus/lightpencil.png');
  var pencil_img_dmode = $('<img>', {'class': 'edit inline_edit_btn pencil_dark_mode', 'inline_input_id': 'caption-' + item.id}).attr('src', '/smedia/minus/lightpencil-darkmode.png');
  var caption_span_text;
  var caption_span_add = $('<span>', {'text': Item.DEFAULT_CAPTION_TEXT, 'class': 'caption_add inline_edit_btn', 'inline_input_id': 'caption-' + item.id});
  var text_box_width = $(caption_div).outerWidth();
  
  var display_edit_div_func = function() {
    $(caption_div).hide();
    $(caption_edit_div).show();
    $(caption_text_box).focus();
	item.gallery_.callListeners_({
	  type: Gallery.EVENT_ITEM_CAPTION_UPDATE
	});
  };
  
  if  (item.caption !== null && item.caption.length > 0) {
//    caption_span_text =  $('<span>', {'text': item.caption, 'class': 'caption_text'});
    caption_span_text =  $('<span>', {'class': 'caption_text'}).html(item.caption); // this will come from backend so it's already encoded
	caption_span_add.hide();
  } else {
	caption_span_text =  $('<span>', {'class': 'caption_text'});
  }
  
  // hide if it is still loading
  if (item.id === null) {
    caption_span_add.hide();
	//pencil_img.hide();
	//pencil_img_dmode.hide();
	pencil_div.hide();
  }
  
  if (caption_edit_div === undefined) {
    caption_div.append(caption_span_text);
	return;
  }
  
  if (caption_div.prev().attr('class') == 'edit') {
    caption_div.prev().remove();
  }
  
  pencil_div.append(pencil_img);
  pencil_div.append(pencil_img_dmode);
  caption_div.before(pencil_div);

  caption_span_add.click(display_edit_div_func);
  $(pencil_img).click(display_edit_div_func);
  $(pencil_img_dmode).click(display_edit_div_func);
  
  caption_div
  	.append(caption_span_text)
	.append(caption_span_add);

  // decode the encoded caption
  var caption_decoded = htmlDecode(item.caption);
  //var caption_text_box = $('<textarea>', {'class': 'caption_text_box inline_edit', 'id': 'caption-' + item.id}).val(item.caption);
  var caption_text_box = $('<textarea>', {'class': 'caption_text_box inline_edit', 'id': 'caption-' + item.id}).val(caption_decoded);
  var caption_loading = $('<div>', {'class': 'caption_loading', 'style': 'display:none;'});
  var caption_loading_img = $('<img>', {'src': Gallery.URL_AJAX_LOADER_SMALL});
  
  caption_loading.append(caption_loading_img);
  
  var item_func = function() {
    item.gallery_.callListeners_({
	  type: Gallery.EVENT_ITEM_CAPTION_UPDATE
	})
  };
 
  caption_text_box
    .css('width', '300px')
	.css('font-size', '14px')
	.css('color', '#555')
	.css('font-family', 'helvetica,arial,sans-serif')
	.css('line-height', '16px')
    .autogrow({callback: item_func});
  
  caption_edit_div
    .append(caption_text_box)
	.append(caption_loading);
  
  $(caption_text_box).blur(function() {
    if (item.gallery_.inline_edit_flag) {
      var value = $(this).val().replace(/\n/g, ' ');
	  $(this).val(value).change();
      item.updateCaption(caption_div, caption_edit_div, caption_text_box);
	} else if (item.gallery_.esc_key_flag) {
	  $(this).val(item.caption).change();
	  $(caption_edit_div).hide();
	  $(caption_div).show();
	  valid_input($(this));
	}
  });
  
  $(caption_text_box).keypress(function(e) {
    if (e.which == '13') {
	  if ($(e.target).is(':visible')) { // Opera fix
	    if (!$(this).hasClass('error')) {
	      var value = $(this).val().replace(/\n/g, ' ');
	      $(this).val(value);
	      item.updateCaption(caption_div, caption_edit_div, caption_text_box);
		}
		e.preventDefault();
	    e.stopPropagation();
	    return false;
	  }
	}
  });
  $(caption_text_box).noNewLines();
}

Presenter.createItemInfo = function(item) {
  var item_info_div = $('<div>', {'class': 'item_info'}).html('<div class="item_info_inside"><span class="dimensions"></span></div>');
  var item_info_text = item.getTextInfo();
  item_info_div
	.css('width', Presenter.PICTURE_INFO_WIDTH + 'px')
	.css('margin-left', (item.thumb.width-120/2)-(Presenter.PICTURE_INFO_WIDTH/2));
  $(item_info_div)
    .find('.dimensions')
	.html(item_info_text);
  if (!item.gallery_.SHOW_INFO) {
    $(item_info_div).hide();
  }
  return item_info_div;
};

Presenter.prototype.galleryListener = function(e) {
  if (e.type == Gallery.EVENT_ALLITEMSLOADED) {
    window.console.log("all items loaded");
    $('div.dm-editor-status').html('').hide();
	this.showHideItemArrows_(false);
    //this.orderNodes_();
	this.orderNodes2_();
	this.setItemFromHash(true);
	this.resetHashLink_();
	this.gallery_.setImageSRC();
  } else if (e.type == Gallery.EVENT_SELECTION) {
    this.showHideItemArrows_();
    this.layout();
  } else if (e.type == Gallery.EVENT_REORDER) {
    //this.orderNodes_(); 
	this.orderNodes2_(); 
	this.resetHashLink_();
	this.showHideItemArrows_();
	this.gallery_.setImageSRC();
  } else if (e.type == Gallery.EVENT_DELETION) {
    //this.orderNodes_();
	this.orderNodes2_();
	this.resetHashLink_();
	this.showHideItemArrows_(false);
	this.gallery_.setImageSRC();
  } else if (e.type == Gallery.EVENT_ITEMLOADED) {
    // Experimental.
    //this.orderNodes_();
	this.orderNodes2_();
	this.showHideItemArrows_(false);
	//this.showHideArrows_();
  } else if (e.type == Gallery.EVENT_DELETED) {
    this.scene_.style.top = 50 + 'px';
    this.scene_.style.opacity = 0;
  } else if (e.type == Gallery.EVENT_ITEM_RENAME) {
	this.updatePictureInfo_(e.item);
  } else if (e.type == Gallery.EVENT_URL_UPDATE) {
    this.resetHashLink_();
  } else if (e.type == Gallery.EVENT_ITEM_CAPTION_UPDATE) {
    this.layout(true);
  } else if (e.type == Gallery.EVENT_RESIZED_IMAGE_LOADED) {
    this.hideLoadingIcon_(e.data);
	this.layout();
  } else if (e.type == Gallery.EVENT_ITEM_UPLOADED) {
    //this.createViewer_(e.data);
  } else if (e.type == Gallery.EVENT_VIDEO_LOADED || e.type == Gallery.EVENT_ITEM_VIEWER_LOADED) {
    //this.layout(true);
	this.layout();
  }
  /*
  else if (e.type == Gallery.EVENT_ITEM_DELETE) {
    this.showHideItemArrows_(false);
  }
  */
}

Presenter.prototype.hideLoadingIcon_ = function(item) {
  var item_node = this.getNodeForItem_(item);
  $(item_node.loading_icon).hide();
  //item.generateThumb_();
  Item.blurImage(item.image, 7, 240, 240, item_node.thumb);
  
  $(item_node.thumb).show();
  
  item_node.image.natural_width = item.image.width;
  item_node.image.natural_height = item.image.height;
  item_node.image.style.top = (-parseInt(item.image.height / 2)) + 'px';
  item_node.image.style.left = (-parseInt(item.image.width / 2)) + 'px';
  Presenter.scale(item_node.image, 0.1);
  Presenter.opacity(item_node.image, 0);
  $(item_node.image).show();
}

Presenter.prototype.updatePictureInfo_ = function(item) {
  var text = item.getTextInfo();
  var item_node = this.getNodeForItem_(item);
  $(item_node)
    .find('.item_info')
	.find('.dimensions')
    .html(text);
}

Presenter.prototype.resetHashLink_ = function() {
  var gallery = this.gallery_;
  var selected_item = this.gallery_.getSelected();

  for (var i = 0, item_node; item_node = this.item_nodes_[i]; i++) {
    if (item_node.item == selected_item) {
	  window.location.hash = '#' + (i+1);
	}
    var valid_image = Gallery.isValidImage(item_node.item);
    var item_number = gallery.findItemNumber(item_node.item);
    var image_link = $(item_node).find('input[name="image_link"]');
	var gallery_link = $(item_node).find('input[name="gallery_link"]');
	var forum_code = $(item_node).find('input[name="forum_code"]');
	var html_code = $(item_node).find('input[name="html_code"]');
	var file_link = $(item_node).find('input[name="link"]');
	
	var full_reader_url = '';
    if (gallery.getItems().length > 1) {
      full_reader_url = window.location.protocol + '//' + 'min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url + '#' + item_number;
    } else {
      full_reader_url = window.location.protocol + '//' + 'min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url;
    }
	
	var image_url = '';
    if (valid_image) {
	  // #186
      //image_url = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_ITEM + item_node.item.id + '.' + extension(item_node.item.name);
	  image_url = item_node.item.getImageViewLink();
	  //var img_url_j = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_THUMB + item_node.item.id + '.' + extension(item_node.item.name);
	  var img_url_k = window.location.protocol + '//' + 'i.min.us' + Gallery.URL_SMALL + item_node.item.id + '.jpg';
	  forum_code.val('[URL=' + full_reader_url + '][IMG]' + img_url_k + '[/IMG][/URL]');
	  html_code.val(Presenter.htmlCode(full_reader_url, img_url_k));
    } else {
      image_url = window.location.protocol + '//min.us' + Gallery.URL_VIEWER_BASE + gallery.reader_url + '#' + item_number;
    }
	
	image_link.val(image_url);
	gallery_link.val(full_reader_url);
	file_link.val(full_reader_url);
  }
}

// Show/Hide Arrows depending on the selected image
Presenter.prototype.showHideArrows_ = function() {
  var left = $(this.left_arrow_);
  var right = $(this.right_arrow_);
  var items_number = this.gallery_.getItems().length;
  if (items_number <= 1) {
    //this.left_arrow_.style = 'display:none;';
	//this.right_arrow_.style = 'display:none;';
	left.hide();
	right.hide();
	return;
  }
  
  var selected = this.gallery_.getSelected();
  var number = parseInt(this.gallery_.findItemNumber(selected));
  
  if (!isNaN(number)) {
    if (number === 1) {
	  //this.left_arrow_.style = 'display:none;';
	  left.hide();
	  right.show();
	} else if (number === items_number) {
	  left.show();
	  right.hide();
	} else {
	  right.show();
	  left.show();
	}
  } else {
    left.hide();
	right.hide();
  }
}

// Show/Hide Current item arrows
Presenter.prototype.showHideItemArrows_ = function(with_effects) {
  var items_number = this.gallery_.getItems().length;
  if (with_effects === undefined) {
    with_effects = true;
  }
  if (BrowserDetect.browser == "Explorer") { // no fadeIn effect for IE
    with_effects = false;
  }
  var selected = this.gallery_.getSelected();
  for (var i = 0, item_node; item_node = this.item_nodes_[i]; i++) {
    var item = item_node.item;
	if (item == selected && items_number > 1) {
	    $(item_node.left_arrow).show();
		$(item_node.right_arrow).show();
		if (!$(item_node.picture_info).find('.caption_edit').is(':visible')) {
		  $(item_node.picture_info).find('.caption').show();
		} else {
		  $(item_node.picture_info).find('.caption_edit').show();
		}
		if (item.id) {
		  // TODO edit
		  //$(item_node.picture_info).find('.edit').show();
		  $(item_node.picture_info).find('div.edit').show();
		}
	  break;
	} else if (item == selected && items_number == 1) {
	  if (!$(item_node.picture_info).find('.caption_edit').is(':visible')) {
	    $(item_node.picture_info).find('.caption').show();
      }
	  if (item.id) {
	    // TODO edit
	    //$(item_node.picture_info).find('.edit').show();
		$(item_node.picture_info).find('div.edit').show();
	  }
	  //$(item_node.link).find('.item_info .dimensions').show(); // #185
	  $(item_node.left_arrow).hide();
      $(item_node.right_arrow).hide();
	  break;
	}
  }
}

Presenter.prototype.createNode_ = function(item) {
  window.console.log("Presenter.prototype.createNode_. Name: "+ item.name + ". ID: "+item.id);
  // This probably causes layout.
  var cx = this.node_.offsetWidth / 2;
  var cy = this.node_.offsetHeight / 2 - Presenter.MIN_EDGE_SPACING / 2 ;
  
  var item_node = createElement('div', 'itemnode');
  var valid_image = Gallery.isValidImage(item);
  var item_name_trunc = item.name;
  var items_number = this.gallery_.getItems().length;
  
  if (item.name.length > 20) {
    item_name_trunc = item.name.substr(0, 17) + '...';
  }
  
  item_node.item = item;
  item_node.style.top = cy + 'px';
  item_node.style.left = cx + 'px';
  item_node.id = item.id;
  item_node.image = item.image;
  item_node.image.className = item.image_class;
  //if (valid_image) {
  //  item_node.image.className = 'image';
  //} else {
  //  item_node.image.className = 'file_icon';
  //}

  item_node.appendChild(item_node.image);
  item_node.image.natural_width = item.image.width;
  item_node.image.natural_height = item.image.height;
  item_node.image.style.top = (-parseInt(item.image.height / 2)) + 'px';
  item_node.image.style.left = (-parseInt(item.image.width / 2)) + 'px';
  Presenter.scale(item_node.image, 0.1);
  Presenter.opacity(item_node.image, 0);
  
  $(item_node.image).hide();
  
  item_node.thumb = item.thumb;
  item_node.thumb.className = 'thumb';
  item_node.appendChild(item_node.thumb);
  item_node.thumb.natural_width = item.thumb.width;
  item_node.thumb.natural_height = item.thumb.height;
  item_node.thumb.width = item.thumb.width;
  item_node.thumb.height = item.thumb.height;
  item_node.thumb.style.top = -parseInt(item.thumb.height / 2) + 'px';
  item_node.thumb.style.left = -parseInt(item.thumb.width / 2) + 'px';
  
  $(item_node.thumb).hide();
  
  item_node.loading_icon = item.loading_icon;
  item_node.loading_icon.className = 'loading_icon';
  item_node.appendChild(item_node.loading_icon);
  
  item_node.viewer = item.viewer;
  if (item_node.viewer !== null) {
    item_node.appendChild(item_node.viewer);
    $(item_node.viewer).hide();
  }

  // this is a problem! don't know why.
  // $(item_node).attr('itemid',item.id).attr('filename',item.name);
  // window.console.log('Picture height:' + item.image.width);

  // links/codes
  // #142
  //var ajax_loader = '<div class="picture_detail"><div class="direct_image_link" style="width: ' + Presenter.UPLOAD_STATUS_WIDTH + 'px; margin-left: ' + ((item.thumb.width/2)-(Presenter.UPLOAD_STATUS_WIDTH/2)) + 'px;"><img src="/smedia/minus/images/ajax-loader-bar.gif"/></div></div>';
  var ajax_loader = '<div class="picture_detail"><div class="direct_image_link"><img src="/smedia/minus/images/ajax-loader-bar.gif"/></div></div>';
  var html_links = Presenter.createShareLinks(ajax_loader, item, this.gallery_);
  
  item_node.left_arrow = createElement('div', 'node_left_arrow', item_node);
  item_node.right_arrow = createElement('div', 'node_right_arrow', item_node);
  item_node.picture_info = createElement('div', 'picture_info', item_node);
  item_node.link = createElement('div', 'picture_link', item_node);
  
  $(item_node.left_arrow)
    .html('<span class="left">&nbsp;</span>')
	.hide();
  $(item_node.right_arrow)
    .html('<span class="right">&nbsp;</span>')
	.hide();
	
  var selected = this.gallery_.getSelected();
  if (selected == item && item.id !== null && items_number > 1) {
    $(item_node.left_arrow).show();
    $(item_node.right_arrow).show();
  }
	
  $(item_node.left_arrow).find('span').click(this.gallery_.selectPrev.bind(this.gallery_));
  $(item_node.right_arrow).find('span').click(this.gallery_.selectNext.bind(this.gallery_));
  $(item_node.picture_info)
    .css('position', 'absolute')
	.css('margin-top',-cy)
	.css('margin-left',-item.thumb.width-120/2)
	.css('height','28px')
	.css('width',item.image.width)
    .css('color','#888');
	
  var caption_div = $('<div>', {'class': 'caption'}).html('&nbsp;');
  var caption_edit_div = $('<div/>', {'class': 'caption_edit'});

  var picture_info_inside = $('<div>', {'class': 'item_info'}).html('<div class="item_info_inside"><span class="dimensions"></span></div>');
  caption_edit_div.hide();
  $(item_node.picture_info)
	.append(caption_div)
	.append(caption_edit_div);
	
  if (this.gallery_.IS_EDITOR) {
    Presenter.createCaption(item, caption_div, caption_edit_div);
  } else {
    if (item.caption !== null && item.caption.length > 0) {
	  Presenter.createCaption(item, caption_div);
	}
  }

  // this one will have the fixed width
  picture_info_inside
	.css('width', Presenter.PICTURE_INFO_WIDTH + 'px')
	.css('margin-left', (item.thumb.width-120/2)-(Presenter.PICTURE_INFO_WIDTH/2));
  
  $(item_node.link)
    .css('margin-left',-item.thumb.width-120/2)
	.css('text-align','center')
    //.css('margin-top',cy)
	.css('margin-top', (item.image.height / 2))
    .css('width',item.image.width)
    .css('height','30px')
    .css('color','#888')
	.attr('filename',item.name)
	.css('position', 'absolute')
	.html(ajax_loader);
	
  $(item_node.link).find('.picture_detail').attr('filename',item.name);

  $(item_node.picture_info).attr('filename',item.name);
  if(item.id) {
    $(item_node.link).show();
	
	var item_info_text = item.getTextInfo();
	$(picture_info_inside)
	  .find('.dimensions')
	  .html(item_info_text);

	//$(item_node.link).empty().append(picture_info_inside).append(html_links); // #78
	$(item_node.link).empty().append(html_links).append(picture_info_inside); // #185

	if (this.gallery_.SHOW_INFO) {
	  $(picture_info_inside).show();
	} else {
	  $(picture_info_inside).hide();
	}
	
	var item_links_container = $(item_node).find('.links_container');
	var links_container_width = item_links_container.outerWidth();
		
	var share_div_margin_left = parseFloat(html_links.css('margin-left'));
	var details_margin_left = (item.thumb.width-120/2)-share_div_margin_left-(links_container_width/2);		
    item_links_container.css('margin-left', details_margin_left);
		
	if(this.gallery_.SHOW_LINKS) {
	  item_links_container.show();
	}
	$(item_node.link).find('.picture_detail').hide();
		
	if (item != selected) {
	  $(item_node).find('.picture_info').children().hide();
	}
  }

  Presenter.scale(item_node.thumb, 0.1);
  Presenter.opacity(item_node.thumb, 0);

  var item_node_share_button = $(item_node.link).find('.share_button');
  var item_node_info_button = $(item_node.link).find('.info_button');
  var gallery = this.gallery_;
  var show_links_func = function() {
    var links = $(item_node).find('.links_container');
	var info = $(item_node).find('.item_info');
	info.hide();
	gallery.SHOW_INFO = false;
    if (links.is(':hidden')) {
	  gallery.SHOW_LINKS = true;
      links.slideDown(100);
    } else {
	  gallery.SHOW_LINKS = false;
      links.slideUp(100);
    }
  };
  
  var show_info_func = function() {
    var links = $(item_node).find('.links_container');
    var info = $(picture_info_inside);
	links.hide();
	gallery.SHOW_LINKS = false;
    if (info.is(':hidden')) {
	  gallery.SHOW_INFO = true;
      info.slideDown(100);
	} else {
	  gallery.SHOW_INFO = false;
	  info.slideUp(100);
	}
  };
  
  item_node_share_button.bind('click', show_links_func);
  item_node_info_button.bind('click', show_info_func);

  // chrome is having conflicts when share button is clicked; restricted click event to image and thumb
  //addEventListenerWrapper(item_node, 'click', this.imageClicked_.bind(this, item));
  addEventListenerWrapper(item_node.image, 'click', this.imageClicked_.bind(this, item));
  addEventListenerWrapper(item_node.thumb, 'click', this.imageClicked_.bind(this, item));
  addEventListenerWrapper(item_node.loading_icon, 'click', this.imageClicked_.bind(this, item));
  
  //window.console.log("Presenter.prototype.createNode_. end of call");
  return item_node;
}

/**
 * Creates a node if it doesn't already exist.
 */
Presenter.prototype.getNodeForItem_ = function(item) {
  for (var u = 0, item_node; item_node = this.item_nodes_[u]; u++) {
    if (item_node.item == item) {
      return item_node;
    }
  }
  window.console.log('Presenter.prototype.getNodeForItem_');
  return this.createNode_(item);
}

// updated version of orderNodes_
// add item nodes without removing them all and adding them back again
Presenter.prototype.orderNodes2_ = function() {
  var items = this.gallery_.getItems();
  if (!items) { return }

  // If there has been a deletion.
  for (var i = 0, node; node = this.item_nodes_[i]; i++) {
    node.processed = false;
  }
  
  var prev = null;
  var new_item_nodes = [];
  for (var i = 0, item; item = items[i]; i++) {
    if (!item.loaded) {
	  prev = null;
	  continue;
	}
	
	// find if the node is not yet added to this.scene_
	var item_node = this.getNodeForItem_(item);
	if (item.id !== null && item_node.id == '') {
	  item_node.id = item.id;
	  //$(item_node).attr('id', item.id);
	}
	
	if ($(this.scene_).find('#' + item.id).length == 0 || item.id == null) {
	  // Add it
	  if (prev == null) {
	    $(this.scene_).append(item_node);
	  } else {
	    $(this.scene_).find('#' + prev).after(item_node);
	  }
	  if (item_node.viewer !== null) {
		item.viewerAdded_();
      }
	}
	item_node.processed = true;
	new_item_nodes.push(item_node);
	prev = item.id;
  }
  
  // Find untouched nodes-they must have been deleted.
  if (new_item_nodes.length != this.item_nodes_.length) {
    for (var i = 0, node; node = this.item_nodes_[i]; i++) {
      if (node.processed == false) {
        this.scene_.removeChild(node);
        delete node;
      }
    }
  }
  
  this.item_nodes_ = new_item_nodes;
  this.layout();
}

Presenter.prototype.orderNodes_ = function() {
  window.console.log('Presenter.prototype.orderNodes_');
  var items = this.gallery_.getItems();
  if (!items) { return }

  // If there has been a deletion.
  for (var i = 0, node; node = this.item_nodes_[i]; i++) {
    node.processed = false;
  }
  var new_item_nodes = [];
  for (var i = 0, item; item = items[i]; i++) {
    if (item.loaded) continue;
    // Find or create our node.
    var item_node = this.getNodeForItem_(item);
    item_node.processed = true;
	this.scene_.appendChild(item_node);
    new_item_nodes.push(item_node);
  }

  // Find untouched nodes-they must have been deleted.
  if (new_item_nodes.length != this.item_nodes_.length) {
    for (var i = 0, node; node = this.item_nodes_[i]; i++) {
      if (node.processed == false) {
        this.scene_.removeChild(node);
        delete node;
      }
    }
  }
  //for (var i = 0, node; node = this.item_nodes_[i]; i++) {
     //log('Presenter.prototype.orderNodes_: ' + $(node).find('span.direct_image_link').attr('filename'));
  //}
  this.item_nodes_ = new_item_nodes;
  this.layout();
}

// if there is a # from the url get its value and set it as the selected item
Presenter.prototype.setItemFromHash = function(call_listeners) {
  var hash = window.location.hash;
  var hash_num = parseInt(hash.substr(1));
  
  if (call_listeners === undefined) {
    call_listeners = true;
  }
  
  if (!isNaN(hash_num)) {
    if (this.item_nodes_.length >= hash_num) {
      var item = this.item_nodes_[hash_num-1].item;
	  this.gallery_.setSelected(item, call_listeners);
    } else {
      // last image?
    }
  }
}

Presenter.prototype.updateViewer_ = function(item, item_node) {
  if (item.viewer == null) {
    // generate
	item.generateViewer_();
	if (item.viewer !== null) {
	   item_node.viewer = item.viewer;
	   item_node.appendChild(item_node.viewer);
	   $(item_node.viewer).hide();
	   if (item.viewer_type == 'audio_flash') {
	     item.getFlashMusicPlayer();
	   }
	}
  } else {
    // just update the view (resize, etc...)
	item.updateViewer_(item_node);
  }
}

Presenter.getWidthSpaceValue = function() {
  return Presenter.GAL_MENU_WIDTH*2 + Presenter.ARROW_SPACE*2;
}

Presenter.prototype.layout = function(current_item_only) {
  resize_editor_window();
  if (current_item_only === undefined) {
    current_item_only = false;
  }
  var selected = this.gallery_.getSelected();
  var cx = this.node_.offsetWidth / 2;
  var cy = this.node_.offsetHeight / 2 - Presenter.MIN_EDGE_SPACING / 2 + 30;
  //var max_width = this.node_.offsetWidth - Presenter.MIN_EDGE_SPACING * 2;
  //var max_width = this.node_.offsetWidth - Presenter.MIN_EDGE_SPACING * 2 - Presenter.ARROW_SPACE;
  var max_width = this.node_.offsetWidth - Presenter.getWidthSpaceValue();
  var max_height = this.node_.offsetHeight - Presenter.MIN_EDGE_SPACING * 2;
  var x = 0;

  $('.picture_link').hide().removeClass('picture_link_selected');  
  for (var i = 0, item_node; item_node = this.item_nodes_[i]; i++) {
    var item = item_node.item;
	var valid_image = Gallery.isValidImage(item);
	
    var full_width = item.image.width;
    var full_height = item.image.height;
	
	item_node.image.className = item.image_class;
	this.updateViewer_(item, item_node); // update the item_node 
	
    if ((BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 9) || BrowserDetect.browser == "Opera") {
        if (item.image.natural_width) {
            full_width = item.image.natural_width;
        }
        if (item.image.natural_height) {
            full_height = item.image.natural_height;
        }
    }
	
	//item.image.style.top = -parseInt(item.image.height / 2);
    //item.image.style.left = -parseInt(item.image.width / 2);

    var thumb_width = item.thumb.width;
    var thumb_height = item.thumb.height;

	if ((BrowserDetect.browser == "Explorer" && BrowserDetect.version <= 9) || BrowserDetect.browser == "Opera") {
        if (item.thumb.natural_width) {
            thumb_width = item.image.natural_width;
        }
        if (item.thumb.natural_height) {
            thumb_height = item.image.natural_height;
        }        
    }

	if (item.resized_loaded) {
	  $(item_node.thumb).show();
	  $(item_node.image).show();
	  $(item_node.loading_icon).hide();
	} else {
	  //item_node.loading_icon.natural_width = item.loading_icon.width;
      //item_node.loading_icon.natural_height = item.loading_icon.height;
	  //if (BrowserDetect.browser == 'Explorer') {
	  //  alert(item.loading_icon.width);
      //}
      //item_node.loading_icon.width = item.loading_icon.width;
      //item_node.loading_icon.height = item.loading_icon.height;
      //item_node.loading_icon.style.top = -parseInt(item.loading_icon.height / 2);
      //item_node.loading_icon.style.left = -parseInt(item.loading_icon.width / 2);
	  //item_node.loading_icon.position = 'absolute';
	  
	  $(item_node.loading_icon)
	    .css('height', 'auto')
		.css('width', 'auto');
	  var loading_icon_height = $(item_node.loading_icon).height();
	  var loading_icon_width = $(item_node.loading_icon).width();
	  
	  //$(item_node.loading_icon)
	  //  .css('height', '')
	  //	.css('width', '');
	  $(item_node.loading_icon)
	    .css('top', -loading_icon_height/2)
		.css('left', -loading_icon_width/2)
	    .css('position', 'absolute');
	}
	
    if (item == selected) { 
      if(item.loaded) $(item_node).find('.picture_link').show().addClass('picture_link_selected');
      x -= Presenter.SPACING / 2;
      x -= Presenter.SELECTED_OVERLAP;

      // Figure out the right size for the fullsize image.
      var scale = min(max_width / full_width, max_height / full_height);
      scale = (scale < 1) ? scale : 1;

      var width = full_width * scale;
      var height = full_height * scale;
      var additional_margin_top = 0;
	  
	  if (!item.resized_loaded) {
	    width = item.loading_icon.width;
		height = item.loading_icon.height;
	  } else if (!valid_image && item_node.viewer) {
		// TODO this is not good for browsers that support transition effect
		// for viewers that are resizing...
		width = $(item_node.viewer).outerWidth();
		height = $(item_node.viewer).outerHeight();
		// override it
		width = parseInt($(item_node.viewer).css('width')) || width;
		height = parseInt($(item_node.viewer).css('height')) || height;
	  }
	  
	  if (height < $(item_node.left_arrow).height()) {
	    additional_margin_top = additional_margin_top + $(item_node.left_arrow).height() - height;
	  }
	  
      //window.console.log("Full width: " + full_width + ". Full height: "+full_height+". Scale: "+scale);
	  //window.console.log("width: " + width + ". height: "+height);

      $(item_node.left_arrow)
	    .css('margin-left', -(width/2+40));
		
      $(item_node.right_arrow)
		.css('margin-left', width/2+10);
	  
	  // picture info
	  $(item_node.picture_info)
	    .show()
	    .css('margin-left',-width/2)
	    .css('width', width);

	  var caption_div = $(item_node.picture_info).find('.caption');
	  var caption_text = $(item_node.picture_info).find('.caption_text');
	  var caption_add_text = $(item_node.picture_info).find('.caption_add');
	  var caption_height = 0;
	  var hide_later = false;
	  
	  if (!caption_div.is(':visible')) {
	    hide_later = true;
	    caption_div.show();
	  }
	  
	  $(item_node.picture_info)
		.css('top', '-10000')
		.css('left', '-10000')
		.show();
		
	  if (item.caption !== null && item.caption.length > 0) {
	    caption_height = caption_text.height();
		if (item.id !== null) {
		  caption_text.show();
		}
	  } else {
		if (this.gallery_.IS_EDITOR) {
	      caption_height = caption_add_text.height();
		  if (item.id !== null) {
		    caption_add_text.show();
		  }
		} else {
		  caption_height = caption_div.height();
		}
	  }
	  
	  var caption_width = caption_div.width();
	  var picture_info_width = $(item_node.picture_info).width();
	  
	  if (hide_later) {
	    caption_div.hide();
	  }
	  
	  $(item_node.picture_info)
		.css('top', '')
		.css('left', '');

	  $(item_node.picture_info)
		.css('margin-top', -(height/2+caption_height+additional_margin_top+10));
	
	  // caption textarea
	  if (picture_info_width < caption_width && caption_width <= parseInt(caption_div.css('min-width'))) {
	    $(item_node).find('.caption_edit textarea').css('width', caption_width).change();
	  } else {
	    $(item_node).find('.caption_edit textarea').css('width', picture_info_width).change();
	  }
	  
	  // picture info inside
	  $(item_node).find('.item_info')
	    .css('margin-left', (width/2)-(Presenter.PICTURE_INFO_WIDTH/2));
	  
	  // picture links
      $(item_node).find('.picture_link')
		.css('margin-top', (height/2)+additional_margin_top+15) // #125
        .css('margin-left',-width/2)
        .css('width',width);
	
	  if ($(item_node).find('.caption_edit').is(':visible')) {
	    var add_height = 0;
	    if (!$(item_node).find('.caption_loading').is(':visible')) {
		  add_height = $(item_node).find('textarea').height();
        } else {
		  add_height = $(item_node).find('.caption_loading').height();
		}
		
		$(item_node).find('.picture_info')
		  .css('margin-top', -(height/2+add_height+additional_margin_top+10));
		  
        $(item_node).find('.picture_link')
		  .css('margin-top', (height/2)+15);
	  }
		
	  // share buttons div
	  $(item_node).find('.share_div')
	    .css('margin-left', (width/2)-(Presenter.SHARE_BUTTONS_WIDTH/2));
	  
	  // link container
	  var links_container = $(item_node).find('.links_container');
	  var links_container_width = links_container.outerWidth();

	  links_container
		.css('margin-left', (width/2)-((width/2)-(Presenter.SHARE_BUTTONS_WIDTH/2))-(links_container_width/2));

	  if (item.resized_loaded) {
	    $(item_node.image).show();
        Presenter.opacity(item_node.image, 1);
		if (item_node.viewer !== undefined && item_node.viewer !== null && item.id !== null) {
		  $(item_node.image).hide();
		  $(item_node.viewer).show();
		  
		  // TODO remove this and move to item.js
		  if (item.viewer_type == 'video') {
	        var height = $(item_node.viewer).height();
	        var width = $(item_node.viewer).width();
	        $(item_node.viewer)
	          .css('margin-top', -height/2)
		      .css('margin-left', -width/2);
	      }
		}
	  } else {
	    $(item_node.image).hide();
	    Presenter.opacity(item_node.loading_icon, 1);
	  }
	
	  if (current_item_only) {
	    break;
	  }
	
	  if (this.gallery_.SHOW_LINKS) {
	    $(item_node).find('.links_container').show();
	  }
	  if (this.gallery_.SHOW_INFO) {
	    $(item_node).find('.item_info').show();
	  }
	  
	  if (width < 200) {
	    x += Presenter.SMALL_IMAGE_SPACE;
	  } else {
	    x += width / 2;
	  }

      // Position the item. 
      item_node.style.left = x + 'px';
      item_node.style.top = cy + 'px';

      item_node.className = 'itemnode selected';
	  Presenter.scale(item_node.image, scale);

      // Figure out the right size for the thumbnail.
      var thumb_scale = width / thumb_width;
	  
      Presenter.scale(item_node.thumb, thumb_scale);
      Presenter.opacity(item_node.thumb, 0);

      this.scene_.style.left = (cx - x) + 'px';
	  
	  if (width < 200) {
	    x += Presenter.SMALL_IMAGE_SPACE;
		x += Presenter.SPACING / 2;
		x -= Presenter.SELECTED_OVERLAP;
	  } else {
	    x += width / 2;
        x += Presenter.SPACING / 2;
		x -= Presenter.SELECTED_OVERLAP;
	  }
	  
      // added for arrows
      x += 60;
    } else {
	
	  // hide the picture info
	  $(item_node).find('.picture_info').hide();
	  $(item_node).find('.links_container').hide();
	  $(item_node).find('.item_info').hide();
	  
      $(item_node.left_arrow).hide();
	  $(item_node.right_arrow).hide();
	  $(item_node).find('.picture_info').children().hide();
	  //$(item_node.link).find('.item_info .dimensions').hide(); // #185
	  
	  // fix for 'flashing images' bug in chrome, (safari too)
	  // chrome has a slide effect when changing the position of an element
	  // hide the item container so we can't see the slide effect, show it later
	  if (BrowserDetect.browser == 'Chrome' || BrowserDetect.browser == 'Safari') {
	    $(item_node).hide();
	  }
	  
      item_node.style.top = cy + 'px';
      item_node.style.left = x + 'px';
	  
	  if (item_node.style.MozTransition !== undefined) {
	    item_node.style.MozTransition = 'none 0s ease';
	  }
	  
	  item_node.className = 'itemnode';
	  
      // Figure out the size of the full image.
      var scale = min(Presenter.SMALL_WIDTH / full_width, Presenter.SMALL_HEIGHT / full_height);
      scale = (scale < 1) ? scale : 1;
      Presenter.scale(item_node.image, scale);
      Presenter.opacity(item_node.image, 0);

      // Figure out the size of the thumb image.
      var thumb_scale = min(Presenter.SMALL_WIDTH / thumb_width, Presenter.SMALL_HEIGHT / thumb_width);
      thumb_scale = (thumb_scale < 1) ? thumb_scale : 1;
	  
      Presenter.scale(item_node.thumb, thumb_scale);
      Presenter.opacity(item_node.thumb, 0.5);
	  
	  if (item_node.viewer) {
	    $(item_node.viewer).hide();
	  }
	  
      x += Presenter.SPACING;
	  // added for arrows
	  x += 60;
    }
  }
  
  // cont. of 'flashing images' bug fix
  // display the hidden containers
  if (BrowserDetect.browser == 'Chrome' || BrowserDetect.browser == 'Safari') {
    for (var i = 0, item_node; item_node = this.item_nodes_[i]; i++) {
	  if (!$(item_node).is(':visible')) {
	    $(item_node).show();
	  }
    }
  }
}

Presenter.prototype.setZoom = function(zoom) {
  if (zoom) {
    this.zoom_ = true;
    this.node_.className = 'presentation zoomed';
  } else {
    this.zoom_ = false;
    this.node_.className = 'presentation';
  }
  this.layout();
}

Presenter.prototype.imageClicked_ = function(item) {
  if (item != this.gallery_.getSelected()) {
    this.gallery_.setSelected(item);
    for (var i = 0; i < this.item_nodes_.length; i++) {
      if (this.item_nodes_[i].item == item) {
	    window.location.hash = '#' + (i+1);
		break;
	  }
    }
  } else {   
    // #143
	if (item.id) {
      var valid_image = Gallery.isValidImage(item);
      var href = '';
      if (valid_image) {
        //href = '/i' + item.id + '.' + extension(item.name);
		//href = item.getLink();
		href = '/l' + item.id;
        window.open(href);
      } else {
        //href = 'http://k.min.us/i' + item.id + '.' + extension(item.name);
		href = item.getLink();
		if (BrowserDetect.browser == "Explorer") {
		  var download_link = $('<a>', {'href': href, 'target': '_blank', 'text': 'temp', 'style': 'position:absolute; display:block'});
		  //download_link.appendTo(document.body);
		  download_link[0].click(); // simulate a click
		} else {
          window.open(href);
		}
      }
	}
/*  
    if (window.event.target && window.event.offsetX > window.event.target.offsetWidth / 2) {
      this.gallery_.selectNext();
    } else {
      this.gallery_.selectPrev();
    }
	*/
  }
  if(this.gallery_) save_event('image_clicked', this.gallery_.id, item.id);		
}
