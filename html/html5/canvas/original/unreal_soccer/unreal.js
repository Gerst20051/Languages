// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

var timers = [];
var timernames = {};

function Timer(name)
{
  this.name = name;
  this.time = 0;

  timernames[name] = this;
  timers.push(this);

  this.start = function()
    {
      this.starttime = gettime();
    }

  this.stop = function()
    {
      this.time += gettime() - this.starttime;
    }
}

function starttimer(name)
{
  if (debug)
    timernames[name].start();
}

function stoptimer(name)
{
  if (debug)
    timernames[name].stop();
}

function printtimers()
{
  if (!debug)
    return;
  var sum = 0;
  var blank = "                    ";
  timers = timers.sort(function(a, b) { return b.time - a.time; });
  timers.foreach(function(t) { sum += t.time; });
  timers.foreach(function(t)
		 {
		   var ms = "" + t.time;
		   var ps = "" + parseInt(ms / sum * 100);
		   puts(t.name + ":" + blank.substr(0, 20 - name.length) + " " +
			ms.substr(0, 5) + " " + ps + "%");
		 });
}

function get(id)
{
  return document.getElementById(id);
}

function rand(num)
{
  return Math.floor(Math.random() * num);
}

Array.prototype.foreach = function(fn)
{
  for (var i = 0; i < this.length; i++)
    fn(this[i]);
};

Array.prototype.foreachwithindex = function(fn)
{
  for (var i = 0; i < this.length; i++)
    fn(this[i], i);
}

Array.prototype.foreachinorder = function(indices, fn)
{
  for (var i = 0; i < indices.length; i++)
    fn(this[indices[i]]);
}

Array.prototype.shuffle = function()
{
  this.sort(function(a, b) { rand(2) == 0; });
};

Array.prototype.find = function(fn)
{
  for (var i = 0; i < this.length; i++)
    if (fn(this[i]))
      return this[i];

  return null;
}

function Lowest(init)
{
  this.init = init;
  this.lowest = init;
  this.object = null;

  this.register = function(obj, num)
    {
      if (!this.lowest || num < this.lowest)
	{
	  this.lowest = num;
	  this.object = obj;
	}
    }

  this.reset = function()
    {
      this.lowest = this.init;
      this.object = null;
    }
}

function Highest(init)
{
  this.highest = init;
  this.object = null;

  this.register = function(obj, num)
    {
      if (num > this.highest || !this.highest)
	{
	  this.highest = num;
	  this.object = obj;
	}
    }
}

var ctx;
var canvas;
var players;
var zones;
var teams;
var points;
var ball;
var sortorders = [];
var things;
var fieldzone;
var goalzone0;
var shootzone0;
var goalzone1;
var shootzone1;
var settings;
var ais;
var updatables;
var starttime;
var debug = false;
var gametimelength = 0;
var markers;

function GameState(name, ballinplay, blink)
{
  this.name = name;
  this.ballinplay = ballinplay;
  this.blink = blink;
}

var initializing = new GameState("initializing", false);
var kickoff = new GameState("kickoff", true);
var play = new GameState("play", true);
var outside = new GameState("outoffield", false, true);
var kickin = new GameState("kickin", true);
var goal = new GameState("goal", false, true);
var nullgamestate = new GameState("null", false, false);
var paused = new GameState("paused", false, true);
var ended = new GameState("ended", false, true);
var gamestate = nullgamestate;

function getgamestate()
{
  return gamestate;
}

function changegamestate(state)
{
  //  ais.foreach(function(ai) { ai.gamestatechanged(gamestate, state); });
  ais.foreach(function(ai) { ai.queue(ai.gamestatechanged, gamestate, state); });
  gamestate = state;
  if (gamestate == kickoff && !selected && playerteam)
    {
      selectplayer(playerteam.members[1 + rand(2)]);
    }
  updategamestate();
}

function getrandomorder()
{
  return sortorders[rand(sortorders.length)];
}


var loadingimgcount = 0;
var imgs = {};

function loadimages(names)
{
  names.foreach(function(n)
		{
		  loadingimgcount += 1;
		  var img = new Image();
		  img.src = n + ".png";
		  img.onload = imageloaded;
		  imgs[n] = img;
		});
}

function getimage(name)
{
  return imgs[name];
}

function imageloaded()
{
  loadingimgcount -= 1;
  if (loadingimgcount == 0)
    onallimagesloaded();
}

