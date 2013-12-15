// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

window.addEventListener('load', initialize, false);
window.addEventListener('DOMContentLoaded', initializeDOM, false);

var widgetmode = null;
if (typeof widget != "undefined" && typeof opera != "undefined")
  widgettype = "operawidget";
else
  widgettype = "webpage";

var checker = new VersionChecker("http://henrikfalck.com/unrealsoccer/latest.xml", widgettype, onversioncheckcomplete);

function initializeDOM()
{
  initializesettings();
  readsettings();
  initializestrings(settings["lang"]);
}

function initialize()
{
  loadallimages();
  initializegame();
  initializegui();

  ieCanvasInit();
  if (settings["checkforupdates"])
    checker.run();

  onstartshow();
  showinganim = createpane("start", onstartshow);
  createpane("main", onmainshow, onmainhide);
  createpane("instructions", oninstructionsshow, oninstructionshide);
  createpane("playtime", onplaytimeshow);
  createpane("options", onoptionsshow, onoptionshide);
  createpane("teamselect", onteamselectshow);
  createpane("gameover", ongameovershow);

  createonclick("menuplaygame", onplaygame);
  createonclick("menuinstructions", oninstructions);
  createonclick("menuoptions", onoptions);
  createonclick("menuquit", onquit);
  createonclick("instructionsback", showmainmenu);
  createonclick("optionsback", showmainmenu);
  createonclick("optionsreturn", onoptionsback);
  createonclick("gameoverback", showmainmenu);
  createonclick("playtimeback", showmainmenu);
  createonclick("teamselectback", onplaygame);
  createonclick("demomode", ondemomode);

  createonclick("playtime02", onplaytime);
  createonclick("playtime05", onplaytime);
  createonclick("playtime10", onplaytime);
  createonclick("playtime20", onplaytime);
  createonclick("playtime30", onplaytime);

  createonclick("offensive", onoffensive);
  createonclick("defensive", ondefensive);

  createonclick("configbutton", onconfigbutton);

  createonchange("language", onlanguagechanged);
}

function initializegui()
{
}

var preloadedimages = [];

function preloaduigraphics()
{
  teamnames.foreachwithindex(function(n, i)
			     {
			       if (i > teammax)
				 setTimeout(function()
					    {
					      var i = new Image();
					      i.src = "teampics/" + n + "unavailable.png";
					      preloadedimages.push(i);
					    }, 100 * i * 500);
			       else
				 setTimeout(function()
					    {
					      var i = new Image();
					      i.src = "teampics/" + n + "inactive.png";
					      preloadedimages.push(i);
					      i = new Image();
					      i.src = "teampics/" + n + "active.png";
					      preloadedimages.push(i);
					      i = new Image();
					      i.src = "teampics/" + n + "selected.png";
					      preloadedimages.push(i);
					    }, 100 + i * 500);
			     });
}

var versionchecked = false;
var versioncheckurl = null;

function onstartshow()
{
  resetgame();
  settext("revision", strings["revision"].replace("@@", "" + version));
  updateversioncheckresult();
  //  var instrframe = get("instructionsframe");
  //  instrframe.setAttribute("src", "instructions-" + settings["lang"] + ".html");
  //  instrframe.setAttribute("lang", settings["lang"]);
}

function oninstructionsshow()
{
  var instren = get("instructionsframe-en");
  var instrja = get("instructionsframe-ja");
  instren.style.visibility = settings["lang"] == "en" ? "visible" : "hidden";
  instrja.style.visibility = settings["lang"] == "ja" ? "visible" : "hidden";
}

function oninstructionshide()
{
  var instren = get("instructionsframe-en");
  var instrja = get("instructionsframe-ja");
  instren.style.visibility = "hidden";
  instrja.style.visibility = "hidden";
}

function updateversioncheckresult()
{
  if (!versionchecked)
    return;
  var url = versioncheckurl;

  var elm1 = get("revision");
  var elm2 = get("revision2");

  if (!url)
    {
      elm2.textContent = strings["latestversion"];
      return;
    }

  elm1.textContent = strings["updateavailable"];
  elm1.style.color = "red";
  elm1.style.opacity = 0.9;
  elm2.innerHTML = "<a href=\"" + url + "\">" + url + "</a>";
  elm2.style.opacity = 0.9;
}

