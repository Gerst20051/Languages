(function() {
if(!window.goomPartnerId) {
    return;
}

var env = window.goomEnv == "" || !window.goomEnv ? "" : window.goomEnv+".";
var baseURL = 'http://slam.' + env + 'goomradio.com/?partnerId=' + encodeURI(window.goomPartnerId);
var queryStr = [];

// Skin
if(window.goomBgColor) {
    queryStr.push('&bgColor=' + encodeURI(window.goomBgColor));
}
if(window.goomBgURL) {
    queryStr.push('&bgURL=' + encodeURI(window.goomBgURL));
}
if(window.goomBgXPos) {
    queryStr.push('&bgXPos=' + encodeURI(window.goomBgXPos));
}
if(window.goomBgYPos) {
    queryStr.push('&bgYPos=' + encodeURI(window.goomBgYPos));
}
if(window.goomPlayerColor) {
    queryStr.push('&playerColor=' + encodeURI(window.goomPlayerColor));
}
if(window.goomMyLogoURL) {
    queryStr.push('&myLogoURL=' + encodeURI(window.goomMyLogoURL));
}
if(window.goomMyLogoLinkURL) {
    queryStr.push('&myLogoLinkURL=' + encodeURI(window.goomMyLogoLinkURL));
}
if(window.goomCoverButtonURL) {
    queryStr.push('&coverButtonURL=' + encodeURI(window.goomCoverButtonURL));
}


// Global Settings:
if(window.goomDefaultRadio) {
    queryStr.push('&defaultRadio=' + encodeURI(window.goomDefaultRadio));
} else if(window.goomLocalRadioId && window.goomDomain) {
	queryStr.push('&defaultRadio=' + A2ItoGoom(encodeURI(window.goomLocalRadioId), encodeURI(window.goomDomain), 1));
}
if(window.goomMountPolicy) {
    queryStr.push('&mountPolicy=' + encodeURI(window.goomMountPolicy));
}
if(window.goomAutoPlay) {
    queryStr.push('&autoPlay=' + encodeURI(window.goomAutoPlay));
}
if(window.goomPopUpMode) {
    queryStr.push('&popupMode=' + encodeURI(window.goomPopUpMode));
}
if(window.goomKnobType) {
    queryStr.push('&knobType=' + encodeURI(window.goomKnobType));
} else {
	queryStr.push('&knobType=partner');
}
if(window.goomDefaultVolume) {
    queryStr.push('&defaultVolume=' + encodeURI(window.goomDefaultVolume));
}
if(window.goomCachedMute) {
    queryStr.push('&cachedMute=' + encodeURI(window.goomCachedMute));
}
if(window.goomCustomUI) {
    queryStr.push('&customUI=' + encodeURI(window.goomCustomUI));
}
if(window.goomBuySong) {
	queryStr.push('&buySong=' + encodeURI(window.goomBuySong));
}
if(window.goomShuffleMode) {
    queryStr.push('&shuffleMode=' + encodeURI(window.goomShuffleMode));
}
if(window.goomActiveZoneURL) {
    queryStr.push('&activeZoneURL=' + encodeURI(window.goomActiveZoneURL));
}

// Height/Width:
var sizeArr = [300, 300];
if(window.goomHeight) {
    queryStr.push('&height=' + encodeURI(window.goomHeight));
    sizeArr[0] = parseInt(window.goomHeight,10);
}
if(window.goomWidth) {
    queryStr.push('&width=' + encodeURI(window.goomWidth));
    sizeArr[1] = parseInt(window.goomWidth, 10);
}

function readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') c = c.substring(1,c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
    }
    return null;
}

function A2ItoGoom(radioId, domainId, originId) {
	return (radioId * Math.pow(2, 8)) + (domainId * Math.pow(2, 4)) + originId;
}

if(!window.goomPopUp) {
    document.writeln('<iframe frameborder="0" scrolling="no" style="border:0px;margin:0px;padding:0px;height:'+ sizeArr[0] +'px;overflow:hidden;width:'+sizeArr[1]+'px;border:0px;margin:0px;padding:0px;" src="'+ baseURL + queryStr.join('') +'"></iframe>');
} else {
    if(!readCookie('__goompopplayer')) {
        var winPop = document.open(queryStr.join(''),'goomPopup', 'width=' +  (sizeArr[1] + 10) + ',height=' +  (sizeArr[0] + 10));
        if(winPop) {
             var date = new Date();
             date.setTime(date.getTime()+(15*60*1000));
             document.cookie = '__goompopplayer'+'=goom; expires=' + date.toGMTString() +'; path=/';
             if (window.focus) {
                winPop.focus()
             }
        }
    }

    if(window.goomPopUpBanner) {
        var winPopOnClick = 'document.open(\'' + queryStr.join("") + '\',\'goomPopup\',\'width=' + (sizeArr[1] + 10) + ',height=' + (sizeArr[0] + 10) + '\')';
        document.writeln('<img onclick="' + winPopOnClick + '" src="' + window.goomPopUpBanner  +'">');
    }
}

})()
