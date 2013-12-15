function Menu(gallery, user, num_galleries, login_name, login_first, logout_first, username_slugify) {
  this.login_name = login_name;  
  this.login_first = login_first;
  this.logout_first = logout_first;
  if (gallery) {
    this.page_gallery_ = gallery;
    this.page_gallery_.addListener(this.galleryListener_.bind(this));
  }
  this.signed_in_ = user.signed_in;
  this.num_galleries = num_galleries;
  // Doesn't contain valid gallery data, just id and name.
  this.galleries_ = user.mocklists;
  this.username_ = user.name;
  this.sign_in_url_ = user.sign_in_url;
  this.sign_out_url_ = user.sign_out_url;
  
  this.option_icons_created = false;
  
  this.username_slugify = username_slugify;
  
  if (BrowserDetect.browser != 'Explorer') {
	$('#minus_toolbar').css('z-index', 10000);
  }
  
  var toolbar_menu = $('#toolbar_menu');
  
  if (toolbar_menu.length > 0 && toolbar_menu.find('#menu_home_button').length > 0) {
    return; // don't create
  }
  
  if (toolbar_menu.length > 0) {
    var divider = $('<div>', {'class': 'divider'});

	this.toolbar_new = $('<div>', {'class': 'menu_button button', 'id': 'menu_home_button'}).html('<a id="menu_home_link" class="menu_button_text" href="/">Home</a><span id="menu_home_link_active" class="menu_button_text active no_link" style="display:none;">Home</span>');
	//this.toolbar_my_galleries = $('<div>', {'class': 'menu_button button'}).html('<a class="menu_button_text" href="/mine">Mine (<span id="gallery_num">' + this.num_galleries + '</span>)</a>');
	this.toolbar_my_galleries = $('<div>', {'class': 'menu_button button'}).html('<a id="menu_mine_link" class="menu_button_text" href="/mine">Mine</a><span id="menu_mine_link_active" class="menu_button_text active no_link" style="display:none;">Mine</span>');
	this.toolbar_options = $('<div>', {'class': 'menu_button button'}).html('<span class="menu_button_text sign_in_button">Sign In</span><span class="menu_button_text register_button fancy_button" ><span>Sign Up</span></span><span class="menu_button_text sign_out_button" style="display:none;">Sign Out</span>');
	
	toolbar_menu
	  .append(divider.clone())
	  .append(this.toolbar_new)
	  .append(divider.clone())
	  .append(this.toolbar_my_galleries)
	  .append(divider.clone())
	  .append(this.toolbar_options);

    if (this.username_slugify || this.username_slugify.length > 0) {
	  this.toolbar_userpage = $('<div>', {'class': 'menu_button button'}).html('<a id="menu_me_link" class="menu_button_text me_button" href="/u/' + this.username_slugify + '">'+this.username_slugify+'</a><span id="menu_me_link_active" class="menu_button_text active no_link" style="display:none;">'+this.username_slugify+'</span>');
	  toolbar_menu.find('#menu_home_button').after(this.toolbar_userpage).after(divider.clone());
	} else {
	  //this.toolbar_userpage = $('<div>', {'class': 'menu_button button'}).html('<a class="menu_button_text me_button" href="#">Me</a>');
	  this.toolbar_userpage = $('<div>', {'class': 'menu_button button'}).html('<span id="me_guest_btn" class="menu_button_text">Me</a>');
	  toolbar_menu.find('#menu_home_button').after(this.toolbar_userpage).after(divider.clone());
	}
	
	/*
	var curr_pathname = window.location.pathname;
	if (curr_pathname.length > 1 && curr_pathname[curr_pathname.length-1] == '/') {
	  curr_pathname = curr_pathname.substr(0, (curr_pathname.length-1)); // remove the last slash
	}
	var active_links = toolbar_menu.find('a[href="' + curr_pathname + '"]').addClass('active');
	if (active_links.length == 0) {
	  $(toolbar_menu).find('a').each(function() {
	   if ($(this).attr('href') == curr_pathname) {
		  $(this).addClass('active');
	   } else if ($(this).attr('href') == window.location.protocol + '//' + window.location.host + curr_pathname) { // IE7 fix
	     $(this).addClass('active');
	   }
      });
	}
	*/
	this.updateActiveLink_();
	
    /*
	$(this.toolbar_new).hover(function() {
	  Menu.button_hover_in.bind(this)();
	  $(this).find('span').css('color', '#FF3300');
	}, function() {
	  Menu.button_hover_out.bind(this)();
	  $(this).find('span').css('color', '');
	});
	*/
	//$(this.toolbar_new).hover(Menu.button_hover_in, Menu.button_hover_out);
	//$(this.toolbar_my_galleries).hover(Menu.button_hover_in, Menu.button_hover_out);
	//$(this.toolbar_options).hover(Menu.button_hover_in, Menu.button_hover_out);
	
	this.createOptionIcons_();
	this.layout();
  } else {  
    this.node_ = createElement('div', 'home', document.body);
    this.node_new_ = createElement('a', 'menuitem', this.node_);
    this.node_new_.href = '/';
  }
  
  /*
  this.node_menu_ = createElement('select', 'menuitem', this.node_);
  var opt = new Option("Your pictures:", "", false, false);
  this.node_menu_.options[this.node_menu_.length] = opt;
  addEventListenerWrapper(this.node_menu_, 'change', this.menuChanged_.bind(this));
  */
  
  /* // Draw sign in links at bottom-right.
  if (this.signed_in_) {
    this.node_name_ = createElement('a', 'menuitem', this.node_);
    setText(this.node_name_, this.username_);

    this.node_sign_ = createElement('a', 'menuitem', this.node_);
    this.node_sign_.href = this.sign_out_url_;
    setText(this.node_sign_, "Sign out");
  } else {
    this.node_sign_ = createElement('a', 'menuitem', this.node_);
    this.node_sign_.href = this.sign_in_url_;
    setText(this.node_sign_, "Sign in");
  }
    */
	
  this.update_();
  if (window.onresize) {
    var old_resize = window.onresize;
	window.onresize = function() {
	  old_resize();
	  this.layout();
	}.bind(this);
  } else {
    window.onresize = this.layout.bind(this);    
  }
}

