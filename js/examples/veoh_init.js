window.onload = function () {

	setTimeout ( function(){}, 0 );

	// Protected Methods
	function loadScript ( url ) {
		var script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = url;
		document.body.appendChild ( script );
	}

	// Load External Javascript Resource
	loadScript ('assets/scripts/lib/index.js');
};

function init () 
{
    if ( !isVeohWebPlayerInstalled() ) 
    {
        document.getElementById("menu").innerHTML = '';
        document.getElementById("errMessage").innerHTML = 'Please install the Veoh Web Player Video Finder';
		document.getElementById("errMessage").style.visibility = 'visible';
        return;
	}

    var player;
    var playerVersion;
    var xml = getXML(); 
    
    // INITIALIZATION PROCEDURES:
    player = getPlayer();
    playerVersion = getPlayerVersion();
    // Finish initializing the player:
    try
    {
        player.LoadPlayList( xml );
        player.PlayVideoAtPos( 0 );
    }
    catch(e)
    {
        var browserWidth = VEOH.Util.windowWidth();
        var sliderWidth = Math.floor((browserWidth - 150)/100)*100; 

        document.getElementById('slider').style.width = sliderWidth + "px";
        document.getElementById('slideview').style.width = (sliderWidth + 50) + "px";
        
         
	    document.getElementById('errMessage').innerHTML = 'There are no videos in the Web Player library available for playback.<br></br>Visit Veoh.com to discover additional videos to download to your library'; 
        document.getElementById('errMessage').style.margin = '15px'; 
		document.getElementById("errMessage").style.visibility = 'visible';
        player.style.visibility = "hidden";
    }
	
    loadVideoLibraryFromXML();
    Slider(VEOH.Util.windowWidth());

	// Source Selector
	(function () {

		var down = document.getElementById('scroll-down');
		var up = document.getElementById('scroll-up');

		function list () {
			return document.getElementById('source-select');
		}

		function nodes () {
			return document.getElementById('source-select').getElementsByTagName('li');
		}

		function currentSelection () {
			return document.getElementById('source-select').getElementsByTagName('li')[0];
		}

		function lastOption () {
			var nodes = document.getElementById('source-select').getElementsByTagName('li');
			return nodes[ nodes.length - 1 ];
		}

		up.onclick = function () {

			var pocket = currentSelection();
			list().removeChild( pocket );
			list().appendChild( pocket );

			callbackFromSourceSelectValue( currentSelection().innerHTML.toLowerCase() );
		};

		down.onclick = function () {

			var pocket = lastOption();
			list().removeChild( pocket );
			list().insertBefore( pocket, currentSelection() );

			callbackFromSourceSelectValue( currentSelection().innerHTML.toLowerCase() );
		};

		function callbackFromSourceSelectValue ( string ) {


				if(string.indexOf("library") != -1)
                {
					try { library(); } catch (e) {}
                }
				if (string.indexOf("related") != -1)
                {
                    try { related(); } catch (e) {}
               }
		}

		// Source Selector callbacks:

		function library () {
			document.getElementById("errMessage").style.visibility = "hidden";
		}

		function related () {
             loadRelatedVideosFromCurrentlyPlaying();
		}

	})();

	// Button Behaviors
	(function () {

		var initialPosition = VEOH.Util.pixelValue( VEOH.Util.getStyle( document.getElementById('slide-left'), "top" ) );
		var depressedPosition = initialPosition + 1;
		var unit = 'px';

		document.getElementById('slide-left').onmousedown = function (e) {
			this.style.top = depressedPosition + unit;
		};

		document.getElementById('slide-left').onmouseup = function (e) {
			this.style.top = initialPosition + unit;
		};

		document.getElementById('slide-right').onmousedown = function (e) {
			this.style.top = depressedPosition + unit;
		};

		document.getElementById('slide-right').onmouseup = function (e) {
			this.style.top = initialPosition + unit;
		};

	})();

	// Button Behaviors
	(function () {

		var initialPosition = VEOH.Util.pixelValue( VEOH.Util.getStyle( document.getElementById('scroll-up'), "top" ) );
        var depressedPosition = initialPosition + 1;
		var unit = 'px';

		document.getElementById('scroll-up').onmousedown = function (e) {
			this.style.top = depressedPosition + unit;
		};

		document.getElementById('scroll-up').onmouseup = function (e) {
			this.style.top = initialPosition + unit;
		};

	})();

	// Button Behaviors
	(function () {

		var initialPosition = VEOH.Util.pixelValue( VEOH.Util.getStyle( document.getElementById('scroll-down'), "top" ) );
		var depressedPosition = initialPosition + 1;
		var unit = 'px';

		document.getElementById('scroll-down').onmousedown = function (e) {
			this.style.top = depressedPosition + unit;
		};


		document.getElementById('scroll-down').onmouseup = function (e) {
			this.style.top = initialPosition + unit;
		};

	})();

	// Initial player resize
	(function () {

		var player = getPlayer();

        function windowWidth () {
			return VEOH.Util.windowWidth() - 10;
		}

		function windowHeight () {
            //The 112px accounts for the title header, menu, and navigation
            return VEOH.Util.windowHeight();
		}

		if (!player) {
			return;
		}
        
        if(player.id == "VeohVideoPlayer")
        {
            player.setAttribute('height', windowHeight() - 158 );
        }
        else
        {
            player.setAttribute('height', windowHeight() - 128);
        }
    
        player.setAttribute('width', windowWidth() );
        
	})();
    
    // Player Resize
	(function () {
        window.onresize = resizePlayer;
	})();
}


