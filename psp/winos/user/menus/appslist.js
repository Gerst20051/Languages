var SkipMaxMsg = 0;

var strApps = new Array;
var intAppsMinMax = new Array;
var intAppsPath = new Array;
var strAppsIconPath = new Array;
var intAppsStartMax = new Array;
var intApps = 0;

function App(AppName,MinMax,AppPath,IconPath,StartMax){
	strApps[intApps]=AppName;
	intAppsMinMax[intApps]=MinMax;
	intAppsPath[intApps]=AppPath;
	strAppsIconPath[intApps]=IconPath;
	intAppsStartMax[intApps]=StartMax;
	intApps=intApps+1;
}

//BEGIN LIST (add new apps below here)
App("About",1,"../apps/about/index.htm","../images/icons/menu/apps/about.png",0);
App("Analog Clock",1,"../apps/analogclock.swf","../images/icons/menu/apps/analogclock.png",0);
App("Browser Info",1,"../apps/browserinfo/index.htm","../images/icons/menu/apps/browserinfo.png",0);
App("Calculator",1,"../apps/calculator/index.htm","../images/icons/menu/apps/calculator.png",0);
App("Calender",1,"../apps/calender/index.htm","../images/icons/menu/apps/calender.png",0);
App("Conversion Calc.",1,"../apps/conversioncalculators/index.htm","../images/icons/menu/apps/conversioncalculators.png",0);
App("Day Organizer",1,"../apps/dayorganizer/index.html","../images/icons/menu/apps/dayorganizer.png",0);
App("FLA Save",1,"../apps/flasave/index.htm","../images/icons/menu/apps/flasave.png",0);
App("Note List",1,"../apps/notelist/index.html","../images/icons/menu/apps/notelist.png",0);
App("Paint",1,"../apps/paint.swf","../images/icons/menu/apps/paint.png",1);
App("PDF Viewer",1,"../apps/pdfviewer/index.html","../images/icons/menu/apps/pdfviewer.png",0);
App("Percentage Calc.",1,"../apps/percentagecalculator/index.htm","../images/icons/menu/apps/percentagecalculator.png",0);
App("Periodic Table",1,"../apps/periodictable/index.htm","../images/icons/menu/apps/periodictable.png",0);
App("Phone Num Conv.",1,"../apps/phonenumberconverter/index.htm","../images/icons/menu/apps/phonenumberconverter.png",0);
App("Pixel Fixer",1,"../controlpanel/fixpix.htm","../images/icons/menu/apps/pixelfixer.png",0);
App("Roman Num Conv.",1,"../apps/romannumeralconverter/index.htm","../images/icons/menu/apps/romannumeralconverter.png",0);
App("Schedule",1,"../apps/schedule/index.htm","../images/icons/menu/apps/schedule.png",0);
App("Skweezer",1,"../apps/skweezer/index.htm","../images/icons/menu/apps/skweezer.png",0);
App("Stopwatch",1,"../apps/stopwatch/index.htm","../images/icons/menu/apps/stopwatch.png",0);
App("System Info",1,"../controlpanel/system.htm","../images/icons/menu/apps/systeminfo.gif",0);
App("Text Editor",1,"../apps/texteditor/index.htm","../images/icons/menu/apps/texteditor.png",0);
App("Timeline",1,"../apps/timeline.swf","../images/icons/menu/apps/timeline.png",0);
App("TXT Pad",1,"../apps/txtpad/index.htm","../images/icons/menu/apps/txtpad.png",0);
App("Wob Zip",1,"../apps/wobzip/index.html","../images/icons/menu/apps/wobzip.png",0);
//END LIST (add new apps above here)