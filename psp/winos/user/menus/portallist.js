var portalName = new Array;
var portalURL = new Array;
var intPortal = 0;

function Portal(strPortalName,strPortalURL){
	portalName[intPortal]=strPortalName;
	portalURL[intPortal]=strPortalURL;
	intPortal++;
}

//BEGIN LIST
Portal("Browse...","../apps/runsearch/index.htm");
Portal("Portal A","file:/portala.htm");
Portal("Portal B","file:/portalb.htm");
Portal("Portal C","file:/portalc.htm");
Portal("Portal D","file:/portald.htm");
Portal("Portal E","file:/portale.htm");
Portal("Portal F","file:/portalf.htm");
//END LIST

for (i=0;i<portalName.length;i++){
	document.write("<option value="+portalURL[i]+">"+portalName[i]+"</option>");
}