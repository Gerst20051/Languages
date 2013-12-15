function enable_dragndrop(gal_opts, e_opts, m_opts, drop_opts) {
  var gal_conf = $.extend({
    initial_screen: true,
	is_editor: true,
	light_mode: true,
	user: ''
  }, gal_opts);
  
  var e_conf = $.extend({
    expanded_menu: 'True'
  }, e_opts);

  var m_conf = $.extend({
    user: {},
	num_gal: 0,
	username: '',
	login_first_time: '',
	logout_first_time: false,
	login_slug: ''
  }, m_opts);

  var gallery = new Gallery();
  gallery.INITIAL_SCREEN  = gal_conf.initial_screen;
  gallery.IS_EDITOR  = gal_conf.is_editor;
  gallery.LIGHT_MODE = gal_conf.light_mode;
  gallery.USER = gal_conf.user;
  
  var droplistener = new DropListener(gallery, drop_opts);
  var editor = new Editor(gallery, '', e_conf.expanded_menu);
  var presenter = new Presenter(gallery, document.body);
  var menu = new Menu(gallery, m_conf.user, m_conf.num_gal, m_conf.username, m_conf.login_first_time, m_conf.logout_first_time, m_conf.login_slug); 
}

function DropListener(gallery, opts) {
  this.settings = $.extend({
    hide_id: '',
	drop_callback: function() {}
  }, opts);
  this.gallery_ = gallery;
  this.gallery_.addListener(this.galleryListener_.bind(this));
}

DropListener.prototype.galleryListener_ = function(e) {
  if (e.type == Gallery.EVENT_ITEMSDROPPED) {
    $(this.settings.hide_id).hide();
	$('.main_logo_wrapper').css('z-index', '');
	$('body').css('overflow', 'hidden');
	this.settings.drop_callback();
  }
}