var resizePlayer = function() 
{
    var player = getPlayer(); 	
    var playerVersion = getPlayerVersion();
    
    var delay = 100;
    var timeout;

    
    return function(){
        if(timeout){
            clearTimeout(timeout);
        }
			
    timeout = setTimeout(function() {

        function windowWidth () {
            return VEOH.Util.windowWidth() - 10;
        }

        function windowHeight () {
            //The 112px accounts for the title header, menu, and navigation
            return VEOH.Util.windowHeight();
        }

        if (!player) {
            return;
        }

        if(player.id == "VeohVideoPlayer")
        {
            player.setAttribute('height', windowHeight() - 158 );
        }
        else
        {
            player.setAttribute('height', windowHeight() - 128);
        }
  
        player.setAttribute('width', windowWidth() );
        Slider(VEOH.Util.windowWidth());
    
    }, delay);
    }();
}

var resizeFlashPlayer = function() 
{
    var player = document.getElementById("flashPlayer");

    var delay = 100;
    var timeout;

    return function()
    {
        if(timeout)
        {
            clearTimeout(timeout);
        }
			
        timeout = setTimeout(function() 
        {

            function windowWidth () 
            {
                return VEOH.Util.windowWidth() - 10;
            }

            function windowHeight () 
            {
                //The 112px accounts for the title header, menu, and navigation
                return VEOH.Util.windowHeight();
            }

            if (!player) 
            {
                return;
            }
            if(navigator.platform.indexOf("Win" != 0))
            {
                player.setAttribute('height', windowHeight() - 148 );
            }
            else
            {
                player.setAttribute('height', windowHeight() - 123 );
            }
            player.setAttribute('width', windowWidth() );

        }, delay);
    }();
}

