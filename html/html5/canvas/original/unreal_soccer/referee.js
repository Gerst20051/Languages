// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function TeamStatistics()
{
  this.kickinscaused = 0;
  this.ballpossession = 0;
  this.shots = 0;
}

function RefereeAI()
{
  var self = this;

  this.outofplaytime = 0;
  this.inplaytime = 0;
  this.endtime = gametimelength;
  this.extratime = 0;
  this.suddendeath = false;
  this.gamestarted = false;
  this.dropballwaittime = gettime();
  this.kickoffwaittime = gettime();
  this.dropp = new Coord(149, 199);

  this.initialize = function()
    {
      this.setupzoneviews();
      teams[0].statistics = new TeamStatistics();
      teams[1].statistics = new TeamStatistics();
    }

  this.gamestatechanged = function(from, to)
    {
      if (to == outside)
	{
	  var p = to.ball.p.clone();
	  if (p.y <= 2)
	    {
	      p.y = 14;
	      p.x = p.x < 150 ? 10 : 288;
	    }
	  else if (p.y >= 397)
	    {
	      p.y = 386;
	      p.x = p.x < 150 ? 10 : 288;
	    }
	  else if (p.x <= 2)
	    p.x = 8;
	  else if (p.x >= 297)
	    p.x = 292;
	  outside.dropp = p;
	  self.clearevents();
	  self.team.player.intercept(outside.ball);
	  ball.lasttouch.team.statistics.kickinscaused++;
	}
      else if (to == play || to == kickin || to == kickoff)
	{
	  self.arrived(self.team.player);
	}
      else if (to == goal)
	{
	  self.clearevents();
	  if (self.suddendeath)
	    self.endgame();
	  else
	    self.team.player.intercept(goal.ball);
	}
      if (!self.gamestarted && to == play)
	{
	  self.gamestarted = true;
	  starttime = gettime();
	}
    }

  this.hold = function(p, b)
    {
      if (gamestate == initializing || gamestate == outside || gamestate == goal)
	return true;

      return false;
    }

  this.holding = function(p)
    {
      if (!p.referee)
	return;
      if (gamestate == initializing)
	{
	  self.dropp.x = 160;
	  self.dropp.y = 200;
	  p.runto(160, 200);
	}
      else if (gamestate == outside)
	{
	  self.dropp.set(outside.dropp);
	  p.runto(outside.dropp);
	}
      else if (gamestate == goal)
	{
	  self.dropp.x = 149;
	  self.dropp.y = 199;
	  p.runto(149, 199);
	}
    }

  this.checkallplayersontheirhalf = function()
    {
      var foul = players.find(function(p)
			      {
				if (p.team.idx == 0 && p.p.y < 195)
				  return true;
				else if (p.team.idx == 1 && p.p.y > 205)
				  return true;
				return false;
			      });
      if (foul)
	return false;

      return true;
    }

  this.checkopposingplayersawayfromball = function(goodteam)
    {
      var team = teams[goodteam.oppidx];
      var foul = team.members.find(function(p)
				   {
				     if (p.p.dist(ball.p) < 70)
				       return true;
				     return false;
				   });
      if (foul)
	return false;

      return true;
    }

  this.dropball = function()
    {
      var p = self.team.player;
      
      if (gettime() - self.dropballwaittime > 15000)
	{
	  var goodteam = gamestate == outside ? kickin.team : kickoff.team;
	  var team = teams[goodteam.oppidx];
	  moveawayplayers(team);
	  if (gamestate == initializing || gamestate == goal)
	    {
	      players.foreach(function(p)
			      {
				if (p.team.idx == 0 && p.y < 200)
				  p.p.y = 380;
				else if (p.team.idx == 1 && p.y > 200)
				  p.p.y = 20;
			      });
	    }
	}
      if (gamestate == initializing || gamestate == goal)
	{
	  if (self.checkallplayersontheirhalf()
	      && self.checkopposingplayersawayfromball(kickoff.team))
	    {
	      p.ball.release();
	      changegamestate(kickoff);
	      self.kickoffwaittime = gettime();
	      self.checkdropball = false;
	    }
	}
      else if (gamestate == outside)
	{
	  if (self.checkopposingplayersawayfromball(kickin.team))
	    {
	      p.ball.release();
	      self.kickoffwaittime = gettime();
	      changegamestate(kickin);
	      self.checkdropball = false;
	    }
	}
    }

  this.arrived = function(p)
    {
      if (gamestate == outside || gamestate == initializing || gamestate == goal)
	{
	  if (!self.checkdropball && fieldzone.contains(p.ball.p) && self.dropp.dist(p.p) < 2)
	    {
	      self.dropballwaittime = gettime();
	      self.checkdropball = true;
	    }
	  else
	    {
	      p.runto(self.dropp);
	    }
	}
      else
	self.moveyerpenguinasssomewhere();
    }

  this.moveyerpenguinasssomewhere = function()
    {
      var p = self.team.player;

      if (gamestate == play || gamestate == kickoff || gamestate == kickin)
	{
	  var zv = self.findzone(p, function(r, z)
				 {
				   return (z.ball() ? -50 : 0) - r.p.dist(z.midp().p) / 200.0 - 3 * z.count() + (r.zone == z.zone ? 15 : 0) - ball.p.dist(z.midp().p) / 25;
				 });
	  if (zv.zone != p.zone)
	    {
	      var mp = zv.midp();
	      p.runto(mp.p.x, mp.p.y);
	    }
	  else
	    {
	      p.dirto = p.p.angle(ball.p);
	      p.turning = true;
	    }
	}
    }

  this.pulse = function()
    {
      if (gamestate == nullgamestate)
	{
	  changegamestate(initializing);
	}
      else if ((gamestate == goal || gamestate == outside) && (!self.team.player.holding))
	self.team.player.intercept(ball);
      else if (self.checkdropball)
	{
	  self.dropball();
	}
      else
	self.moveyerpenguinasssomewhere();

      if (gamestate == outside || gamestate == initializing)
	self.outofplaytime += 50;
      else if (!gamestate.ballinplay)
	self.outofplaytime += 10;
      else
	{
	  self.inplaytime++;
	  if (ball.lasttouch && ball.lasttouch.team.statistics)
	    ball.lasttouch.team.statistics.ballpossession++;
	}

      if (self.gamestarted &&
	  gamestate != ended && !self.suddendeath &&
	  gettime() - starttime >= self.endtime + self.extratime)
	{
	  if (self.extratime == 0)
	    self.extratime = 1 + self.outofplaytime;
	  else if (teams[0].score == teams[1].score)
	    self.suddendeath = true;
	  else if (!ball.holder && ball.speed < 20)
	    self.endgame();
	}

      if ((gamestate == kickoff || gamestate == kickin) &&
	  gettime() - self.kickoffwaittime > 20000)
	changegamestate(play);
    }

  this.endgame = function()
    {
      changegamestate(ended);
    }

  this.ballchangedzone = function()
    {
      self.moveyerpenguinasssomewhere();
    }

  this.ballshot = function(p, b)
    {
      p.team.statistics.shots++;
    }
}
RefereeAI.prototype = new AI();
