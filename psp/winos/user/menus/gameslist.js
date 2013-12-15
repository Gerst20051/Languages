var strGames = new Array;
var intGamesMinMax = new Array;
var intGamesPath = new Array;
var strGamesIconPath = new Array;
var intGamesStartMax = new Array;
var intGames = 0;

function Game(GameName,MinMax,GamePath,IconPath,StartMax){
	strGames[intGames]=GameName;
	intGamesMinMax[intGames]=MinMax;
	intGamesPath[intGames]=GamePath;
	strGamesIconPath[intGames]=IconPath;
	intGamesStartMax[intGames]=StartMax;
	intGames=intGames+1;
}

//BEGIN LIST
Game("Agent M69",1,"../games/agentm69.swf","../images/icons/menu/games/agentm69.png",1);
Game("Agent Greenny",1,"../games/agentgreenny.swf","../images/icons/menu/games/agentgreenny.png",1);
Game("Ants",1,"../games/ants.swf","../images/icons/menu/games/ants.png",1);
Game("Arm Wrestling",1,"../games/armwrestling.swf","../images/icons/menu/games/armwrestling.png",1);
Game("Cannon Comm.",1,"../games/cannoncommander.swf","../images/icons/menu/games/cannoncommander.png",1);
Game("Cat O Mania",1,"../games/catomania.swf","../images/icons/menu/games/catomania.png",1);
Game("Click Puzzle",1,"../games/clickpuzzle.swf","../images/icons/menu/games/clickpuzzle.png",1);
Game("Crabball",1,"../games/crabball.swf","../images/icons/menu/games/crabball.png",1);
Game("Escape",1,"../games/escape.swf","../images/icons/menu/games/escape.png",1);
Game("Frog & Flies",1,"../games/frog&flies.swf","../images/icons/menu/games/frog&flies.png",1);
Game("Go Marching In",1,"../games/gomarchingin.swf","../images/icons/menu/games/gomarchingin.png",1);
Game("Gold Miner",1,"../games/goldminer.swf","../images/icons/menu/games/goldminer.png",1);
Game("Hexxagon",1,"../games/hexxagon.swf","../images/icons/menu/games/hexxagon.png",1);
Game("Launch Fighters",1,"../games/launchfighters.swf","../images/icons/menu/games/launchfighters.png",1);
Game("Mastermind",1,"../games/mastermind.swf","../images/icons/menu/games/mastermind.png",1);
Game("Mind Reader",1,"../games/mindreader/index.htm","../images/icons/menu/games/mindreader.png",1);
Game("Parking Zone",1,"../games/parkingzone.swf","../images/icons/menu/games/parkingzone.png",1);
Game("Portaball",1,"../games/portaball.swf","../images/icons/menu/games/portaball.png",1);
Game("Reflex",1,"../games/reflex/index.htm","../images/icons/menu/games/reflex.png",1);
Game("Rotation",1,"../games/rotation.swf","../images/icons/menu/games/rotation.png",1);
Game("Snake",1,"../games/snake.swf","../images/icons/menu/games/snake.png",1);
Game("Soap Bubble",1,"../games/soapbubble.swf","../images/icons/menu/games/soapbubble.png",1);
Game("Telescope",1,"../games/telescope.swf","../images/icons/menu/games/telescope.png",1);
Game("Tic Tac Toe",1,"../games/tictactoe.swf","../images/icons/menu/games/tictactoe.png",1);
//END LIST