var playerteam = null;

function gettime()
{
  return new Date().getTime();
}

function onallimagesloaded()
{
}

function gameisrunning()
{
  return starttime != null || gamestate == initializing;
}

function startgame()
{
  //  starttime = new Date().getTime();
  gamestate = nullgamestate;
  lasttime = gettime();

  run();
}

var selected = null;
var hovered = null;
var mousepoint = new Coord(0, 0);
var mousemoveevt = false;
var mouseclickevt = false;
var keypressevt = false;
var keycode = 0;

function onfieldhovered(evt)
{
  if (typeof mousepoint == "undefined")
    return;
  mousepoint.x = evt.clientX - 24;
  mousepoint.y = evt.clientY - 20;
  mousepoint.ctrl = evt.ctrlKey;
  mousepoint.ingoalzone = shootzone1.contains(mousepoint);
  mousemoveevt = true;
}

var checkfieldhoveredlow = new Lowest(500);

function checkfieldhovered()
{
  var low = checkfieldhoveredlow;
  low.reset();

  things.foreach(function(t)
		 {
		   if (t.controllable || t.interceptable)
		     low.register(t, t.p.distsq(mousepoint));
		 });
  
  hovered = low.object;
}

function onkeypress(evt)
{
  keycode = typeof opera != "undefined" ? evt.keyCode : evt.charCode;

  if (gameispaused())
    checkkeypress();
  else
    keypressevt = true;
}

function checkkeypress()
{
  if (!playerteam)
    return;

  switch (keycode)
    {
    case 49:
      selectplayer(playerteam.members[0]);
      break;
    case 50:
      selectplayer(playerteam.members[1]);
      break;
    case 51:
      selectplayer(playerteam.members[2]);
      break;
    case 52:
      selectplayer(playerteam.members[3]);
      break;
    case 9:
    case 113:
    case 81:
      var low = new Lowest();
      playerteam.members.foreach(function(p) { low.register(p, p.p.distsq(ball.p)); });
      selectplayer(low.object);
      break;
    case 65:
    case 97:
      playerteam.ai.offend();
      updateaicontrol();
      break;
    case 80:
    case 112:
      pausegame();
      break;
    case 82:
    case 114:
    case 68:
      playerteam.ai.defend();
      updateaicontrol();
      break;
    case 88:
    case 120:
      exitgame();
      break;
    case 32:
      var p = ball.holder;
      if (p && p.team == playerteam && selected)
	{
	  p.kick(ball, selected.p);
	  selected.receive(ball);
	}
      break;
    }
}


function selectplayer(p)
{
  if (!p || !p.controllable)
    return;
  if (selected)
    selected.selected = false;
  selected = p;
  selected.selected = true;
}

function onfieldclicked(evt)
{
  onfieldhovered(evt);
  mousemoveevt = false;
  mouseclickevt = true;
}

var selectaction = 1;
var passaction = 2;
var interceptaction = 3;
var kickaction = 4;
var shootaction = 5;
var runaction = 6;

var actioncolors = [null, null, "darkorchid", "red", "yellow", "red", "white"];
var actionimages = [null, "hovermarker", "passmarker", "targetmarker", null, null, null];

function getaction()
{
  if (!selected && hovered && hovered.controllable)
    return selectaction;
  if (!selected)
    return null;
  // selected
  if (!selected.holding)
    {
      if (hovered)
	{
	  if (hovered.controllable)
	    return selectaction;
	  if (hovered.interceptable || hovered.holding)
	    return interceptaction;
	}
      return runaction;
    }
  // selected.holding
  if (hovered)
    {
      if (hovered.controllable && (mousepoint.ctrl || settings["passbeatsselect"]))
	return passaction;
      else if (hovered.controllable)
	return selectaction;
      else if (mousepoint.ctrl && mousepoint.ingoalzone)
	return shootaction;
      else if (mousepoint.ctrl)
	return kickaction;
      return runaction;
    }
  // selected && selected.holding && !hovered
  if (mousepoint.ingoalzone)
    return shootaction;
  else if (mousepoint.ctrl)
    return kickaction;
  return runaction;
}

