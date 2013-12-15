var intlight = 0;
var strlight;
var strFunction;
var intColumn;

if(strPrograms.length>12){
	if(strPrograms.length>24){
		intColumn=Math.ceil(strPrograms.length/3);
	}else{
		intColumn=Math.ceil(strPrograms.length/2);
	}
}else{
	intColumn=strPrograms.length;
}

frames['mprograms'].style.height=(intColumn)*19;

document.write("<table width='120' cellpadding='0.4' cellspacing='0'>");
document.write('<tr class="istart_dark" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','visible');showHideLayer('muser','hidden');showHideLayer('mlinks','hidden'); showHideLayer('mgames','hidden'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','hidden');showHideLayer('mfavapps','hidden');" + '"> <td width="20"><img src="../images/icons/menu/applications.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Applications</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');
document.write('<tr class="istart_light" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','hidden');showHideLayer('muser','hidden');showHideLayer('mlinks','hidden'); showHideLayer('mgames','visible'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','hidden');showHideLayer('mfavapps','hidden');" + '"> <td width="20"><img src="../images/icons/menu/games.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Games</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');
document.write('<tr class="istart_dark" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','hidden');showHideLayer('muser','hidden');showHideLayer('mlinks','hidden'); showHideLayer('mgames','hidden'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','visible');showHideLayer('mfavapps','hidden');" + '"> <td width="20"><img src="../images/icons/menu/portals.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Portals</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');
document.write('<tr class="istart_light" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','hidden');showHideLayer('muser','hidden');showHideLayer('mlinks','visible'); showHideLayer('mgames','hidden'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','hidden');showHideLayer('mfavapps','hidden');" + '"> <td width="20"><img src="../images/icons/menu/links.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Links</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');
document.write('<tr class="istart_dark" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','hidden');showHideLayer('muser','hidden');showHideLayer('mlinks','hidden'); showHideLayer('mgames','hidden'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','hidden');showHideLayer('mfavapps','visible');" + '"> <td width="20"><img src="../images/icons/menu/favoriteapps.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Favorite Apps</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');
document.write('<tr class="istart_light" onClick="' + "showHideLayer('mprograms','visible');showHideLayer('mapps','hidden');showHideLayer('muser','visible');showHideLayer('mlinks','hidden'); showHideLayer('mgames','hidden'); showHideLayer('mpspwindowsvista','hidden');showHideLayer('mportals','hidden');showHideLayer('mfavapps','hidden');" + '"> <td width="20"><img src="../images/icons/menu/useradditions.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">User Additions</a></td><td><img src="../images/arrow.gif" width="10" height="18"></td></tr>');

for (i=0;i<strPrograms.length;i++){
	if(i==intColumn||i==intColumn*2){
		document.write("</table></td><td valign='top'><table width='120' cellpadding='0.4' cellspacing='0'>");
		intlight=0;
	}

	if(intlight==1){
		intlight=0;
		strlight="istart_light";
	}else{
		intlight=1;
		strlight= "istart_dark";
	}

	strFunction='setTitle("' + intProgramsMinMax[i] + '","' + strPrograms[i] + '","' + intProgramsPath[i] + '","' + strProgramsIconPath[i] + '",' + '"' + intProgramsStartMax[i] + '"' + ')';
	document.write("<tr class='");
	document.write(strlight);
	document.write("' onClick='");
	document.write(strFunction);
	document.write("')><td width='22'><img src='");
	document.write(strProgramsIconPath[i]);
	document.write("' width='20' height='20'></td><td><a href='#a' class='a"+strlight+"'>");
	document.write(strPrograms[i]);
	document.write("</a></td><td></td></tr>");
}
document.write('<tr class="istart_light" onClick="' + "showHideLayer('welcomecenter','visible');showHideLayer('welcomeExit','visible');showHideLayer('welcomecheckbox','visible');hideMenu();" + '"> <td width="20"><img src="../images/icons/menu/apps/welcomecenter.png" width="20" height="20" border="0"></td><td><a href="#" class="aistart_dark">Welcome Center</a></td><td></td></tr>');
document.write("</table>");
