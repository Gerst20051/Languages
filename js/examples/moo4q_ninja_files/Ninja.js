var Ninja = new Class({
	
	Extends: Human,
	Implements: Warrior,

		options: {
			side: 'evil',
			attack: {
				frames: 44,
				fileName: 'images/attack/attack_',
				speed: 40
			},
			rest:{
				frames: 14,
				fileName: 'images/ninja-rest/rest_',
				speed: 75 
			}
		},

	jQuery: 'ninja',

	initialize: function(selector, options){
		this.parent(selector, options);
		this.loadImages(this.options.attack);
		this.side = this.options.side;
	}

});