// Slide Events
var Slider = function (_browserWidth) 
{
    document.getElementById('menu').style.left = "0";
    
    // configuration properties
    var browserWidth = _browserWidth;
    var sliderWidth = Math.floor((browserWidth - 150)/100)*100; 
    var delta = sliderWidth;
    var slideWidth = 99;

    // private members
    var newPosition = 0;
    var currentPosition = document.getElementById('menu').style.left;
    var slider = document.getElementById('menu');

    document.getElementById('slider').style.width = sliderWidth + "px";
    document.getElementById('slideview').style.width = (sliderWidth + 55) + "px";

    document.getElementById('slide-left').onclick = function (e) {
        this.blur();
        slide(-1);
        return VEOH.Util.stopEvent(e);
    };

    document.getElementById('slide-right').onclick = function (e) {
        this.blur();
        slide();
        return VEOH.Util.stopEvent(e);
    };


    //Methods
    function numberOfSlides () 
    {
        return document.getElementById('menu').getElementsByTagName('div').length;
    }

    function maxScrollWidth () 
    {
        return numberOfSlides() * slideWidth;
    }

    function slide ( direction ) 
    {
        // scroll backwards (add to the left offset)
        if ( direction && direction == -1 ) 
        {
            newPosition = newPosition + delta;
        }

        // scroll forward (subtract from the left offset)
        else 
        {
            newPosition = newPosition - delta;
        }

        // if accessing the element doesn't return an object, it just hasn't been set yet.
        if ( !currentPosition ) 
        {
            currentPosition = 0;			
        }

        else 
        {
            if ( currentPosition.indexOf && currentPosition.indexOf('px') > 0 ) 
            {
                currentPosition = currentPosition.substr ( currentPosition.indexOf('px'), currentPosition.length );
            }
            currentPosition = parseInt ( currentPosition );
        }

        // validate positions

        // can't go below zero
        if ( newPosition > 0 ) 
        {
            newPosition = currentPosition;
            return;
        }

        // can't exceed maximum length
        if ( newPosition < (-1 * maxScrollWidth()) ) 
        {
            newPosition = currentPosition;
            return;
        }

        var slide = new YAHOO.util.Anim( document.getElementById('menu'), { left: { from: currentPosition, to: newPosition } }, 0.25, YAHOO.util.Easing.easeOutStrong );
        slide.animate();
        currentPosition = newPosition;

    }

}

function loadVideoLibraryFromXML () 
{
	var xml = getXML();
    var dom = XMLDOM( xml );

	if ( !dom )
    {
		var prompt = Message('alert');
		prompt.setBody("Please select a video in the Veoh Web Player.");
		prompt.place( document.getElementById('aux') );	
		return;
	}

        
	// instantiate video menu
	var videoCollection = dom.getElementsByTagName('library')[0].getElementsByTagName('video');

	document.getElementById('menu').innerHTML = '';
        
        if(videoCollection.length)
        {
            for ( var i=0; i < videoCollection.length; i++ ) {
                document.getElementById("menu").appendChild( Video( videoCollection[i], i ) );
            }

            // Initialize page with data for currently playing video:

            // the first video object in the xml feed is the currently playing video
            document.getElementById('title').innerHTML = videoCollection[0].getAttribute('Title');
        }
        
        else
        {
            document.getElementById('errMessage').innerHTML = 'There are no videos in the Web Player library available for playback.<br></br>Visit Veoh.com to discover additional videos to download to your library'; 
            document.getElementById('errMessage').style.margin = '15px'; 
            document.getElementById('errMessage').style.visibility = 'visible';
       }
    
    }

function loadRelatedVideosFromCurrentlyPlaying () 
{
	var position = player.GetCurrentVideoPosition();
	var video = document.getElementById("menu").getElementsByTagName('div')[position];

	if ( !video ) {
		document.getElementById('menu').innerHTML = '';
		return;
	}

	if ( !video.PermaLinkID ) {
            document.getElementById('menu').innerHTML = '';
            document.getElementById('errMessage').style.visibility = "visible";            
            return;
	}

	var searchByVideo = 'http://www.veoh.com/recs' 
	+ "/v2v/" 
	+ video.PermaLinkID 
	+ "?callback=listV2V" 
	+ '&contentSource=veoh&maxResults=50&fallback=0&contentRatingId=1&maxContentRating=2&safe=false';

	document.getElementById('menu').innerHTML = '';

	VEOH.comm.request ({
		'url': searchByVideo,
		'async':'false',
        'contentType': 'text/plain',
		'responseType': 'text/xml',
		'onSuccess': function ( dom ) {

                // instantiate video menu
			var videoCollection = dom.getElementsByTagName('video');

			if ( !videoCollection || videoCollection.length <= 0 ) {
                    document.getElementById('menu').innerHTML = '';
                    document.getElementById('errMessage').style.visibility = "visible";            
				return;
			}

			document.getElementById('menu').innerHTML = '';

			for ( var i=0; i < videoCollection.length; i++ ) {
				document.getElementById("menu").appendChild( Video( videoCollection[i], i, {'flash' : true } ) );
			}

		},
		'onFailure': function ( payload ) {
                document.getElementById('menu').innerHTML = '';
                document.getElementById('errMessage').style.visibility = "visible";            
		}
	});
}