Menu.DARK_MODE_ICON = '/smedia/minus/images/dark-mode.png';
Menu.LIGHT_MODE_ICON = '/smedia/minus/images/light-mode.png';
Menu.DARK_MODE_TEXT = 'Lights On';
Menu.LIGHT_MODE_TEXT = 'Lights Off';

Menu.button_hover_in = function() {
  $(this).removeClass('darker_button');
  $(this).removeClass('button').addClass('button_hover');
}

Menu.button_hover_out = function() {
  $(this).addClass('darker_button');
  $(this).removeClass('button_hover').addClass('button');
}

Menu.show_option_description = function() {
  var offset = $(this).offset();
  var description = $('div').css('position', 'absolute').css('top', offset.top).css('left', offset.left);
  
}

Menu.prototype.galleryListener_ = function(e) {
  if (e.type == Gallery.EVENT_SAVED) {
    if (!this.getGalleryById(this.page_gallery_.id)) {
      this.galleries_.push({
        'id' : this.page_gallery_.id,
        'name' : this.page_gallery_.name
      });
      this.update_();
    }
  } 
  else if (e.type == Gallery.EVENT_NAMECHANGED) {
    this.getGalleryById(this.page_gallery_.id).name = this.page_gallery_.name;
    this.update_();
  } else if (e.type == Gallery.EVENT_DELETED) {
    this.node_.style.bottom = 25 + 'px';
    this.node_.style.opacity = 0;
  }
  else if (e.type == Gallery.EVENT_SAVEDID) {
	this.num_galleries++;
	$('span#gallery_num').html(this.num_galleries);
	if (!this.option_icons_created) {
	  this.createOptionIcons_();
	}
  } else if (e.type == Gallery.EVENT_GALLERY_HISTORY_CLEARED) {
    this.num_galleries--;
	this.update_();
  }
}

Menu.prototype.getGalleryById = function(id) {
  for (var i = 0, gallery; gallery = this.galleries_[i]; i++) {
    if (gallery.id == id) {
      return gallery;
    }
  }
  return false;
}

