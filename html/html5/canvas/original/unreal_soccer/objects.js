// This file is part of Unreal Soccer
// by Henrik Falck <f@lck.nu> 2006

function Ball(x, y)
{
  this.p = new Coord(x, y);
  this.maxspeed = 100.0;
  this.accel = 20.0;
  this.dir = 0.0;
  this.speed = 0.0;
  this.visible = true;
  this.active = true;
  this.solid = true;
  this.targetable = true;
  this.interceptable = true;
  this.sin = 0.0;
  this.cos = 0.0;
  this.radius = 7;
  this.holder = null;
  this.height = 0.0;
  this.heightaccel = 0.0;
  this.intercepting = [];

  this.updatepos = function(dt)
    {
      var f = this.height < 0.1 ? 1.1 : 0.9;
      this.p.x += dt * f * this.speed * this.cos;
      this.p.y += dt * f * this.speed * this.sin;
      this.x = this.p.x;
      this.y = this.p.y;
      this.height += this.heightaccel * dt;
      if (this.height > 0.0)
	this.heightaccel -= 5.0 * dt;
      else if (this.heightaccel < 0.5)
	{
	  this.heightaccel = -0.5 * this.heightaccel;
	  this.height = 0.0;
	}
      else
	{
	  this.height = 0.0;
	  this.heightaccel = 0.0;
	}
    }

  this.update = function(dt)
    {
      if (this.holder)
	{
	  this.speed = this.holder.speed;
	  this.p.x = this.holder.p.x + this.holder.cos * this.radius;
	  this.p.y = this.holder.p.y + this.holder.sin * this.radius;
	}
      if (this.speed > 0.0)
	{
	  this.speed -= this.accel * dt;
	  if (this.speed < 0.0)
	    this.speed = 0.0;
	}

      this.updatepos(dt);
    }

  this.kick = function(dir, speed, who)
    {
      if (!this.holder)
	return false;
      if (!gamestate.ballinplay)
	return false;
      if (who && this.holder != who)
	{
	  if (settings["debug"])
	    opera.postError("holder: " + this.holder.toString() + " kicker: " + who.toString());
	  return false;
	}
      if (!gamestate.ballinplay)
	return false;
      if (!this.holder.referee)
	this.lasttouch = this.holder;
      this.holder.holding = false;
      this.dir = dir;
      this.cos = Math.cos(dir);
      this.sin = Math.sin(dir);
      this.speed = Math.min(this.maxspeed, speed);
      this.heightaccel = 5.0 * this.speed / this.maxspeed;
      var ball = this;
      ais.foreach(function(ai) { ai.queue(ai.ballkicked, ball, ball.holder, ball.dir, ball.speed); });
      this.holder = null;
      return true;
    }

  this.release = function()
    {
      if (!this.holder)
	return;
      if (!this.holder.referee)
	this.lasttouch = this.holder;
      this.holder.holding = false;
      this.dir = this.holder.dir;
      this.speed = Math.min(this.holder.speed, this.maxspeed);
      this.cos = this.holder.cos;
      this.sin = this.holder.sin;
      this.holder = null;
    }

  this.collide = function(t)
    {
    }

  this.intercepted = function(t)
    {
      this.height = 0.0;
      this.heightaccel = 0.0;
      this.intercepting.foreach(function(t2) { if (t2.intercepting) t2.intercepted(this, t); });
      this.intercepting = [];
    }

  this.addintercepting = function(t)
    {
      this.intercepting.push(t);
    }

  this.changedir = function(dir)
    {
      this.dir = dir;
      this.cos = Math.cos(dir);
      this.sin = Math.sin(dir);
    }
}

function BallPainter(ball, img)
{
  this.ball = ball;
  this.img = img;
  this.width = 14;
  this.height = 14;
  this.xoffset = -7;
  this.yoffset = -7;
  this.curframe = 0;
  this.framestate = 1;
  this.dtcount = 0.0;
  this.framespeed = 5000.0;
  this.speedframe = 0;

  this.updatecurframe = function()
    {
      this.curframe += this.framestate;
      if (this.curframe == 4)
	{
	  this.curframe = 2;
	  this.framestate = -1;
	}
      else if (this.curframe == -1)
	{
	  this.curframe = 1;
	  this.framestate = 1;
	}
      if (this.curframe == 1)
	this.framestate = rand(2) == 0 ? 1 : -1;
      this.dtcount -= this.framespeed;
      if (this.ball.speed < 25)
	this.speedframe = 0;
      else if (this.ball.speed < 50)
	this.speedframe = 1;
      else
	this.speedframe = 2;
    }
  
  this.paint = function(ctx, dt)
    {
      this.dtcount += dt * 800 * this.ball.speed;
      if (this.dtcount >= this.framespeed)
	this.updatecurframe();
      ctx.drawImage(this.img, this.curframe * this.width,
		    this.speedframe * this.height,
		    this.width, this.height,
		    parseInt(this.ball.p.x) + this.xoffset, parseInt(this.ball.p.y) + this.yoffset,
		    this.width, this.height);
    }
}

function Goal(x, y)
{
  this.p = new Coord(x, y);
  this.solid = false;
  this.visible = true;
  this.active = false;
}

function GoalPainter(goal, img)
{
  this.goal = goal;
  this.img = img;
  this.width = 60;
  this.height = 32
  this.xoffset = -30;
  this.yoffset = -32;
  this.curframe = 0;
  this.framestate = 1;
  this.dtcount = 0;
  this.framespeed = 1000;

  this.updatecurframe = function()
    {
      this.curframe += this.framestate;
      if (this.curframe == 4)
	{
	  this.curframe = 2;
	  this.framestate = -1;
	}
      else if (this.curframe == -1)
	{
	  this.curframe = 1;
	  this.framestate = 1;
	}
      if (this.curframe == 1)
	this.framestate = rand(2) == 0 ? 1 : -1;
      this.dtcount -= this.framespeed;
    }
  
  this.paint = function(ctx, dt)
    {
      this.dtcount += dt * 1000 * (1 + Math.random());
      if (this.dtcount >= this.framespeed)
	this.updatecurframe();
      ctx.drawImage(this.img, 0, this.curframe * this.height,
		    this.width, this.height,
		    this.goal.p.x + this.xoffset, this.goal.p.y + this.yoffset,
		    this.width, this.height);
    }
}

function Goalpost(x, y)
{
  this.p = new Coord(x, y);
  this.radius = 12;
  this.active = true;
  if (false && debug)
    {
      this.painter = new GoalpostPainter(this);
      this.visible = true;
    }

  this.update = function(dt)
    {
    }

  this.collide = function(t)
    {
      if (!(t instanceof Ball))
	return true;
      if (t.holder)
	return true;
      if (t.height > 2.0)
	return true;
      if (gamestate == goal || gamestate == outside)
	return true;
      do {
	var dir = Math.random() * 2 * Math.PI;
      } while (Math.abs(t.dir - dir) < 0.2);
      t.changedir(dir);
      t.speed *= 0.8;
    }
}

function GoalpostPainter(post)
{
  this.post = post;

  this.paint = function(ctx, dt)
    {
      ctx.save();
      ctx.fillStyle = "red";
      ctx.beginPath();
      ctx.moveTo(this.post.p.x, this.post.p.y);
      ctx.arc(this.post.p.x, this.post.p.y, this.post.radius, 0, 2 * Math.PI, true);
      ctx.closePath();
      ctx.stroke();
      ctx.restore();
    }
}
