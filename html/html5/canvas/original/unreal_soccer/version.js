// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

var version = 4;

function VersionChecker(url, type, callback)
{
  var req = null;

  this.run = function()
    {
      try
	{
	  if (window.XMLHttpRequest)
	    {
	      req = new XMLHttpRequest();
	      req.onreadystatechange = this.onchange;
	      req.open("GET", url, true);
	      req.send(null);
	    }
	  else if (window.ActiveXObject)
	    {
	      req = new ActiveXObject("Microsoft.XMLHTTP");
	      if (req)
		{
		  req.onreadystatechange = this.onchange;
		  req.open("GET", url, true);
		  req.send();
		}
	    }
	}
      catch (e)	{ }
    }

  this.onchange = function()
    {
      if (!req)
      	return;
      if (req.readyState != 4)
	return;
      if (req.status != 200)
	return;
      var data = req.responseXML.getElementsByTagName(type);
      if (!data || data.length == 0)
	return;
      data = data.item(0);
      var ver = null;
      var url = null;
      for (var i = 0; i < data.childNodes.length; i++)
	{
	  var n = data.childNodes.item(i);
	  if (n.childNodes.length == 0)
	    continue;
	  var v = n.childNodes.item(0).nodeValue;
	  if (n.nodeName == "version")
	    ver = parseInt(v);
	  else if (n.nodeName == "url")
	    url = v;
	}
      if (!ver || !url)
	return;
      if (ver <= version)
	callback(null);
      else
	callback(url);
    }
}
