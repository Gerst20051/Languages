var intlight = 0;
var strlight;
var strFunction;
var intColumn;
var intColumnNo;
var FavAppsLen = strFavApps.length+1;
intFavApps++;

if(FavAppsLen>12){
	if(FavAppsLen>26){
		intColumnNo=3;
		intColumn=Math.ceil(strFavApps.length/3);
	}else{
		intColumnNo=2;
		intColumn=Math.ceil(strFavApps.length/2);
	}
}else{
	inColumnNo=1;
	intColumn=strFavApps.length;
}

frames['mfavapps'].style.top="0px";
frames['mfavapps'].style.height="0px";

document.write("<table width='115' cellpadding='0' cellspacing='0'>");

for (i=-2;i<intFavApps-1;i++){
	if((intColumnNo>2)&&(i==intColumn-2||i==(intColumn*2)-2)){
		document.write("</table></td><td valign='top'><table width='115' cellpadding='0' cellspacing='0'>");
		intlight=0;
	}

	if(i>-1){
		if(intlight==1){
			intlight=0;
			strlight="istart_light";
		}else{
			intlight=1;
			strlight="istart_dark";
		}

		strFunction='setTitle("' + intFavAppsMinMax[i] + '","' + strFavApps[i] + '","' + intFavAppsPath[i] + '","' + strFavAppsIconPath[i] + '",' + '"' + intFavAppsStartMax[i] + '"' + ')';
		document.write("<tr class='");
		document.write(strlight);
		document.write("' onClick='");
		document.write(strFunction);
		document.write("')><td width='22'><img src='");
		document.write(strFavAppsIconPath[i]);
		document.write("' width='20' height='20'></td><td colspan='2'><a href='#a' class='a"+strlight+"'>");
		document.write(strFavApps[i]);
		document.write("</a></td></tr>");
	}
}

document.write("</table>");