function getXML()
{
    var playerVersion = getPlayerVersion();
    
    // Browser inconsistency:
	// The W3C player object features a getPlaylistXML() accessor method for the XML, but the IE object just has a public property called "PlaylistXML"
	// We look for the property, and keep it if it's found. W3C browsers won't see it, so they default to the else conditional and use getPlaylistXML()
    try {
        
         if ( playerVersion.PlaylistXML ) 
         {
             var xml = playerVersion.PlaylistXML;
            return xml;
         }
	    else 
        {
            var xml = playerVersion.getPlaylistXML();
            return xml;
        }
    }
    catch(e)
    {
        return null;
        /*
        document.getElementById('errMessage').innerHTML = 'There are no videos in the Web Player library available for playback.<br></br>Visit Veoh.com to discover additional videos to download to your library'; 
        document.getElementById('errMessage').style.margin = '15px'; 
        document.getElementById('errMessage').style.visibility = 'visible';
        */
    }
}

function getPlayer()
{
	// Browser inconsistency:
	// W3C browsers will find their player in the npVeohVideoPlayer object, but IE won't use that <embed> object
    
    player = document.getElementById("npVeohVideoPlayer");
	if ( !player ) 
    {
	    player = document.getElementById("VeohVideoPlayer");
    }
    
    //This is deprecated but kept it in case we want to use it later 
    if ( !player ) 
    {
	  	player = document.getElementById("flashPlayer");
    }

    return player;
}

function getPlayerVersion()
{
	playerVersion = document.getElementById("npVeohWebPlayerVersion");
	if ( !playerVersion ) 
    {
		playerVersion = document.getElementById("VeohWebPlayerVersion");
    }

    return playerVersion;
}

function Video ( xml, position, params ) {

	var video = document.createElement('div');
	video.position = position;
	video.PermaLinkID = xml.getAttribute("PermaLinkID") != null ? xml.getAttribute("PermaLinkID"):xml.getAttribute("permalinkId");
    video.FilePath = xml.getAttribute("FilePath");
	video.IconPath = xml.getAttribute("IconPath");
	video.Title = xml.getAttribute("Title") != null ? xml.getAttribute("Title"):xml.getAttribute("title");
	video.Description = xml.getAttribute("Description") != null ? xml.getAttribute("Description"):xml.getAttribute("description");
	video.Publisher = xml.getAttribute("Publisher") != null ? xml.getAttribute("Publisher"):xml.getAttribute("username");
	video.DateAdded = xml.getAttribute("DateAdded") != null ? xml.getAttribute("DateAdded"): xml.getAttribute("dateAdded");
	video.DatePublished = xml.getAttribute("DatePublished");
	video.WebSite = xml.getAttribute("WebSite");
	video.IconUrl = xml.getAttribute("IconUrl");
	video.Duration = xml.getAttribute("Duration");
	video.Size = xml.getAttribute("Size");
	video.id = xml.getAttribute("id");
	video.IsAd = xml.getAttribute("IsAd");
	video.VideoType = xml.getAttribute("VideoType");
	video.LogSource = xml.getAttribute("LogSource");
	video.Compressor = xml.getAttribute("Compressor");

	video.id = video.PermaLinkID;
	video.className = 'video';
	video.innerHTML = ('<a href="#" class="thumbnail" title="' + video.Title + '"><img height="68" alt="' + video.Title + '" class="icon" src="assets/images/videothumb.jpg" /></a><br class="clear"/>');

	video.select = function ( exclusive ) {

		if ( exclusive )  {

			var collection = document.getElementsByTagName('div');

			for ( var i=0; i < collection.length; i++ ) {
				if ( collection[i].className && collection[i].className.indexOf('video') >= 0 && collection[i].className.indexOf('playing') >= 0 ) {
					collection[i].className = 'video';
				}
			}
		}

		this.className = 'playing video';
	};

	if ( params && params.isLocal ) {

		video.onclick = function (e) {
                if(document.getElementById("flashPlayer"))
                {
                    document.getElementById("flashContainer").innerHTML = '';
                    document.getElementById("veohContainer").style.visibility = 'visible';
                    window.onresize = resizePlayer;
                }
                
                document.getElementById('title').innerHTML = this.Title;
			
                player.PlayVideoAtPos ( this.position );
			return VEOH.Util.stopEvent(e);
		};
	}

        else if ( params && params.flash ) 
        {
	    video.innerHTML = ('<a href="http://www.veoh.com/videos/' + video.PermaLinkID + '" class="thumbnail" title="' + video.Title + '"><img height="68" alt="' + video.Title + '" class="icon" src="assets/images/videothumb.jpg" /></a><br class="clear"/>');
    }


	else {
		video.onclick = function (e) {
                if(document.getElementById("flashPlayer"))
                {
                    document.getElementById("flashContainer").innerHTML = '';
                    document.getElementById("veohContainer").style.visibility = 'visible';
                    window.onresize = resizePlayer;
                }
                document.getElementById('title').innerHTML = this.Title;
                player.PlayVideoAtPos ( this.position );
			return VEOH.Util.stopEvent(e);
		};

	}

	if ( video.PermaLinkID  && document.location.host == "www.veoh.com") {
            fetchThumbnailByPermalink ( video.PermaLinkID );	
	}

	return video;
}

