// Authentication: updated 1/28/2011
// Dependencies: jQuery 
(function($){

var _cbStr = (typeof _cbVarStr!=="undefined") ? _cbVarStr : "b1002";
	
var CONST_ThirdPartyConfig = {
	recaptcha: {
		url: "/ajax/authentication/recaptcha.jsp",
		elmId: "recaptcha_div",
		id: "6LcspgwAAAAAAPIfJ3yUt1aUl_Thr8TQfQmR1oRP", // this is a global ID newly defined for all of fox domains.
		settings: {
			theme: "clean"
		}
	},
	disqus: {
		widget: "DQIntegrationService",
		cookie: {
			name: "p_DQS",
			expires: 14 // days
		},
		hash: {
			empty: "e30=",
			expires: 60 // in minutes
		}
	},
	idms: {
		cookie: {
			name: "p_IDMS",
			expires: 14 // days
		},
		hash: {
			empty: "e30=",
			expires: "5" // minutes
		},
		callback: {
			login: "FOX_IDMS_auth_logged_in",
			logout: "FOX_IDMS_auth_logged_in"
		}
	}
};

var AuthService = window.AuthService = function(initVars) {
	this.validationUrl = '/authentication/registration/validate/';
	this.getRegistrationDataUrl = '/authentication/registration/get';
	this.loginSubmitUrl = '/authentication/login/submit';
	this.logoutSubmitUrl = '/authentication/logout/submit';
	this.requestLogoutUrl = '/authentication/logout/submit';
	this.refreshAuthUrl = '/authentication/login/refresh?'+_cbStr+"=t";
	this.forgottenPasswordSubmitUrl = '/authentication/forgotten_password/submit';
	this.registrationSubmitUrl = '/authentication/registration/submit';
	this.ajaxRetrieveHtmlUrl = '/ajax/authentication/includes.jsp';
	this.newsAlertSubscriptionUrl = '/portal/newsalertsubscribe';
	this.defaultDomain = 'foxnews.com';
	for (i in initVars) {
		this[i] = initVars[i];
	}
};

AuthService.prototype = {
	init: function(config) {
		var as = this;
		for (i in config) {
			this[i] = config[i];
		}
		
		this.thirdPartyAPI.root = this; // extend
		this.thirdPartyAPI.init();
	},
	createObjFromJSON: function(jsonString) {
		
		if (typeof jsonString==="string") {
			if (($.trim(jsonString)).length>0) {
				return eval('('+unescape(jsonString).replace(/\+/g, " ")+')');
			}
		} 
		return false;
	},
	isLoggedIn: function() {
		if($.cookie('p_UN')=='undefined' || $.cookie('p_UN')=='' || $.cookie('p_UN')==null){
			return false;
		} else {
			return true;
		}
	},
	isThirdParty: function() {
		if($.cookie('p_Thirdparty')!='undefined' && $.cookie('p_Thirdparty')!=null){
			return true;
		} else {
			return false;
		}
	},
	getDisplayName: function() {
		var as = this;
		
		// piped in setup
		function pipeCheck(str) {
			var separator = "|id:";
			var idx = str.lastIndexOf(separator);
			if (idx<1) { return str; }
			return str.substr(0,idx);
		}
		
		if (as.isLoggedIn()){
			var displayName = false;
			if (typeof $.cookie("p_DN")==="string") {
				var dn = $.cookie("p_DN");
				if (dn.length>0) {
					dn = pipeCheck(dn);
					displayName = dn.replace(/\+/g," ").replace(/\"/g,"");
				}
			}
			return (displayName) ? displayName : as.getUsername();
		}
		return "";
	},
	getUsername: function() {
		var as = this;
		if(as.isLoggedIn()){
			return $.cookie('p_UN');
		}
	},
	getUserEmail: function() {
		var as = this;
		if (as.isLoggedIn()){
			return $.cookie('p_EM');
		} else {
			return "";
		}
	},
	getUserLocation: function() {
		var as = this;
		if (as.isLoggedIn()){
			return $.cookie('p_UL');
		} else {
			return "";
		}
	},
	
	// AJAX to get overlay HTML
	// - Supports: 'login-provider', 'registration-overlay', 'forgot-password-overlay'
	getOverlayHtml: function(overlayView, origDomain, callbackFn){
		callbackFn = callbackFn || false;
		var as=this;
		if(origDomain==null){origDomain=as.defaultDomain;}
		$.ajax({
			type:"GET",
			async:false,
			url:as.ajaxRetrieveHtmlUrl,
			data:({view:overlayView,authDomain:origDomain,cb:_cbStr}),
			success:function(data,textStatus){
				$('body').append(data);
				if (typeof callbackFn==="function") { callbackFn(); }
			}
		});
	},
	
	closeOverlay: function(elementId){
		var as = this;
	    $('#' + elementId).find('input[type*=text]').val('');
	    $('#' + elementId).find('input[type*=password]').val('');
	    $('#' + elementId).find('input:checkbox:checked').attr('checked', '');
	    $('#' + elementId).find('input[type*=text]').removeClass('error');
	    as.overlay.hide('#'+elementId);
	},
	
	providerOverlay: function(authDomain,callbackFn){
		var as = this;
		if(!as.isLoggedIn()){
			// ajax to get login-provider html and put into obj
			if($('#login-provider').length==0) {
				as.getOverlayHtml('login-provider',authDomain,callbackFn);
			}
			
			as.initProviderOverlay();
			
			as.overlay.show("#login-provider");
			var selfAdUnit = $(".overlay-module .ad"), adUnit = $(".ad");
			adUnit.not(selfAdUnit.removeClass("unitRem")).addClass("unitRem");
			$('#user-login-form-overlay').find('input[name*=username]').focus();
			
			return false;
		}
		return true;
	},
	
	initProviderOverlay: function(){
		var as = this;	
		
		var login_validation_options = {
		    onkeyup: false,
	    	errorPlacement: function(error, element) {
			},
			invalidHandler: function(form, validator) {
				$(this).find('input[type*=submit]').attr("disabled", "disabled"); 
				var errors = validator.numberOfInvalids();
				if (errors) {
					var message = 'User name and password are required.';
					$(this).find(".error-box-login").html(message).show();
				} else {
					$(this).find(".error-box-login").hide();
				}
				$(this).find('input[type*=submit]').removeAttr("disabled"); 
			},
			submitHandler: function(form) {
				$(form).find('input[type*=submit]').attr("disabled", "disabled");
				$(form).find(".error-box-login").hide();
				as.login($(form));
				return false;
			}
		};
		$('#user-login-form-overlay').validate(login_validation_options);
		
		$('.thirdparty a img').hover(function() {
            var thirdPartyTooltip = $(this).parent().parent().siblings(0);
			thirdPartyTooltip.find('.icon-desc-bg p').html($(this).attr("alt"));
			thirdPartyTooltip.addClass($(this).attr("name")).css('visibility','visible');
	    	},
		    function() {
		    	var thirdPartyTooltip = $(this).parent().parent().siblings(0);
		    	thirdPartyTooltip.find('.icon-desc-bg p').html('');
				thirdPartyTooltip.removeClass($(this).attr("name")).css('visibility','hidden');
		    }
		);
		
		var reset_password_validation_options = {
	    	onkeyup: false,
		    errorPlacement: function(error, element) {
			},
			invalidHandler: function(form, validator) {
				$(this).find('input[type*=submit]').attr("disabled", "disabled"); 
				var errors = validator.numberOfInvalids();
				if (errors) {
					var message = 'The email address you entered is invalid.';
					$(this).find(".error-box-login").html(message).show();
				} else {
					$(this).find(".error-box-login").hide();
				}
				$(this).find('input[type*=submit]').removeAttr("disabled"); 
			},
			submitHandler: function(form) {
				$(form).find('input[type*=submit]').attr("disabled", "disabled");
				$(form).find(".error-box-login").hide();
				as.forgotPassword($(form));
				return false;
			}
    	};
    	$('#reset-password .user-email-reset-form').validate(reset_password_validation_options);
    	
    	$('#forgot-password-overlay').click(function(){
 		   $('#reset-password.reset').toggle();
 		});
	},
	
	registrationOverlay: function(obj, authDomain, callbackFn){
		var as = this;
		if(!as.isLoggedIn()){
			if($('#registration-overlay').length==0) {
				as.getOverlayHtml('registration-overlay',authDomain, callbackFn);
			}
			
			increaseRefresh();
			$('#reg_user_name').removeAttr('readonly');
	        $('#reg_user_name').next('.validate').next('.validate').children('.sprites').show();
			
			as.initRegistrationOverlay();
			
	        as.overlay.show("#registration-overlay");
	        var selfAdUnit = $(".overlay-module .ad"), adUnit = $(".ad");
			adUnit.not(selfAdUnit.removeClass("unitRem")).addClass("unitRem");
	        //$('#reg_user_name').focus();
	        
			return false;
		}
		return true;
	},
	
	initRegistrationOverlay: function(){
		var as = this;
		var registration_validation_options = {
			submitHandler: function(form){
				as.register($(form));
				return false;
			},
			errorElement: 'p',
			errorPlacement: function(error, element) {
		        var elem_pos = $(element).position();
		        var error_message = $(error[0]).html();
		        var error_name = $(element).attr('id').replace('reg','err');
		        var error_html = '<div class="regoverlay_container error-tip" id="'+error_name+'">'+
					'<div class="overlaytip_top"></div><div class="form-note">'+
					'<img src="/static/all/img/alert-yellow.gif" alt="" /><p class="err-msg">'+error_message+
					'</p></div><div class="overlaytip_btm"></div></div>';
		        $('div.regoverlay_container.error-tip').hide();
		        
				if ($('div#'+error_name).length>0) {
					$('div#'+error_name+' p.err-msg').html(error_message);
					if (error_message != '')
					   $('div#'+error_name).show();
				} else {
					$(element).after(error_html);
					if (error_message == '')
					   $('div#'+error_name).hide();
				}
			
				if ($("#err_user_name").is(":visible")) {
					$(".req-user").remove();
				}
				
				if ($("#err_password").is(":visible")) {
					$(".req-password").remove();
				}
			
				$('div#'+error_name).css('top', (elem_pos.top-70)+'px');
				$('div#'+error_name).next('.validate').children('.sprites').attr('class','sprites red-exclamation');
				$('div#'+error_name).next('.validate').children('.sprites').show();
			},
			success: function(label) {
				if (!$("input#agreement").is(":checked")) {
					$("div#agreement, div#agreement .err-msg").css("display","block");
				}
				var errDiv = 'div#'+$(label).attr('htmlfor').replace('reg','err');
				$(errDiv).hide();
				$(errDiv).next('div.validate').children('a.sprites').attr('class','sprites check');
			},			
			rules: {
				propID_username: {required:true, minlength:5, maxlength:15, validChars:true,
					remote: {url: as.validationUrl, type: "post"}
				},
				propID_email: {required:true, email:true, maxlength:250, strictEmail:true,
					remote: {url: as.validationUrl, type: "post"}			
				},
				propID_pass1: {required:true, noSpaces:true, minlength:5, maxlength:12},
				propID_pass2: {required:true, noSpaces:true, minlength:5, maxlength:12, equalTo: "#reg_password"},
				propID_country: {required:true},
				propID_zip: {digits:true, maxlength:5, minlength:5, zipRequired: true, zipCheck: true,
					remote: {url: as.validationUrl, type: "post"}
				},
				propID_birthyear: {required:true, min:1900, isMaxYear:true},
				propID_terms: {required: true}
			},
			messages: {
				propID_username: {
					required: "Please enter a username",
					minlength:"Your username must consist of at least 5 characters",
					maxlength:"Your username is too long",
					validChars: "please supply valid characters only.",
					remote:"This username is already in use."
				},
				propID_email: {
					required: "Please enter an email address.",
					email: "Your email address must be valid.",
					maxlength:"Your email address is too long.",
					remote:"A user is already registered with this email."
				},
				propID_pass1: {
					required: "Please enter a password.",
					minlength:"Your password must be at least 5 characters.",
					maxlength:"Your password is too long."
				},
				propID_pass2: {
					required: "Please confirm your password.",
					equalTo: "Your passwords do not match."
				},
				propID_terms: {
					required: "In order to join Fox News community you must agree to our terms and conditions. Please review and check the box to join."
				},
				propID_country: {
					required: "Please select your country of residence."
				},
				propID_zip: {
					digits:"Please enter a valid zip code.",
					maxlength:"A valid zip code is five digits.",	
					minlength:"A valid zip code is five digits.",
					required: "Please enter a zip code.",
					remote:"Please enter a valid United States zip code."
				},
				propID_birthyear: {
					required:"Please select your birth year." 
				}
			}
		};
		
		as.instantiateValidators();
		
		$("a.sprites.question").tooltip({
			track:true,
			showURL: false,
			bodyHandler: function(){
				var tt_html = '<div class="regoverlay_container error-tip">'+
					'<div class="overlaytip_top"></div><div class="form-note">'+
					'<p class="err-msg">'+this.tooltipText+'</p></div>'+
					'<div class="overlaytip_btm"></div></div>';
				return tt_html;
			}
		});
		
		$("#registration").validate(registration_validation_options);
		as.showRecaptcha();
		
		// change links for foxbusiness
		if (as.domain) {
			if (as.domain==="foxbusiness.com" || as.domain==="smallbusiness.foxbusiness.com") {
				try { 
					fox.site.linkChange(); 
				} catch(err) { 
					// fail silently
				}
			}
		}
	},
	
	initOpxLogin: function(){
		var as = this;
		
		var reset_password_validation_options = {
	    	onkeyup: false,
		    errorPlacement: function(error, element) {
			},
			invalidHandler: function(form, validator) {
				$(this).find('input[type*=submit]').attr("disabled", "disabled"); 
				var errors = validator.numberOfInvalids();
				if (errors) {
					var message = 'The email address you entered is invalid.';
					$(this).find(".error-box-login").html(message).show();
				} else {
					$(this).find(".error-box-login").hide();
				}
				$(this).find('input[type*=submit]').removeAttr("disabled"); 
			},
			submitHandler: function(form) {
				$(form).find('input[type*=submit]').attr("disabled", "disabled");
				$(form).find(".error-box-login").hide();
				as.forgotPassword($(form));
				return false;
			}
    	};
    	$('#reset-password .user-email-reset-form').validate(reset_password_validation_options);
	},
	
	initEditProfile: function() {
		var as = this;	
		
		var edit_profile_validation_options = {
			debug: false,
			submitHandler: function(form) {
				if ($('#reg_email').val() == '' && $('#reg_email2').val() == '') {
					$('#reg_email').val($('#reg_curemail').val());
					$('#reg_email2').val($('#reg_curemail').val());
				}
				form.submit();
				return false;
			},
			errorElement: 'p',
			errorPlacement: function(error, element) {
			    var elem_pos = $(element).position();
				var error_message = $(error[0]).html();
				var error_name = $(element).attr('id').replace('reg','err');
				var error_html = 
					'<div class="regoverlay_container error-tip" id="'+error_name+'">'+
					'<div class="overlaytip_top"></div>'+
					'<div class="form-note">'+
					'<img src="/static/all/img/alert-yellow.gif" alt="" /><p class="err-msg">'+error_message+
					'</p></div>'+
					'<div class="overlaytip_btm"></div>'+
					'</div>';
			        $('div.regoverlay_container.error-tip').hide();
				if ($('div#'+error_name).length>0) {
					$('div#'+error_name+' p.err-msg').html(error_message);
					if (error_message != '')
					   $('div#'+error_name).show();
				} else {
					$(element).after(error_html);
					if (error_message == '')
					   $('div#'+error_name).hide();
				}
			    $('div#'+error_name).css('top', (elem_pos.top-80)+'px');
			    $('div#'+error_name).css('left', (elem_pos.left+50)+'px');
				$('div#'+error_name).next('.validate').children('.sprites').attr('class','sprites red-exclamation');
				$('div#'+error_name).next('.validate').children('.sprites').show();
			},
			success: function(label) {
				var errDiv = 'div#'+$(label).attr('htmlfor').replace('reg','err');
				$(errDiv).hide();
				$(errDiv).next('div.validate').children('a.sprites').attr('class','sprites check');
			},
			rules: {
				propID_email: {required:false, email:true, maxlength:250, strictEmail:true},
				propID_email2: {equalTo: "#reg_email"},
				propID_pass1: {noSpaces:true, minlength:5, maxlength:12},
				propID_pass2: {noSpaces:true, minlength:5, maxlength:12, equalTo: "#reg_password"},
				propID_country: {required:true},
				propID_zip: {digits:true, maxlength:5, minlength:5, zipRequired: true, zipCheck: true},
				propID_gender: {required:true},
				propID_birthyear: {required:true, min:1900, isMaxYear:true}
			},
			messages: {
				propID_email: {
					required: "Please enter an email address.",
					email: "Your email address must be valid.",
					maxlength:"Your email address is too long.",
					remote:"A user is already registered with this email."
				},
				propID_email2: {
					required: "Please confirm your email address.",
					equalTo: "Please double check your email. They do not match."
				},
				propID_pass1: {
					required: "Please enter a password.",
					minlength:"Your password must be at least 5 characters.",
					maxlength:"Your password is too long."
				},
				propID_pass2: {
					required: "Please confirm your password.",
					equalTo: "Your passwords do not match."
				},
				propID_country: {
					required: "Please select your country of residence."
				},
				propID_zip: {
					digits:"Please enter a valid zip code.",
					maxlength:"A valid zip code is five digits.",	
					minlength:"A valid zip code is five digits.",
					required: "Please enter a zip code."
				},
				propID_gender: {
					required:"Please submit your gender."
				},

				propID_birthyear: {
					required:"Please select your birth year." 
				}
			}
		};
		as.instantiateValidators();
		$("#reg_profile").validate(edit_profile_validation_options);
	},
	
	forgotPasswordOverlay: function(obj){	
		var as = this;
		if(!as.isLoggedIn()){
			if($('#forgot-password-overlay').length==0) {
				as.getOverlayHtml('forgot-password-overlay');			
			}
			
			var offset = $(obj).offset();
			var leftPos = offset.left - $('#forgot-password-overlay').width();
			/*$('#forgot-password-overlay').css({'position':'absolute', 'left':leftPos+'px', 'top':'10px'});
			$('#forgot-password-overlay').show();*/
			
			as.overlay.show("#forgot-password-overlay");
			
			return false;
		}
		return true;
	},
	
	login: function(formObj){
		var as = this;
	    var user = $(formObj).find('input[name*=username]').val(); 
	    var pass = $(formObj).find('input[name*=password]').val();
	    var rememberMe = ($(formObj).find('input[name*=rememberMe]:checked').length > 0 )?'true':'false'; 
	    var numOfDays = 365;
	    if(rememberMe=='false'){numOfDays = 0;}
		$.ajax({
			type: "POST",
			url: as.loginSubmitUrl,
			data: ({username:user, password:pass, authDomain:as.domain, rememberMe:rememberMe}),
			dataType: "text",
			success: function(data, textStatus){
				var loginResponse = as.createObjFromJSON(data);
				if (!loginResponse) { showToConsole("[authSrv.login] No data returned from request.",true); return false; }
				if(loginResponse.authentication.status=='ok'){

					if (as.domain!=="foxnews.com") { window.location.reload(); }
					as.onSuccessLogin();
					as.thirdPartyAPI.disqus.login(loginResponse); // disqus
					as.thirdPartyAPI.idms.login(loginResponse); // idms
					as.closeOverlay('login-provider');
					/*	DEPRECATED: authentication is not tied up to a new commenting on vcs anymore
					if (typeof vcs_service!=="undefined") {
						if (typeof vcs_service.newComment!=="undefined") {
							vcs_service.newComment();
						}
					}
					*/
				} else {
					$(formObj).find('input[type*=submit]').removeAttr("disabled"); 
					$(formObj).find('.login-error').html('We are sorry, please try again.').show();	
				}
			},
			error:function(XMLHttpRequest, textStatus, errorThrown){
				$(formObj).find('input[type*=submit]').removeAttr("disabled"); 
				$(formObj).find('.login-error').html('We are sorry, please try again later.').show();
			}
		});
	},
	
	logout: function(origDomain){
		var as = this;
		if(origDomain==null){origDomain=as.defaultDomain;}
		$.ajax({
			type: "POST",
			url: as.logoutSubmitUrl,
			data:({authDomain:origDomain}),
			success: function(data, textStatus){
				if(data) {
					var authObj = as.createObjFromJSON(data);
					if (!authObj) { showToConsole("[authSrv.logout] No data returned from request.",true); return false; }
					if(authObj.authentication.status=='ok'){
						// disqus
						as.thirdPartyAPI.disqus.logout(authObj,function(){
							as.thirdPartyAPI.idms.logout(authObj, function(){
								if (window.location.pathname.indexOf("/community/user/profile")>-1) { 
									// if already on profile page, redirect to index
									window.location.href = 'http://www.foxnews.com/index.html';
								} else {
									window.location.reload();
								}
							});
							
						});
					}
				}
			},
			error:function(XMLHttpRequest, textStatus, errorThrown){
			}
		});
	},
	
	// valid logged out hash
	requestLogOut: function(config) {
		config = config || {};
		var as = this;

		$.ajax({
			type: "GET",
			url: as.requestLogoutUrl,
			success: function(data) {
				var authObj = as.createObjFromJSON(data);
				if (!authObj) { showToConsole("[authSrv.requestLoginInfo] No data returned from request.",true); return false; }
				if (typeof config.callback==="function") {
					config.callback(authObj);
				}
			}
		});
	},
	
	// hash validation
	requestLoginInfo: function(config) {
		config = config || {};
		var as = this;
		
		$.ajax({
			type: "POST",
			url: as.refreshAuthUrl,
			success: function(data) {
				var authObj = as.createObjFromJSON(data);
				if (!authObj) { showToConsole("[authSrv.requestLoginInfo] No data returned from request.",true); return false; }
				if (typeof config.callback==="function") {
					config.callback(authObj);
				}
			}
		});
	},
	
	forgotPassword: function(formObj){
		var as = this;
	    var email = $(formObj).find('input[name*=emailResetField]').val();
		$.ajax({
   			type: "POST",
   			url: as.forgottenPasswordSubmitUrl,
   			data: ({email:email}),
			dataType: "text",
   			success: function(data, textStatus){
				var forgotPasswordResponse = as.createObjFromJSON(data);
				if (!forgotPasswordResponse) { showToConsole("[authSrv.forgotPassword] No data returned from request.",true); return false; }
				if(forgotPasswordResponse.authentication.status=='ok'){
					$(formObj).find('.error-box-login').hide();
					$(formObj).find('.reset-input-container').hide();
					$(formObj).find('.confirm-box-login').html('A reset link has been sent to the email provided.').show();
				} else {
					$(formObj).find('input[type*=submit]').removeAttr("disabled"); 
					$(formObj).find('.error-box-login').html('The email address you entered is invalid').show();
				}
			},
			error:function(XMLHttpRequest, textStatus, errorThrown){
				$(formObj).find('input[type*=submit]').removeAttr("disabled"); 
				$(formObj).find('.error-box-login').html('We re sorry your password cannot be reset at this time. Please try again later.').show();
			}
	 	 });
	},
	
	register: function(formObj){
		var as = this;
		var agreement = ($('#agreement:checked').length > 0 )?'true':'false';
		var fnbreakingnews = ($('#fnbreaking:checked').length > 0 )?'Yes':'No';
		var fbbreakingnews = ($('#fbbreaking:checked').length > 0 )?'Yes':'No';
		var privacy = ($('#reg_location_confirm:checked').length > 0 )?'hide':'';
		var challenge = as.thirdPartyAPI.recaptcha.getChallenge();
		var response = as.thirdPartyAPI.recaptcha.getResponse();
		
		//whatcounts
		var slids='';
		if (fnbreakingnews=='Yes'){
			slids+='C2F278094FACCEA62391025B7A52D8EB';
		}
		if (fbbreakingnews=='Yes'){
			if (slids!=''){slids+=','};
			slids+='5C84B893BD6D939E84FAE1A8E6E9525A';
		}		
		if (slids!=''){
			$.ajax({
				type: "GET",
				url: as.newsAlertSubscriptionUrl,
				data: ({
					slids:slids,
					email:$('#reg_email').val()}),
				dataType:"text",
				success: function(data, textStatus){
					// skip
				},
				error:function(XMLHttpRequest, message, errorThrown){
					// skip
				}
			});
		}
		
		$.ajax({
			type: "POST",
			url: as.registrationSubmitUrl,
			data: ({
				authDomain:as.domain,
				realm: "realm1",
				username: $('#reg_user_name').val(),
				password: $('#reg_password').val(),
				email: $('#reg_email').val(),
				zip: $('#reg_zip_code').val(),
				country: $('#reg_country').val(),
				location_privacy: privacy,
				birth_year: $('#reg_birth_year').val(),
				//breaking_news: breakingnews,
				recaptcha_challenge_field: challenge,
				recaptcha_response_field: response}),
			dataType:"text",
			success: function(data, textStatus){				
				var registrationResponse = as.createObjFromJSON(data);
				if (!registrationResponse) { showToConsole("[authSrv.register] No data returned from request.",true); return false; }
				if(registrationResponse.authentication.status=='ok'){
					$('#registration-error').hide();
					$('#registration-overlay').hide();
					as.getOverlayHtml('registration-thankyou-overlay');
					decreaseRefresh();
					/*	DEPRECATED: authentication is not tied up to a new commenting on vcs anymore
					if (typeof vcs_service!=="undefined") {
						if (typeof vcs_service.newComment!=="undefined") {
							vcs_service.newComment();
						}
					}
					*/
					authentication_service.onSuccessLogin();
					as.thirdPartyAPI.disqus.login(registrationResponse);  // disqus
					as.overlay.show("#registration-thankyou-overlay");
					
					$.scrollTo('#registration-thankyou-overlay');
					$('#clickToClose').focus();
				} else {
					var errorMsg = registrationResponse.authentication.message;
					as.showRecaptcha();
					$('#registration-error').html('We are sorry, we could not complete your registration. Please fix the error and try again. '+ '<br> *'+errorMsg).show();
					$.scrollTo('#registration-error');
				}
			},
			error:function(XMLHttpRequest, message, errorThrown){
				as.showRecaptcha();
				$('#registration-error').html('We are sorry, we could not complete your registration. Please try again.'+ '<br> *'+ message).show();
				$.scrollTo('#registration-error');
			}
		});		
	},
	
	overlay: {
		init: function(action,target) {
			target = target||"#registration-overlay";
			var thisObj = this;
			$(action).each(function() {
				$(action).unbind("click").click(function() {
					thisObj.show(target);
				});	
			});
		},
		
		show: function(target) {
			var thisObj = this;
			if (typeof(increaseRefresh)!=='undefined') { increaseRefresh(); }
			$(target+" .validate a.sprites").unbind("click");
			if ($(".overlay-layer").size() < 1) {
				$("body").prepend('<div class="overlay-layer"></div>');
			}
			$(".overlay-layer").show();
			$(target).each(function(){
				$(this).show().fixedBox();
				if ($(window).height() < $(this).height()) {
					var scrollTop = $(document).scrollTop();
					var overlayTop = ($(this).css("top")).replace(/[a-z]/g,'');
					var abs = Math.abs(overlayTop)+scrollTop;
					$(this).css({
						top: abs+"px",
						position: "absolute"
					});
				}
				
				// reset items
				$(".field input",this).removeClass("error").attr("value","");
				$(".validate a.sprites,.error-tip",this).hide();
				
				var usr = $("#reg_user_name",this);
				var usrParent = $(usr).parent();
				if ($("span.req-text",usrParent).size()<1) {
					$(usr).after('<span class="req-text req-user">5-15 characters, cannot be changed</span>');
					$(usrParent).focus(function(){ $(".req-user",this).remove(); });
				}
				var pwd = $("#reg_password",this);
				var pwdParent = $(pwd).parent();
				if ($("span.req-text",pwdParent).size()<1) {
					$(pwd).after('<span class="req-text req-password">5-12 characters, no space, case-insensitive</span>');
					$(pwdParent).focus(function(){ $(".req-password",this).remove(); });
				}
			});

			var close = $(target+" .close").parent();
			var signUpReg = $(".sign_up_registration");
			$(close).unbind("click").attr("onclick","").click(function(){thisObj.hide(target);});
			$(target+" .buttons input[type='reset']").click(function(){thisObj.hide(target);});
			signUpReg.click(function(){$(".overlay-layer").show();});
		},
		hide: function(target) {
			if (typeof(decreaseRefresh)!=='undefined') { decreaseRefresh(); }
			$(".overlay-layer").hide(); 
			$(".ad").removeClass("unitRem");
			$(target).hide();
		}
	},
	
	instantiateValidators: function() {
		var as = this;
		// check for unwanted characters
		$.validator.addMethod('validChars', function(value) {
			var result = true;
			var iChars = "!@#$%^&*()+=-[]\\\';,./{}|\":<>?";
			for (var i = 0; i < value.length; i++) {
				if (iChars.indexOf(value.charAt(i)) != -1) {
					return false;
				}
			}
			return result;
		}, 'Please supply valid characters only.');

		// stricter email address character set check
		$.validator.addMethod('strictEmail', function(value) {
			return value.match(/^[\w\-\.]+@[\w\-\.]+$/) != null || value == '';
		}, 'Please supply valid email characters only.');

		// check for spaces
		$.validator.addMethod('noSpaces', function(value) {
			var result = true;
		    if (value!="") { 
		    	var strArr = new Array();
		    	strArr = value.split(" ");
		    if (strArr.length > 1){
		    	return false;
		    }
	    }        
		return result;
		}, 'Please remove the blank spaces.');
		
		// check for name characters
		$.validator.addMethod('validCharsName', function(value) {
			var resultName = true;	
			var iChars = "!@#$%^&*()+=[]\\\;,./{}|\":<>?";
			var iCharshp = "-";
			var iCharsap = "'";
			var counterhp = 0;
			var counterap = 0;
			for (var i = 0; i < value.length; i++) {
				if (iChars.indexOf(value.charAt(i)) != -1) {
					return false;
				}
				if(iCharshp.indexOf(value.charAt(i)) != -1){
					counterhp = counterhp+1;
				}
				if(iCharsap.indexOf(value.charAt(i)) != -1){
					counterap = counterap+1;
				}
			}
			if(counterhp > 1){
				return false;
			}
			if(counterap > 1){
				return false;
			}
			return resultName;
		}, 'Please supply valid characters only.');
		
		// is max year
		$.validator.addMethod('isMaxYear', function(value) {
			var result = true;	
			var d = new Date();
			var curr_year = d.getFullYear();
			if("#reg_birth_year" > (curr_year - 13)){
				return false;
			}
			return result;
		}, 'You must be at least 13 years old to register for this site.');
		
		$.validator.addMethod('zipRequired', function(value) {
			var result = true;	
			country = $("#reg_country").val();
			if(country == "United States" && value == ""){
				return false;
			}
			return result;
		}, 'Zip Code is required.');
		
		$.validator.addMethod('zipCheck', function(value) {
			var result = true;
			if ($('#reg_country').val() == 'United States') {
				$('#reg_location').show();
				$.ajax({
					type: "GET",
					url: as.getRegistrationDataUrl,
					async: false,
					data: ({zip:$('#reg_zip_code').val()}),
				    dataType: "text",
				    success: function(data, textStatus){
						if (data){
							var stateObj = as.createObjFromJSON(data);
							if (!stateObj) { showToConsole("[authSrv.validator] No data returned from request.",true); return false; }
							var state = stateObj.state_for_zip;
							if (state != '') {
								$('#reg_location').val(state);
							} else {
								$('#reg_location').val('');
								result = false;
							}
						} else {
							$('#reg_location').val('');
							result = false;
						}
				    },
				    error:function(XMLHttpRequest, textStatus, errorThrown){
				    	$('#reg_location').val('');
				    }
				});
				if ($('#reg_location').val() == '')
					result = false;
				else
					result = true;
			} else {
				$('#reg_location').hide();
			}
			return result;
		}, 'Please enter a valid United States zip code.');
	},
	
	showRecaptcha: function(){
		var as = this;
		as.thirdPartyAPI.recaptcha.create();
	}
};

/***** Third Party APIs *****//* 
Third party API calls
******************************/
AuthService.prototype.thirdPartyAPI = {
	// disqus
	init: function() {
		this.disqus.root = this.recaptcha.root = this.idms.root = this.root;
		
		this.disqus.set();
		this.idms.set();
	},
	recaptcha: {
		set: function(fn) {
			var RC = this;
			var root = this.root;
			var cfg = CONST_ThirdPartyConfig.recaptcha;

			if (typeof Recaptcha!=="undefined") {
				if (typeof fn==="function") { fn(); }
			} else {
				// if not defined yet, set up asynchronous request
				window.Recaptcha_Create_Instance = function() { // set async callback
					var timeout = false;
					var max = 30;
					var attempts = 0;

					function attempt() {
						if (typeof Recaptcha!=="undefined") {
							if (typeof fn==="function") { fn(); }
						} else if (attempts > max) {
							showToConsole("[authSrv.recaptcha] Recaptcha load attempt timed out.");
						} else {
							attempts++;
							timeout = setTimeout(function(){
								attempt();
							},500);
						}
					}

					attempt();
				}

				// replace this when async call is ready
				// Recaptcha_Create_Instance(); 

				// async request here
				(function(){ // script injection
					var e = document.createElement('script');
					e.src = document.location.protocol+"\/\/"+document.location.hostname+cfg.url;
					e.type = "text/javascript";
					e.async = true;
					$("head").get(0).appendChild(e);
				}());
			}
		},
		create: function() {
			var RC = this;
			var cfg = CONST_ThirdPartyConfig.recaptcha;
			var recaptcha_id = cfg.id;
			var divId = cfg.elmId;
			var settings = cfg.settings;
			
			RC.set(function(){
				Recaptcha.create(recaptcha_id, divId, settings);
			});
		},
		getChallenge: function() {
			var RC = this;
			if (typeof Recaptcha!=="undefined") {
				return Recaptcha.get_challenge();
			}
			return false;
		},
		getResponse: function() {
			var RC = this;
			if (typeof Recaptcha!=="undefined") {
				return Recaptcha.get_response();
			}
			return false;
		}
	},
	disqus: {
		set: function() {
			var root = this.root;
			var DQ = this;

			$(document).ready(function(){
				DQ.validateHash(function(hash){
					DQ.integrate({ token:hash, usr:DQ.cookie("get","p_UN") });
				});
			});
			
		},
		validateHash: function(fn) {
			var DQ = this;
			var root = this.root;
			var cookieVal = this.cookie("get");
			var cfg = CONST_ThirdPartyConfig.disqus;
			
			if (cookieVal) { 
				unescape(cookieVal); 
			} else if (root.isLoggedIn()) {
				// root.logout(root.domain);
			}
			
			function trigger() {
				var data = { dqs: cookieVal };
				root.requestLoginInfo({
					data: data,
					callback: function(obj) {
						if (!obj) { showToConsole("[api.disqus] Refresh service returned no hash value."); return false; }
						try {
							var hash = obj.authentication.dqs;
							DQ.cookie("set",hash);
							if (typeof fn==="function") {
								fn(hash);
								showToConsole("[api.disqus] Hash Refresh Initiated");
							}
						} catch(err) { showToConsole("[api.disqus] Refresh service: " + err); }
					}
				});
			}
			
			function loggedOutCookie() {
				root.requestLogOut({
					callback: function(obj) {
						if (!obj) { showToConsole("[api.disqus] logout hash service returned no value."); return false; }
						try {
							var hash = obj.authentication.dqs;
							DQ.cookie("set",hash);
							fn(hash);
						} catch(err) { showToConsole("[api.disqus] Refresh service: " + err); return false; }
					}
				});
			}
			
			function hasExpired(hasExpiredFn) {
				
				var cDate = new Date();
				var curr = {
					time: cDate,
					epoch: Math.round(cDate.getTime()/1000),
					expire: new Date(cDate.getTime() + (cfg.hash.expires * 60 * 1000))
				};
				curr.expireEpoch = Math.round(curr.expire/1000);
				
				function getLoggedOut() {
					return "new";
				}
				
				// check: if no cookie
				if (!cookieVal) {
					return getLoggedOut(); 
				}

				var arr = cookieVal.split(" ");
				var hash = arr[0];
				var timestamp = arr[2];
				
				// check: if time value undefined
				if (typeof timestamp==="undefined") {
					return getLoggedOut(); 
				} 
				
				// check: if time value not a num
				if (isNaN(timestamp)) {
					return getLoggedOut(); 
				}
				
				// check: if not logged in and not an empty hash
				if (!root.isLoggedIn()) {
					if (hash!==cfg.hash.empty) {
						return getLoggedOut(); 
					}
				}
				
				timestamp = parseInt(timestamp,10); // convert to int
				var tDate = new Date(timestamp*1000);
				var ts = {
					time: new Date(tDate),
					epoch: timestamp,
					expire: new Date(tDate.getTime() + (cfg.hash.expires * 60 * 1000)) // 1 hr
				};
				ts.expireEpoch = Math.round(ts.expire/1000);

				if (curr.epoch >= ts.expireEpoch) {
					if (arr[0]!==cfg.hash.empty) {
						return "yes";
					}
				}
				
				return "no";
			}
			
			switch (hasExpired()) {
				case "new": 
					loggedOutCookie(); 
					break;
				case "no" : 
					fn(cookieVal);
					break;
				case "yes": 	
					showToConsole("[api.disqus] Hash Expired");
					trigger();
					break;
			}
		},
		login: function(data,fn) {
			data = data || false;
			fn = fn || false;
			if (!data) { return false; }
			if (typeof data!=="object") { return false; }
			var DQ = this;
			
			if (data.authentication.dqs) {
				showToConsole("[api.disqus] Logging In");
				DQ.cookie("set",data.authentication.dqs);
				DQ.integrate({ token:data.authentication.dqs, usr:DQ.cookie("get","p_UN") });
			} else {
				// showToConsole("[api.disqus] No Disqus key value.");
			}
			
			if (typeof fn==="function") { fn(); } // callback
		},
		logout: function(data,fn) {
			data = data || false;
			fn = fn || false;
			if (!data) { return false; }
			if (typeof data!=="object") { return false; }
			var DQ = this;
			
			if (data.authentication.dqs) {
				showToConsole("[api.disqus] Logging Out");
				DQ.cookie("set",data.authentication.dqs);
				DQ.integrate({ token:data.authentication.dqs, usr:DQ.cookie("get","p_UN") });
			} else {
				// showToConsole("[api.disqus] No Disqus key value");
			}

			if (typeof fn==="function") { fn(); } // callback
		},
		integrate: function(config) {
			config = config || {};
			var cfg = CONST_ThirdPartyConfig.disqus;
			
			function trigger() {
				if (typeof fox==="undefined") { return false; }
				if (typeof fox.widgets==="undefined") { return false; }
				if (typeof fox.widgets[cfg.widget]!=="function") { 
					// showToConsole("[api.disqus] Integration services unavailable."); 
					return false; 
				} else {
					showToConsole("[api.disqus] Integrating ");
					fox.widgets[cfg.widget](config);
				}
			}
			
			$(document).ready(function(){ trigger(); });
			
		},
		cookie: function(type,data) {
			data = data || false;
			var cfg = CONST_ThirdPartyConfig.disqus;
			var cName = cfg.cookie.name;
			var root = this.root;
			var host = "*." + root.domain;
			
			var exp = new Date();
			exp.setTime(exp.getTime() + (cfg.cookie.expires * 24 * 60 * 60 * 1000));
			var options = { path: "/", host:host, expires:exp, domain:root.domain };
			
			switch(type) {
				case "set": 
					if (data) { 
						$.cookie(cName,data,options);
					} 
					break;
					
				case "remove": 
					$.cookie(cName,null,options); break;
				
				case "get":
					cName = (data) ? data : cName;
					var val = ($.cookie(cName)) ? $.cookie(cName) : false;
					return val;
			}
			
		}
	},
	idms: {
		set: function() {
			var ID = this, root = this.root;
			
			$(document).ready(function(){
				ID.validateHash(function(hash){
					ID.integrate({ token:hash, usr:ID.cookie("get","p_UN") });
				});
			});
		},
		integrate: function(config) {
			config = config || {};
			var cfg = CONST_ThirdPartyConfig.idms, fnName = cfg.callback.login;
			
			// trigger callback function available
			if (typeof window[fnName]==="function") { window[fnName](config); }
		},
		validateHash: function(fn) {
			var ID = this, root = this.root, cookieVal = this.cookie("get"), cfg = CONST_ThirdPartyConfig.idms;
			
			function loggedOutCookie() {
				// remove cookie on logout instance
				ID.cookie("remove");
				fn(false);
			}
			
			function hasExpired(hasExpiredFn) {
				
				function getLoggedOut() { return "new"; }
				
				// check: if no cookie
				if (!cookieVal) { return getLoggedOut(); }

				// check: if not logged in and not an empty hash
				var arr = cookieVal.split(" "), hash = arr[0];
				if (!root.isLoggedIn() && hash!==cfg.hash.empty) { return getLoggedOut(); }
				
				// otherwise, no need to validate
				return "no";
			}
			
			switch (hasExpired()) {
				case "new": loggedOutCookie(); break;
				case "no" : fn(cookieVal); break;
			}
		},
		login: function(data,fn) {
			data = data || false;
			fn = fn || false;
			if (!data) { return false; }
			if (typeof data!=="object") { return false; }
			var ID = this;
			
			if (data.authentication.idms) {
				showToConsole("[api.idms] Logging In");
				ID.cookie("set",data.authentication.idms);
				ID.integrate({ token:data.authentication.idms, usr:ID.cookie("get","p_UN") });
			} else {
				showToConsole("[api.idms] No IDMS authentication found.");
			}
			
			if (typeof fn==="function") { fn(); } // callback
		},
		logout: function(data,fn) {
			var ID = this, cfg = CONST_ThirdPartyConfig.idms, fnName = cfg.callback.logout;
			ID.cookie("remove"); // remove cookie
			// trigger logout callback function available
			if (typeof window[fnName]==="function") { window[fnName](config); }
			if (typeof fn==="function") { fn(); } // callback
		},
		cookie: function(type,data) {
			data = data || false;
			var cfg = CONST_ThirdPartyConfig.idms, cName = cfg.cookie.name, root = this.root, host = "*." + root.domain, exp = new Date();
			
			exp.setTime(exp.getTime() + (cfg.cookie.expires * 24 * 60 * 60 * 1000));
			var options = { path: "/", host:host, expires:exp, domain:root.domain };
			
			switch(type) {
				case "set": if (data) { $.cookie(cName,data,options); } break;
				case "remove": $.cookie(cName,null,options); break;
				case "get":
					cName = (data) ? data : cName;
					var val = ($.cookie(cName)) ? $.cookie(cName) : false;
					return val;
			}
		}
	}
};
	
var auth_service = window.auth_service = new AuthService();

var disablezip = window.disablezip = function(value) {
	if (value == "United States") {
		$('#reg_zip_code').removeAttr('readonly');
		$('#reg_zip_code').css('background', '#fff');
        $('#err_zip_code').next('.validate').children('.sprites').hide();
		$('#reg_location').val('');
	} else {
		$('#reg_zip_code').val('');  
		$('#reg_zip_code').attr('readonly', true);
		$('#reg_zip_code').css('background', '#ddd');
		$('#reg_location').val(value);
	}
}

var formatField = window.formatField = function(id, value){
	switch(id) {
		case "reg_first_name": {
			$('#'+id+'').val(value.join('')); break; 
		}
		case "reg_last_name": {
			$('#'+id+'').val(value.join('')); break; 
		}
		default: {
			$('#'+id+'').val(value.toLowerCase().split(' ').join('')); break;
		}
	}
}

var formatTrim = window.formatTrim = function(id, value){
	$('#'+id+'').val(value.split(' ').join(''));
}

var isNumberKey = window.isNumberKey = function(evt){
   var charCode = (evt.which) ? evt.which : evt.keyCode
   if (charCode > 31 && (charCode < 48 || charCode > 57))
      return false;

   return true;
}

var increaseRefresh = window.increaseRefresh = function(){
	$("meta[name='refresh'],meta[http-equiv='refresh']").each(function(){
		var item = $(this);
		var refresh = item.attr("content");
		var orig = (typeof item.attr("content")!=="undefined") ? refresh : false;
		item.data("RefreshOrig",orig);
		// replace
		item.attr("content","7200");
	});
}

var decreaseRefresh = window.decreaseRefresh = function(){
	$("meta[name='refresh'],meta[http-equiv='refresh']").each(function(){
		var item = $(this);
		var orig = item.data("RefreshOrig");
		if (orig) { item.attr("content",orig); }
	});
}

function showToConsole(str,force) {
	force = force || false;
	var search = (window.location.search).substr(1);
	if (search.length>0) {
		var pairs = search.split('&');
		var valuePair = {};
		for (var x=0;x<pairs.length;x++) {
			var split = pairs[x].split('=');
			valuePair[split[0]] = split[1];
		}

		if (valuePair["stc"]==="y" || force) {
			if (typeof window.console==='object') { console.log(str); }
		}
	}
}
})(jQuery);
