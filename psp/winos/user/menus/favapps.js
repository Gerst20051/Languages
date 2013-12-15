var strFavApps = new Array;
var intFavAppsMinMax = new Array;
var intFavAppsPath = new Array;
var strFavAppsIconPath = new Array;
var intFavAppsStartMax = new Array;
var intFavApps = 0;

function FavApp(FavAppName,MinMax,FavAppPath,IconPath,StartMax){
	strFavApps[intFavApps]=FavAppName;
	intFavAppsMinMax[intFavApps]=MinMax;
	intFavAppsPath[intFavApps]=FavAppPath;
	strFavAppsIconPath[intFavApps]=IconPath;
	intFavAppsStartMax[intFavApps]=StartMax;
	intFavApps=intFavApps+1;
}

//BEGIN LIST
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
FavApp("Favorite",1,"None.swf","../images/icons/menu/run.png",1);
//END LIST