// HELPER METHODS:

function XMLDOM ( string ) {

	try {			
		var parsedObject = new ActiveXObject("Microsoft.XMLDOM");
			parsedObject.async = false;
			parsedObject.validateOnParse = false;
			parsedObject.loadXML( string );
		    return parsedObject;
	}
	catch (e) {}

	var parser = new DOMParser();
    
    return parser.parseFromString( string, "text/xml");
}

function fetchThumbnailByPermalink ( permalink ) {
    
	function createMetadataRequestByPermalink ( permalink ) {
		return document.location.protocol 
		+ '//'
		+ document.location.host
		+ '/rest/v2/execute.xml?method=veoh.search.search&type=video&amp;maxResults=1&permalink=' 
		+  permalink
		+ '&contentRatingId=1&apiKey=5697781E-1C60-663B-FFD8-9B49D2B56D36';
	}

	var searchByPermalinkId = createMetadataRequestByPermalink ( permalink );

	VEOH.comm.request ({
		'url': searchByPermalinkId,
		'contentType': 'text/plain',
		'responseType': 'text/xml',
		'onSuccess': function ( payload ) {

			try {
			    var video = payload.getElementsByTagName('video')[0].getAttribute('medResImage');

			    if ( !video ) {
				    return;				
			    }
				document.getElementById( permalink ).getElementsByTagName('img')[0].src = video;
			}catch(e){}

		},
		'onFailure': function ( payload ) {}
	});
}

// Player Interface functions, the plugin calls these
function OnPermalinkChanged () 
{
}


function OnPlaylistChanged ( xml ) 
{
    loadVideoLibraryFromXML();
}

function OnVideoTitleChanged ( strText ) 
{    
    try
    {
        strText = decodeString(strText);
        document.getElementById('title').innerHTML = strText;
	}
	catch (e) {
	}
}

function OnErrorMessage(strMessage) 
{   
    try 
    {
        document.getElementById("menu").innerHTML = '';
        document.getElementById("errMessage").innerHTML = strMessage;
        document.getElementById("errMessage").style.visibility = 'visible';
    }
    catch (e) 
    {
    }
}

function OnVideoPositionChanged ( string ) 
{
}

//End Player Interface functions

