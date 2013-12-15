/* jQuery Alternative */

var ready = (function () {
  function ready( f ) {
    if( ready.done ) return f();

    if( ready.timer ) {
      ready.ready.push(f);
    } else {
      addEvent( window, "load" isDOMReady );
      ready.ready = [ f ];
      ready.timer = setInterval(isDOMReady, 13);
    }
  };

  function isDOMReady() {
    if( ready.done ) return false;

    if( document && document.getElementsByTagName && document.getElementById && document.body ) {
      clearInterval( ready.timer );
      ready.timer = null;
      for( var i = 0; i < ready.ready.length; i++ ) {
        ready.ready[i]();
      }
      ready.ready = null;
      ready.done = true;
    }
  }

  return ready;
})();

/* jQuery Source */

function bindReady(){
    if ( readyBound ) return;
    readyBound = true;

    // Mozilla, Opera and webkit nightlies currently support this event
    if ( document.addEventListener ) {
        // Use the handy event callback
        document.addEventListener( "DOMContentLoaded", function(){
                document.removeEventListener( "DOMContentLoaded", arguments.callee, false );
                jQuery.ready();
        }, false );

    // If IE event model is used
    } else if ( document.attachEvent ) {
        // ensure firing before onload,
        // maybe late but safe also for iframes
        document.attachEvent("onreadystatechange", function(){
                if ( document.readyState === "complete" ) {
                        document.detachEvent( "onreadystatechange", arguments.callee );
                        jQuery.ready();
                }
        });

        // If IE and not an iframe
        // continually check to see if the document is ready
        if ( document.documentElement.doScroll && window == window.top ) (function(){
                if ( jQuery.isReady ) return;

                try {
                        // If IE is used, use the trick by Diego Perini
                        // http://javascript.nwbox.com/IEContentLoaded/
                        document.documentElement.doScroll("left");
                } catch( error ) {
                        setTimeout( arguments.callee, 0 );
                        return;
                }

                // and execute any waiting functions
                jQuery.ready();
        })();
    }

    // A fallback to window.onload, that will always work
    jQuery.event.add( window, "load", jQuery.ready );
}

/* Begin Event Listener */

function addEvent(obj, evt, fn) {
	if (typeof obj.attachEvent != 'undefined') {
		obj.attachEvent("on" + evt, fn);
	} else if (typeof obj.addEventListener != 'undefined') {
		obj.addEventListener(evt, fn, false);
	} else { // support old browser
	}
}

addEvent(window, 'load', function() {
//  Initialization stuff here
});

/* End Event Listener */