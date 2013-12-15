function Introduction(gallery) {
  this.gallery_ = gallery;
  this.gallery_.addListener(this.galleryListener_.bind(this));

  function loadPlUploadSwf(asAnotherOption){
        if (asAnotherOption){
                if ($('#pluploader').size() == 0){
				  var $timeline_header = $('div#introduction').find('#feed_header');
				  var plform = "<form id='plupload_form' enctype='multipart/form-data'><center><div id='pluploader' style='width:450px;margin-top:5px;border:solid 1px #aaa;padding:0;'><p>You browser doesn't have Flash, Silverlight, Gears, BrowserPlus or HTML5 support.</p></div></center></form>";
				  if ($timeline_header.length > 0) {
				    $timeline_header.before(plform);
				  } else {
				    //$('div#introduction').append(plform);
					//$('div#intro_video').before(plform);
					$('div#intro_video_below').before(plform);
					$.waypoints('refresh');
				  }
                }
        }else{
		  var start_msg = 'To start sharing';
		  if ($('#intro_video').length > 0) {
		    start_msg = 'To share as a guest';
		  }
		  var $form_cont = $('<div>', {'id':'plupload_cont', 'style': 'width:500px;margin:0 auto;display:none;'})
		    .append("<form id='plupload_form' enctype='multipart/form-data'><div id='pluploader' style='width:500px;margin-top:5px;border:solid 1px #aaa;padding:0;'><p>You browser doesn't have Flash, Silverlight, Gears, BrowserPlus or HTML5 support.</p></div></form>")
			.append('<br/><span class="desc" style="font-size:14px;color:#444;margin-top:0px;font-style-italic;">Minus is optimized for Firefox 3.6+ <a href="http://firefox.com" target="_blank"><img width="20px;" style="border:0px white;" src="/smedia/minus/images/firefox_icon.png" /></a>, Chrome <a href="http://www.google.com/chrome" target="_blank"><img width="20px;" src="/smedia/minus/images/chrome-icon.png" style="border:0px white;"/></a>, and Safari on Mac <a href="http://www.apple.com/safari/" target="_blank"><img width="20px;" src="/smedia/minus/images/safari-icon.png" style="border:0px white;"/></a>.</span>')
			.append("<p><span class='desc' style='font-size:14px;color:#444;'><b>If you're seeing this message, your browser does not support Drag & Drop upload.</b></span>");
		  
		  var $intro2 = $('div#introduction2').html('')
		    .css('width', '500px')
			.append('<center class="lb_howto">' + start_msg + ', <span class="lb_link">select your files</span>.</center>')
			.append($form_cont);
			
		  $intro2.find('.lb_link').click(function() {
		    $(this)
			  .unbind('click')
			  .addClass('inactive');
			$form_cont.show();
			$.waypoints('refresh');
			$('#feed_user_container').css('padding-top', '470px');
		  });
        }
	$("#pluploader").pluploadQueue({
		// General settings
		runtimes : 'html5, flash, html4', 
		url : '/api/plupload/',
		unique_names : false,
		multipart: true,
		max_file_size : Gallery.MAX_FILE_SIZE_PLUPLOAD,
		browse_button: 'more_options',
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
				$("#pluploader").find('.plupload_container').removeAttr('title');
			});
		}
	});
  }
  
  var $feed_info = $('#feed_user_container');
  var $introduction = $('#introduction');

  if (isValidBrowser()) {
    $('#introduction2').hide();
    $('#intro-bg2').hide();
    $introduction.css('display','block');
    
	$('#feed_header').appendTo('#introduction').show();
	
	setTimeout(function() {
      //$introduction.css('top', '40%');
	  $introduction.css('top', '20%').css('margin-top', '-80px');
	  if ($('#feed_container').length > 0) {
	    $introduction.css('top', '0').css('margin-top', '0');
		$feed_info.css('padding-top', '130px');
	    $introduction.attr('class', 'sticky_intro');
	  } else {
	    $('.main_logo_wrapper').css('z-index', '');
	  }
	  $introduction.fadeTo(0,1);
    }, 1);
    $('#intro-bg').addClass('compat').show();
    $('#instructions').css('display','block').show();
    $('#introduction').show();
    $('#more_options').click(function(){
        loadPlUploadSwf(true);
		if ($('#feed_container').length == 0) {
		  //var new_div = $('<div>', {'style': 'height: 100px;'});
	      //$('#introduction').find('form').after(new_div);
		} else {
		  $feed_info.css('padding-top', '375px');
		  $.waypoints('refresh');
		}
      $('span.browse_section').hide();
	  $('span.browse_opened').show();	  
      return false;
    });
  } else {
    loadPlUploadSwf(false);
	var $introduction2 = $('#introduction2');
	//$('#feed_header').show();
	//$feed_info.appendTo('#introduction2');
    $introduction2.css('display','block');

	$("#pluploader").pluploadQueue().trigger('refresh'); // http://www.plupload.com/punbb/viewtopic.php?id=391
    setTimeout(function() {
      $introduction2.css('top','35%');
	  if ($('#feed_container').length > 0) {
	    //$introduction2.css('top', '80px').css('margin-top', '0');
		// new testing..
		$('#feed_header').appendTo('#introduction2').show();
	    $introduction2.addClass('sticky_intro').css('top', '0').css('margin', '0').css('width', '100%');
	    $feed_info.css('padding-top', '130px');
	  }
	  if (BrowserDetect.browser != 'Explorer' || (BrowserDetect.browser == 'Explorer' && BrowserDetect.version >= 9)) {
        //$('#introduction2').fadeTo(0,'100%');  // feed doesn't show in ie7/8 when this is called
	    $introduction2.fadeTo(0, 1);
      }
	  if ($('#intro_video').length > 0) {
	    $introduction2.css('top', '80px').css('margin-top', '0').css('width', '500px');
		//$('#howtotext').prependTo('#introduction2').text('To share as a guest, select your files.').css('padding', '10px 0 0');
		$('#signup_msg').prependTo('#introduction2').css('width', '100%');
	    $('#intro_video').prependTo('#introduction2').css('margin', '0 auto');
		$('#intro_video_below').appendTo('#introduction2');
	  }
    }, 1);
	
    $('#lowbrowser').css('display','block');
	if (BrowserDetect.browser == 'Explorer' && BrowserDetect.version == 7) {
	  //var $intro2_parent = $introduction2.parent();
	  $('div.presentation').css('z-index', '3000').hide();
	}
  }
}

Introduction.prototype.galleryListener_ = function(e) {
  if (e.type == Gallery.EVENT_ITEMSDROPPED) {
    //$('#intro-bg').fadeTo(0,'0');
    $('#introduction, #signup_msg, #feed_user_container').fadeTo(0,'0').css('top','60%').hide();
	$('.main_logo_wrapper').css('z-index', '');
	/*
    setTimeout(function() {
      //$('#intro-bg').hide();
      $('#introduction').hide();
    }, 550);
	*/
  }
}
