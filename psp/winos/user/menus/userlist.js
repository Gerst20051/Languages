var strUsers = new Array;
var intUsersMinMax = new Array;
var intUsersPath = new Array;
var strUsersIconPath = new Array;
var intUsersStartMax = new Array;
var intUsers = 0;

function User(UserName,MinMax,UserPath,IconPath,StartMax){
	strUsers[intUsers]=UserName;
	intUsersMinMax[intUsers]=MinMax;
	intUsersPath[intUsers]=UserPath;
	strUsersIconPath[intUsers]=IconPath;
	intUsersStartMax[intUsers]=StartMax;
	intUsers=intUsers+1;
}

//BEGIN LIST
User("Instructions",1,"../apps/addition.html","../images/icons/menu/user/instructions.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
User("My Addition",1,"../apps/addition.html","../images/icons/menu/user/user.png",0);
//END LIST