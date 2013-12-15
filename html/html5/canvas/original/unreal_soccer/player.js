// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function Stats(str, agi, sta, skill, tactics, shadowsize, color)
{
  this.str = str;
  this.agi = agi;
  this.sta = sta;
  this.skill = skill;
  this.tactics = tactics;
  this.shadowsize = shadowsize;
  this.color = color;

  this.maxspeed = function() { return 3.0 * this.sta + 0.5 * this.str + 2.0 * this.agi; }
  this.accel = function() { return 1.5 * this.str + 3.0 * this.agi; }
  this.maxdirchangespeed = function() { return 2.0 * this.agi; }
  this.kicknoise = function() { return 2 * (12 - this.skill); }
  this.unpredictability = function() { return 10 - this.tactics; }
  this.hasgoalierole = function() { return this.tactics >= 2; }
  this.hasoffensiverole = function() { return this.tactics >= 5; }
  this.thinkahead = function() { return this.tactics >= 6; }
  this.maxkickspeed = function() { return 10 * this.skill + 10 * this.str + 5 * this.agi; }
  this.ballconquerscore = function() { return 2 * this.str + this.agi + this.skill }
  this.hascenterrole = function() { return this.tactics >= 8; }
  this.shootonkickoffchance = function() { return this.tactics < 6 ? 0 : this.tactics / 12; }
  this.shootchance = function() { return 0.5 + Math.abs(5 - this.tactics) / 10; }
  this.hasimprovedgoalie = function() { return this.tactics >= 5; }
  this.isoffensive = function() { return this.str >= 6 && this.agi >= 2 && (this.tactics <= 2 || this.tactics >= 9); } // vikings, scriptkids
  this.passwaittime = function() { return (10 - this.tactics) * 1000; }
}

var teamstats = {};
teamstats["penguin"] =
  new Stats(5, 9, 5, 5, 5, 0, "lightgray");
teamstats["pirate"] =
  new Stats(4, 3, 4, 1, 1, 1, "red");
teamstats["viking"] =
  new Stats(6, 2, 5, 2, 2, 1, "lime");
teamstats["monkey"] =
  new Stats(2, 8, 7, 1, 2, 1, "brown");
teamstats["robot"] =
  new Stats(7, 2, 5, 8, 5, 1, "lightsteelblue");
teamstats["alien"] =
  new Stats(7, 6, 8, 6, 3, 1, "green");
teamstats["android"] =
  new Stats(10, 1, 5, 9, 8, -1, "gold");
teamstats["ninja"] =
  new Stats(5, 10, 7, 5, 6, 1, "#8470FF" /*lightslateblue*/);
teamstats["scriptkid"] =
  new Stats(9, 8, 9, 10, 9, 1, "#D0D0D0");

function Team(idx, idname, ai)
{
  this.stats = teamstats[idname];
  this.idx = idx;
  this.oppidx = idx == 0 ? 1 : 0;
  this.id = idname;
  this.playerimgname = idname;
  this.ai = ai;
  if (ai)
    ai.team = this;
  this.members = [];
  this.score = 0;
  
  this.startpoints = [[149, 25], [49, 120], [249, 120], [149, 70]];

  this.constructplayer = function()
    {
      var idx = this.members.length;
      var p = new Player(this, idx);
      var startp = this.coord(this.startpoints[idx][0], this.startpoints[idx][1]);
      p.p.set(this.coord(149 + (startp.x - 149) / 3 + rand(60) - 30, - rand(60)));
      p.startp = startp;
      var pp = new PlayerPainter(p, getimage(this.playerimgname));
      p.painter = pp;
      this.members.push(p);

      return p;
    }

  this.constructgoal = function()
    {
      var g = new Goal(150, this.idx == 0 ? 399 : 32);
      var gp = new GoalPainter(g, getimage("goal"));
      g.painter = gp;

      return g;
    }

  this.coord = function(x, y)
    {
      return new Coord(x, this.idx == 1 ? y : (399 - y));
    }
}