// unencode html encoded strings
function decodeString(strText) {
    try {
        // look for html encoding &# and ;
        var nFind = -1;
        var nFind2 = -1;
        nFind = strText.indexOf("&#");

        var strNew = "";
        var nCounter = 0;
        while (nFind != -1) {
            nCounter++;
            // truncate test string
            var strTest = strText.substring(nFind + 2);
            nFind2 = strTest.indexOf(";");

            if (nFind2 > 0 && nFind2 < 5) {
                var strFind = strText.substring(nFind + 2, nFind + 2 + nFind2);
                strNew += strText.substring(0, nFind);
                var strTemp = strText.substring(0, nFind);
                // add left and value to new string
                var nChar = String.fromCharCode(strFind);
                strNew += nChar;
                // truncate test string
                strText = strText.substring(nFind + 2 + nFind2 + 1);
            }
            else {
                // truncate test string
                strText = strText.substring(nFind + 2);
            }
            nFind = strText.indexOf("&#");

            if (nCounter > 100) {
                // hung script decodeString
                break;
            }
        }
        // add any text left in test string
        strNew += strText;
        // replace other escaped chars
        strNew = strNew.replace("&amp;", "&");
        strNew = strNew.replace("&lt;", "<");
        strNew = strNew.replace("&gt;", ">");
        strNew = strNew.replace("&quot;", "\"");
    }
    catch (e) {
    }
    
    return strNew;
}

//Generic functions called from HTML
function fieldClear(id) {

    if(document.getElementById(id).query.value == 'Search Videos' || 

        document.getElementById(id).query.value == 'Search Channels' || 

        document.getElementById(id).query.value == 'Search People' ||

        document.getElementById(id).query.value == 'Search Groups')

    {

        document.getElementById(id).query.value = ''; 

    }
}

//Change image
function changeimage(imgx,placex)  
{
    img3     = new Image(70,25);
    img3.src = "assets/images/miniNav/mininav_logo_default.gif";
    img4     = new Image(70,25);
    img4.src = "assets/images/miniNav/mininav_logo_ovr.gif";

    if (document.images) 
    {
        document.images[placex].src = eval(imgx + ".src");
    }
}
//End Generic Functions

//Deprecated Functions that may be used in the future
function Flash ( params ) { 

    var baseURL = document.location.protocol + '//' + document.location.host + document.location.pathname.substring( 0, document.location.pathname.lastIndexOf('/') );

    if ( !params.permalinkId ) {

       // add error message

       return;

    }

    var flashVersion = deconcept.SWFObjectUtil.getPlayerVersion();

    if ( flashVersion['major'] <  9 || flashVersion['major'] == 9 && flashVersion['rev'] < 115 ) 
    {
        var player = document.getElementById("npVeohVideoPlayer");
    	var playerVersion = document.getElementById("npVeohWebPlayerVersion");

    	if ( !player ) {
	    	player = document.getElementById("VeohVideoPlayer");
	    	playerVersion = document.getElementById("VeohWebPlayerVersion");
	    }
    
        if( player )
        {
            player.StopVideo();
        }
    
        document.getElementById("veohContainer").style.visibility = "hidden";
        document.getElementById("flashContainer").style.visibility = "visible";
    
        var flashUpgradeMsg='<div id="flashPlayer"><div id="noFlashMessaging">';

        flashUpgradeMsg+='<p><img src="http://ll-appserver.veoh.com/images/alert.gif" />&nbsp;Adobe Flash Player 9.0.115 or newer is required to watch video.</p>';

        flashUpgradeMsg+='<a href="http://www.adobe.com/shockwave/download/download.cgi?P1_Prod_Version=ShockwaveFlash" target="_blank" class="buttonD_130">Get Flash Now</a>';

        flashUpgradeMsg+='</div></div>';

        document.getElementById("flashContainer").innerHTML = flashUpgradeMsg;

        return;

     }


    //var swf = baseURL + '/assets/flash/veohplayer_p2p.swf';
    var swf = 'http://www.veoh.com/static/flash/players/veohplayer.swf';

    var permalinkId = params.permalinkId;
    
    var destination = document.getElementById('flashContainer');

    var width = VEOH.Util.windowWidth() - 10;

    var height = VEOH.Util.windowHeight() - 148;

    var version = deconcept.SWFObjectUtil.getPlayerVersion();

    var backgroundColor = '#000';

    var p2pPort;

    try

    {

        if ( typeof document.getElementById('npVeohWebPlayerVersion') == 'function' ) 

        {

            p2pPort = document.getElementById('npVeohWebPlayerVersion').getP2PPort();

        }

        else if ( typeof document.getElementById('VeohWebPlayerVersion') == 'object' ) 

        {

            p2pPort = document.getElementById('VeohWebPlayerVersion').P2PPort;

        }

        else {

            // add error message

            // this could potentially just be the case for loading the normal player

            return;     

        }

    }

    catch(e)

    {

        return;
    }

    var so = new SWFObject( swf, "flashPlayer", width, height, version, backgroundColor );

    so.addVariable("permalinkId", permalinkId);

    so.addVariable("player", "videodetails");

    so.addVariable("playAuto", 1);

    so.addVariable("external", 0);

    so.addVariable("webPlayerInstalled", "true");
    
    so.addVariable("webPlayerVersion", version);
    
    so.addVariable("webPlayerPort", p2pPort);

    so.addParam("name", "value");

    so.addParam("id", "value");

    so.addParam("quality", "high");

    so.addParam("wmode", "opaque");

    so.addParam("allowFullScreen", "true");

    so.addParam("align", "middle");

    so.addParam("allowScriptAccess", "always");

    so.write( destination );
   
    
    //stop the player, make veoh invisible, and then show the flash player
    var player = document.getElementById("npVeohVideoPlayer");
	var playerVersion = document.getElementById("npVeohWebPlayerVersion");

	if ( !player ) {
		player = document.getElementById("VeohVideoPlayer");
		playerVersion = document.getElementById("VeohWebPlayerVersion");
	}
    
    if( player )
    {
        player.StopVideo();
    }
    
    document.getElementById("veohContainer").style.visibility = "hidden";
    destination.style.visibility = "visible";
    
    window.onresize = resizeFlashPlayer;
}

