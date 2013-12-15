var Warrior = new Class({

	energy: 1000,
	kills: 0,
	attack: function(target){
		if(!this.animatingOnce) {
			this.fireEvent('attack', target);
			this.animateOnce(this.options.attack);
			if(target.isAlive){
				var delay = this.options.attack.frames / 2.3 * this.options.attack.speed;
				target.getAttacked.delay(delay, target, this);
				this.fireEvent.delay(delay, this, ['attackComplete', target]);
			}
		}
		return this;
	},
	
	kill: function(victim){
		this.kills++;
		this.fireEvent('kill', victim);
		return this;
	}
	
});

