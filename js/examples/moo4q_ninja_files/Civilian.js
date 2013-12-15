var Civilian = new Class({

	Extends: Human,
	Implements: Victim,

		options: {
			die: {
				frames: 29,
				fileName: 'images/die/die_',
				speed: 75
			},
			attackedSrc: 'images/die/die_0000.png'
		},

	jQuery: 'civilian',
	
	initialize: function(selector, options){
		this.parent(selector, options);
		this.loadImages(this.options.die);
	}

});

