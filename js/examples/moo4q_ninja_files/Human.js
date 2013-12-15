var Human = new Class({

	Implements: [Options, Events, Loop],
	
		options: {
			rest: {
				frames: 14,
				fileName: 'images/rest/rest_',
				speed: 100
			}
		},

	jQuery: 'human',

	energy: 5,
	isAlive: true,
	animatingOnce: false,

	initialize: function(selector, options){
		this.setOptions(options);
		this.image = $(selector);
		this.name = this.image.attr('name') || 'unkown';
		this.currentAnimation = this.options.rest;
		this.loadImages(this.options.rest);
		this.setLoop(this.animate, this.currentAnimation.speed).startLoop();
	},
	
	eat: function(){
		if(this.isAlive){
			this.energy++;
			this.fireEvent('eat');
		}
	},
	
	die: function(killer){
		if(this.isAlive){
			this.energy = 0;
			this.isAlive = false;
			killer.kill(this);
			this.fireEvent('die', killer);
		}
		return this;
	},
	
	revive: function(){
		if(!this.isAlive){
			this.energy = 1;
			this.isAlive = true;
			this.fireEvent('revive');
		}
		return this;
	},
		
	animate: function(){
		if(this.loopCount > this.currentAnimation.frames) this.loopCount = 1;
		this.image.attr('src', this.currentAnimation.fileName + this.loopCount.zeroPad(4) + '.png');
		return this;
	},
	
	changeAnimation: function(animation){
		if(!this.animatingOnce){
			this.currentAnimation = animation;
			this.resetLoop().setLoop(this.animate, animation.speed);	
		}
		return this;
	},
	
	animateOnce: function(animation, stop){
		if(!this.animatingOnce){
			var original = this.currentAnimation;
			this.changeAnimation(animation);
			this.animatingOnce = true;
			var delay = this.currentAnimation.frames * this.currentAnimation.speed;
			var self = this;
			(function(){
				self.animatingOnce = false;
				(stop) ? self.stopLoop() : self.changeAnimation(original);
			}).delay(delay);
		}
		return this;
	},
	
	loadImages: function(animation){
		(animation.frames - 1).times(function(num){
			$('<img/>', { src: animation.fileName + (num + 1).zeroPad(4) + '.png'});
		}, this);
		return this;
	}

});

Number.implement({
	zeroPad: function(length){
		var str = '' + this;
		while (str.length < length) str = '0' + str;
		return str;
	}
});