function RefereeTeam(ai)
{
  this.ai = ai;
  ai.team = this;
  this.idx = -1;
  this.stats = teamstats["penguin"];
  this.name = "The Referees";
  this.shortname = "Referees";

  this.constructplayer = function()
    {
      var p = new Player(this, 0);
      p.p.x = 330;
      p.p.y = 250;
      var pp = new PlayerPainter(p, getimage("penguin"));
      p.painter = pp;
      p.referee = true;
      this.player = p;
      return p;
    }
}

function Player(team, idx)
{
  this.p = new Coord(0, 0);
  this.team = team;
  this.idx = idx;
  this.running = false;
  this.turning = false;
  this.facing = 0;
  this.maxdirchangespeed = team.stats.maxdirchangespeed();
  this.speedto = 0.0;
  this.basemaxspeed = team.stats.maxspeed();
  this.maxspeed = this.basemaxspeed;
  this.accel = team.stats.accel();
  this.kicknoise = team.stats.kicknoise();
  this.maxkickspeed = team.stats.maxkickspeed();
  this.dir = 0.0;
  this.dirto = 0.0;
  this.speed = 0.0;
  this.visible = true;
  this.active = true;
  this.solid = true;
  this.targetable = true;
  if (!this.team.ai)
    this.controllable = true;
  else if (this.team.ai.assist)
    this.controllable = true;
  this.sin = 0.0;
  this.cos = 0.0;
  this.lastruntod = 999999999999999999999;
  this.runtop = new Coord(0, 0);
  this.runfromp = new Coord(0, 0);
  this.zone = null; // set by Zone
  this.radius = 15;
  this.closestour = new Lowest();
  this.closestopp = new Lowest();
  this.closeoppcount = 0;
  var self = this;

  this.updatepos = function(dt)
    {
      if (isNaN(this.cos)) // but why??
	return;
      this.p.x += dt * this.speed * this.cos;
      this.p.y += dt * this.speed * this.sin;
      this.x = this.p.x;
      this.y = this.p.y;
    }

  this.runto = function(x, y, speed)
    {
      this.speedto = speed ? Math.min(speed, this.maxspeed) : this.maxspeed;
      this.runfromp.set(this.p);
      if (x instanceof Coord)
	this.runtop.set(x);
      else
	{
	  this.runtop.x = x;
	  this.runtop.y = y;
	}
      this.turning = true;
      this.running = true;
      this.intercepting = false;
      this.receiving = false;
      this.arrived = false;
      this.lastruntod = 9999999999999999999999;
    }

  this.updatemovement = function(dt)
    {
      if (this.running || this.turning || this.intercepting || this.receiving)
	this.updateruntomovement(dt);
      this.updatespeedtodirtomovement(dt);
    }

  this.updateruntomovement = function(dt)
    {
      if ((this.receiving || this.intercepting) && this.ball)
	{
	  this.runtop.set(this.ball.p);
	  var p = this.ball.p;
	  var d = Math.atan2(p.y - this.p.y, p.x - this.p.x);
	  if (Math.abs(d - this.dirto) > 0.4)
	    this.turning = true;
	  if (this.receiving && this.ball.speed < 25.0 && this.lastruntod > 10.0)
	    {
	      this.intercepting = true;
	      this.speedto = this.maxspeed;
	    }
	}

      if (this.turning)
	{
	  var p = this.receiving ? this.ball.p : this.runtop;
	  this.dirto = Math.atan2(p.y - this.p.y, p.x - this.p.x);
	  this.turning = false;
	  return;
	}

      if (this.running)
	{
	  var runtod = this.runtop.dist(this.p);
	  if (runtod < 2)
	    this.speedto = 0.1 * this.maxspeed;
	  else if (runtod < 5)
	    this.speedto = 0.3 * this.maxspeed;
	  else if (runtod < 10)
	    this.speedto = 0.5 * this.maxspeed;
	  if (this.running && runtod > this.lastruntod)
	    {
	      if (!this.referee && (this.intercepting || this.receiving))
		{
		  this.speedto = this.maxspeed;
		}
	      else
		{
		  this.running = false;
		  this.speedto = 0.0;
		  if (!this.arrived && this.team.ai)
		    this.team.ai.queue(this.team.ai.arrived, this);
		  this.arrived = true;
		}
	    }
	  this.lastruntod = runtod;
	}
    }

  this.updatespeedtodirtomovement = function(dt)
    {
      this.maxspeed = this.holding ? this.basemaxspeed * 0.7 : this.basemaxspeed;
      if (this.dir != this.dirto)
	{
	  if (this.speed * Math.abs(this.dirto - this.dir) > this.maxdirchangespeed)
	    {
	      this.turning = true;
	      this.speed -= this.accel * dt;
	      if (this.speed < 0.0)
		this.speed = 0.0;
	    }
	  else
	    {
	      this.dir = this.dirto;
	      this.cos = Math.cos(this.dir);
	      this.sin = Math.sin(this.dir);
	      this.updatefacing();
	      this.speedto = 0.0;
	      if (this.receiving && !this.intercepting)
		this.speedto = 0.3 * this.maxspeed;
	      else if (this.running || this.receiving)
		this.speedto = this.maxspeed;
	      this.running = true;
	    }
	}
      else if (this.speedto > this.speed && this.speed < this.maxspeed)
	{
	  this.turning = false;
	  this.speed += this.accel * dt;
	  if (this.speed > this.maxspeed)
	    this.speed = this.maxspeed;
	}
      else if (this.speedto > 0.0 && this.speedto < this.speed)
	{
	  this.speed -= this.accel * dt;
	}
      else if (this.speedto == 0.0 && this.speed > 0.01)
	{
	  this.turning = false;
	  this.speed -= this.accel * dt;
	  if (this.speed < 0.0)
	    this.speed = 0.0;
	}
    }

  this.update = function(dt)
    {
      this.updatemovement(dt);
      this.updatepos(dt);
    }

  this.updatefacing = function()
    {
      var dir = this.dir;
      if (dir >= 0 && dir < 0.25 * Math.PI)
	this.facing = 1;
      else if (dir >= 0.25 * Math.PI && dir < 0.75 * Math.PI)
	this.facing = 2;
      else if (dir >= 0.75 * Math.PI)
	this.facing = 3;
      else if (dir < -0.75 * Math.PI)
	this.facing = 3;
      else if (dir >= -0.75 * Math.PI && dir < -0.25 * Math.PI)
	this.facing = 0;
      else if (dir >= -0.25 * Math.PI)
	this.facing = 1;
    }

  this.collide = function(t)
    {
      if (t instanceof Ball)
	{
	  if (t.height > 2.5)
	    return true;
	  if (!gamestate.ballinplay && !this.referee)
	    return true;
	  if (gamestate == kickoff || gamestate == kickin)
	    {
	      if (gamestate.team != this.team)
		return true;
	      else
		{
		  ball.lastoutofplaytime = gettime();
		  changegamestate(play);
		}
	    }
	  
	  if (this.kicktime)
	    if (gettime() - this.kicktime < 1000)
	      return true;
	    else
	      kicktime = null;
	  if (t.holder)
	    {
	      var o = t.holder;
	      if (o.team == self.team)
		return true;
	      if (rand(this.team.stats.ballconquerscore()) > rand(o.team.stats.ballconquerscore()))
		{
		  if (debug)
		    puts("conquer won: " + this + " / " + o);
		}
	      else
		{
		  if (debug)
		    puts("conquer lost: " + this + " / " + o);
		  return;
		}
	    }
	  if (this.team.ai)
	    if (!this.team.ai.hold(this, t))
	      return;
	  t.intercepted(this);
	  t.holder = this;
	  this.holding = true;
	  if (!this.referee)
	    t.lasttouch = this;
	  this.ball = t;
	  if (self.controllable && settings["selectonhold"] && !self.selected)
	    {
	      self.stop();
	      selectplayer(self);
	    }
	  var player = this;
	  ais.foreach(function(ai) { ai.queue(ai.holding, player); });
	}
    }

  this.kick = function(b, p, hard)
    {
      if (!this.holding || this.ball != b || !b)
	return false;

      this.lastoutofplaytime = null;

      var d = (hard ? 2.0 : 1.0) * this.p.dist(p);
      var f = d / 100.0;

      var p = p.clone();
      p.x += f * (this.kicknoise - rand(this.kicknoise * 2));
      p.y += f * (this.kicknoise - rand(this.kicknoise * 2));

      var dir = Math.atan2(p.y - this.p.y, p.x - this.p.x);
      
      var k = 20 + this.kicknoise - rand(this.kicknoise * 2);
      if (d > 100)
	k -= 5;
      if (d > 125)
	k -= 7.5;
      if (d > 150)
	k -= 7.5;
      if (d > 175)
	k -= 5;
      if (d > 200)
	k -= 5;
      if (d > 210)
	k -= 3;
      if (d > 250)
	k += 20;
      if (d > 275)
	k += 50;
      var s = d / 2 + k;
      s = Math.min(s, this.maxkickspeed);
      if (b.kick(dir, s, this))
	{
	  this.kicktime = gettime();
	  return true;
	}
      return false;
    }

  this.release = function(b)
    {
      if (!this.holding || this.ball != b)
	return;
      this.receiving = false;
      this.intercepting = false;
      b.release();
      this.kicktime = gettime() - 2000;
    }

  this.receive = function(b)
    {
      this.ball = b;
      b.addintercepting(this);
      this.intercepting = false;
      this.receiving = true;
      this.runfromp.set(this.p);
      this.runtop.set(b.p);
      this.turning = true;
      this.running = true;
      this.lastruntod = 9999999999999999999999;
    }

  this.intercept = function(b)
    {
      this.ball = b;
      b.addintercepting(this);
      this.intercepting = true;
      this.receiving = false;
      this.runfromp.set(this.p);
      this.runtop.set(b.p);
      this.turning = true;
      this.running = true;
      this.lastruntod = 9999999999999999999999;
    }

  this.intercepted = function(b, t)
    {
      this.intercepting = false;
      this.receiving = false;
      this.running = false;
      this.turning = false;
      this.speedto = 0.0;
    }

  this.stop = function()
    {
      this.intercepting = false;
      this.receiving = false;
      this.running = true;
      this.turning = false;
      this.speedto = 0.0;
      if (!this.holding)
	this.ball = null;
    }

  this.toString = function()
    {
      return this.team.id + "(" + this.idx + ")";
    }

  this.updatedata = function(dt)
    {
      self.closestour.reset();
      self.closestopp.reset();
      self.closeoppcount = 0;
      players.foreach(function(p)
		      {
			if (p.referee)
			  return;
			var d = self.p.dist(p.p);
			if (p.team == self.team)
			  self.closestour.register(p, d);
			else if (p.team.idx == self.team.oppidx)
			  {
			    self.closestopp.register(p, d);
			    if (d < 50)
			      self.closeoppcount++;
			  }
		      });
    }
}

function PlayerPainter(player, img)
{
  this.player = player;
  this.img = img;
  this.curframe = 0;
  this.framestate = 1;
  this.width = 24;
  this.height = 32;
  this.dtcount = 0;
  this.framespeed = 5000;
  this.xoffset = -12;
  this.yoffset = -26;

  this.updatecurframe = function()
    {
      this.curframe += this.framestate;
      if (this.curframe == 3)
	{
	  this.curframe = 1;
	  this.framestate = -1;
	}
      else if (this.curframe == -1)
	{
	  this.curframe = 1;
	  this.framestate = 1;
	}
      this.dtcount -= this.framespeed;
    }

  this.paint = function(ctx, dt)
    {
      this.dtcount += dt * 1000 * (Math.random() * 5 + this.player.speed);
      if (this.dtcount >= this.framespeed)
	this.updatecurframe();

      ctx.drawImage(this.img,
		    this.curframe * this.width,
		    this.player.facing * this.height,
		    this.width,
		    this.height,
		    parseInt(this.player.p.x) + this.xoffset,
		    parseInt(this.player.p.y) + this.yoffset,
		    this.width,
		    this.height);
    }
}
