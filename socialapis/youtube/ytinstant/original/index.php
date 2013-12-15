<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
    <!--
    
    YouTube Instant
    http://ytinstant.com
    
    Author     Feross Aboukhadijeh
    Website    http://www.feross.org
    Copyright  2010. Feross Aboukhadijeh.
    
    Data is not information.
    Information is not knowledge.  
    Knowledge is not understanding.
    Understanding is not wisdom.
    
    
    Far better is it to dare mighty things, to win glorius triumphs,
    even though checkered by failure... than to rank with those poor spirits
    who neither enjoy nor suffer much, because they live in a gray twilight
    that knows not victory nor defeat.
                                                    � Theodore Roosevelt
    --> 
    
    <meta http-equiv="content-type" content="text/html; charset=utf-8"/> 
    <title>YouTube Instant - Real-time YouTube video surfing.</title> 
    <meta name="title" content="YouTube Instant" /> 
    <meta name="description" content="Real-time YouTube video surfing." /> 
    <meta name="keywords" content="youtube instant, google instant, instant, youtube, feross" /> 
    
    <meta property="og:title" content="YouTube Instant"/> 
    <meta property="og:type" content="website"/> 
    <meta property="og:image" content="http://ytinstant.com/images/og_logo.png"/> 
    <meta property="og:url" content="http://ytinstant.com/"/> 
    <meta property="og:site_name" content="YouTube Instant"/> 
    <meta property="fb:admins" content="1347524341"/> 
    <meta property="og:description" content="Real-time YouTube video surfing."/> 
 
	<link href="favicon.ico" rel="icon" type="image/x-icon" /> 
    <link href='http://fonts.googleapis.com/css?family=Reenie+Beanie|Josefin+Sans+Std+Light' rel='stylesheet' type='text/css' /> 
    <link rel="stylesheet" type="text/css" href="style_min.css" /> 
    <script src="http://www.google.com/jsapi?key=ABQIAAAANiTe_niYMpLXI3UDeUzpMRSVMlTdmhaYd1yaX4iiQjGBtAzFhxS96_Ey_JaUcWxxFqT4QZQh3TV1jQ" type="text/javascript"></script> 
    <script src="instant_min.js" type="text/javascript"></script> 
    
    <script type="text/javascript" src="http://delicious-button.googlecode.com/files/jquery.delicious-button-1.0.min.js"></script> 
    <script type="text/javascript"> 
 
      var _gaq = _gaq || [];
      _gaq.push(['_setAccount', 'UA-3898076-10']);
      _gaq.push(['_trackPageview']);
 
      (function() {
        var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
        ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
        var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
      })();
 
    </script> 
