$.fn.copyTo = function(to) {
    var to = $(to);
    for ( var i = 1; i < arguments.length; i++ )
        to.set( arguments[i], this.get(0)[ arguments[i] ] );
    return this;
};

new function() {
       // $.fn.validate = validate() {};
    $.fn.validate = {
        init: function(o) {
          if(o.name == 'username') { this.username(o) };
          if(o.name == 'password') { this.password(o) };
          if(o.name == 'email') { this.email(o) };
          if(o.name == 'dob') { this.dob(o) };
        },
        username: function(o) {
          var user = /[(\*\(\)\[\]\+\.\,\/\?\:\;\'\"\`\~\\#\$\%\^\&\<\>)+]/;
           if (!o.value.match(user)) {
             doValidate(o);
            } else {
             doError(o,'no special characters allowed');
            };
        },
        password: function(o) {
          var pass = /[(\*\(\)\[\]\+\.\,\/\?\:\;\'\"\`\~\\#\$\%\^\&\<\>)+]/;
           if (!o.value.match(pass)) {
             doValidate(o);
            } else {
             doError(o,'no special characters allowed');
            };
        },
        email: function(o) {
          var email  = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
           if (o.value.match(email)) {
              doSuccess(o);
            } else {
              doError(o,'not a valid email');
            };
        },
        dob: function(o) {
          var dob  = /(0[1-9]|1[012])+\/(0[1-9]|[12][0-9]|3[01])+\/(19|20)\d\d/;
            if (o.value.match(dob)) {
              doSuccess(o);
            } else {
              doError(o,'not a valid date');
            };
        }
     };

     function doSuccess(o) {
              $('#' + o.id + '_img').html('<img src="images/accept.gif" border="0" style="float:left;" />');
              $('#' + o.id + '_li').removeClass("error");
              $('#' + o.id + '_msg').html("");
              $('#' + o.id + '_li').addClass("success");
     }

     function doError(o,m) {
              $('#' + o.id + '_img').html('<img src="images/exclamation.gif" border="0" style="float:left;" />');
              $('#' + o.id + '_li').addClass("error");
              $('#' + o.id + '_msg').html(m);
              $('#' + o.id + '_li').removeClass("success");
     }
     //private helper, validates each type after check
     function doValidate(o) {
        	$('#' + o.id + '_img').html('<img src="images/loading.gif" border="0" style="float:left;" />');
        	$.post('ajax.php', { id: o.id, value: o.value }, function(json) {
                  	eval("var args = " + json);
                        if (args.success == true)
                  	{
                  	  doSuccess(args);
                  	}
                  	else
                  	{
                          doError(args,args.msg);
                  	}
                  });
    };

};
$.fn.match = function(m) {
	$('#' + this.get(0).id + '_img').html('<img src="images/loading.gif" border="0" style="float:left;" />');
	if ($(this).get(0).val() == $(m.match).val()) {
          $('#' + this.get(0).id + '_img').html('<img src="images/accept.gif" border="0" style="float:left;" />');
          $(m.error).removeClass("error");
          $(m.error).addClass("success");
          $('#' + this.get(0).id + '_msg').html("");
        } else {
          $('#' + this.get(0).id + '_img').html('<img src="images/exclamation.gif" border="0" style="float:left;" />');
          $(m.error).addClass("error");
          $(m.error).removeClass("success");
          $('#' + this.get(0).id + '_msg').html("The passwords don't match, please try again");
        };
};
$(document).ready(function()
{

  $("//[@class=validated]/input").blur(function() {
          $(this).validate.init(this);
  });
  

  $("#confirmpass").blur(function() {
          $(this).match({match: '#password', error: '#confirmpass_li'});
  });


  $("#password").keyup(function() {
          $(this).copyTo("#password_copy","value");
  });

  // This Used To Be HOVER, But I.E. Didnt Like It
  //$(".form li").hover(function(){$(this).addClass("selected");},function(){$(this).removeClass("selected");});
  $(".form li").mouseover(function() {
          $(this).addClass("selected");
  });
  $(".form li").mouseout(function() {
          $(this).removeClass("selected");
  });
});