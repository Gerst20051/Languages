// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function Coord(x, y)
{
  this.x = x;
  this.y = y;

  this.dist = function(p)
    {
      var dx = p.x - this.x;
      var dy = p.y - this.y;
      return Math.sqrt(dx * dx + dy * dy);
    }

  this.distsq = function(p)
    {
      var dx = p.x - this.x;
      var dy = p.y - this.y;
      return dx * dx + dy * dy;
    }

  this.set = function(p)
    {
      this.x = p.x;
      this.y = p.y;
    }

  this.angle = function(p)
    {
      return Math.atan2(p.y - this.y, p.x - this.x);
    }

  this.clone = function() { return new Coord(this.x, this.y); }

  this.toString = function() { return "(" + this.x + ", " + this.y + ")"; }
}

function Point(zone, x, y)
{
  this.zone = zone;
  this.p = new Coord(x, y);
  this.closest = [null, null];
  this.closestd = [null, null];

  var self = this;

  this.updatedata = function(dt)
    {
      self.closest = [null, null];
      self.closestd = [9999999999, 9999999999];
      players.foreach(function(p)
		      {
			var d = self.p.distsq(p.p);
			var i = p.team.idx;
			if (i != 0 && i != 1)
			  return;
			if (d < self.closestd[i])
			  {
			    self.closestd[i] = d;
			    self.closest[i] = p;
			  }
		      });
    }
}

function PointView(team, point)
{
  this.ouridx = team.idx;
  this.oppidx = team.oppidx;
  this.point = point;

  this.ourclosest = function() { return this.point.closest[this.ouridx]; }
  this.ourclosestd = function() { return this.point.closestd[this.oppidx]; }
  this.oppclosest = function() { return this.point.closest[this.oppidx]; }
  this.oppclosestd = function() { return this.point.closestd[this.oppidx]; }
  this.p = function() { return this.point.p; }
}

function Zone(id, x, y, width, height)
{
  this.id = id;
  this.x = x;
  this.y = y;
  this.width = width;
  this.height = height;
  this.midp = new Point(this, x + width / 2, y + height / 2);
  this.ball = false;
  this.playercount = [0, 0];

  var self = this;

  var contains = function(z, p)
    {
      return p.x >= z.x && p.x < z.x + z.width && p.y >= z.y && p.y < z.y + z.height;
    }

  this.contains = function(p) { return contains(self, p); }

  this.updatedata = function(dt)
    {
      self.playercount[0] = 0;
      self.playercount[1] = 0;

      players.foreach(function(p)
		      {
			if (contains(self, p.p))
			  {
			    if (!p.zone || !p.zone.contains(p.p))
			      p.zone = self;
			    if (p.team.idx >= 0)
			      self.playercount[p.team.idx]++;
			  }
		      });
      if (contains(self, ball.p))
	{
	  if (!self.ball)
	    ais.foreach(function(ai) { ai.queue(ai.ballchangedzone); });
	  ball.zone = self;
	  self.ball = true;
	}
      else
	self.ball = false;
    }
}

function ZoneView(team, zone)
{
  var self = this;

  if (team.idx >= 0)
    {
      this.ouridx = team.idx;
      this.oppidx = team.oppidx;
    }
  this.zone = zone;

  this.ourcount = function() { return self.zone.playercount[self.ouridx]; }
  this.oppcount = function() { return self.zone.playercount[self.oppidx]; }
  this.count = function() { return self.zone.playercount[0] + self.zone.playercount[1]; }
  this.midp = function() { return this.zone.midp; }
  this.ball = function() { return this.zone.ball; }
}