function reportProblem()
{
    try
    {
        versionNumber =  getPlayerVersion().getVersion();
    }
    catch(e)
    {
        try
        {
            versionNumber = getPlayerVersion().Version;
        }
        catch(e)
        {
            versionNumber = "42";
        }
    }
	
    //Get current video playing

    currentVideo = getPlayer().GetCurrentVideoPosition();
    
    var xml = getXML();
    var dom = XMLDOM( xml );
   
	var videoCollection = dom.getElementsByTagName('library')[0].getElementsByTagName('video');
    var lastVideoPlayed = videoCollection[currentVideo].getAttribute("PermaLinkID") != null ? videoCollection[currentVideo].getAttribute("PermaLinkID"):videoCollection[currentVideo].getAttribute("permalinkId");
   
   // Create a form
    var f = document.createElement("form");

    // Add it to the document body
    document.body.appendChild(f);

    // Add action and method attributes
    f.action = "http://www.veoh.com/veohtv/problem.htm";
    f.method = "get";
    try
    { 
        var player = document.createElement("<input name=\"player\" type=\"hidden\">");
    }
    catch(e)
    {
        var player = document.createElement("input");
        player.setAttribute("name", "player");
        player.setAttribute("type", "hidden");
    }
    f.appendChild(player);
    player.value = "web";

    try
    { 
        var version = document.createElement("<input name=\"version\" type=\"hidden\">");
    }
    catch(e)
    {
        var version = document.createElement("input");
        version.setAttribute("name", "version");
        version.setAttribute("type", "hidden");
    }
    f.appendChild(version);
    version.value = versionNumber;
  
    try
    { 
        var lastvideo = document.createElement("<input name=\"lastvideo\" type=\"hidden\">");
    }
    catch(e)
    {
        var lastvideo = document.createElement("input");
        lastvideo.setAttribute("name", "lastvideo");
        lastvideo.setAttribute("type", "hidden");
    }
    
    f.appendChild(lastvideo);
    lastvideo.value = lastVideoPlayed;

    // Call the form's submit method
    f.submit();
}
