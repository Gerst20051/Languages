// Framekiller
if (top != self) {
    top.location.href = self.location.href;
}

$(document).ready(function() {
    // Fix IE 6 box heights and disable background flickering
    if (jQuery.browser.msie && jQuery.browser.version == '6.0') {
        try {
            document.execCommand('BackgroundImageCache', false, true);
        } catch (e) {};
        
        $('div.box').each(function(i) {
            _IEupdateBoxSize($(this));
        });
        
        $('div.container').each(function(i) {
            _IEupdateContainerSize($(this));
        });
        
        $('div.quick-select div.dropdown').bind('mouseenter', function (e) {
            $(this).find('ul').addClass('visible');
        }).bind('mouseleave', function (e) {
            $(this).find('ul').removeClass('visible');
        });
    }
    
    // User warning
    $('a.warn-user').bind('click', function(e) {
        var reason = '';
        
        while (reason == '') {
            var reason = prompt('Grund für die Verwarnung/Ban:', '');
            
            if (reason != null) {
                reason = reason.replace(/^\s+/, '').replace(/\s+$/, '');
            }
        }
        
        if (reason != null) {
            $.post(_IEhrefFix($(this).attr('href')), {reason: reason}, function() {
                window.location.reload();
            });
        }
        
        return false;
    });

    // Post deletion
    $('a.delete-post').bind('click', function(e) {
        if (window.confirm('Soll der Post wirklich gelöscht werden?')) {
            return true;
        }
        
        return false;
    });
    
    // Thread deletion
    $('a.delete-thread').bind('click', function(e) {
        if (window.confirm('Soll der Thread wirklich gelöscht werden?')) {
            return true;
        }
        
        return false;
    });
    
    // Proposal deletion
    $('a.delete-proposal').bind('click', function(e) {
        $this = $(this);
        
        if (window.confirm('"' + $this.attr('title') + '" löschen?')) {
            $.post(_IEhrefFix($this.attr('href')), {});
        
            $this.parents('div.container:eq(0)').remove();
        }
        
        return false;
    });
    
    // Inline search
    $('form.inline-search a').bind('click', function(e) {
        $(this).parents('form:eq(0)').get(0).submit();
        
        return false;
    });
        
    // Blank links
    $('a.blank').bind('click', function(e) {
        window.open(_IEhrefFix(this.href));
        
        return false;
    });
    
    // Lightbox
    if ($.fn.lightBox) {
        $('a.picture-zoom').lightBox();
    }
       
    // Avatar select
    $('#avatar-select li').bind('click', function (e) {
        $('#avatar-select li.active').removeClass('active');
        $(this).addClass('active');
        $('#avatar-select-value').val($(this).find('img').attr('alt'));
    })
    
    // Avatar select (IE Fix)
    if (jQuery.browser.msie) {
        $('#avatar-select li').bind('mouseenter', function (e) {
            $(this).opacity(1);
        }).bind('mouseleave', function (e) {
            if (!$(this).hasClass('active')) {
                $(this).opacity(0.7);
            }
        });
    }
    
    // MarkItUp
    if (typeof($.markItUp) != 'undefined') {
        $('textarea.bb-code').markItUp({
            markupSet: [
                {name:'Fett', className: 'bbcode-bold', key:'B', openWith:'[b]', closeWith:'[/b]'},
                {name:'Kursiv', className: 'bbcode-italic', key:'I', openWith:'[i]', closeWith:'[/i]'},
                {name:'Durchgestrichen', className: 'bbcode-stroke', key:'S', openWith:'[s]', closeWith:'[/s]'},
                {name:'Farbe', className: 'bbcode-color', openWith:'[color=[![Farbe]!]]', closeWith:'[/color]', dropMenu: [
                    {name:'Red', openWith:'[color=red]', closeWith:'[/color]', className:"bbcode-color-red" },
                    {name:'Yellow', openWith:'[color=yellow]', closeWith:'[/color]', className:"bbcode-color-yellow" },
                    {name:'Blue', openWith:'[color=blue]', closeWith:'[/color]', className:"bbcode-color-blue" },
                    {name:'Green', openWith:'[color=green]', closeWith:'[/color]', className:"bbcode-color-green" },
                    {name:'Gray', openWith:'[color=gray]', closeWith:'[/color]', className:"bbcode-color-gray" },
                    {name:'Maroon', openWith:'[color=maroon]', closeWith:'[/color]', className:"bbcode-color-maroon" },
                    {name:'Fuchsia', openWith:'[color=fuchsia]', closeWith:'[/color]', className:"bbcode-color-fuchsia" },
                    {name:'Lime', openWith:'[color=lime]', closeWith:'[/color]', className:"bbcode-color-lime" },
                    {name:'Black', openWith:'[color=black]', closeWith:'[/color]', className:"bbcode-color-black" }
                ]},
                {name:'Link', className: 'bbcode-link', key:'L', openWith:'[url=[![Url]!]]', closeWith:'[/url]', placeHolder:'Ziel-URL'},
            ]
        });
    }
    
    // Download counter
    var downloadCounter = $('#download-counter');
    if (downloadCounter.length > 0) {
        var countdown = parseInt($('#download-countdown').text());
        var countdownTimer = function() {
            $('#download-countdown').text(--countdown);
            
            if (countdown == 0) {
                $('#download-counter').css('display', 'none');
                $('#download-button').css('display', 'inline');
            } else {
                setTimeout(countdownTimer, 1000);
            }
        } 
        
        setTimeout(countdownTimer, 1000);
    }

    // Add flash game if required
    var flashGame = $('#flash-game');
    if (flashGame.length > 0) {
        var data = flashGame.attr('title').split('|');
        flashGame.attr('title', '');
        
        var swfUri = data[0];
        var width  = data[1];
        var height = data[2];
    
        var swfPlayer = new SWFObject(swfUri, 'game', width, height, '6.0.0');
        swfPlayer.write('flash-game');
        
        if (jQuery.browser.msie) {
            _IEupdateContainer(flashGame.parent().parent());
        }
    }
    
    // Add Video-Player if required
    var videoPlayer = $('#video-player');
    if (videoPlayer.length > 0 && requireFlashVersion(9)) {
        var data = videoPlayer.attr('title').split('|');
        videoPlayer.attr('title', '');
        videoPlayer.text('');
        
        var mediaUri = data[0];
        var imageUri = data[1];
        var showLogo = (data[2] == 'true');
        var title    = data[3];
        var length   = data[4];
        
        var img = $('<img width="478" height="380"/>');
        img.attr('src', imageUri);
        videoPlayer.append(img);

        var play = $('<div class="play"/>');
        videoPlayer.append(play);
        
        var info = $('<div class="info"/>');
        
        info.append($('<span class="title"/>').text(title));
        
        if (length !== 'null') {
            info.append($('<br/>'));
            info.append($('<span class="length"/>').text(length));
        }
        
        videoPlayer.append(info);
        
        var options = {
            key: '#$bdc645d9c4b8e29e1f3',
            
            canvas: {
                backgroundGradient: 'none'
            },
            
            plugins: {
                controls: {
                    url: mediaUri + '/swf/flowplayer.controls-tube-3.1.2.swf',
                    
                    progressGradient: 'none',
                    bufferGradient: 'none',
                    timeBgColor: '#262626',
                    tooltipTextColor: '#D00000',
                    buttonColor: '#000000',
                    volumeSliderGradient: 'none',
                    volumeSliderColor: '#FFEEBF',
                    buttonOverColor: '#FF0000',
                    timeColor: '#ffffff',
                    backgroundColor: '#929292',
                    tooltipColor: '#C9C9C9',
                    backgroundGradient: 'high',
                    borderRadius: '0',
                    durationColor: '#ffffff',
                    sliderColor: '#929292',
                    sliderGradient: 'none',
                    progressColor: '#FFAD0C',
                    bufferColor: '#FFEEBF',
                    height: 24,
                    opacity: 1.0,
                    
                    tooltips: {
                        buttons: true
                    }
                }
            },
                       
            clip: {
                autoPlay: true,
                autoBuffering: true,
                scaling: 'fit'
            }
        };
        
        if (showLogo) {
            options.logo = { 
                url: mediaUri + '/swf/watermark.swf',
                bottom: '2%',
                right: '1',
                width: '20%',
                height: '20%',
                
                opacity: 0.5,
                
                fullscreenOnly: false,
                displayTime: 0,
                
                linkUrl: 'http://www.rofl.to'
            };
        }
        
        videoPlayer.flowplayer(mediaUri + '/swf/flowplayer.commercial-3.1.2.swf', options);
        
        if (jQuery.browser.msie) {
            _IEupdateContainer(videoPlayer.parent().parent());
        }
    }
    
    // Bind search quickinfo
    var searchInput = $('#search input.text');
    if (searchInput.val() == '' || typeof(searchInput.val()) == 'undefined') {
        searchInput.val(searchInput.attr('title'));
    }

    searchInput.bind('focus', function(e) {
        if ($(this).val() == $(this).attr('title')) {
            $(this).val('');
        }
    }).bind('blur', function(e) {
        if ($(this).val() == '' || typeof(searchInput.val()) == 'undefined') {
            $(this).val($(this).attr('title'));
        }
    });

    // Bind favorite functionallity
    $('a.add-favorite, a.remove-favorite').bind('click', function (e) {
        $this = $(this);
        
        if ($this.parents('ul.video-list').length > 0) {
            $.post(_IEhrefFix($this.attr('href')), {mode: 'remove'}, function (data) {
                window.location.reload();
            });
        } else {
            if ($this.hasClass('add-favorite')) {
                var mode = 'add';
                $this.removeClass('add-favorite').addClass('remove-favorite')
                     .html('<span></span>' + translate('Remove favorite'));
            } else {
                var mode = 'remove';
                $this.removeClass('remove-favorite').addClass('add-favorite')
                     .html('<span></span>' + translate('Add favorite'));
            }
        
            $.post(_IEhrefFix($this.attr('href')), {mode: mode});
        }
    
        return false;
    });
    
    // Bind buddy functionallity
    $('p.buddy-acceptance a').bind('click', function (e) {
        $this = $(this);
        var ul = $this.parents('ul:eq(0)');
        
        $.post(_IEhrefFix($this.attr('href')), {}, function() {
            if (ul.find('li').length == 0) {
                ul.parents('div.box:eq(0)').remove();
                
                window.location.reload();
            }
        });

        $this.parents('li:eq(0)').remove();
        
        return false;
    });
    
    var buddyChangeFunc = function (e) {
        $this = $(this);
        
        if ($this.parents('ul.buddies').length > 0) {
            if (!window.confirm(sprintf(translate('Do you really want to remove your buddy %s?'), $this.parent().find('a.name').text()))) {
                return false;
            }
            
            $.post(_IEhrefFix($this.attr('href')), {mode: 'remove'});
            
            $(this).parent().remove();
        } else {
            if ($this.hasClass('add-buddy')) {
                var mode = 'add';
                $this.removeClass('add-buddy').addClass('remove-buddy')
                     .html('<span></span>' + translate('Remove buddy'));
            } else {
                if (!window.confirm(translate('Do you really want to remove this buddy?'))) {
                    return false;
                }
                
                var mode = 'remove';
                $this.removeClass('remove-buddy').addClass('add-buddy')
                     .html('<span></span>' + translate('Add buddy'));
            }
            
            $.post(_IEhrefFix($this.attr('href')), {mode: mode});
        }
    
        return false;
    }
    
    $('a.add-buddy, a.remove-buddy').bind('click', buddyChangeFunc);
    
    $('#load-buddies').bind('click', function (e) {
        var ul = $(this).parent().parent();
        
        $(this).remove();
        
        if (jQuery.browser.msie) {
            _IEupdateContainer(ul.parent().parent().parent());
        }
        
        $.post(_IEhrefFix($(this).attr('href')), {}, function (data) {
            ul.children().remove();
            
            for (var i = 0; i < data.length; i++) {               
                var buddy = data[i];
                
                var li = $('<li/>');
                
                if (buddy.deleteUrl) {
                    var a = $('<a/>');
                    a.attr('href', buddy.deleteUrl)
                     .addClass('remove-buddy')
                     .attr('title', translate('Remove buddy'))
                     .append($('<span/>'))
                     .bind('click', buddyChangeFunc);
                    
                    li.append(a);
                    li.append($('<span> </span>'));
                }
                
                var span = $('<span/>');
                span.addClass('online-status')
                    .addClass(buddy.isOnline ? 'online' : 'offline')
                    .attr('title', (buddy.isOnline ? translate('Online') : translate('Offline')));
                
                li.append(span);
                li.append($('<span> </span>'));
                
                var a = $('<a/>');
                a.attr('href', buddy.url)
                 .addClass('name')
                 .text(buddy.username);
                
                li.append(a);
                
                ul.append(li);  
            }
            
            if (jQuery.browser.msie) {
                _IEupdateContainer(ul.parent().parent().parent());
            }
        },
        'json');
        
        return false;
    });
    
    // Proposal rate functionallity
    $('a.rate-exists').bind('click', function (e) {
        $this = $(this);

        $this.blur();

        $.post(_IEhrefFix($this.attr('href')), {});
      
        var value = $(this).find('span.value');
        value.text(parseInt(value.text()) + 1);
        
        var newSpan = $('<span class="rate-exists"/>').append($(this).children())
        $(this).before(newSpan).remove();
        
        return false;
    });
    
    $('a.vote-box').bind('click', function (e) {
        $this = $(this);

        $this.blur();
        
        if ($this.hasClass('yes')) {
            var yesBox = $this;
            var noBox  = $this.next();
            var vote   = 'yes';
        } else {
            var yesBox = $this.prev();
            var noBox  = $this;
            var vote   = 'no';
        }

        $.post(_IEhrefFix($this.attr('href')), {});
        
        var valueSpan = $this.find('span.value');
        valueSpan.text(parseInt(valueSpan.text()) + 1);
        
        yesBox.find('span.label').slideUp('slow', function() {
            var box = $(this).parents('a.vote-box.yes');
            
            if (vote == 'no') {
                $(this).remove();
            }
            
            var newBox = $('<span class="vote-box yes"/>').append(box.children())
            box.before(newBox).remove();
            
            if (vote == 'yes') {
                var label = newBox.find('span.label');
                label.text(translate('thanks'));
                label.slideDown('slow');
            }
        });
        
        noBox.find('span.label').slideUp('slow', function() {
            var box    = $(this).parents('a.vote-box.no');

            if (vote == 'yes') {
                $(this).remove();
            }
            
            var newBox = $('<span class="vote-box no"/>').append(box.children())
            box.before(newBox).remove();
            
            if (vote == 'no') {
                var label = newBox.find('span.label');
                label.text(translate('thanks'));
                label.slideDown('slow');
            }           
        });
                
        return false;
    });
    
    // Logout
    $('.redirect-logout').bind('click', function (e) {
        $this = $(this);
        var action = _IEhrefFix($this.attr('href'));

        $.get(action, {},
            function(data) {
                window.location.reload();
            },
            'json'
        );
        
        return false;
    });
    
    // Login widget
    $('#login-widget').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action   = $this.attr('action');
        var username = $this.find("#login_username").val();
        var password = $this.find('#login_password').val();

        $this.hide();
        $('#login-loading').show();
        $('#login-loading span.label').text(translate('Login in progress'));
                 
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#login-widget').parent().parent().parent());
        }

        $.post(action, {username: username, password: password},
            function(data) {
                if (data.success) {
                    if (data.target == null) {
                        window.location.reload();
                    } else {
                        window.location.href = data.target;
                    }
                } else {
                    $('#login-loading').hide();
                    $('#login-widget').show();
                    $('#login-widget span').text(data.message)
                                           .append($('<br/><br />'))
                                           .css('display', 'inline');
                    
                    if (jQuery.browser.msie) {
                        _IEupdateContainer($('#login-widget').parent().parent().parent());
                    }
                }
            },
            'json'
        );
    });
    
    // Forgot credentials
    $('#forgot-credentials').bind('click', function (e) {
        $('#login-widget').hide();
        $('#forgot-credentials-widget').show();
        
        $('#login-widget-title').text(translate('Recover credentials'));
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#login-widget').parent().parent().parent());
        }
        
        return false;
    });
    
    $('#forgot-credentials-widget').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action = $this.attr('action');
        var email  = $this.find("#forgot_email").val();

        $this.hide();
        $('#login-loading').show();
        $('#login-loading span.label').text(translate('Checking e-mail'));
                 
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#login-widget').parent().parent().parent());
        }

        $.post(action, {email: email},
            function(data) {
                $('#login-loading').hide();
                
                if (data.success) {
                    var span = $('<span/>').text(translate('A recovery mail was sent to you'));
                    $('#forgot-credentials-widget').after(span);
                } else {
                    $('#forgot-credentials-widget').show();
                    $('#forgot-credentials-widget span').css('display', 'inline')
                                                        .text(data.message);
                }
                
                if (jQuery.browser.msie) {
                    _IEupdateContainer($('#login-widget').parent().parent().parent());
                }
            },
            'json'
        );
    });
    
    // Bind star rating functionallity
    var rateElement = function(e) {
        var percent = ((e.pageX - $(this).offset().left) / 110);
        percent = Math.ceil(percent * 5) * 20;
        
        if (percent == 20) {
            var text = translate('Average!');
        } else if (percent == 40) {
            var text = translate('Worth Watching!');
        } else if (percent == 60) {
            var text = translate('Nice!');
        } else if (percent == 80) {
            var text = translate('Cool!');
        } else if (percent == 100) {
            var text = translate('Awesome!');
        }
        
        $(this).find('span').css('width', percent + '%');
        
        $(this).parent().find('span.text').text(text);
    }
    
    $('a.star-rate span.rating-big').bind('mouseenter', function (e) {
        this.originalRating = $(this).find('span').css('width');
        this.originalText   = $(this).parent().find('span.text').text();
          
        $(this).bind('mousemove', rateElement);
    }).bind('mouseleave', function (e) {
        $(this).unbind('mousemove', rateElement);
        
        $(this).find('span').css('width', this.originalRating);
        $(this).parent().find('span.text').text(this.originalText);
    }).bind('click', function (e) {      
        $(this).unbind('mousemove')
               .unbind('mouseenter')
               .unbind('mouseleave')
               .unbind('click')
               .bind('click', function (e) { return false; });
    
        $(this).parent().find('span.text').text(translate('Thanks for rating!'));
    
        var action = _IEhrefFix($(this).parents('a.star-rate').attr('href'));
        var rating = parseInt($(this).find('span').css('width')) / 20;
        
        $.post(action, {rating: rating});        
        
        return false;
    });

    // Bind copy-link fields
    $('input.copy-link').bind('focus', function (e) {
        $(this).select();

        if (jQuery.browser.msie) {
            if (window.clipboardData && clipboardData.setData) {
                clipboardData.setData("Text", this.value);
            }
        }
    }).bind('click', function (e) {
        $(this).select();

        if (jQuery.browser.msie) {
            if (window.clipboardData && clipboardData.setData) {
                clipboardData.setData("Text", this.value);
            }
        }
    });
    
    // Bind embed-code fields
    $('input.embed-code').bind('focus', function (e) {
        $(this).select();

        if (jQuery.browser.msie) {
            if (window.clipboardData && clipboardData.setData) {
                clipboardData.setData("Text", this.value);
            }
        }
    }).bind('click', function (e) {
        $(this).select();

        if (jQuery.browser.msie) {
            if (window.clipboardData && clipboardData.setData) {
                clipboardData.setData("Text", this.value);
            }
        }
    });
    
    $('span.embed-options-switch').bind('click', function (e) {
        $('div.embed-options').toggle();
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($this.parents('div.container:eq(0)'));
        }
    });

    $('div.embed-options a').bind('click', function (e) {
        $('div.embed-options a').removeClass('current');
        
        var $this = $(this);
        $this.addClass('current');
        var width  = parseInt($(this).find('span.width').text());
        var height = parseInt($(this).find('span.height').text());
        
        $('input.embed-code').val($('input.embed-code').val().replace(/embed\/js\/\d+\/\d+/, 'embed/js/' + width + '/' + height));
        
        this.blur();
        
        return false;
    });
    
    // Bind duplicity check
    if (typeof(duplicityCheckUrl) != 'undefined') {
        $('#register-username').bind('blur', function (e) {
           var username = $(this).val();
           
           if (username == '') {              
               $('#username-check').css('display', 'none');               
               return;
           }
           
           $('#username-check').css('display', 'block');
           $('#username-check span.label').text(translate('Checking username'));
           $('#username-check span.icon').removeClass('true')
                                         .removeClass('false')
                                         .addClass('loading');
                     
           $.post(duplicityCheckUrl, {username: username},
               function(data) {
                   $('#username-check span.icon').removeClass('loading');
                   
                   if (data & 0x04) {
                       $('#username-check span.icon').addClass('false');
                       $('#username-check span.label').text(translate('Username is invalid'));
                   } else if (data & 0x01) {
                       $('#username-check span.icon').addClass('false');
                       $('#username-check span.label').text(translate('Username is in use'));
                   } else {
                       $('#username-check span.icon').addClass('true');
                       $('#username-check span.label').text(translate('Username is available'));
                   }
               },
               'json'
           );
        });

        $('#register-email').bind('blur', function (e) {
           var email = $(this).val();
           
           if (email == '') {              
               $('#email-check').css('display', 'none');
               return;
           }
           
           $('#email-check').css('display', 'block');
           $('#email-check span.label').text(translate('Checking e-mail'));
           $('#email-check span.icon').removeClass('true')
                                      .removeClass('false')
                                      .addClass('loading');
           
           $.post(duplicityCheckUrl, {email: email},
               function(data) {
                   $('#email-check span.icon').removeClass('loading');
                   
                   if (data & 0x08) {
                       $('#email-check span.icon').addClass('false');
                       $('#email-check span.label').text(translate('E-Mail is invalid'));
                   } else if (data & 0x02) {
                       $('#email-check span.icon').addClass('false');
                       $('#email-check span.label').text(translate('E-Mail is in use'));
                   } else {
                       $('#email-check span.icon').addClass('true');
                       $('#email-check span.label').text(translate('E-Mail is available'));
                   }
               },
               'json'
           );
        });
    }
    
    // Bind password check
    $('#old-password').bind('blur', function (e) {
        var password = $(this).val();
        
        if (password == '') {              
            $('#old-password-check').css('display', 'none');
            return;
        }
        
        $('#old-password-check').css('display', 'block');
        $('#old-password-check span.label').text(translate('Checking password'));
        $('#old-password-check span.icon').removeClass('true')
                                          .removeClass('false')
                                          .addClass('loading');
               
        $.post(passwordCheckUrl, {password: password},
            function(data) {
                $('#old-password-check span.icon').removeClass('loading');
                
                if (!data) {
                    $('#old-password-check span.icon').addClass('false');
                    $('#old-password-check span.label').text(translate('Password is not correct'));
                } else {
                    $('#old-password-check span.icon').addClass('true');
                    $('#old-password-check span.label').text(translate('Password is correct'));
                }
            },
            'json'
        );
     });
    
    var passwordCheck = function (e) {
        var password = $(this).val();
        
        if (password == '') {              
            $('#password-check').css('display', 'none');
            return;
        }
        
        $('#password-check').css('display', 'block');
        
        if (password.length < 4) {
            $('#password-check span.icon').removeClass('true')
                                          .addClass('false');
            $('#password-check span.label').text(translate('Password is too short'));
        } else {
            $('#password-check span.icon').removeClass('false')
                                          .addClass('true');
            $('#password-check span.label').text(translate('Password is valid'));
        }
    }
    
    var passwordConfirmCheck = function (e) {
        var password     = $(this).val();
        var prevPassword = $('#register-password').val();
        
        if (password == '') {              
            $('#password-confirm-check').css('display', 'none');           
            return;
        }
        
        $('#password-confirm-check').css('display', 'block');
        
        if (password != prevPassword) {
            $('#password-confirm-check span.icon').removeClass('true')
                                                  .addClass('false');
            $('#password-confirm-check span.label').text(translate('Password doesn\'t match'));
        } else {
            $('#password-confirm-check span.icon').removeClass('false')
                                                  .addClass('true');
            $('#password-confirm-check span.label').text(translate('Password matches'));
        }
     }
    
    $('#register-password').bind('blur', passwordCheck)
                           .bind('keyup', passwordCheck);
    
    $('#register-password-confirm').bind('blur', passwordConfirmCheck)
                                   .bind('keyup', passwordConfirmCheck);
    
    // Bind email form functionallity
    $('p.email-link a').bind('click', function (e) {       
        // Reset the form
        var additionReceivers = $('#send-email').find('input.additional-receiver');
        additionReceivers.each(function(i) {
            var $this      = $(this);
            var $container = $this.parents('div.container:eq(0)')
            
            $this.next().remove();
            $this.remove();
        });
        
        $('#receiver-delete').css('display', 'none');
        $('#email-form-loading-animation').css('display', 'none');
        
        $('#send-email').find("input[@name='receiver[]']").val('');
        $('#send-email').find('textarea').val('');
        
        // Show the form
        $('#send-email-form').show();
        $('#send-email-form').next('p').remove();
        
        // Fix IE
        if (jQuery.browser.msie) {
            $('#send-email').show();
            _IEupdateContainer($('#send-email').find('div.container'));
        } else {
            $('#send-email').fadeIn('slow');
        }
    });
    
    $('#send-email-form').bind('submit', function(e) {
        // Prevent the form from submitting
        e.preventDefault();
    
        // Get necessary data
        var $this  = $(this);
        var action = this.action;
        var emails = $this.find("input[@name='receiver[]']");

        var receivers = new Array();
        emails.each(function(i) {
            receivers[receivers.length] = $(this).val();
        });

        var personalMessage = $this.find("textarea[@name='message']").val();
        var senderEmail     = $this.find("input[@name='sender-email']").val();
        var senderName      = $this.find("input[@name='sender-name']").val();
        
        $this.hide();
        $('#email-form-loading-animation').css('display', 'inline');
        
        var existingInfo = $this.next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
        }
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($this.parents('div.container:eq(0)'));
        }
        
        // Send the email
        $.post(action, {receivers: receivers.join(','), personalMessage: personalMessage, senderEmail: senderEmail, senderName: senderName},
            function(data) {
                $('#email-form-loading-animation').css('display', 'none');
                
                if (data.success) {
                    var p = createInfoPara(data.message, true);
                    $('#send-email-form').after(p);
                } else {
                    var p = createInfoPara(data.message, false);
                    $('#send-email-form').after(p)                   
                                         .show();
                }
                
                if (jQuery.browser.msie) {
                    _IEupdateContainer($('#send-email-form').parents('div.container:eq(0)'));
                }
            },
            'json'
        );
    });
    
    $('#receiver-add').bind('click', function (e) {
        var block = $(this).parent();
        
        $('#receiver-delete').css('display', 'inline');
        
        if (block.find('input').length == 5) {
            return false;
        }
        
        var input = $(document.createElement('input'));
        input.attr('class', 'additional-receiver');
        input.attr('type',  'text');
        input.attr('name',  'receiver[]');
          
        var br = $(document.createElement('br'));
        
        block.append(input);
        block.append(br);
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($(this).parents('div.container:eq(0)'));
        }
        
        return false;
    });
    
    $('#receiver-delete').bind('click', function (e) {
        var block     = $(this).parent();
        var inputs    = block.find('input');
        var numInputs = inputs.length 
        
        if (numInputs == 1) {
            return false;
        } else if (numInputs == 2) {
            $('#receiver-delete').css('display', 'none');
        } 
        
        var lastInput = inputs.eq(numInputs - 1);
        var lastBr    = lastInput.next();
        
        lastInput.remove();
        lastBr.remove();
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($(this).parents('div.container:eq(0)'));
        }
        
        return false;
    });
    
    // Bind direct login functionallity
    var directLoginLink  = $('#direct-login-link');
    var directLogoutLink = $('#direct-logout-link');
    
    directLoginLink.bind('click', function (e) {
        $('#write-comment-block').hide();
        $('#login-block').show();
    
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#login-block').parent().parent());
        }
    
        return false;
    });
    
    $('#quit-direct-login-link').bind('click', function (e) {
        $('#login-block').hide();
        $('#write-comment-block').show();

        if (jQuery.browser.msie) {
            _IEupdateContainer($('#write-comment-block').parent().parent());
        }
    
        return false;
    });
    
    if (directLoginLink.length > 0) {
        var logoutUrl = _IEhrefFix(directLoginLink.attr('href'));
    } else if (directLogoutLink.length > 0) {
        var logoutUrl = _IEhrefFix(directLogoutLink.attr('href'));
    }
    
    var logout = function (e) {
        $.post(logoutUrl, {});
        
        $this    = $(this);
        username = $(this).prev('strong');
        
        var label = $(document.createElement('label'))
                  .attr('for', 'comment_name')
                  .text(translate('Name') + ':');
        var input = $(document.createElement('input'))
                  .attr('type', 'text')
                  .attr('name', 'name')
                  .attr('id', 'comment_name')
                  .attr('maxlength', 14);
        var a = $(document.createElement('a'))
              .attr('href', '#')
              .attr('id', 'direct-login-link')
              .text(translate('Login now!'))
              .bind('click', function (e) {
                  $('#write-comment-block').hide();
                  $('#login-block').show();
            
                  return false;
              });
              
        username.before(label)
                .before(input)
                .before(a)
                .remove();
                
        $this.remove();
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#write-comment-block').parent().parent());
        }
    
        return false;
    };
    directLogoutLink.bind('click', logout);
    
    $('#login').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action   = $this.attr('action');
        var username = $this.find('#login_name').val();
        var password = $this.find('#login_password').val();

        $this.hide();
       
        var existingInfo = $this.next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
            
            if (jQuery.browser.msie) {
                _IEupdateContainer($('#login-block').parent().parent());
            }
        }

        $.post(action, {username: username, password: password},
            function(data) {
                if (data.success) {
                    $('#login').show();
                    $('#login-block').hide();
                    $('#write-comment-block').show();
                    
                    var nameInput = $('#comment_name');
                    var nameLabel = nameInput.prev('label');
                    var loginLink = nameInput.next('a');

                    if (nameLabel.length > 0) {
                        var username = $(document.createElement('strong')).text(data.username);
                        var spacer   = $(document.createTextNode(' '));
                        var a        = $(document.createElement('a'))
                                     .attr('href', '#')
                                     .attr('id', 'direct-logout-link')
                                     .text(translate('logout'))
                                     .bind('click', logout);
                        nameLabel.before(username)
                                 .before(spacer)
                                 .before(a);
                        
                        nameInput.remove();
                        nameLabel.remove();
                        loginLink.remove();
                    }
                    
                    if (jQuery.browser.msie) {
                        _IEupdateContainer($('#write-comment-block').parent().parent());
                    }
                } else {
                    var p = createInfoPara(data.message, false);
                    $('#login').after(p);
                    $('#login').show();
                    
                    if (jQuery.browser.msie) {
                        _IEupdateContainer($('#login-block').parent().parent());
                    }
                }
            },
            'json'
        );
    });
    
    // Reload fix
    $('#comment_replyId').val('0');
    
    // Bind character limit
    var commentCharacterLimit = 800;
    
    $('#comment_content, #entry_content').bind('keydown', function (e) {
        var keyCode = (window.event ? event.keyCode : e.which);

        if ((e.ctrlKey || e.metaKey) && (keyCode == 67 /*c*/ || keyCode == 65 /*a*/ || keyCode == 86 /*v*/ || keyCode == 88 /*x*/)) {
            return true;
        }
        
        if (keyCode == 32 || keyCode == 13 || keyCode > 47) {
            if ($(this).val().length >= commentCharacterLimit) {
                e.preventDefault();
                return false;
            }
        }
    }).bind('keyup', function(e) {
        if ($(this).val().length > commentCharacterLimit) {
            $(this).val($(this).val().substr(0, commentCharacterLimit));
        }
    }).bind('blur', function(e) {
        if ($(this).val().length > commentCharacterLimit) {
            $(this).val($(this).val().substr(0, commentCharacterLimit));
        }
    }).bind('paste', function (e) {
        if ($(this).val().length > commentCharacterLimit) {
            $(this).val($(this).val().substr(0, commentCharacterLimit));
        }
    });
    
    // Bind post limit
    var postCharacterLimit = 8000;
    
    $('#post_content').bind('keydown', function (e) {
        var keyCode = (window.event ? event.keyCode : e.which);

        if ((e.ctrlKey || e.metaKey) && (keyCode == 67 /*c*/ || keyCode == 65 /*a*/ || keyCode == 86 /*v*/ || keyCode == 88 /*x*/)) {
            return true;
        }
        
        if (keyCode == 32 || keyCode == 13 || keyCode > 47) {
            if ($(this).val().length >= postCharacterLimit) {
                e.preventDefault();
                return false;
            }
        }
    }).bind('keyup', function(e) {
        if ($(this).val().length > postCharacterLimit) {
            $(this).val($(this).val().substr(0, postCharacterLimit));
        }
    }).bind('blur', function(e) {
        if ($(this).val().length > postCharacterLimit) {
            $(this).val($(this).val().substr(0, postCharacterLimit));
        }
    }).bind('paste', function (e) {
        if ($(this).val().length > postCharacterLimit) {
            $(this).val($(this).val().substr(0, postCharacterLimit));
        }
    });
    
    // Bind smileys functionallity
    $('ul.smileys li img').bind('click', function (e) {
        var content = $('#comment_content, #entry_content, #post_content');
        
        content.replaceSelection(' ' + $(this).attr('alt') + ' ', true);
        content.get(0).focus();
        
        if (content.attr('id') === 'post_content') {
            if (content.val().length > postCharacterLimit) {
                content.val(content.val().substr(0, postCharacterLimit));
            }
        } else {
            if (content.val().length > commentCharacterLimit) {
                content.val(content.val().substr(0, commentCharacterLimit));
            }
        }
    });
    
    // Bind comment put functionallity
    $('#comments-put').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action  = $this.attr('action');
        var name    = $this.find('#comment_name').val();
        var content = $this.find('#comment_content').val();
        var replyId = $this.find('#comment_replyId').val();

        if ($this.find('#comment_name').length > 0) {
            if (window.confirm(translate('Do you want to register now for free?'))) {
                window.location.href = registerUrl;
                return;
            }
        }
        
        $this.hide();
        $('#comment-form-loading-animation').css('display', 'inline');

        var existingInfo = $this.next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
        }
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#write-comment-block').parent().parent());
        }

        $.post(action, {name: name, content: content, replyId: replyId},
            function(data) {
                $('#comment-form-loading-animation').css('display', 'none');
            
                if (data.success) {
                    $('#comment_content').val('');
                    
                    var p = createInfoPara(data.message, true);
                    $('#comments-put').after(p);
                    
                    var replyInfo =  $('#comments-put').prev('p').remove();

                    if (replyId == '0' || typeof(loadCommentsList.currentUrl) == 'undefined') {
                        loadCommentsList(data.firstPageUrl);
                    } else {
                        loadCommentsList(loadCommentsList.currentUrl);
                    }
                } else {
                    var p = createInfoPara(data.message, false);
                    $('#comments-put').after(p)                   
                                      .show();
                }
                
                if (jQuery.browser.msie) {
                    _IEupdateContainer($('#write-comment-block').parent().parent());
                }
            },
            'json'
        );
    });
    
    // Bind comment list functionallity
    bindCommentsListFunctionallity();
    
    // Bind entries put functionallity
    $('#entries-put').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action  = $this.attr('action');
        var content = $this.find('#entry_content').val();
        
        $this.hide();
        $('#entry-form-loading-animation').css('display', 'inline');

        var existingInfo = $this.next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
        }
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#entries-put').parent().parent());
        }

        $.post(action, {content: content},
            function(data) {
                $('#entry-form-loading-animation').css('display', 'none');
            
                if (data.success) {
                    $('#entry_content').val('');
                    
                    var p = createInfoPara(data.message, true);
                    $('#entries-put').after(p);
                    
                    var replyInfo =  $('#entries-put').prev('p').remove();
                    
                    window.location.reload();
                } else {
                    var p = createInfoPara(data.message, false);
                    $('#entries-put').after(p)                   
                                     .show();
                }
                
                if (jQuery.browser.msie) {
                    _IEupdateContainer($('#entries-put').parent().parent());
                }
            },
            'json'
        );
    });
    
    // Bind entries delete functionallity
    $('a.delete-entry').bind('click', function (e) {
        $this    = $(this);
        var href = $this.attr('href');
        
        if (!window.confirm(translate('Do you really want to delete this entry?'))) {
            return false;
        }
        
        $this.parents('li:eq(0)').remove();
        
        $.post(href, {});
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('ul.comments').parent().parent());
        }
        
        return false;
    });
    
    // Bind proposal comments put functionallity
    $('#proposal-comments-put').bind('submit', function (e) {
        // Prevent the form from submitting
        e.preventDefault();

        $this = $(this);
        var action  = $this.attr('action');
        var content = $this.find('#comment_content').val();
        
        $this.hide();
        $('#entry-form-loading-animation').css('display', 'inline');

        var existingInfo = $this.next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
        }
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#proposal-comments-put').parent().parent());
        }

        $.post(action, {content: content},
            function(data) {
                $('#entry-form-loading-animation').css('display', 'none');
            
                if (data.success) {
                    $('#comment_content').val('');
                    
                    var p = createInfoPara(data.message, true);
                    $('#proposal-comments-put').after(p);
                    
                    var replyInfo =  $('#proposal-comments-put').prev('p').remove();
                    
                    window.location.reload();
                } else {
                    var p = createInfoPara(data.message, false);
                    $('#proposal-comments-put').after(p)                   
                                               .show();
                }
                
                if (jQuery.browser.msie) {
                    _IEupdateContainer($('#proposal-comments-put').parent().parent());
                }
            },
            'json'
        );
    });
    
});

