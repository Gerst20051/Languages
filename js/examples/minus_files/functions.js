//function $(o) {return document.getElementById(o);}

Function.prototype.bind = function(thisObj, var_args) {
  var self = this;
  var staticArgs = Array.prototype.splice.call(arguments, 1, arguments.length);

  return function() {
    var args = staticArgs.concat();
    for (var i = 0; i < arguments.length; i++) {
      args.push(arguments[i]);
    }
    return self.apply(thisObj, args);
  };
}

function createElement(type, className, parent) {
  var el = document.createElement(type);
  el.className = className;
  if (parent) parent.appendChild(el);
  return el;
}

function min(a, b) { return (a < b) ? a : b; }
if (!('console' in window)) {
  window.console = [];
  window.console.log = function() {}
}

function setText(node, text) {
  node.innerHTML = '';
  node.appendChild(document.createTextNode(text));
}

//function addEventListener(node, evt, listener) {
//  if (node.addEventListener)
//    node.addEventListener(evt, listener, false);
//  else if (node.attachEvent)
//    node.attachEvent('on' + evt, listener);
//}

function addEventListenerWrapper(node, evt, listener) {
  if (node.addEventListener)
    node.addEventListener(evt, listener, false);
  else if (node.attachEvent)
    node.attachEvent('on' + evt, listener);
}

function removeEventListenerWrapper(node, evt, listener) {
  if (node.removeEventListener)
    node.removeEventListener(evt, listener, false);
  else if (node.detachEvent)
    node.detachEvent('on' + evt, listener);
}

// http://stackoverflow.com/questions/1219860/javascript-jquery-html-encoding
function htmlEncode(value){ 
  return $('<div/>').text(value).html(); 
} 

function htmlDecode(value){ 
  return $('<div/>').html(value).text(); 
}

// http://stackoverflow.com/questions/487073/jquery-check-if-element-is-visible-after-scroling
function isScrolledIntoView(elem) {
  var docViewTop = $(window).scrollTop();
  var docViewBottom = docViewTop + $(window).height();

  var elemTop = $(elem).offset().top;
  var elemBottom = elemTop + $(elem).height();

  return ((elemBottom >= docViewTop) && (elemTop <= docViewBottom));
}

// Browser detection script from http://www.quirksmode.org/js/detect.html
var BrowserDetect = {
  init: function () {	
    this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
    this.version = this.searchVersion(navigator.userAgent)
      || this.searchVersion(navigator.appVersion)
      || "an unknown version";
    this.OS = this.searchString(this.dataOS) || "an unknown OS";
  },
  searchString: function (data) {
    for (var i=0;i<data.length;i++) {
	  var dataString = data[i].string;
	  //alert(dataString);    
      var dataProp = data[i].prop;
      this.versionSearchString = data[i].versionSearch || data[i].identity;
      if (dataString) {
        if (dataString.indexOf(data[i].subString) != -1)
          return data[i].identity;
      }
      else if (dataProp)
        return data[i].identity;
    }
  },
  searchVersion: function (dataString) {
    var index = dataString.indexOf(this.versionSearchString);
    if (index == -1) return;
    return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
  },
  dataBrowser: [
    {
      string: navigator.userAgent,
      subString: "Chrome",
      identity: "Chrome"
    },
    {   string: navigator.userAgent,
      subString: "OmniWeb",
      versionSearch: "OmniWeb/",
      identity: "OmniWeb"
    },
    {
      string: navigator.vendor,
      subString: "Apple",
      identity: "Safari",
      versionSearch: "Version"
    },
    {
      prop: window.opera,
      identity: "Opera"
    },
    {
      string: navigator.vendor,
      subString: "iCab",
      identity: "iCab"
    },
    {
      string: navigator.vendor,
      subString: "KDE",
      identity: "Konqueror"
    },
    {
      string: navigator.userAgent,
      subString: "Firefox",
      identity: "Firefox"
    },
    {
      string: navigator.vendor,
      subString: "Camino",
      identity: "Camino"
    },
    {   // for newer Netscapes (6+)
      string: navigator.userAgent,
      subString: "Netscape",
      identity: "Netscape"
    },
    {
      string: navigator.userAgent,
      subString: "MSIE",
      identity: "Explorer",
      versionSearch: "MSIE"
    },
    {
      string: navigator.userAgent,
      subString: "Gecko",
      identity: "Mozilla",
      versionSearch: "rv"
    },
    {     // for older Netscapes (4-)
      string: navigator.userAgent,
      subString: "Mozilla",
      identity: "Netscape",
      versionSearch: "Mozilla"
    }
  ],
  dataOS : [
    {
      string: navigator.platform,
      subString: "Win",
      identity: "Windows"
    },
    {
      string: navigator.platform,
      subString: "Mac",
      identity: "Mac"
    },
    {
         string: navigator.userAgent,
         subString: "iPhone",
         identity: "iPhone/iPod"
      },
    {
      string: navigator.platform,
      subString: "Linux",
      identity: "Linux"
    }
  ]

};
BrowserDetect.init();