Menu.prototype.update_ = function() {
  window.console.log("Menu updating");
  if ($('#toolbar_menu').length > 0) {
    this.updateToolbar_();
  } else { // TODO del
    $('div.home').html(
      '<div class="ui-blitzer-scoped" style="" style=""><a href="/" style="font-size:13px;width:40px;position:absolute;right:256px;text-align:center;bottom:7px;padding-bottom: 6px;padding-left: 12px;padding-right: 12px;padding-top: 6px;height:15px;" class="fg-button ui-state-default fg-button-icon-left ui-corner-left">New</span></a>' +
	  '<div class="ui-blitzer-scoped" style="" style=""><a href="/pages/galleries" style="font-size:13px;width:130px;position:absolute;right:97px;text-align:center;bottom:7px;padding-bottom: 6px;padding-left: 12px;padding-right: 12px;padding-top: 6px;height:15px;" class="fg-button ui-state-default  ">My Galleries (<span id="gallery_num">' + this.num_galleries + '</span>)</a>' + 
      '<span style="width:65px;height:15px;position:absolute;right:7px;font-size:13px;font-family:helvetica, sans-serif;font-weight:bold;cursor:pointer;text-align:center;bottom:7px;padding-bottom: 6px;padding-left: 12px;padding-right: 12px;padding-top: 6px;" class="sign_in_button fg-button ui-state-default fg-button-icon-left ui-corner-right">Sign In or Register</span>' + 
	  '<span style="display:none;  width:65px;height:15px;position:absolute;right:7px;font-size:13px;font-family:helvetica, sans-serif;font-weight:bold;cursor:pointer;text-align:center;bottom:7px;padding-bottom: 6px;padding-left: 12px;padding-right: 12px;padding-top: 6px;" class="sign_out_button fg-button ui-state-default fg-button-icon-left ui-corner-right">Sign Out</span>' +
	  +'</div>' +
	  "<div class='signed_in_name' style='width:220px;height:15px;position:absolute;overflow:hide;right:7px;font-size:12px;font-family:helvetica;font-weight:bold;" + 
      "cursor:default;text-align:right;bottom:33px;padding-bottom: 6px;padding-left: 12px;padding-right: 12px;padding-top: 6px;" +
	  "color:#777;white-space:nowrap;display:none;'></div>" 
	);
  }
  
  // update also the my galleries heading if it exists
  var my_galleries_h2 = $('#my_galleries_num');
  if (my_galleries_h2.length > 0) {
    $(my_galleries_h2).text(this.num_galleries);
  }
  
  if(!this.login_name || this.login_name=='') {
      $('.signed_in_name').hide();
      $('.sign_out_button').hide();
      $('#clear_all_galleries').show(); // don't hide the clear galleries button
      $('.sign_in_button').show();
	  $('.register_button').show();
    }
    else {
      $('.signed_in_name').html(this.login_name).show();
	  $('.sign_out_button').show();	
	  $('.sign_in_button').hide();
	  $('.register_button').hide();	  
    }
  
  // bug, after login in front page, then upload a  new gallery, this will show again in the gallery view
  /*
  if(this.login_first && this.login_first=='True') {
	//show_confirm('You successfully registered, <b>' + this.login_name + '</b>.');
	show_confirm('You have successfully signed in, <b>' + this.login_name + '</b>.');
	turn_off_dialog();
  }
  
  if(this.logout_first &&  this.logout_first==true) {
	show_confirm('You are signed out.');
	turn_off_dialog_logout();
  }
  */

  // $(".sign_in_button").effect("highlight", {color: ''}, 2000);

  //$("div.home span.sign_in_button").effect( "bounce", 
	//{times:4}, 1000 );

  bind_sign_in_button();
  
  if ($('div.presentation').length > 0) {
    $('div.confirm').detach().appendTo('div.presentation');
    //$('div.login_dialog_package').detach().appendTo('div.presentation'); // causes bug in ie7
  }
  
  $('div.home .fg-button').hover(
	  function(){ 
		  $(this).addClass("ui-state-hover"); 
	  },
	  function(){ 
		  $(this).removeClass("ui-state-hover"); 
	  }
  );
  
  if (this.signed_in_ || this.page_gallery_.key) {
  } else {
    
  }
}

Menu.prototype.updateToolbar_ = function() {
  $(this.toolbar_my_galleries).find('#gallery_num').text(this.num_galleries);
  this.updateActiveLink_();
  if (window.location.pathname == '/') {
    $('#signup_msg').show();
  } else {
    $('#signup_msg').hide();
  }
}