function checkfieldclicked()
{
  mousepoint.ingoalzone = shootzone1.contains(mousepoint);

  var low = checkfieldhoveredlow;
  low.reset();

  things.foreach(function(t)
		 {
		   if (t.controllable || t.interceptable)
		     low.register(t, t.p.distsq(mousepoint));
		 });

  hovered = low.object;

  var action = getaction();
  switch (action)
    {
    case selectaction:
      selectplayer(hovered);
      break;
    case passaction:
      hovered.receive(ball);
      hovered.lastcommandtime = gettime();
      var p = ball.holder;
      if (p && p.team == playerteam && p.kick(ball, mousepoint))
	p.lastcommandtime = gettime();
      if (settings["selectonpasstarget"])
	selectplayer(hovered);
      break;
    case interceptaction:
      selected.intercept(ball);
      selected.lastcommandtime = gettime();
      break;
    case kickaction:
      var p = ball.holder;
      if (p && p.team == playerteam && p.kick(ball, mousepoint))
	p.lastcommandtime = gettime();
      break;
    case shootaction:
      var p = ball.holder;
      if (p && p.team == playerteam && p.kick(ball, mousepoint, true))
	{
	  p.lastcommandtime = gettime();
	  ais.foreach(function(ai) { ai.queue(ai.ballshot, p, ball); });
	}
      break;
    case runaction:
      selected.runto(mousepoint.x, mousepoint.y);
      selected.lastcommandtime = gettime();
      break;
    }
}

function initializesettings()
{
  settings = {};
  settings["selectonhold"] = true;
  settings["selectonpasstarget"] = true;
  settings["passbeatsselect"] = true;
  settings["progression"] = 7;
  settings["checkforupdates"] = true;
  settings["version"] = version;
  settings["lang"] = "en";
}

function initializegame()
{
  if (navigator && navigator.language)
    {
      var l = navigator.language;
      var i = l.indexOf("-");
      if (i != -1)
	l = l.substr(0, i);
      if (langstrings[l])
	settings["lang"] = l;
    }

  canvas = get("canvas");
  ctx = canvas.getContext("2d");
  canvas.addEventListener("click", onfieldclicked, false);
  canvas.addEventListener("mousemove", onfieldhovered, false);
  //  canvas.addEventListener("keydown", onkeypress, false);
  //  ctx.lockCanvasUpdates(true);
  //  var cc = get("canvascontainer");
  //  cc.addEventListener("keydown", onkeypress, false);
  //  window.addEventListener("keydown", onkeypress, false);
  document.documentElement.addEventListener("keydown", onkeypress, false);
}

function setupgame(team1, team2, team1ai)
{
  debug = team1ai && !demomode;

  players = [];
  zones = [];
  points = {};
  things = [];
  ais = [];
  updatables = [];
  markers = [];

  for (var yi = 0; yi < 5; yi++)
    for (var xi = 0; xi < 5; xi++)
      {
	var z = new Zone("z" + yi + "" + xi, xi * 50, yi * 75, 100, 100);
	var p = z.midp;
	zones.push(z);
	points["midp" + yi + "" + xi] = p;
	updatables.push(z);
	//	updatables.push(p);
      }
  
  [
   [119, 15], [179, 15], [119, 387], [179, 387]
   ].foreach(function(c)
	     {
	       var gp = new Goalpost(c[0], c[1]);
	       things.push(gp);
	     });

  updatables.shuffle();
  
  ball = new Ball(285, 200);
  var bp = new BallPainter(ball, getimage("ball"));
  ball.painter = bp;
  things.push(ball);

  var mai = new MonkeyAI(!team1ai);
  ais.push(mai);
  var mai2 = new MonkeyAI(false);
  ais.push(mai2);
  var t1 = new Team(0, team1, mai);
  var t2 = new Team(1, team2, mai2);
  if (!team1ai)
    playerteam = t1;
  else
    playerteam = null;
  teams = [t1, t2];
  kickoff.team = t1;

  var rai = new RefereeAI();
  var rt = new RefereeTeam(rai);
  ais.push(rai);

  for (var i = 0; i < 4; i++)
    {
      var p1 = t1.constructplayer();
      var p2 = t2.constructplayer();
      
      players.push(p1);
      players.push(p2);
      things.push(p1);
      things.push(p2);
      updatables.push(p1);
      updatables.push(p2);
    }
  things.push(t1.constructgoal());
  things.push(t2.constructgoal());

  var rp = rt.constructplayer();
  players.push(rp);
  things.push(rp);
  rp.intercept(ball);

  var numbers = new Array(things.length);
  for (var i = 0; i < numbers.length; i++)
    numbers[i] = i;

  sortorders = new Array(100);
  for (var i = 0; i < sortorders.length; i++)
    {
      sortorders[i] = new Array(things.length);
      numbers.shuffle();
      for (var j = 0; j < numbers.length; j++)
	sortorders[i][j] = numbers[j];
    }

  fieldzone = new Zone("field", 2, 2, 296, 396);
  var gz1 = new Zone("goal0", 84, 334, 130, 65);
  var gz2 = new Zone("goal1", 84, 0, 130, 65);
  zones.push(gz1);
  zones.push(gz2);
  updatables.push(gz1);
  updatables.push(gz2);

  var gp1 = points["goal0"] = new Point(gz1, 149, 394);
  var gp2 = points["goal1"] = new Point(gz2, 149, 5);
  //  updatables.push(gp1);
  //  updatables.push(gp2);

  goalzone0 = new Zone("goalzone0", 119, 381, 60, 20);
  goalzone1 = new Zone("goalzone1", 119, -2, 60, 20);
  shootzone0 = new Zone("shootzone0", 119, 361, 60, 40);
  shootzone1 = new Zone("shootzone1", 110, -2, 60, 40);

  ais.foreach(function(ai) { ai.initialize(); });

  initializepanel();
}

