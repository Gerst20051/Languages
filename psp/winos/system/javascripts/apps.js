var intlight = 0;
var strlight;
var strFunction;
var intColumn;
var intColumnNo;
var AppsLen = strApps.length+1;
intApps++;

if(AppsLen>12){
	if(AppsLen>26){
		intColumnNo=3;
		intColumn=Math.ceil(strApps.length/3);
	}else{
		intColumnNo=2;
		intColumn=Math.ceil(strApps.length/2);
	}
}else{
	inColumnNo=1;
	intColumn=strApps.length;
}

frames['mapps'].style.top="0px";
frames['mapps'].style.height="0px";

document.write("<table width='115' cellpadding='0' cellspacing='0'>");

for (i=-2;i<intApps-1;i++){
	if((intColumnNo>1)&&(i==intColumn-0||i==(intColumn*2)-0)){
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

		strFunction='setTitle("' + intAppsMinMax[i] + '","' + strApps[i] + '","' + intAppsPath[i] + '","' + strAppsIconPath[i] + '",' + '"' + intAppsStartMax[i] + '"' + ')';
		document.write("<tr class='");
		document.write(strlight);
		document.write("' onClick='");
		document.write(strFunction);
		document.write("')><td width='22'><img src='");
		document.write(strAppsIconPath[i]);
		document.write("' width='20' height='20'></td><td colspan='2'><a href='#a' class='a"+strlight+"'>");
		document.write(strApps[i]);
		document.write("</a></td></tr>");
	}
}

document.write("</table>");
