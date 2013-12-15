/**
© Copyright 2006 Opera Software ASA. All rights reserved.
 
This file contains Original Code and/or Contributions to the Original 
Code as defined in the Opera Web Applications License (the “License”). 
You may not use this file except in compliance with the License. Please 
obtain a copy of the License at http://www.opera.com/license/owal/
and read it before using this file. 

The Original Code and all Contributions to the Original Code distributed 
under the License are distributed on an “AS IS” basis, WITHOUT WARRANTY 
OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND OPERA HEREBY DISCLAIMS ALL 
SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT. 

Please see the License for the specific language governing rights and 
limitations under the License. 
**/

/* Version: 0.1 */

var OW_Animation=
{
  time: 15,
  timer: new function()
  {
    var store=[];
    var run=false;
    var interval=0;
    var goStep=function()
    {
      var i=0, noFn=true;
      for(i=0; i<store.length; i++) 
      {
        if(store[i]) 
        {
          store[i]();
          noFn=false;
        }
      }
      if(noFn) stop();
    }
    var stop=function()
    {
      clearInterval(interval);
      run=false;
    }
    var start=function()
    {
      interval=setInterval(goStep, OW_Animation.time);
      run=true;
    }
    this.set=function(fn)
    {
      var i=0;
      while(store[i]) i++;
      store[i]=fn;
      if(!run) start();
      return i;
    }
    this.clear=function(index)
    {
      store[index]=null;
    }
  }
};

/** 
 * To create a animation object.
 * A animation has a counter which will run from 0 to 100. 
 * All properties which are added with a start and a end value will
 * be adjusted relative to that counter. 
 * @constructor 
 */

function Animation(element, speed, onstart, onfinish, accelerationProfile, onstop)
{
  var self=this;
  var currentStragedy=null;
  /** @private */
  this.properties=[];
  /** @private */
  this.current=0;
  /** @private */
  this.interval=0;
  /** @private */
  this._isrunning=false;


  /** pointer to style object */
  this.style=element.style;
  /** pointer to element on which the animation is created */
  this.element=element;
  /** the default speed in 	percents */
  this.speed=speed||this.speed;
  /** a callback which will be called befor the animation starts */
  this.onstart=onstart||null;
  /** a callback which will be called when the animation finishes */
  this.onfinish=onfinish||null;
  /** a callback which will be called when the animation finishes */
  this.onstop=onfinish||null;
  /** a profile for the speed during the animation */
  this.accelerationProfile=accelerationProfile||this.constant;



};

/** @private */
Animation.prototype.reTestHex=/^#/;
/** @private */
Animation.prototype.convertToRGB=function(string)
{
  if(this.reTestHex.test(string))
  {
    var r=0, g=0, b=0, rgb=[], i=0;
    string=string.slice(1);
    if(string.length==3)
    {
      r=string.slice(0,1);
      r+=r;
      g=string.slice(1,2);
      g+=g;
      b=string.slice(2,3);
      b+=b;
    }
    else
    {
      r=string.slice(0,2);
      g=string.slice(2,4);
      b=string.slice(4,6);
    }
    return [parseInt(r,16), parseInt(g,16), parseInt(b,16)];
  }
  else
  {
    rgb=string.slice(string.indexOf('(')+1,string.indexOf(')')).split(',');
    for( ; i<3; i++) rgb[i]=parseInt(rgb[i]);
    return rgb;
  }

}
/* default speed for a animation i persents */
Animation.prototype.speed=6;

/** @private */
Animation.prototype.reIsColor=/^#|^rgb/;
/** the default acceleration profile */
Animation.prototype.constant=function(x)
{
  return 1;
}
/** a acceleration profile which will start slow, accelerate and slow done to the end */  
Animation.prototype.sine=function(x)
{
  var sin=Math.sin;
  var pi=Math.PI;
  var start=.2;
  return (sin((x)/100*pi))+start;
}
/** a acceleration profile which will start slow and then accelerate */ 
Animation.prototype.accelerate=function(x)
{
  var start=.1;
  return start+x*x/100000;
}

/** @private */
Animation.prototype.strategyInt=function(prop, current)
{
  return (prop.from+(Math.round(prop.delta*current/100)))+prop.unit;
}

/** @private */
Animation.prototype.strategyFloat=function(prop, current)
{
  return (prop.from+prop.delta*current/100);
}