/**
 * Create a paragraph info
 *
 * @param  string text
 * @param  string type
 * @return jQuery
 */
function createInfoPara(text, success)
{
    var p = $(document.createElement('p'));
    if (success == true) {
        p.addClass('success-info');
    } else {
        p.addClass('failure-info');
    }
    
    var icon = $(document.createElement('span'));
    icon.addClass('icon');
    p.append(icon);
    
    var textNode = $(document.createTextNode(text));
    p.append(textNode);
    
    return p;
}

/**
 * Loads a new comments page
 */
function loadCommentsList(url)
{
    loadCommentsList.currentUrl = url;

    var comments         = $('ul.comments');
    var commentsHeadline = comments.prev('h1');
    
    comments.next('p.pagination').remove();
    comments.remove();

    $('#loading-animation').show();

    if (jQuery.browser.msie) {
        _IEupdateContainer(commentsHeadline.parent().parent());
    }

    $.post(url, {},
        function(data) {
            $('#loading-animation').hide();
        
            var container = commentsHeadline.parent().parent();
        
            commentsHeadline.after(data)
                            .remove();
                            
            bindCommentsListFunctionallity();
            
            init();
            
            if (jQuery.browser.msie) {
                _IEupdateContainer(container);
            }
        },
        'json'
    );
}