function onversioncheckcomplete(url)
{
  versioncheckurl = url;
  versionchecked = true;
  updateversioncheckresult();
}

function onplaytimeshow()
{
  teammax = settings["progression"];
  preloaduigraphics();
}

function onplaytime(evt)
{
  gametimelength = parseInt(evt.target.id.substr(8, 2)) * 60 * 1000;
  showpane("teamselect");
}

function onoptionsback()
{
  showpane("main");
  pausegame();
}

function onmainshow()
{
  var cfgbtnelm = get("configbutton");
  cfgbtnelm.style.visibility = "visible";
  if (!gameisrunning())
    startgame();
}

function onmainhide()
{
  var cfgbtnelm = get("configbutton");
  cfgbtnelm.style.visibility = "hidden";
}

function onconfigbutton()
{
  if (!gameispaused())
    pausegame();
  showpane("options");
}

function settext(name, text)
{
  get(name).textContent = "" + text;
}

function ongameovershow()
{
  var reselm = get("gameresult");

  var winner = teams[0].score > teams[1].score ? teams[0] : teams[1];
  var loser = teams[0] == winner ? teams[1] : teams[0];
  if (playerteam)
    {
      if (playerteam == winner)
	{
	  reselm.textContent = strings["youwon"];
	  if (progressivegame)
	    {
	      settings["progression"] += 1;
	      writesettings();
	    }
	}
      else
	reselm.textContent = strings["youlost"];
    }
  else
    reselm.textContent = strings["whowon"].replace("@@", strings[winner.id + "name"]);

  settext("gameteam0", strings[teams[0].id + "shortname"]);
  settext("gameteam1", strings[teams[1].id + "shortname"]);
  setcolor("gameteam0", teams[0].stats.color);
  setcolor("gameteam1", teams[1].stats.color);
  settext("gamescore0", teams[0].score);
  settext("gamescore1", teams[1].score);
  settext("gameshots0", teams[0].statistics.shots);
  settext("gameshots1", teams[1].statistics.shots);
  var t = teams[0].statistics.ballpossession + teams[1].statistics.ballpossession;
  var p0 = "" + parseInt(teams[0].statistics.ballpossession * 100 / t) + "%";
  var p1 = "" + parseInt(teams[1].statistics.ballpossession * 100 / t) + "%";
  settext("gameballpossession0", p0);
  settext("gameballpossession1", p1);
  settext("gamekickins0", teams[0].statistics.kickinscaused);
  settext("gamekickins1", teams[1].statistics.kickinscaused);

  if (demomode)
    {
      setTimeout(function()
		 {
		   if (showinganim == panes["gameover"])
		     ondemomode();
		 }, 8000);
    }
}

var checkboxids = ["selectonhold", "selectonpasstarget", "passbeatsselect", "checkforupdates"];

function onoptionsshow()
{
  var retelm = get("optionsreturn");
  retelm.style.visibility = gameisrunning() ? "visible" : "hidden";
  var demoelm = get("demomode");
  demoelm.style.visibility = gameisrunning() ? "hidden" : "visible";
  checkboxids.foreach(function(id)
		      {
			var elm = get(id);
			elm.checked = settings[id] ? true : false;
		      });
  var langelm = get("language");
  var langs = [];
  for (var lang in langstrings)
    langs.push(lang);
  langelm.length = langs.length;

  langs.foreachwithindex(function(lang, i)
			 {
			   var name = langstrings[lang]["languagename"];
			   langelm.options[i].value = lang;
			   langelm.options[i].text = name;
			   langelm.options[i].selected = (settings["lang"] == lang);
			 });
}

function onoptionshide()
{
  var demoelm = get("demomode");
  demoelm.style.visibility = "hidden";

  checkboxids.foreach(function(id)
		      {
			var elm = get(id);
			settings[id] = elm.checked ? true : false;
		      });

  writesettings();
}

function onlanguagechanged()
{
  var elm = get("language");
  settings["lang"] = elm.value;
  var fade = createfade("options", function() { initializestrings(settings["lang"]); });
  fade();
}