/** @private */
Animation.prototype.strategyColor=function(prop, current)
{
  var i=0, temp=[];
  for( ; i<3; i++) temp[i]=prop.fromRGB[i]+(Math.round(prop.deltaRGB[i]*current/100));
  return 'rgb('+temp.join(',')+')';
}

/** @private */
Animation.prototype.getProperty=function(property)
{
  var i=0, pointer=null;
  for( ; pointer=this.properties[i]; i++)
  {
    if(property==pointer.property) return pointer;
  }
  return null;
}

/** to check if the animation is running */
Animation.prototype.checkRun=function()
{
  return this._isrunning;
}

/** to start the animation */
Animation.prototype.run=function()
{
  var self=this;
  if(!this._isrunning)
  { 
    this._isrunning=true;
    this.releaseEvent('OAnimationStart');
    if(this.onstart) this.onstart();
    this.interval=OW_Animation.timer.set(function(){self.onStep()}); 
  }
}

/** @private */
Animation.prototype.onStep=function()
{
  var last=false;
  var temp=null;
  if(((this.current+=this.speed*this.accelerationProfile(this.current))>=100))
  {
    this.current=100;
    OW_Animation.timer.clear(this.interval);
    last=true;
  };
  var i=0, prop=null;
  
  for ( ; prop=this.properties[i]; i++)
  {
    this.style[prop.property]=prop.strategy(prop, this.current, this);
  }
  this.releaseEvent('OAnimationFrame');
  if(last)
  {
    this.current=0;
    this._isrunning=false;
    this.releaseEvent('OAnimationFinish');
    if(this.onfinish) this.onfinish();
  }
}

/** 
  * to add a animation for a certain property.
  * @param property the property which shall be animated
  * @param from the start value as string
  * @param to the end value as string
  */
Animation.prototype.addAnimation=function(property, from, to, customStrategy)
{
  var strat=null, unit='', delta=0;
  var pointer_here=this.getProperty(property)||(this.properties[this.properties.length]={});
  if(this.reIsColor.test(from))
  {
    pointer_here.property=property;
    pointer_here.from=from;
    pointer_here.to=to;
    pointer_here.fromRGB=this.convertToRGB(from);
    pointer_here.toRGB=this.convertToRGB(to);
    pointer_here.deltaRGB=
    [
      (pointer_here.toRGB[0]-pointer_here.fromRGB[0]), 
      (pointer_here.toRGB[1]-pointer_here.fromRGB[1]), 
      (pointer_here.toRGB[2]-pointer_here.fromRGB[2])
    ];
    pointer_here.strategy=this.strategyColor;
  }
  else
  {
    if(property=='opacity')
    {
      strat=this.strategyFloat;
      from=parseFloat(from);
      to=parseFloat(to);
    }
    else if (strat=customStrategy)
    {
      unit=from.replace(/-?\d*/g,'');
      from=parseFloat(from);
      to=parseFloat(to);
    }
    else
    {
      strat=this.strategyInt;
      unit=from.replace(/-?\d*/g,'');
      from=parseInt(from);
      to=parseInt(to);
    }
    pointer_here.property=property;
    pointer_here.unit=unit;
    pointer_here.from=from;
    pointer_here.to=to;
    pointer_here.delta=to-from;
    pointer_here.strategy=strat;
  }
  return this;
}

Animation.prototype.removeAnimation=function(property)
{
  var i=0, pointer=null;
  for( ; pointer=this.properties[i]; i++)
  {
    if(property==pointer.property) 
    {
      this.properties.splice(i, 1);
      return true;
    }
  }
  return false;
}

Animation.prototype.stop=function()
{
  OW_Animation.timer.clear(this.interval);
  this.current=0;
  this._isrunning=false;
  this.releaseEvent('OAnimationStop');
  if(this.onstop) this.onstop();
}

Animation.prototype.releaseEvent=function(name)
{
  var event=document.createEvent('Events');
  event.initEvent(name, true, false);
  event.data=this;
  this.element.dispatchEvent(event);
}

/** 
  * to create a animation.
  * @param speed
  * @param onfinish
  * @param accelerationProfile
  * @returns a animation object
  * @see Animation
  */
HTMLElement.prototype.createAnimation=function(speed, onfinish, accelerationProfile)
{
  return new Animation(this, speed, onfinish, accelerationProfile); 
}