/**
 * Bind comments list functionallity and pagination
 *
 * @return false
 */
function bindCommentsListFunctionallity()
{
    // Bind delete links
    $('a.delete-comment').bind('click', function(e) {
        $this = $(this);
        
        if (window.confirm('Kommentar von "' + $this.attr('title') + '" löschen?')) {
            $.post(_IEhrefFix($this.attr('href')), {});
        
            $this.parents('li:eq(0)').remove();
        }
        
        return false;
    });
    
    // Bind paging functionallity
    $('h1.read-comments + ul.comments + p.pagination a').bind('click', function (e) {
        loadCommentsList(_IEhrefFix(this.href));
        
        return false;
    });
    
    // Bind reply functionallity
    $('ul.comments a.reply').bind('click', function (e) {
        // Display the write comment block
        $('#login-block').hide();
        $('#write-comment-block').show();
        $('#comments-put').show();
        var existingInfo = $('#comments-put').next('p');
        if (existingInfo.length > 0) {
            existingInfo.remove();
        }
    
        // Set the reply to message
        var replyInfo =  $('#comments-put').prev('p');
        if (replyInfo.length == 0) {
            var replyInfo = $('<p/>');
            $('#comments-put').before(replyInfo);
        }
        
        var replyMessage = $(this).parents('li:eq(0)').children('p.comment-text').html();
        replyInfo.html(translate('Reply to') + ': ' + replyMessage);
        
        var idData = $(this).attr('id').split('-');
        $('#comment_replyId').val(idData[1]);
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($('#write-comment-block').parent().parent());
        }
    }); 
    
    // Bind spam functionallity
    $('ul.comments a.spam').bind('click', function (e) {
        $.post(_IEhrefFix(this.href), {});
    
        var a       = $(this).next().clone();
        var parent  = $(this).parent();
        var h2      = $(this).parents('h2');
        var p       = h2.next();
        
        if (p.hasClass('fold-info')) {
            var reason = p;
            p.next().hide();
        } else {
            var reason = $('<p class="fold-info"/>');
            p.before(reason);
            p.hide();
        }
        
        reason.text(translate('Thank you for reporting the comment.'));
        reason.show();
        
        if (jQuery.browser.msie) {
            _IEupdateContainer($(this).parents('div.container:eq(0)'));
        }
        
        $(this).mouseout();
        parent.html('');
        parent.append(a);
    
        return false;
    });
    
    // Bind rate functionallity
    $('ul.comments div.rate a').bind('click', function (e) {
        if ($(this).hasClass('register-link')) { 
            return true;
        }
        
        var parent        = $(this).parent();
        var numberField   = parent.find('span.number');
        var currentRating = parseInt(numberField.text());
        
        var rates = parent.find('a');
        rates.each(function (i) {
            var $this = $(this);
            var span  = $(document.createElement('span'))
                      .addClass('icon');
                      
            if ($this.hasClass('bad')) {
                span.addClass('bad');
            } else {
                span.addClass('good');
            }
            
            span.attr('title', $this.attr('title'))
                .text($this.text());
                
            $this.before(span)
                 .hide();
        });
        
        $.post(_IEhrefFix(this.href), {},
            function(data) {
                if (data.diffValue === null) {
                    parent.find('a.good, a.bad').show();
                    parent.find('span.good, span.bad').remove();
                    
                    alert(sprintf(translate('You can only make ratings every %d seconds!'), 5));
                                       
                    return;
                } else {
                    parent.find('a.good, a.bad').remove();
                }
            
                var newRating = currentRating + data.diffValue;
                
                parent.removeClass('negative')
                      .removeClass('neutral')
                      .removeClass('positive');
                
                if (newRating > 0) {
                    newRating = '+' + newRating;
                    
                    parent.addClass('positive');
                } else if (newRating < 0) {
                    parent.addClass('negative');
                } else {
                    parent.addClass('neutral');
                }
                
                numberField.text(newRating);
            },
            'json'
        );
    
        return false;
    });

    // Bind comment unfold functionallity
    $('a.unfold').bind('click', function (e) {
        var $this = $(this);
    
        $this.css('display', 'none');
        
        if (jQuery.browser.msie) {
            $this.parents('li:eq(0)').find('p.fold-info:eq(0)').hide();
            $this.parents('li:eq(0)').find('p.folded:eq(0)').show();
            $this.parents('li:eq(0)').find('span.folded-options:eq(0)').show();
            
            _IEupdateContainer($this.parents('div.container:eq(0)'));
        } else {
            $this.parents('li:eq(0)').find('p.fold-info:eq(0)').hide('normal', function() {
                $(this).parent().find('p.folded:eq(0)').show('normal', function() {
                     $this.parents('li:eq(0)').find('span.folded-options:eq(0)').show();
                });
            });
        }
 
        return false;
    });
}

