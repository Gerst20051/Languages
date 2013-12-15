function GalleryList(gallery) {
  this.gallery_ = gallery;
  this.gallery_.addListener(this.galleryListener_.bind(this));
}

GalleryList.prototype.galleryListener_ = function(e) {
  if (e.type == Gallery.EVENT_ITEMSDROPPED) {
    $('#my_galleries').fadeTo(0,'0');
	$('body').css('overflow', 'hidden'); // remove the scrollbar
	$('.main_logo_wrapper').css('z-index', '');
	$('.resultsheader ').hide();
  }
}

GalleryList.prototype.clearGallery = function(id, div, refresh_waypoint) {
    if (typeof refresh_waypoint == 'undefined') {
	  refresh_waypoint = false;
	}
	$.ajax({
		type: "GET",
		url: "/api/clear_gallery_from_history/"+id,
		cache: false,
		dataType: "html",
		async: true,
		beforeSend: function() {
			div.hide();
		},
		success: function(html, textStatus) {
			div.remove();
			if (refresh_waypoint) {
			  $.waypoints('refresh');
			}
			this.gallery_.callListeners_({
				type: Gallery.EVENT_GALLERY_HISTORY_CLEARED
			});
		}.bind(this),
		error: function (XMLHttpRequest, textStatus, errorThrown) {
			div.show();
		}
	});	
/*
  $.ajax({
    // TODO add url and pass the gallery_id, user_id
    //type: "GET",
	//url:
	//dataType:
	beforeSend: function() {
	  div.hide();
	},
	success: function() {
	  div.remove();
	},
	error: function() {
	  div.show();
	}
  });
*/

}