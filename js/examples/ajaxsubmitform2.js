    $(document).ready(function () {
        // Username validation logic
        var validateUsername = $('#validateUsername');
        $('#username').keyup(function () {
            // cache the 'this' instance as we need access to it within a setTimeout, where 'this' is set to 'window'
            var t = this; 
            
            // only run the check if the username has actually changed - also means we skip meta keys
            if (this.value != this.lastValue) {
                
                // the timeout logic means the ajax doesn't fire with *every* key press, i.e. if the user holds down
                // a particular key, it will only fire when the release the key.
                                
                if (this.timer) clearTimeout(this.timer);
                
                // show our holding text in the validation message space
                validateUsername.removeClass('error').html('<img src="images/ajax-loader.gif" height="16" width="16" /> checking availability...');
                
                // fire an ajax request in 1/5 of a second
                this.timer = setTimeout(function () {
                    $.ajax({
                        url: 'ajax-validation.php',
                        data: 'action=check_username&username=' + t.value,
                        dataType: 'json',
                        type: 'post',
                        success: function (j) {
                            // put the 'msg' field from the $resp array from check_username (php code) in to the validation message
                            validateUsername.html(j.msg);
                        }
                    });
                }, 200);
                
                // copy the latest value to avoid sending requests when we don't need to
                this.lastValue = this.value;
            }
        });
        
        // avatar validation
        // we use keyup *and* change because 
        $('#avatar').keyup(function () {
            var t = this;
            clearTimeout(this.timer);
            this.timer = setTimeout(function () {
                if (t.value == t.current) {
                    return true;
                }

                var preview = $('#validateAvatar').html('<img src="images/ajax-loader.gif" height="16" width="16" /> trying to load avatar...');
                var i = new Image();

                clearTimeout(t.timeout);

                if (t.value == '') {
                    preview.html('');
                } else {
                    i.src = t.value;
                    i.height = 32;
                    i.width = 32;
                    i.className = 'avatar';

                    // set a timeout of x seconds to load the image, otherwise, show the fail message
                    t.timeout = setTimeout(function () {
                        preview.html('Image could not be loaded.');
                        i = null;
                    }, 3000);

                    // if the dummy image holder loads, we'll show the image in the validation space,
                    // but importantly, we clear the timer set above
                    i.onload = function () {
                        clearTimeout(t.timeout);
                        preview.empty().append(i);
                        i = null;
                    };
                }
                
                t.current = t.value;
            }, 250);
        }).change(function () {
            $(this).keyup(); // call the keyup function
        });
    });