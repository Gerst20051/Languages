var strPrograms = new Array;
var intProgramsMinMax = new Array;
var intProgramsPath = new Array;
var strProgramsIconPath = new Array;
var intProgramsStartMax = new Array;
var intProgram = 0;

function Program(ProgramName,MinMax,ProgramPath,IconPath,StartMax){
	strPrograms[intProgram]=ProgramName;
	intProgramsMinMax[intProgram]=MinMax;
	intProgramsPath[intProgram]=ProgramPath;
	strProgramsIconPath[intProgram]=IconPath;
	intProgramsStartMax[intProgram]=StartMax;
	intProgram=intProgram+1;
}

//BEGIN LIST
Program("AntiVirus",1,"../apps/antivirus/index.html","../images/icons/menu/apps/antivirus.png",0);
Program("Internet",1,"../apps/internet/index.htm","../images/icons/menu/apps/internet.png",0);
Program("Media Center",1,"../apps/mediacenter/index.html","../images/icons/menu/apps/mediacenter.png",0);
Program("Messenger",1,"../apps/messenger/index.htm","../images/icons/menu/apps/messenger.png",0);
Program("Update",1,"../controlpanel/update/index.htm","../images/icons/menu/apps/update.png",0);
//END LIST