function onoffensive()
{
  if (!playerteam)
    return;

  playerteam.ai.offend();
  updateaicontrol();
}

function ondefensive()
{
  if (!playerteam)
    return;

  playerteam.ai.defend();
  updateaicontrol();
}

function showmainmenu()
{
  showpane("start");
}

function onplaygame()
{
  showpane("playtime");
}

function oninstructions()
{
  showpane("instructions");
}

function onoptions()
{
  showpane("options");
}

function onquit()
{
}

function setelm(name, value)
{
  var elm = get(name);
  var shadowelm = get(name + "shadow");
  elm.innerHTML = value;
  shadowelm.innerHTML = value;
}

function setcolor(name, value)
{
  var elm = get(name);
  elm.style.color = value;
}

var demomode = false;

function ondemomode()
{
  resetgame();
  demomode = true;
  gametimelength = 5 * 60 * 1000;
  var team1 = 1 + rand(7);
  var team2 = -1;
  do
    {
      team2 = team1 + rand(5) - 2;
    }
  while (team2 == team1 || team2 < 0 || team2 > 7);
  if (rand(50) == 17)
    setupgame("penguin", "penguin", true);
  else
    setupgame(teamnames[team1], teamnames[team2], true);
  showpane("main");
}

var teamnames = ["pirate", "viking", "monkey", "robot", "alien", "android", "ninja", "scriptkid"];

var playerteamname = null;
var playerteamai = false;

var teammax = 0;
var progressivegame = false;

function teampichovered(i)
{
  setteamstats(teamnames[i]);
  if (i > teammax)
    return;
  if (playerteamname == teamnames[i])
    return;
  var elm = get("teampic" + i);
  elm.src = "teampics/" + teamnames[i] + "active.png";
}

function teampicclicked(i, evt)
{
  if ((!evt.altKey && i > teammax) || playerteamname == teamnames[i])
    return;
  if (!playerteamname)
    {
      playerteamname = teamnames[i];
      playerteamai = evt.shiftKey && evt.altKey;
      var picelm = get("teampic" + i);
      picelm.src = "teampics/" + teamnames[i] + "selected.png";
      var instrelm = get("teamheading");
      instrelm.textContent = strings["chooseopponentteam"];
      if (gametimelength >= 10 * 60 * 1000)
	teammax++;
      updateteamselectavailability();
    }
  else
    {
      if (i == teammax)
	progressivegame = true;
      setupgame(playerteamname, teamnames[i], playerteamai);
      showpane("main");
    }
}

function setteamstats(team)
{
  var s = teamstats[team];
  var props = ["str", "agi", "sta", "skill", "tactics"];
  props.foreach(function(prop)
		{
		  var elm = get("team" + prop);
		  elm.style.width = 10 * s[prop];
		});
  var nameelm = get("teamname");
  nameelm.textContent = strings[team + "name"];
  var descelm = get("teamdesc");
  descelm.textContent = strings[team + "desc"];
}

function teampicunhovered(i)
{
  if (i > teammax || playerteamname == teamnames[i])
    return;
  var elm = get("teampic" + i);
  elm.src = "teampics/" + teamnames[i] + "inactive.png";
}

function onteamselectshow()
{
  playerteamname = null;
  progressivegame = false;
  var instrelm = get("teamheading");
  instrelm.textContent = strings["chooseyourteam"];

  initializeteamselect();
}

function updateteamselectavailability()
{
  for (var i = 0; i < 8; i++)
    {
      if (playerteamname == teamnames[i])
	continue;
      var elm = get("teampic" + i);
      var avail = i <= teammax;
      elm.src = "teampics/" + teamnames[i] + (avail ? "inactive" : "unavailable") + ".png";
      elm.style.cursor = avail ? "pointer" : "help";
      elm.setAttribute("title", avail ? "" : strings["mustbeat"]);
    }
}

