var langstrings = {};

var uistrings = {
  "startcredz.html":"credz",
  menuplaygame:"playgame",
  menuinstructions:"instructions",
  menuoptions:"options",
  menuquit:"quit",
  instructionsback:"backtomainmenu",
  playtimeback:"back",
  playtimeheading:"choosegametime",
  playtime02:"2minutes",
  playtime05:"5minutes",
  playtime10:"10minutes",
  playtime20:"20minutes",
  playtime30:"30minutes",
  playtimeinfo:"playtimeinfo",
  teamheading:"chooseyourteam",
  teamselectback:"back",
  strlbl:"strength",
  agilbl:"agility",
  stalbl:"stamina",
  skilllbl:"skill",
  tacticslbl:"tactics",
  optionsback:"backtomainmenu",
  optionsreturn:"returntogame",
  selectonholddesc:"selectonhold",
  selectonpasstargetdesc:"selectonpasstarget",
  passbeatsselectdesc:"passbeatsselect",
  checkforupdatesdesc:"checkforupdates",
  languagelbl:"language",
  demomode:"runindemomode",
  offensive:"offensive",
  defensive:"defensive",
  gameovertext:"gameover",
  goalslbl:"goals",
  shotslbl:"shots",
  possessionlbl:"possession",
  kickinslbl:"kickins",
  gameoverback:"finish",
  "closebutton.title":"close",
  "configbutton.title":"options"
};

var strings = null;

function initializestrings(lang)
{
  strings = langstrings[lang];

  for (var nid in uistrings)
    {
      var what = null;
      var id = "" + nid;
      if (id.indexOf(".") != -1)
	{
	  var i = id.indexOf(".");
	  what = id.substr(i + 1, id.length - i - 1);
	  id = id.substr(0, i);
	}
      var elm = get(id);
      if (!elm)
	{
	  alert("initializestrings: undefined element id: " + id);
	  return;
	}
      var s = strings[uistrings[nid]];
      if (!s)
	{
	  alert("initializestrings: undefined string: " + uistrings[nid] + " for element id: " + id);
	  return;
	}
      if (what == "html")
	elm.innerHTML = s;
      else if (what)
	elm.setAttribute(what, s);
      else
	elm.textContent = s;
      elm.setAttribute("lang", lang);
    }
}