function resetgame()
{
  printtimers();
  playerteam = null;
  playerteamname = null;
  starttime = null;
  gamestate = nullgamestate;
  selected = null;
  hovered = null;
  pause = false;
  demomode = false;
}

function placemarker(p, color, shortlived)
{
  if (!debug)
    return;
  p.color = color;
  p.time = gettime();
  if (shortlived)
    p.time -= 3000;
  markers.push(p);
}

function loadallimages()
{
  loadimages(["field", "penguin", "hovermarker", "selectmarker", "goal", "ball", "ballmarker", "shadow", "passmarker", "monkey", "robot", "targetmarker", "android", "ninja", "viking", "pirate", "alien", "scriptkid"]);
}

var hoverangle = 0.0;
var selectangle = 0.0;
var shadowwidths = [24, 20, 16, 13, 10, 8];
var shadowheights = [10, 8, 6, 5, 4, 3];

function drawshadow(ctx, x, y, size)
{
  var img = getimage("shadow");
  var w = shadowwidths[size];
  var h = shadowheights[size];
  ctx.drawImage(img, size * 24, 0, 24, 10, x + 1 - 12, y - 1, 24, 10);
}

new Timer("clearrect");
new Timer("the rest");

function resetfield()
{
    ctx.clearRect(0, 0, 300, 400);
    //  ctx.globalCompositeOperation = "copy";
    //  ctx.drawImage(imgs["field"], 0, 0);
    //  ctx.globalCompositeOperation = "source-over";

  var size = 2;
  if (ball.height >= 2.0)
    size = 5;
  else if (ball.height >= 1.0)
    size = 4;
  else if (ball.height >= 0.5)
    size = 3;

  drawshadow(ctx, ball.p.x, ball.p.y + size * 5 - 7, size);

  players.foreach(function(p)
		  {
		    if (p.team.stats.shadowsize >= 0)
		      drawshadow(ctx, p.x, p.y, p.team.stats.shadowsize);
		    if ((!debug && !p.controllable) || !p.running || gamestate == initializing || p.referee || demomode)
		      return;
		    ctx.save();
		    ctx.lineWidth = 2;
		    ctx.lineCap = "round";
		    ctx.globalAlpha = 0.5;
		    ctx.strokeStyle = "lightgray";
		    ctx.beginPath();
		    ctx.moveTo(p.runfromp.x, p.runfromp.y);
		    ctx.lineTo(p.p.x, p.p.y);
		    ctx.closePath();
		    ctx.stroke();
		    if (p.intercepting)
		      ctx.strokeStyle = "red";
		    else if (p.receiving)
		      ctx.strokeStyle = "darkorchid";
		    else
		      ctx.strokeStyle = "white";
		    ctx.beginPath();
		    ctx.moveTo(p.p.x, p.p.y);
		    ctx.lineTo(p.runtop.x, p.runtop.y);
		    ctx.closePath();
		    ctx.stroke();
		    ctx.restore();
		  });
  
  //  if (ball.holder && hovered && mousepoint.ctrl)

  if (selected)
    {
      ctx.save();
      var action = getaction();
      var color = actioncolors[action];
      if (color)
	{
	  ctx.strokeStyle = color;
	  ctx.globalAlpha = 0.5;
	  ctx.lineWidth = 2;
	  ctx.lineCap = "round";
	  ctx.beginPath();
	  if (mousepoint.ctrl && ball.holder)
	    ctx.moveTo(ball.holder.p.x, ball.holder.p.y);
	  else
	    ctx.moveTo(selected.x, selected.y);
	  ctx.lineTo(mousepoint.x, mousepoint.y);
	  ctx.closePath();
	  ctx.stroke();
	}
      ctx.restore();
    }

  if (hovered)
    {
      var action = getaction();
      var img = actionimages[action];
      if (img)
	ctx.drawImage(getimage(img), hovered.p.x - 16, hovered.p.y - 6);
    }

  if (selected)
    {
      ctx.drawImage(getimage("selectmarker"), selected.p.x - 16, selected.p.y - 6);
    }

  if (ball.holder)
    {
      ctx.drawImage(getimage("ballmarker"), ball.holder.p.x - 27, ball.holder.p.y - 8);
    }

  if (debug && markers.length > 0)
    {
      var newmarkers = [];
      markers.foreach(function(p)
		      {
			ctx.save();
			ctx.strokeStyle = p.color;
			ctx.lineWidth = 3;
			ctx.beginPath();
			ctx.moveTo(p.x, p.y);
			ctx.arc(p.x, p.y, 7, 0, 2 * Math.PI, true);
			ctx.closePath();
			ctx.stroke();
			ctx.restore();
			if (gettime() - p.time < 4000)
			  newmarkers.push(p);
		      });
      markers = newmarkers;
    }
}