function initializeteamselect()
{
  for (var i = 0; i < 8; i++)
    {
      var elm = get("teampic" + i);
      var tmp1 = function(j)
	{
	  var k = parseInt("" + j);
	  elm.addEventListener("mouseover", function() { teampichovered(k); }, false);
	};
      var tmp2 = function(j)
	{
	  var k = parseInt("" + j);
	  elm.addEventListener("mouseout", function() { teampicunhovered(k); }, false);
	};
      var tmp3 = function(j)
	{
	  var k = parseInt("" + j);
	  elm.addEventListener("mouseup", function(evt) { teampicclicked(k, evt); }, false);
	};
      tmp1(i);
      tmp2(i);
      tmp3(i);
    }
  updateteamselectavailability();
}

var showinganim = null;
var panes = {};

function initializepanel()
{
  var fn = function(l, s) { return l.length < 16 ? l : s; };
  setelm("name1", fn(strings[teams[1].id + "name"], strings[teams[1].id + "shortname"]));
  setelm("name2", fn(strings[teams[0].id + "name"], strings[teams[0].id + "shortname"]));
  setcolor("name1", teams[1].stats.color);
  setcolor("name2", teams[0].stats.color);

  var aicelm = get("aicontrol");
  aicelm.style.visibility = playerteam ? "visible" : "hidden";

  updatescores();
  updategamestate();
  updateaicontrol();
}

function updatescores()
{
  setelm("score", "" + teams[1].score + "-" + teams[0].score);
}

function updategamestate()
{
  var stateelm = get("gamestate");
  var stateshadowelm = get("gamestateshadow");

  var show = gamestate.team || gamestate.blink;
  setelm("gamestate", show ? strings[gamestate.name] : "");
  //  stateelm.style.textDecoration = gamestate.blink ? "blink" : "";
  //  stateshadowelm.style.textDecoration = gamestate.blink ? "blink" : "";
  setelm("stateteam", gamestate.team ? strings[gamestate.team.id + "shortname"] : "");
  if (gamestate.team)
    setcolor("stateteam", gamestate.team.stats.color);
}

function updatetime()
{
  if (!starttime)
    {
      setelm("time", "");
      return;
    }
  var t = parseInt((gettime() - starttime) / 1000);

  var m = parseInt(t / 60);
  var s = t % 60;

  s = s < 10 ? "0" + s : "" + s;
  m = "" + m;

  setelm("time", m + ":" + s);
}

function updateaicontrol()
{
  if (!playerteam)
    return;

  var offelm = get("offensive");
  var defelm = get("defensive");

  if (playerteam.ai.offensive)
    {
      offelm.style.color = "red";
      defelm.style.color = "white";
    }
  else
    {
      offelm.style.color = "white";
      defelm.style.color = "red";
    }
}

function createfade(name, onchange, maxopacity, speed)
{
  if (!maxopacity)
    maxopacity = 1;
  var elm = get(name);
  var anim = elm.createAnimation();
  if (speed)
    anim.speed = speed;
  else
    anim.speed = 20;
  anim.onfinish = function() {
    if (parseFloat(anim.style.opacity) == 0)
    {
      onchange();
      if (maxopacity != false)
	  anim.addAnimation("opacity", 0, maxopacity).run();
    }
  }
  var fn = function() {
    anim.addAnimation("opacity", maxopacity, 0).run();
  }
  return fn;
}

function createpane(name, onshow, onhide)
{
  var elm = get(name);
  var anim = elm.createAnimation();
  anim.speed = 50;
  anim.onfinish = function() {
    if (parseFloat(anim.style.opacity) == 0)
    {
      anim.style.visibility = "hidden";
      showinganim.style.visibility = "visible";
      showinganim.addAnimation("opacity", 0, 1).run();
      if (onhide)
        onhide();
    }
  }
  anim.onstart = function() {
    var v = parseFloat(anim.style.opacity);
    if ((v == 0 || isNaN(v)) && onshow)
      onshow();
  }

  panes[name] = anim;

  return anim;
}

function createbutton(name, onclick, minopacity, maxopacity)
{
  var elm = get(name);
  var anim = elm.createAnimation();

  elm.addEventListener('mouseover', domouseoverbutton(anim, minopacity, maxopacity), false);
  elm.addEventListener('mouseout', domouseoutbutton(anim, minopacity, maxopacity), false);
  elm.addEventListener('mousemove', domouseoverbutton(anim, minopacity, maxopacity), false);
  elm.addEventListener('click', onclick, false);

  anim.onfinish = function() {
    if (parseFloat(anim.style.opacity) == 0)
      anim.style.visibility = "hidden";
  }

  anim.onstart = function() {
    var v = parseFloat(anim.style.opacity);
    if (v == 0 || isNaN(v))
      anim.style.visibility = "visible";
  }

  return anim;
}