// HAMLET HAMLET HAMLET
function isValidBrowser() {
  var browser = BrowserDetect.browser;
  var version = BrowserDetect.version;
  var OS = BrowserDetect.OS; 
  //alert(browser+','+version+','+OS);
  //window.console.log('----a browser, version, os:' + browser+','+version+','+OS);
  return (
    (browser == 'Chrome' && version >= 6) ||
    (browser == 'Firefox' && version >= 3.6) ||
	(browser == 'Safari' && version >= 5.0 && OS == 'Mac') ||
	//(browser == 'Safari' && version >= 5.0) ||
    ("FileReader" in self && "ondrag" in document)
  );
}


function log(msg) {
    window.console.log(msg);
}

function save_event(type, gallery_id, item_id) {
	if(gallery_id==null || gallery_id=='') {
		gallery_id = 'NONENONE';
	}
	if(item_id==null || item_id=='') {
		item_id = 'NONENONE';
	}
	$.ajax({
		type: "GET",
		url: "/api/event/"+type+'/'+gallery_id+'/'+item_id,
		cache: false,
		dataType: "html",
		async: true,
		success: function(html, textStatus) {
		},
		error: function (XMLHttpRequest, textStatus, errorThrown) {
		}
	});		
    _gaq.push(['_trackEvent', 'New', type]);
}

function turn_off_dialog() {
	$.ajax({
		type: "GET",
		url: "/api/turn_off_dialog",
		cache: false,
		dataType: "html",
		async: true,
		success: function(html, textStatus) {
		},
		error: function (XMLHttpRequest, textStatus, errorThrown) {
		}
	});		
}

function turn_off_dialog_logout() {
	$.ajax({
		type: "GET",
		url: "/api/turn_off_dialog_logout",
		cache: false,
		dataType: "html",
		async: true,
		success: function(html, textStatus) {
		},
		error: function (XMLHttpRequest, textStatus, errorThrown) {
		}
	});		
}

// Making jquery ui dialogs work with scoped themes.
// Wraps the dialog in a div with the scoping class on
// dialog open, removes the scoping class on dialog close
$.fn.dialogScopingWorkaround = function(scopingClass) {
  this.bind('dialogopen', function(event, ui) {
    $(this).parents(".ui-dialog").wrap(
      '<div class="dialogScopingWrapper ' + scopingClass + '"></div>'
    );
  });
  this.bind('dialogclose', function(event, ui) {
    var wrapper = $(this).parents(".dialogScopingWrapper");
    wrapper.replaceWith(wrapper.children());
  });
  return this;
};

function extension(filename) {
  if (filename[0] == '.') return undefined;
  return (/[.]/.exec(filename)) ? /[^.]+$/.exec(filename) : undefined;
}

function filename_noext(filename) {
  return (/(.+?)(\.[^.]*$|$)/.exec(filename)[1]);
}

function streql_in(str1, str2) {
  if (str1.toLowerCase() == str2.toLowerCase()) return true;
  return false;
}

function validate_username(username) {
  var username = $.trim(username);
  if (username.length == 0) {
    return false;
  }
  var res = /[-.\w]+/.exec(username);
  var matched = null;
  if (res) {
    matched =  res[0];
  }
  if (matched && matched == username) return true;
  return false;
}

function resize_editor_window() {
  var editor_height = $('.dm-editor').height();
  var window_height = $(window).height();
  //log('----------resize_editor_window: dmeditor: '+editor_height);  
  //log('----------resize_editor_window: window: '+window_height);  
  if( $('div.is_editor').length > 0 ) {
	if( editor_height  > window_height - 70 ) {
	  $('.filelist').height(window_height-90-200);
	}
  }
  else {
    // log('----------resize_editor_window: viewer');  
  	if( editor_height  > window_height - 70-40 ) {
	  $('.filelist').height(window_height-70-145);
	}
  }
  $('.dm-editor-dark').height($('.dm-editor').height());
}

