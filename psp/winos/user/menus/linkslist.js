var strLinks = new Array;
var intLinksPath = new Array;
var strLinksIconPath = new Array;
var strLinkNewWindow = new Array;
var intLink=0;

function Link(LinkName,LinkURL,IconPath,NewWindow){
	strLinks[intLink]=LinkName;
	intLinksPath[intLink]=LinkURL;
	strLinksIconPath[intLink]=IconPath;
	strLinkNewWindow[intLink]=NewWindow;
	intLink=intLink+1;
}

Link("Aol","http://wap.aol.com/portal/","../images/icons/menu/links/aol.png",0);
Link("eBuddy","http://www.ebuddy.com/mobile/","../images/icons/menu/links/ebuddy.png",0);
Link("Google","http://www.google.com/xhtml","../images/icons/menu/links/google.png",0);
Link("MSN","http://mobile.msn.com","../images/icons/menu/links/msn.png",0);
Link("PSP On Me","http://psponme.com","../images/icons/menu/links/psponme.png",0);
Link("PSP Wallpapers","http://www.pspwallpapers.com/psp/","../images/icons/menu/links/pspwallpapers.png",1);
Link("Station Sony","http://mobile.station.sony.com","../images/icons/menu/links/stationsony.png",1);
Link("Windows Media","http://www.windowsmedia.com/mobile/","../images/icons/menu/links/windowsmedia.png",0);
Link("Yahoo","http://ca.m.yahoo.com","../images/icons/menu/links/yahoo.png",0);