function createswitchbutton(name, paneanim)
{
  var anim = createbutton(name, doshow(paneanim));

  switchbtns[name] = anim;

  return anim;
}

function createinputfield(name, onsubmit, onfocus, onblur)
{
  var elm = get(name);
  var anim = elm.createAnimation();
  
  var keypress = function(evt)
    {
      if (evt.keyCode == 13)
	onsubmit(elm.value);
    }
  
  var focus = function()
    {
      anim.addAnimation("opacity", 0.5, 1).run();
      if (onfocus)
	onfocus();
    }

  var blur = function()
    {
      anim.addAnimation("opacity", 1, 0.5).run();
      if (onblur)
	onblur();
    }

  if (onsubmit)
    elm.addEventListener('keypress', keypress, false);
  elm.addEventListener('focus', focus, false);
  elm.addEventListener('blur', blur, false);
}

function createonchange(id, fn)
{
  var elm = get(id);
  elm.addEventListener('change', fn, false);
}

function createonclick(id, fn)
{
  var elm = get(id);
  elm.addEventListener('click', fn, false);
}

function createonhover(id, fn)
{
  var elm = get(id);
  elm.addEventListener('mouseover', fn);
}

function showfirst(anim)
{
  anim.style.visibility = "visible";
  anim.addAnimation("opacity", 0, 1).run();
  showinganim = anim;
}

function doshow(anim)
{
  return function() { show(anim); }
}

function showpane(name)
{
  show(panes[name]);
}

function show(anim)
{
  if (anim == showinganim)
    return;

  if (showinganim)
    showinganim.addAnimation("opacity", 1, 0).run();
  showinganim = anim;
}

function domouseoverbutton(anim, minopacity, maxopacity)
{
  return function() { mouseoverbutton(anim, minopacity, maxopacity); }
}

function domouseoutbutton(anim, minopacity, maxopacity)
{
  return function() { mouseoutbutton(anim, minopacity, maxopacity); }
}

function setcookie(name, value)
{
  var date = new Date();
  date.setTime(date.getTime() + 30 * 24 * 60 * 60 * 1000);
  var expires = date.toGMTString();
  var path = "/";
  var re = new RegExp("/[a-z]*?:\/\/(.*?)(\/.*/)/");
  var m = re.exec(location.href);
  if (m)
    {
      path = m[2];
    }
  document.cookie = name + "=" + value + "; expires=" + expires + "; path=" + path;
}

function getcookie(name)
{
  var nameeq = name + "=";

  var s = document.cookie;
  s = s.split("; ");
  for (var i = s.length - 1; i >= 0; i--)
    {
      var c = s[i].split("=");
      if (c[0] == name)
	return c[1];
    }

  return null;
}

function setpref(name, value)
{
  var v = "" + value;

  if (typeof value == "boolean" && value == true)
    v = "true";
  else if (typeof value == "boolean" && value == false)
    v = "false";

  if (typeof widget != "undefined")
    widget.setPreferenceForKey(v, name); // people get real!!!
  else
    setcookie(name, v);
}

function getpref(name)
{
  
  var v = null;
  if (typeof widget != "undefined")
    v = widget.preferenceForKey(name);
  else
    v = getcookie(name);

  if (!v)
    return null;

  if (v == "true")
    return true;
  else if (v == "false")
    return false;
  else if (v == "null")
    return null;
  else if (v.match(/^[0-9]+$/))
    return parseInt(v);

  return v;
}

function haspref(name)
{
  if (typeof widget != "undefined")
    return getpref(name) != null;

  return getcookie(name) != null;
}

function puts(str)
{
  if (debug && typeof opera != "undefined")
    opera.postError(str);
}

function readsettings()
{
  for (key in settings)
    if (haspref(key))
      settings[key] = getpref(key);
}

function writesettings()
{
  for (key in settings)
    setpref(key, settings[key]);
}