// MISC funcs
function misc_html_fixups() {
  $('div.footer').detach().appendTo('.presentation');
	
  $('.footer_feedback_link').click( function() {
    UE.Popin.show(); 
  });

  $('a.social_links').hover(
    function() {
      $(this).find('span.hover').show();            
      $(this).find('span.normal').hide();
    },
    function() {
      $(this).find('span.hover').hide();            
      $(this).find('span.normal').show();            
    }
  );
}

function misc_main_logo_hover() {
  //$('.main_logo_wrapper').hover( 
  $('.main_logo_wrapper a').hover( 
	function() {
		$(this).find('img#main_logo').hide();
		$(this).find('img#main_logo_hover').show();
	},
	function() {
		$(this).find('img#main_logo').show();
		$(this).find('img#main_logo_hover').hide();
	}
  );	
}

function clear_gallery_from_history(id, div) {
	$.ajax({
		type: "GET",
		url: "/api/clear_gallery_from_history/"+id,
		cache: false,
		dataType: "html",
		async: true,
		before: function() {
			div.hide();
		},
		success: function(html, textStatus) {
			div.remove();
		},
		error: function (XMLHttpRequest, textStatus, errorThrown) {
			div.show();
		}
	});		
}

function is_valid_input(input) {
 return !$(input).hasClass('error');
}

function valid_input(input) {
  $(input).removeClass('error');
  $(input).hideErrorMsg();
}

// msg - optional
function invalid_input(input, msg) {
  $(input).addClass('error');
  if (msg !== undefined && msg.length > 0) {
    $(input).showErrorMsg({msg: msg});
  }
}

$.extend($.expr[':'], {
  focus: function(element) { 
    return element == document.activeElement; 
  }
});

function add_source(elem, path, type) {
  var source = $('<source>').attr('src', path);
  if (typeof type !== 'undefined') {
    source.attr('type', type);
  }
  source
    .appendTo(elem);
}

function show_announcement(text) {
  if (text != '' && text.length > 0) {
    $('#announcement .announcement_container').html(text);
    $('#announcement').fadeIn(1000, function() {
		$(this).animate({ dummy: 1 }, 4000, function() {
			hide_announcement();
		});				
	});
  }
}

function hide_announcement(text) {
  $('#announcement').fadeOut(2000);
}

function is_debug() {
  return IS_DEBUG;
}

function gal_thumb_on_error(){
  if ($(this).attr('item_ext')) {
    var src = Item.getSmallIcon($(this).attr('item_ext'));
    $(this).unbind("error").attr("src", src).css('height', '100px').css('width', '160px').css('border', '0');
  }
};