var curupdatableidx = 0;

function updateupdatables(dt)
{
  for (var i = curupdatableidx; i < Math.min(curupdatableidx + 5, updatables.length); i++)
    updatables[i].updatedata(dt);

  curupdatableidx += 5;
  if (curupdatableidx >= updatables.length)
    curupdatableidx = 0;
}

var lastgoaltime = null;

function scoregoal(teamidx)
{
  if (lastgoaltime && new Date().getTime() - lastgoaltime < 10000)
    return;
  lastgoaltime = new Date().getTime();
  goal.ball = ball;
  goal.team = teams[teamidx];
  goal.team.score += 1;
  kickoff.team = teams[teams[teamidx].oppidx];
  if (ball.holder)
    ball.holder.release(ball);
  changegamestate(goal);
  updatescores();
}

function checkcollissions(t)
{
  var now = new Date().getTime();
  things.foreachinorder(getrandomorder(), function(t2)
			{
			  if (t2 != t && t2.active && t2.holder != t && t.holder != t2)
			    {
			      var d = t.p.dist(t2.p);
			      if (d < Math.max(t.radius, t2.radius))
				{
				  var time1 = t.collidetime;
				  var time2 = t2.collidetime;
				  if ((time1 && now - time1 < 500) ||
				      (time2 && now - time2 < 500))
				    ; // no collission
				  else
				    {
				      var ignore = t.collide(t2);
				      ignore = ignore || t2.collide(t);
				      if (!ignore)
					{
					  t.collidetime = now;
					  t2.collidetime = now;
					}
				    }
				}
			    }
			});
}

var lastpulse = gettime();
var lasttime = gettime();

var pause = false;
var pausetime = null;

function exitgame()
{
  window.close();
}

function pausegame()
{
  if (gamestate == paused)
    {
      var interval = gettime() - pausetime;
      if (interval < 500)
	return;
      lasttime += interval;
      if (starttime)
	starttime += interval;
      gamestate = gamestate.prevstate;
      updategamestate();
      pause = false;
      pausetime = gettime();
      setTimeout(run, 50);
    }
  else
    {
      var interval = gettime() - pausetime;
      if (interval < 500)
	return;
      pause = true;
    }
}

function gameispaused()
{
  return gamestate == paused;
}

function drawpause()
{
  ctx.save();
  ctx.globalAlpha = 0.5;
  ctx.fillStyle = "black";
  ctx.fillRect(0, 0, 300, 400);
  ctx.restore();
}

function moveawayplayers(team)
{
  team.members.foreach(function(p)
		       {
			 if (p.p.dist(ball.p) < 70)
			   {
			     var dir = ball.p.angle(p.p);
			     p.p.x = ball.p.x + Math.cos(dir) * 70;
			     p.p.y = ball.p.y + Math.sin(dir) * 70;
			     p.speed = 0.0;
			     p.speedto = 0.0;
			   }
		       });
}