/**
 * Simple message translate
 */
function translate(messageId)
{
    if (typeof(messageIDs) != 'undefined' && typeof(messageIDs[messageId]) != 'undefined') {
        return messageIDs[messageId];
    } 
    
    return messageId;
}

/**
 * Simple sprintf implementation
 */
function sprintf() {
    if (sprintf.arguments.length < 2) {
        return;
    }

    var data = sprintf.arguments[0];
    for (var k = 1; k < sprintf.arguments.length; k++) {
        switch (typeof(sprintf.arguments[k])) {
            case 'string':
                data = data.replace(/%s/, sprintf.arguments[k]);
                break;
                
            case 'number':
                data = data.replace(/%d/, sprintf.arguments[k]);
                break;
                
            case 'boolean':
                data = data.replace(/%b/, sprintf.arguments[k] ? 'true' : 'false');
                break;

            default:
                break;
        }
    }

    return data;
}

if (!String.sprintf) {
    String.sprintf = sprintf;
}

/**
 * Fix a href URL in IE
 * 
 * @param  string href
 * @return string
 */
function _IEhrefFix(href)
{
    if (!jQuery.browser.msie) {
        return href;
    } 
    
    return href.replace(/^about:/, '')
}

/**
 * Update a container in IE.
 *
 * This is required to call when the size of a container changes, so IE
 * updates the box dimensions correctly.
 *
 * @param  jQuery container
 * @return void
 */
