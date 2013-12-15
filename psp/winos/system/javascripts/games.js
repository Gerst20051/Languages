var intlight = 0;
var strlight;
var strFunction;
var intColumn;
var intColumnNo;
var GamesLen = strGames.length+1;
intGames++;

if(GamesLen>12){
	if(GamesLen>26){
		intColumnNo=3;
		intColumn=Math.ceil(strGames.length/3);
	}else{
		intColumnNo=2;
		intColumn=Math.ceil(strGames.length/2);
	}
}else{
	inColumnNo=1;
	intColumn=strGames.length;
}

frames['mgames'].style.top="0px";
frames['mgames'].style.height="0px";

document.write("<table width='115' cellpadding='0' cellspacing='0'>");

for (i=-2;i<intGames-1;i++){
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

		strFunction='setTitle("' + intGamesMinMax[i] + '","' + strGames[i] + '","' + intGamesPath[i] + '","' + strGamesIconPath[i] + '",' + '"' + intGamesStartMax[i] + '"' + ')';
		document.write("<tr class='");
		document.write(strlight);
		document.write("' onClick='");
		document.write(strFunction);
		document.write("')><td width='22'><img src='");
		document.write(strGamesIconPath[i]);
		document.write("' width='20' height='20'></td><td colspan='2'><a href='#a' class='a"+strlight+"'>");
		document.write(strGames[i]);
		document.write("</a></td></tr>");
	}
}

document.write("</table>");
