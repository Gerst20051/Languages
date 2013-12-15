
function fitHeadline(tar, opt_length){
    var t = document.getElementById('testCont');
    t.setAttribute('class', '');
    tar.setAttribute('class', '');

    var et = goog.dom.getRawTextContent(tar);
    goog.dom.setTextContent(t, et);

    // height of 2 lines is 54
    if(t.clientHeight > 55){
        t.setAttribute('class', 'f20');
        tar.setAttribute('class', 'f20');

        // height of 2 lines is 42
        if(t.clientHeight > 45){
            while(t.clientHeight > 45){
                var tt = goog.dom.getRawTextContent(t);
                goog.dom.setTextContent(t, tt.substr(0, tt.length-1));
            }
            // add ellipses
            var tt = goog.dom.getRawTextContent(t);
            tt = tt.substr(0, tt.length-3) + '...';
            // set it in the cont finally
            goog.dom.setTextContent(tar, tt);
        }
    }
};

function truncate(string, opt_numChars, opt_preserveWhite) {

    var numChars = (opt_numChars == null) ? 80 : opt_numChars;
    var white = (opt_preserveWhite == null) ? false : opt_preserveWhite;

    if(string.length > numChars){

        if(white == true){
            var l = string.lastIndexOf(' ', numChars);
            return string.substr(0, l) + '...';
        }else{
            return string.substr(0, numChars-3) +'...';
        }
    }

    return string;

};