function _IEupdateContainer(container)
{
    // This hacks may not be called for non-IE browsers
    if (!jQuery.browser.msie) {
        return;
    } 
    
    var box = container.parents('div.box:eq(0)');
    
    // IE requires us to redefine all widtsh and heights of the container and
    // it's parent box
    _IEupdateContainerSize(container);
    _IEupdateBoxSize(box);

    // This hack is required to actually update the box layout
    container.css('display', 'none').css('display', 'block');
    box.css('display', 'none').css('display', 'block');
} 

/**
 * Update a container size for IE
 *
 * @param jQuery container
 * @return void
 */
function _IEupdateContainerSize(container)
{
    // Do not execute this hack for non-IE browsers
    if (!jQuery.browser.msie) {
        return;
    }

    var $inner = container.find('div.container-inner');
    
    if ($inner.length == 0) {
        return;
    }
    
    var inner  = $inner.get(0);
    var width  = inner.offsetWidth;
    var height = inner.offsetHeight;

    container.css('height', height + 'px');
    $inner.find('div.container-top').css('width', width + 'px');
    $inner.find('div.container-bottom').css('width', width + 'px');
    $inner.find('div.container-left').css('height', height + 'px');
    $inner.find('div.container-right').css('height', height + 'px');
}

/**
 * Update a box size for IE
 *
 * @param jQuery box
 * @return void
 */