</head> 
<body> 
<div id="outerWrapper"> 
    <div id="wrapper"> 
        <div id="header" class="clearfix"> 
                <div id="logo"> 
                    <a href="http://ytinstant.com"><span>YouTube Instant</span></a> 
                </div> 
            <input id="searchBox" value="" spellcheck="false"></input> 
            <span id="searchTermKeyword"><strong>Search YouTube Instantly</strong></span> 
        </div> 
        <div id="main" class="clearfix"> 
            <div id="videoDiv"> 
                <div id="innerVideoDiv"> 
                    Loading...
                </div> 
                <div id="videoTools" class="clearfix"> 
                    <div class="boxHeader"> 
                        Share this video
                    </div> 
                    <!-- <div id="shareButtons">
                        <a href="javascript:void(0);" onclick="doFBShare();">
                            <img src="fb_share.png" alt="Share on Facebook">
                        </a>
                    </div> --> 
                    <div id="shareTools"> 
                        <span>Link </span><input id="linkUrl" value="" spellcheck="false"></input> 
                        <span>Embed </span><input id="embedUrl" value="" spellcheck="false"></input> 
                    </div> 
                </div> 
            </div> 
            <div id="playlistWrapper"> 
                <div id="buttonControl" class="pauseButton"><a href="javascript:void(0);">&nbsp;</a></div> 
                <div id="playlist" class="clearfix">&nbsp;</div> 
            </div> 
        </div> 
        <div id="footer" class="clearfix"> 
            <div id="tipDiv" class="footerBox"> 
                <div class="boxHeader"> 
                    Quick Tip
                </div> 
                <div id="tip"> 
                    &nbsp;
                </div> 
            </div> 
            <div id="socialButtons" class="footerBox"> 
                <div class="boxHeader"> 
                    Make It Viral
                </div> 
                <div id="share1"> 
                    <div id="likeButton"> 
                        <iframe src="http://www.facebook.com/plugins/like.php?href=http%3A%2F%2Fytinstant.com&amp;layout=button_count&amp;show_faces=false&amp;width=90&amp;action=like&amp;colorscheme=light&amp;height=21" scrolling="no" frameborder="0" style="border:none; overflow:hidden; width:90px; height:21px;" allowTransparency="true"></iframe> 
                    </div> 
                    <div id="stumbleButton"> 
                        <script src="http://www.stumbleupon.com/hostedbadge.php?s=1&amp;r=http://ytinstant.com"></script> 
                    </div> 
                </div> 
                <div id="share2"> 
                    <div id="tweetButton"> 
                        <a href="http://twitter.com/share" class="twitter-share-button" data-count="horizontal" data-via="FreeTheFeross" data-url="http://ytinstant.com">Tweet</a><script type="text/javascript" src="http://platform.twitter.com/widgets.js"></script> 
                    </div> 
                    <div id="deliciousButton"> 
                        <div style='float:right; margin-right:10px;'> 
                        <a class="delicious-button"  href="http://delicious.com/save"> 
                           <!-- {
                           url:"http://ytinstant.com/"
                           ,title:"YouTube Instant - Real-time YouTube video surfing."
                           ,button:"wide"
                           } --> 
                           Save on Delicious
                        </a> 
                        </div> 
                    </div> 
                </div> 
                <div id="share3"> 
                    <div id="buzzButton"> 
                        <a title="Post to Google Buzz" class="google-buzz-button" href="http://www.google.com/buzz/post" data-button-style="small-count" data-url="http://ytinstant.com" data-imageurl="http://ytinstant.com/images/logo.png"></a> 
                        <script type="text/javascript" src="http://www.google.com/buzz/api/button.js"></script> 
                    </div> 
                    <div id="addthisButton"> 
                        <div class="addthis_toolbox addthis_default_style"> 
                        <a href="http://www.addthis.com/bookmark.php?v=250&amp;username=xa-4c99aba8164be257" class="addthis_button_compact">Share</a> 
                        </div> 
                        <script type="text/javascript" src="http://s7.addthis.com/js/250/addthis_widget.js#username=xa-4c99aba8164be257"></script> 
                    </div> 
                </div> 
            </div> 
            <div id="credit" class="footerBox"> 
                <p> 
                    Created by <a href="http://www.feross.org">Feross Aboukhadijeh</a>.
                </p> 
                <p> 
                    Read the <a href="http://www.feross.org/youtube-instant-media-frenzy/">YouTube Instant story</a>.
                </p> 
                <p> 
                    <span style="color: red; font-weight: bold;">Whoa!</span> <a href="http://www.feross.org/one-million-visitors-in-10-days/">1 million visitors</a> in 10 days!
                </p> 
                <p> 
                   Additional <a href="javascript:void(0)" onclick="showCredits();">credits</a>.
                </p> 
                <div id="additionalCredits"> 
                    <p> 
                        Shoutout to my friend Jake Becker.
                    </p> 
                    <p> 
                        Useful design suggestions:
                    </p> 
                    <div> 
                        <ul> 
                            <li><a href="http://creativerepository.com/">Abdus Salam</a> (logo)</li> 
                            <li><a href="http://www.reflectiv.net/">Antoine Minoux</a> (layout)</li> 
                        </ul> 
                    </div> 
                    <p> 
                        This site proudly hosted at <a href="http://www.linode.com/?r=307513b509e8c0d3292536d446f17f0cdca0e767">Linode</a>.
                    </p> 
                </div> 
            </div> 
        </div> 
    </div> 
</div> 
 
<div id="hidden"></div> 
 
<!-- Start of StatCounter Code --> 
<script type="text/javascript"> 
var sc_project=6215406;
var sc_invisible=1;
var sc_security="cf3bc309";
</script> 
 
<script type="text/javascript"
src="http://www.statcounter.com/counter/counter_xhtml.js"></script><noscript><div 
class="statcounter"><a title="web statistics"
class="statcounter"
href="http://statcounter.com/free_web_stats.html"><img 
class="statcounter"
src="http://c.statcounter.com/6215406/0/cf3bc309/1/"
alt="web statistics" /></a></div></noscript> 
<!-- End of StatCounter Code --> 
</body> 
</html>