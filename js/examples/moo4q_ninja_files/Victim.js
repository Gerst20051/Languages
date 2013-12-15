var Victim = new Class({

	getAttacked: function(attacker){
		this.stopLoop();
		this.image.attr('src', this.options.attackedSrc);
		this.startLoop.delay(100, this);
		this.energy--;
		this.fireEvent('getAttacked', attacker);
		if(this.energy == 0) this.die(attacker);
	},

	die: function(killer){
		if(this.isAlive){
			this.parent(killer);
			this.animateOnce(this.options.die, true);
		}
		return this;
	},
	
	revive: function(){
		if(!this.isAlive){
			this.parent();
			this.changeAnimation(this.options.rest).startLoop();
		}
		return this;
	}

});