function _IEupdateBoxSize(box)
{
    // Do not execute this hack for non-IE browsers
    if (!jQuery.browser.msie) {
        return;
    }

    var $inner = box.find('div.box-inner');
    
    if ($inner.length == 0) {
        return;
    }
    
    var inner  = $inner.get(0);
    var width  = inner.offsetWidth;
    var height = inner.offsetHeight;
    
    box.css('height', height + 'px');
    $inner.find('div.box-top').css('width', width + 'px');
    $inner.find('div.box-bottom').css('width', width + 'px');
    $inner.find('div.box-left').css('height', height + 'px');
    $inner.find('div.box-right').css('height', height + 'px');
    
    if (jQuery.browser.version == '6.0') {
        $('#footer').css('display', 'none').css('display', 'block');
    }
}

/**
 * Detect flash version
 * 
 * @param  integer requiredVersion
 * @return boolean
 */
function requireFlashVersion(requiredVersion)
{
    if (typeof(navigator.plugins['Shockwave Flash']) == 'object') {
        var description      = navigator.plugins['Shockwave Flash'].description;
        var availableVersion = parseInt(description.substr(16, (description.indexOf('.', 16) - 16)));

        return (availableVersion >= requiredVersion); 
    } else if (typeof(ActiveXObject) == 'function') {
        try {
            if (typeof(new ActiveXObject('ShockwaveFlash.ShockwaveFlash.' + requiredVersion)) == 'object') {
                return true;
            }
        } catch (exception) {
            // Do nothing
        }
    }

    return false;
}