//from http://stackoverflow.com/questions/931207/is-there-a-jquery-autogrow-plugin-for-text-fields
(function($){
    $.fn.autoGrowInput = function(o) {
        o = $.extend({
            maxWidth: 1000,
            minWidth: 0,
            comfortZone: 70,
			callback: function() {}
        }, o);

        this.filter('input:text').each(function(){
            var minWidth = o.minWidth || $(this).width(),
                val = '',
                input = $(this),
                testSubject = $('<tester/>').css({
                    position: 'absolute',
                    top: -9999,
                    left: -9999,
                    width: 'auto',
                    fontSize: input.css('fontSize'),
                    fontFamily: input.css('fontFamily'),
                    fontWeight: input.css('fontWeight'),
                    letterSpacing: input.css('letterSpacing'),
                    whiteSpace: 'nowrap'
                }),
                check = function() {
                    if (val === (val = input.val())) {return;}

                    // Enter new content into testSubject
                    var escaped = val.replace(/&/g, '&amp;').replace(/\s/g,'&nbsp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
                    testSubject.html(escaped);

                    // Calculate new width + whether to change
                    var testerWidth = testSubject.width(),
                        newWidth = (testerWidth + o.comfortZone) >= minWidth ? testerWidth + o.comfortZone : minWidth,
                        currentWidth = input.width(),
                        isValidWidthChange = (newWidth < currentWidth && newWidth >= minWidth)
                                             || (newWidth > minWidth && newWidth < o.maxWidth);
					
                    // Animate width
                    if (isValidWidthChange) {
                        input.width(newWidth);
                    } else {
					  // added
					  if (newWidth > o.maxWidth) {
					    // use the maxWidth
					    input.width(o.maxWidth);
					  }
					}
				    o.callback();
                };
            testSubject.insertAfter(input);
			//$(this).change(check).keyup(check).keydown(check).blur(check);
            $(this).bind('keyup keydown blur update change paste', check);
        });

        return this;
    };
})(jQuery);

// edited:
//http://javascriptly.com/examples/jquery-grab-bag/js/jquery.autogrow-textarea.js
(function($) {

    /*
     * Auto-growing textareas; technique ripped from Facebook
     */
    $.fn.autogrow = function(options) {
	    var settings = $.extend({
		  maxHeight: 0,
		  callback: function() {},
		  textarea_width_subtract: 8,
		  textarea_height_add: 3,
		  textarea_new_height_add: 0,
		  br_with_space: false
		}, options);
        
        this.filter('textarea').each(function() {
            
            var $this       = $(this),
                minHeight   = $this.height(),
                lineHeight  = $this.css('lineHeight');
			
			var textarea_width = parseInt($this.width()||$this.css('width')); //edit
			
            var shadow = $('<div></div>').css({
                position:   'absolute',
                top:        -10000,
                left:       -10000,
                width:      textarea_width - settings.textarea_width_subtract, //edit
                fontSize:   $this.css('fontSize'),
                fontFamily: $this.css('fontFamily'),
				fontWeight: $this.css('fontWeight'),
                lineHeight: $this.css('lineHeight'),
                resize:     'none',
				wordWrap:   'break-word' // added
            }).appendTo(document.body);
            
            var update = function() {
			    var new_width = parseInt($this.width()||$this.css('width')) - settings.textarea_width_subtract; //edit
			    shadow.css('width', new_width);
			    var prev_height = shadow.height();
				
				var val = this.value.replace(/</g, '&lt;')
                                    .replace(/>/g, '&gt;')
                                    .replace(/&/g, '&amp;');
				if (settings.br_with_space) {
					val = val.replace(/\n/g, '<br/>&nbsp;');
				} else {
					val = val.replace(/\n/g, '<br/>');
				}
									
				if ($.trim(val) == '') {
				  shadow.html('&nbsp;');
				} else {
                  shadow.html(val);
				}
				
				var new_height = Math.max(shadow.height() + settings.textarea_new_height_add, minHeight);
				new_height += settings.textarea_height_add;
				
				if (settings.maxHeight == 0 || new_height <= settings.maxHeight) {
                  $(this).css('height', new_height);
				  $(this).css('overflow-y', '');
				} else {
				  $(this).css('height', settings.maxHeight);
				  $(this).css('overflow-y', 'auto');
				}
				
				if (prev_height !== shadow.height()) {
				  settings.callback();
				}
            }
            $(this).change(update).keyup(update).keydown(update);
            update.apply(this);
        });
        return this;
    };
})(jQuery);

(function($) {
  var data_key = 'data_error_msg';
  $.fn.showErrorMsg = function(options) {
    this.filter('input').each(function() {
	  var settings = $.extend({
	    msg: '',
	    div_class: 'input_error_div',
	    msg_class: 'input_msg_div',
	    arrow_class: 'arrow',
		additional_left: 0
	  }, options);
	
	  var data = $(this).data(data_key);
	  if (data !== undefined && data !== null) {
	    var error_div = data.error_div;
		var msg_div = data.msg_div;
		var arrow_div = data.arrow_div;
		if (options === undefined) {
		  settings = data.settings;
		}
		var new_data = {error_div: error_div, msg_div: msg_div, arrow_div: arrow_div, settings: settings};
		$(this).data(data_key, new_data);
	  } else {
	    if (options === undefined) return;
	    var error_div = $('<div>');
	    var msg_div = $('<div>');
	    var arrow_div = $('<div>', {'class': 'arrow'});
		var new_data = {error_div: error_div, msg_div: msg_div, arrow_div: arrow_div, settings: settings};
		$(this).data(data_key, new_data);
	  }
	  
	  var $this = $(this);
	  var offset = $this.offset();
	  var input_height = parseInt($this.outerHeight(true) || $this.css('width'));
	  var input_width = parseInt($this.outerWidth(true) || $this.css('width'));
	  
	  msg_div.addClass(settings.msg_class);
	  
	  if (data === undefined) {
	    for (var i = 6; i > 0; i--) {
	      arrow_div.append($('<div>', {'class': 'row' + i}));
	    }
	    error_div.append(msg_div).append(arrow_div);
	    error_div.appendTo(document.body);
	  }
	  
	  error_div
		.addClass(settings.div_class);
		
	  if (settings.msg.length > 0) {
	    $(msg_div).text(settings.msg);
	  } else {
	    error_div.hide();
	    return;
	  }

	  error_div.show();
	  var left = offset.left + input_width - 3*input_width/4 - (parseInt(arrow_div.css('margin-left')) + (arrow_div.width()/2)) + settings.additional_left;
	  error_div.css('left', left);
	  
	  var top = offset.top - error_div.outerHeight(true) - 2;
	  error_div.css('top', top);

	  error_div.click(function(e) {
	    $(this).hide();
		e.preventDefault();
		e.stopPropagation();
		return false;
	  });
	});
	return this;
  };
  // hides and clears the error message
  $.fn.hideErrorMsg = function(options) {
    this.filter('input').each(function() {
	  var data = $(this).data(data_key);
	  if (data !== undefined && data !== null) {
	    data.settings.msg = '';
	    data.error_div.hide();
		$(this).data(data_key, data);
	  }
	});
	return this;
  };
})(jQuery);

(function($) {
$.fn.noNewLines = function() {
  this.filter('textarea').each(function() {
    $(this).bind('paste', function() {
      var get_pasted_text_func = function() {
        var curr_val = $(this).val();
        var temp_input = $('<input>', {'type': 'text', 'id': 'caption-temp-input'})
          .css({position: 'absolute', top: -10000, left: -10000})
          .appendTo(document.body);
        temp_input.val(curr_val);
        $(this).val(temp_input.val()).change(); // behave like input text field
        $(temp_input).remove();
      }.bind(this);
	  setTimeout(get_pasted_text_func, 150);
	});
  });
  return this;
};
})(jQuery);

(function($) {
var desc_key = 'option_description';
$.fn.optionDescription = function(options) {
  var settings = $.extend({
	div_class: 'option_description'
  }, options);
	  
  $(this).each(function() {
    var desc = $(this).attr('option_description');
	var add_top = 5;
    if (desc && desc.length > 0) {
	  var div = $(this).data(desc_key);
	  if (div) {
	    div.text(desc);
	  } else {
	    var offset = $(this).offset();
	    var new_div = $('<div>', {'text': desc, 'class': settings.div_class})
		  .css('position', 'absolute');
		//  .css('top', offset.top-$(this).height()-add_top)
		//  .css('left', (offset.left+($(this).outerWidth()/2)))
		//  .css('margin-left', -$(new_div).outerWidth()/2);
		//if ($(new_div).outerWidth()/2 > offset.left+($(this).outerWidth()/2)) {
		//  new_div.css('margin-left', offset.left);
		//}
		$(this).data(desc_key, new_div);
		new_div = $(this).data(desc_key);
		new_div.appendTo($(document.body)).hide();
		$(this).hover(function() {
		  var new_desc = $(this).attr('option_description');
		  var new_offset = $(this).offset();
		  var new_margin_left = -$(new_div).outerWidth()/2;
		  var new_left = new_offset.left+($(this).outerWidth()/2);
		  if (-new_margin_left > new_left) {
		    new_left = new_offset.left;
			new_margin_left = 0;
		  }
		  $(new_div)
		    .text(new_desc)
		    .css('top', (new_offset.top-$(this).height()-add_top))
			.css('left', new_left)
			.css('margin-left', new_margin_left)
		    .show();
		}, function() {
		  $(new_div).hide();
		});
	  }
	}
  });
  return this;
};
})(jQuery);


(function($) {
$.fn.infiniteScroll = function(options) {
  //window.console.log('---- infscrol');
  var $this = $(this);
  var settings = $.extend({
	loading_div: $('<div>', {'text': 'Loading...'}),
	loading_append_id: '',
	url: function() {},
	entries_class: '',
	entries_container: '',
	before_append_callback: function(data) {},
	with_entry_callback: function(data) {},
	no_entry_callback: function() {},
	waypoint_opts: {}
  }, options);

  $this.waypoint(function(event, direction){
    //window.console.log('---- waypoint');
    $this.waypoint('remove');
	$(settings.loading_append_id).append(settings.loading_div);
	var url = settings.url();
	if (url.length > 0) {
	  $.get(url, function(data) {
	  var data = $(data);
	    var new_entries = data.find(settings.entries_class);
	    if (settings.loading_append_id.length > 0) {
	       settings.loading_div.detach();
		}
	    if (new_entries.length > 0) {
		  settings.before_append_callback(data);
          $(settings.entries_container).append(new_entries);
		  $this.waypoint(settings.waypoint_opts);
		  settings.with_entry_callback(data);
	    } else {
	      settings.no_entry_callback();
	    }
	  });
	}
  }, settings.waypoint_opts);
  
  return this;
};
})(jQuery);