function putplayersontheirsideofthefield()
{
  players.foreach(function(p)
		  {
		    var slack = p.intercepting || p.holding ? 10 : 0;
		    if (p.team.idx == 0 && p.p.y < 200 - slack)
		      {
			p.speed = 0.0;
			p.speedto = 0.0;
			p.p.y = 200 - slack;
		      }
		    else if (p.team.idx == 1 && p.p.y > 200 + slack)
		      {
			p.speed = 0.0;
			p.speedto = 0.0;
			p.p.y = 200 + slack;
		      }
		  });
}

new Timer("update");
new Timer("collissions");
new Timer("zonechecks");
new Timer("events");
new Timer("resetfield");
new Timer("paint");
new Timer("ai");

function isdefined(x) { return typeof x != "undefined"; }

function checkzones()
{
  if (goalzone0.contains(ball.p) && gamestate == play && ball.height < 2)
    {
      scoregoal(1);
      ball.speed /= 3;
    }
  else if (goalzone1.contains(ball.p) && gamestate == play && ball.height < 2)
    {
      scoregoal(0);
      ball.speed /= 3;
    }
  else if (!fieldzone.contains(ball.p) && gamestate != outside && gamestate != goal && (!ball.lastoutofplaytime || gettime() - ball.lastoutofplaytime > 7000))
    {
      if (ball.holder)
	{
	  if (ball.holder.referee)
	    return;
	  ball.holder.release(ball);
	}
      outside.ball = ball;
      kickin.team = teams[ball.lasttouch.team.oppidx];
      kickin.ball = ball;
      ball.speed /= 3;
      changegamestate(outside);
    }
}

function run()
{
  var start = gettime();
  var diff = start - lasttime;
  var dt = diff / 1000.0;
  if (dt == 0)
    {
      setTimeout(run, 50);
      return;
    }
  else if (diff > 200)
    {
      var timeslip = diff - 200;
      if (starttime)
	starttime += timeslip;
      dt = 0.2;
    }
  else if (diff < 80)
    {
      dt = 0.05;
    }

  lasttime = start;
  starttimer("update");
  things.foreachinorder(getrandomorder(), function(t)
			{
			  if (t.active)
			    {
			      t.update(dt);
			    }
			});
  stoptimer("update");

  starttimer("collissions");
  checkcollissions(ball);
  stoptimer("collissions");

  starttimer("zonechecks");
  checkzones();
  stoptimer("zonechecks");
  if (gamestate == kickin || gamestate == kickoff)
    moveawayplayers(teams[gamestate.team.oppidx]);
  if (gamestate == kickoff)
    putplayersontheirsideofthefield();

  //  ctx.lockCanvasUpdates(false);
  //  ctx.updateCanvas();
  //  ctx.lockCanvasUpdates(true);

  starttimer("events");
  if (keypressevt)
    {
      checkkeypress();
      keypressevt = false;
    }
  if (pause)
    {
      pausetime = gettime();
      paused.prevstate = gamestate;
      gamestate = paused;
      updategamestate();
      drawpause();
      pause = false;
      return;
    }
  if (mousemoveevt)
    {
      checkfieldhovered();
      mousemoveevt = false;
    }
  if (mouseclickevt)
    {
      checkfieldclicked();
      mouseclickevt = false;
    }
  stoptimer("events");

  starttimer("resetfield");
  resetfield();
  stoptimer("resetfield");
  
  starttimer("paint");
  things.sort(function(a, b) { return a.p.y - b.p.y; });
  
  things.foreach(function(t)
		 {
		   if (t.visible)
		     t.painter.paint(ctx, dt);
		 });
  stoptimer("paint");

  starttimer("ai");
  if (start - lastpulse > 1000)
    {
      ais.foreach(function(ai) { ai.queue(ai.pulse); });
      lastpulse = start;
      updatetime();
    }
  else
    {
      updateupdatables(dt);
    }
  ais.foreach(function(ai) { ai.run(); });
  stoptimer("ai");

  if (gamestate == ended)
    {
      showpane("gameover");
    }
  else
    {
      var taken = gettime() - start;
      if (debug)
	window.status = "" + diff + " | " + taken + " | " + dt;
      setTimeout(run, Math.max(10, 50 - taken));
      canvas.focus();
    }
}
