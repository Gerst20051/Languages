var intlight = 0;
var strlight;
var strFunction;
var intColumn;
var intColumnNo;
var UsersLen = strUsers.length+1;
intUsers++;

if(UsersLen>12){
	if(UsersLen>26){
		intColumnNo=3;
		intColumn=Math.ceil(strUsers.length/3);
	}else{
		intColumnNo=2;
		intColumn=Math.ceil(strUsers.length/2);
	}
}else{
	inColumnNo=1;
	intColumn=strUsers.length;
}

frames['muser'].style.top="0px";
frames['muser'].style.height="0px";

document.write("<table width='115' cellpadding='0' cellspacing='0'>");

for (i=-2;i<intUsers-1;i++){
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

		strFunction='setTitle("' + intUsersMinMax[i] + '","' + strUsers[i] + '","' + intUsersPath[i] + '","' + strUsersIconPath[i] + '",' + '"' + intUsersStartMax[i] + '"' + ')';
		document.write("<tr class='");
		document.write(strlight);
		document.write("' onClick='");
		document.write(strFunction);
		document.write("')><td width='22'><img src='");
		document.write(strUsersIconPath[i]);
		document.write("' width='20' height='20'></td><td colspan='2'><a href='#a' class='a"+strlight+"'>");
		document.write(strUsers[i]);
		document.write("</a></td></tr>");
	}
}

document.write("</table>");
