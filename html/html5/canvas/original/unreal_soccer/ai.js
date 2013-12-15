// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function AI()
{
  this.events = [];

  this.queue = function(fn, arg1, arg2, arg3, arg4)
    {
      if (!fn)
	return;

      if (arg4)
	this.events.push([fn, arg1, arg2, arg3, arg4]);
      else if (arg3)
	this.events.push([fn, arg1, arg2, arg3]);
      else if (arg2)
	this.events.push([fn, arg1, arg2]);
      else if (arg1)
	this.events.push([fn, arg1]);
      else
	this.events.push([fn]);
    }

  this.run = function()
    {
      while (this.events.length > 0)
	{
	  var e = this.events.shift();
	  if (!e[0])
	    continue;
	  if (e.length == 1)
	    e[0]();
	  else if (e.length == 2)
	    e[0](e[1]);
	  else if (e.length == 3)
	    e[0](e[1], e[2]);
	  else if (e.length == 4)
	    e[0](e[1], e[2], e[3]);
	  else if (e.length == 5)
	    e[0](e[1], e[2], e[3], e[4]);
	}
    }

  this.clearevents = function()
    {
      this.events = [];
    }
  
  this.setupzoneviews = function()
    {
      this.zoneid2view = {};
      this.zones = [];
      var self = this;
      zones.foreach(function(z)
		    {
		      var zv = new ZoneView(self.team, z);
		      self.zoneid2view[z.id] = zv;
		      zv.score = 0.0;
		      self.zones.push(zv);
		    });
    }

  this.getzoneview = function(id)
    {
      return this.zoneid2view[id];
    }

  this.setupplayers = function()
    {
      this.players = [];
      for (var i = 0; i < this.team.members.length; i++)
	this.players.push(this.team.members[i]);
    }

  this.findzone = function(p, fn)
    {
      this.zones.foreach(function(zv) { zv.score = fn(p, zv); } );
      this.zones.sort(function(a, b) { return b.score - a.score; } );
      return this.zones[0];
    }

  this.findplayer = function(p, fn)
    {
      this.players.foreach(function(pl) { pl.score = fn(p, pl); } );
      this.players.sort(function(a, b) { return b.score - a.score; } );
      return this.players[0];
    }
}
