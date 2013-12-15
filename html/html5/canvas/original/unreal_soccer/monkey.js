// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function MonkeyAI(assist)
{
  this.assist = assist;
  var self = this;

  this.random = function() { return self.team.members[1 + rand(self.team.members.length - 1)]; }
  this.randomexcept = function(p)
    {
      while (true)
	{
	  var r = self.random();
	  if (self.assist && r.selected)
	    continue;
	  if (r != p)
	    return r;
	}
    }

  this.ourside = function(y)
    {
      return self.team.idx == 0 && y >= 200 || self.team.idx == 1 && y < 200;
    }

  this.initialize = function()
    {
      this.setupzoneviews();
      this.setupplayers();
      if (self.team.stats.hasgoalierole())
	{
	  self.goalie = self.players[0];
	  self.goalie.goalie = true;
	}
      if (self.team.stats.hasoffensiverole())
	{
	  self.players[1].offensive = true;
	  self.players[2].offensive = true;
	}
      if (self.team.stats.hascenterrole())
	{
	  self.players[3].center = true;
	}
      this.goaltarget = new PointView(self.team, points["goal" + self.team.oppidx]);
      this.ourgoal = new PointView(self.team, points["goal" + self.team.idx]);
      this.ourgoalzone = this.getzoneview("goal" + self.team.idx);
      this.guardzone = this.getzoneview(self.team.idx == 0 ? "z42" : "z02");
      this.players.foreach(function(p) { p.lastactiontime = new Date().getTime(); });
      this.centerp = new Coord(149, 199);
      this.lastpasstime = gettime();
    }
  
  this.gamestatechanged = function(from, to)
    {
      self.lastgamestate = from;
      if (to == initializing)
	{
	  self.players.foreach(function(p) { p.runto(p.startp.x, p.startp.y); });
	}
      else if (!self.assist && to == kickoff && kickoff.team == self.team)
	{
	  self.offensive = true;
	  var p = self.random();
	  p.intercept(ball);
	}
      else if (!self.assist && to == kickoff && kickoff.team != self.team)
	{
	  self.offensive = false;
	}
      else if (to == play)
	{
	  self.players.foreach(function(p)
			       {
				 if (!p.goalie && !p.holding)
				   self.runsomewhere(p);
			       });
	}
      else if (!self.assist && to == kickin && kickin.team == self.team)
	{
	  self.offensive = true;
	  var p = self.findplayer(null, function(dummy, p)
				  {
				    return -p.p.dist(kickin.ball) - (p.goalie ? 200 : 0);
				  });
	  p.intercept(kickin.ball);
	}
      else if (!self.assist && to == kickin && kickin.team != self.team)
	{
	  self.offensive = false;
	}
      else if (to == outside || to == goal)
	{
	  if (!self.assist)
	    self.offensive = false;
	  self.players.foreach(function(p) { if (!p.selected) p.stop(); });
	}
      if (from == initializing)
	{
	  self.players.foreach(function(p)
			       {
				 if (p.p.x <= 80)
				   p.leftfield = true;
				 else if (p.p.x >= 219)
				   p.rightfield = true;
			       });
	}
      if (to == kickoff && self.goalie && !self.goalie.selected)
	{
	  self.runsomewhere(self.goalie);
	}
    }
  
  this.hold = function(p, b)
    {
      return true;
    }

  this.findbestpass = function(p)
    {
      var closetogoal = p.p.dist(self.guardzone.midp().p) < 50;

      return self.findplayer(p, function(p, pl)
			     {
			       var zv = self.getzoneview(pl.zone.id);
			       var d = p.p.dist(pl.p);
			       return 0.0
				 + (pl.selected ? 10 : 0)
				 + pl.closestopp.lowest / 10.0
				 - 3 * zv.oppcount()
				 - (d < 35 ? 25 : d / (25.0 + self.team.stats.skill * 3))
				 - (d > self.team.stats.maxkickspeed() * 2 ? (d / 2 - self.team.stats.maxkickspeed() * 2) : 0)
				 - (pl.goalie ? 20.0 : 0.0)
				 - self.goaltarget.p().dist(pl.p) / 10.0
				 - (pl.closestopp.lowest < 20 ? 25 : 0)
				 - (self.guardzone.midp().p.dist(pl.p) < 50 ? 15 : 0)
				 - (pl == p ? 2 * (15 - self.team.stats.str) : self.team.stats.kicknoise() / 2)
				 - (pl == p && p.goalie ? 50 : 0)
				 + (closetogoal && pl.p.dist(self.guardzone.midp().p) > 80 ? 50 : 0)
				 - (pl.closeoppcount * (15 - self.team.stats.str * 2))
				 ;
				 //				 + (new Date().getTime() - pl.lastactiontime) / 2500.0;
			     });
    }

  this.findbestzone = function(p)
    {
      var offfn = function(pl, zv)
      {
	return 0.0
	- self.goaltarget.p().dist(zv.midp().p) / 25.0
	+ rand(self.team.stats.unpredictability())
	- (pl.center ? Math.abs(zv.midp().p.y - 200) / 4 : 0)
	- (gamestate == goal && !pl.goalie && zv.midp().p.dist(ball.p) < 100 ? 100 : 0)
	- (gamestate == outside && !pl.goalie && zv.midp().p.dist(ball.p) < 100 ? 100 : 0)
	+ (pl.rightfield && zv.midp().p.x <= 80 ? 3 : 0)
	+ (pl.leftfield && zv.midp().p.x >= 219 ? 3 : 0)
	+ (pl.holding && !self.ourside(zv.midp().p.y) ? 5 : 0)
	- p.p.dist(zv.midp().p) / 50.0
	+ (pl.offensive && !self.ourside(zv.midp().p.y) ? 10 : 0)
	- (pl.holding ? 0 : (pl.goalie ? pl.p.dist(self.guardzone.midp().p) / 25.0 : 3.0 * zv.oppcount() + self.team.stats.tactics * zv.ourcount()))
	- (zv == self.ourgoalzone ? 100 : 0)
	;
      }
      
      var deffn = function(pl, zv)
      {
	return 0.0
	- self.ourgoal.p().dist(zv.midp().p) / 25.0
	+ rand(self.team.stats.unpredictability())
	- (pl.center ? Math.abs(zv.midp().p.y - 200) / 6 : 0)
	- (gamestate == kickoff && !self.ourside(zv.midp().p.y) ? 20 : 0)
	- ((gamestate == kickoff || gamestate == kickin) && gamestate.team != self.team && zv.midp().p.dist(ball.p) < 80 ? 100 : 0)
	- (gamestate == goal && !pl.goalie && zv.midp().p.dist(ball.p) < 100 ? 100 : 0)
	- (gamestate == outside && !pl.goalie && zv.midp().p.dist(ball.p) < 100 ? 100 : 0)
	+ (pl.rightfield && zv.midp().p.x <= 80 ? 3 : 0)
	+ (pl.leftfield && zv.midp().p.x >= 219 ? 3 : 0)
	+ (gamestate == goal && self.ourside(zv.midp().p.y) ? 25 : 0)
	- p.p.dist(zv.midp().p) / 50.0
	+ (pl.offensive && !self.ourside(zv.midp().p.y) ? 5 : 0)
	- (pl.holding ? 0 : (pl.goalie ? pl.p.dist(self.guardzone.midp().p) / 15.0 : -3.0 * zv.oppcount() + 3.0 * zv.ourcount()))
	- (gamestate != kickoff ? 0 : pl.p.dist(self.guardzone.midp().p) / 20)
	- (zv == self.ourgoalzone ? 100 : 0)
	;
      }

      var fn = self.offensive ? offfn : deffn;
      
      return self.findzone(p, fn);
    }

  this.runsomewhere = function(p)
    {
      if (self.assist && p.selected)
	return true;
      if (self.assist && p.lastcommandtime && gettime() - p.lastcommandtime < 15000)
	return true;
      if (p.goalie && !p.holding)
	{
	  var origin = (gamestate != kickin && gamestate != outside) ? self.ourgoal.p() : self.guardzone.midp().p;
	  if (self.team.stats.hasimprovedgoalie() && p.p.dist(ball.p) < 50 && origin.dist(p.p) < 80)
	    {
	      p.intercept(ball);
	      return true;
	    }
	  var target = gamestate == goal ? self.centerp : ball.p;
	  var a = origin.angle(target);
	  var d = origin.dist(target);
	  var pt = origin.clone();
	  placemarker(origin, "orange", true);
	  pt.x += Math.cos(a) * (15 + d * (self.defensive ? 0.1 : 0.25));
	  pt.y += Math.sin(a) * (15 + d * (self.defensive ? 0.1 : 0.25));
	  if (p.p.dist(pt) > 5)
	    p.runto(pt.x, pt.y, d < 50 ? 0.1 * p.maxspeed : (d > 100 ? p.maxspeed : 0.2 * p.maxspeed));
	  p.lastactiontime = gettime() - 9000;
	  placemarker(pt, "yellow", true);
	  placemarker(p.p, "yellow", true);
	  return true;
	}
      if (ball.holder && ball.holder.team.idx == self.team.oppidx)
	{
	  //	  var x = self.findplayer(p, function(p, pl)
	  //				  {
	  //				    return -pl.p.dist(ball.p);
	  //				  });
	  //	  if (x.score > -90)
	  if (ball.holder.closestopp.object == p)
	    {
	      p.intercept(ball);
	      return true;
	    }
	}
      var zv = self.findbestzone(p);
      if (zv.zone != p.zone)
	{
	  var mp = zv.midp().p;
	  p.runto(mp.x + rand(30) - 15, mp.y + rand(30) - 15);
	  p.lastactiontime = gettime() - rand(10000);
	  return true;
	}
      return false;
    }

  this.shoot = function(p)
    {
      if (self.assist)
	return false;
      if (self.lastgamestate == kickoff
	  && self.team.stats.shootonkickoffchance() <= Math.random())
	return false;
      var dist = self.goaltarget.p().dist(p.p);
      if (dist > 50 && self.team.stats.shootchance() <= Math.random())
	return false;
      if (dist > self.team.stats.maxkickspeed() * 1.5)
	return false;
      if (p.p.x < 50 || p.p.x > 249)
	if (p.p.y < 20 || p.p.y > 379)
	  return false;

      ais.foreach(function(ai) { ai.queue(ai.ballshot, p, p.ball); });
      p.kick(p.ball, self.goaltarget.p(), true);
      return true;
    }

  this.ballshot = function(p, b)
    {
      if (p.team == self.team || !b)
	return;

      self.players.foreach(function(p)
			   {
			     if (p.goalie && !p.selected)
			       {
				 p.intercept(b);
			       }
			     else if (!p.selected && p.p.dist(self.guardzone.midp().p) < 60)
			       {
				 p.intercept(b);
			       }
			   });
    }
  
  this.holding = function(p)
    {
      if (p.selected)
	return;
      if (gamestate.ballinplay && p.team != self.team && !p.referee)
	{
	  if (!self.assist)
	    {
	      self.offensive = false;
	      self.players.foreach(function(pl) { self.runsomewhere(pl); });
	    }
	  var x = p.closestopp.object;
	  if (x && p.closestopp.lowest < 90 && !x.selected)
	    x.intercept(ball);
	  return;
	}
      if (p.team != self.team)
	return;
      p.stop();
      self.holder = p;
    }

  this.decideholderaction = function()
    {
      var p = self.holder;
      self.holder = null;
      if (!p.holding || p.selected || self.shoot(p))
	return;
      var canpass = p.goalie || gettime() - self.lastpasstime > self.team.stats.passwaittime();
      var o = canpass ? self.findbestpass(p) : null;
      if (canpass && p.ball && o != p && o.p.dist(p.p) > 25)
	{
	  p.kick(p.ball, o.p);
	  o.receive(p.ball);
	  self.runsomewhere(p);
	  self.lastpasstime = gettime();
	}
      else if (!self.runsomewhere(p))
	{
	  var x = self.randomexcept(p);
	  self.runsomewhere(x);
	}
    }
  
  this.arrived = function(p)
    {
      if (p.selected)
	return;
      if (p.holding)
	{
	  self.holding(p);
	}
      else
	{
	  self.players.foreach(function(p) { if (p.holding) self.holding(p); });
	}
    }

  this.ballchangedzone = function()
    {
      if (gamestate != play)
	return;

      var x = self.findplayer(null, function(dummy, pl)
			      {
				return -pl.p.dist(ball.p) - (pl.selected ? 100 : 0 - (pl.goalie ? 100 : 0));
			      });
      if (x.selected)
	return;
      if (x.score > -self.team.stats.maxspeed())
	{
	  ball.intercepting.foreach(function(pl)
				    {
				      if (!pl.selected && !pl.receiving && pl.team == self.team)
					{
					  pl.stop();
					}
				    });
	  x.intercept(ball);
	}
    }

  this.ballkicked = function(b, x, dir, speed)
    {
      if (x && x.team == self.team)
	return;
      if (!self.team.stats.thinkahead())
	return;

      var pt = b.p.clone();
      pt.x += b.cos * speed;
      pt.y += b.sin * speed;
      var p = self.findplayer(null, function(dummy, pl)
			      {
				return -pl.p.dist(pt) - (pl.goalie ? 25 : 0) - (pl.selected ? 25 : 0);
			      });
      placemarker(pt, "red");
      if (!p.selected && p.score > -100)
	{
	  if (!p.receiving && !p.intercepting)
	    {
	      placemarker(p.p, "red");
	      p.runto(pt.x, pt.y);
	      p.lastcommandtime = gettime();
	    }
	}
    }
  
  this.pulse = function()
    {
      if ((gamestate == play || (!self.assist && ((gamestate == kickin || gamestate == kickoff) && gamestate.team == self.team))) && ball.speed < 10.0 && !ball.holder)
	{
	  var i = ball.intercepting.find(function(x) { return x.team == self.team; });
	  var p = self.findplayer(null, function(dummy, p)
				  {
				    return (p.intercepting ? -100 : 0) - ball.p.dist(p.p) - (p.goalie ? 100 : 0);
				  });
	  if (!i || i.p.dist(ball.p) > p.p.dist(ball.p))
	    {
	      if (i && !i.selected)
		i.stop();
	      if (!p.selected)
		p.intercept(ball);
	    }
	}
      self.lastgamestate = null;

      self.players.foreach(function(p)
			   {
			     if (((gamestate == goal || gamestate == outside) || (!p.receiving && !p.intercepting)) && gettime() - p.lastactiontime > 10000)
			       self.runsomewhere(p);
			   });

      if (self.goalie && self.goalie.intercepting && !self.goalie.selected)
	{
	  var d = self.goalie.p.distsq(ball.p);
	  var o = ball.intercepting.find(function(x)
					 {
					   return x != self.goalie && x.team == self.team && x.p.distsq(ball.p) <= d;
					 });
	  if (o)
	    {
	      self.goalie.stop();
	      self.runsomewhere(self.goalie);
	    }
	}

      if (gamestate == goal)
	{
	  self.players.foreach(function(p)
			       {
				 if (p.intercepting && !p.selected)
				   {
				     p.stop();
				     self.runsomewhere(p);
				   }
			       });
	}

      if (self.assist)
	return;

      if (self.holder)
	self.decideholderaction();

      if (gamestate == play && ball.lasttouch)
	{
	  if (ball.holder && ball.holder.team == self.team && ball.holder.goalie)
	    self.offensive = self.team.stats.isoffensive();
	  else
	    {
	      if (self.team.stats.isoffensive())
		{
		  if (ball.holder)
		    self.offensive = ball.holder.team == self.team;
		  else if (self.ourside(ball.p.y))
		    self.offensive = false;
		  else
		    self.offensive = true;
		}
	      else
		self.offensive = ball.lasttouch.team == self.team;
	    }
	}
      else if ((gamestate == kickin || gamestate == outside) && outside.dropp)
	self.offensive = !self.ourside(outside.dropp.y);
    }

  this.offend = function()
    {
      if (gamestate == goal)
	return;
      self.offensive = true;
      self.players.foreach(function(p) { self.runsomewhere(p); });
    }

  this.defend = function()
    {
      self.offensive = false;
      self.players.foreach(function(p) { self.runsomewhere(p); });
    }
}
MonkeyAI.prototype = new AI();