Menu.prototype.updateActiveLink_ = function() {
  var toolbar_menu = $('#toolbar_menu');
  var curr_pathname = window.location.pathname;
  toolbar_menu.find('a').removeClass('active');
  toolbar_menu.find('.no_link').hide();
  if (curr_pathname.length > 1 && curr_pathname[curr_pathname.length-1] == '/') {
    curr_pathname = curr_pathname.substr(0, (curr_pathname.length-1)); // remove the last slash
  }
  var active_links = toolbar_menu.find('a[href="' + curr_pathname + '"]').addClass('active');
  
  active_links.each(function() {
    var id = $(this).attr('id');
	$('#' + id + '_active').show();
  });
  
  if (active_links.length == 0) { // try again
    $(toolbar_menu).find('a').each(function() {
      if ($(this).attr('href') == curr_pathname) {
        $(this).addClass('active');
		$('#' + $(this).attr('id') + '_active').show();
	  } else if ($(this).attr('href') == window.location.protocol + '//' + window.location.host + curr_pathname) { // IE7 fix
        $(this).addClass('active');
		$('#' + $(this).attr('id') + '_active').show();
      }
    });
  }
}

Menu.prototype.createOptionIcons_ = function() {
  if ($('#toolbar_menu').length > 0 && this.option_icons_created == false) {
    var divider = $('<div>', {'class': 'divider'});
  
    //if (window.location.pathname == '/' || (this.page_gallery_ && this.page_gallery_.id)) {
	
	  //var background_button = $('<div>', {'class': 'content button darker_button'});
	  var background_button = $('<div>', {'class': 'option_button button darker_button', 'id': 'option_background'});//, 'option_description': 'Background'});
	  var dark_icon = $('<img>', {'src': Menu.DARK_MODE_ICON});
	  var light_icon = $('<img>', {'src': Menu.LIGHT_MODE_ICON});
	  
	  // TODO check setting if what mode currently using
	  $(background_button)
	    .append(dark_icon)
		.append(light_icon);
	
	  if (this.page_gallery_.LIGHT_MODE) {
	    $(light_icon).hide();
		$(background_button).attr('option_description', Menu.LIGHT_MODE_TEXT);
		this.page_gallery_.useLightMode();
	  } else {
	    $(dark_icon).hide();
		$(background_button).attr('option_description', Menu.DARK_MODE_TEXT);
		this.page_gallery_.useDarkMode();
	  }
	  
	  $('#toolbar_middle')
	    .append(divider.clone())
	    .append(background_button)
		.append(divider.clone());
	
	/*
	  $(background_button).hover(function() {
	    //$(this).removeClass('darker_button');
	    Menu.button_hover_in.bind(this)();
	  }, function() {
	    //$(this).addClass('darker_button');
	    Menu.button_hover_out.bind(this)();
	  });
	*/
	  
	  $(background_button).click(function() {
	    this.page_gallery_.switchColor();
		$(background_button).find('img').toggle();
	  }.bind(this));
	  
	  $(background_button).optionDescription();
	  
	  // position to 'center'
	  //var left_links = $('#toolbar_links').width();
	  this.option_icons_created = true;
	  this.centerOptionIcons_();
	  
	//}
  }
}

Menu.prototype.centerOptionIcons_ = function() {
  var doc_width = $(document).width();
  var mid_width = $('#toolbar_middle').width();
  var menu_width = $('#toolbar_menu').width();
  //var links_width = 0;
  //if ($('#toolbar_links').is(':visible')) {
  //  links_width = $('#toolbar_links').width();
  //}
  
  var links_width = $('#toolbar_left').width();
  
  var doc_width_center = doc_width/2;
  var new_margin_left = doc_width_center - links_width - (mid_width/2);

  if (new_margin_left > 0) {
    $('#toolbar_middle').css('margin-left', new_margin_left);
	var copyright_width = $('#toolbar_copyright').width();
	if (new_margin_left + menu_width + mid_width + copyright_width + 5 > doc_width) {
      $('#toolbar_middle').css('margin-left', '');
    }
  } else {
    $('#toolbar_middle').css('margin-left', '');
  }
}

Menu.prototype.showHideLinks_ = function() {
  var isMobile = DetectMobileQuick();
  if ($(document).width() < 1000 || isMobile) {
    $('#toolbar_links').hide();
  } else {
    $('#toolbar_links').show();
  }
}

Menu.prototype.layout = function() {
  this.showHideLinks_();
  this.centerOptionIcons_();
}

