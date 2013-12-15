$(document).ready(function(){
	
	// create our humans
	$('#bob').civilian(); // creates instance
	$('#ryu').ninja({side: 'evil'}); // pass in some options
	
	// keyboard events
	$(document).bind('keydown',function(event){
		
		if(event.keyCode == 65)
			$('#ryu').ninja('attack', $('#bob').civilian()); 
			// $('#bob').civilian() a second time returns the instance
			// just like `var bob = new Civilian('#bob')`, it returns a object like `bob`
			// so `$('#bob').civilian().die() ~= $('#bob').civilian('die')`
			// except the first returns the object and the second returns the jquery object
			
		if(event.keyCode == 82) $('#bob').civilian('revive');
			
		if(event.keyCode == 69) $('#bob').civilian('eat');
			
	});
	
	// log function
	var setLog = function(message, className){
		className = className || 'message';
		var top = $('#log')[0].scrollHeight;
		$('#log').append('<p class=' + className + '>' + message + '</p>').stop().animate({scrollTop: top}, 1000);
	}

	// instance events
	// when things happen we can do stuff, like the callbacks you're used to
	$('#bob').civilian().addEvents({
		onDie: function(killer){
			// events can have arguments, killer is the instance of $('#ryu').ninja()
			// and `this` is the instance of $('#bob').civilian() in here.
			setLog(this.name + ' dies by the hand of ' + killer.name, 'die'); 
		},
		onRevive: function(){ 
			setLog(this.name + ' is Revived', 'revive'); 
		},
		onGetAttacked: function(attacker){ 
			setLog(this.name + ' is hit! He has ' + this.energy + ' energy left after ' + attacker.name + "'s attack!", 'getAttacked'); 
		},
		onEat: function(){
			setLog(this.name + ' ate some food. His energy is now ' + this.energy, 'eat');
		}
	});
	
	$('#ryu').ninja('addEvents',{
		onAttack: function(target){ 
			setLog(this.name + ' attacks ' + target.name + '!', 'attack'); 
		},
		onKill: function(target){
			var plural = (this.kills == 1) ? 'kill' : 'kills';
			setLog(this.name + ' killed ' + target.name + '!! Ryu now has ' + this.kills + ' ' + plural, 'kill');
			setLog.delay(3000, this, ["You find an <span>Emporer's Hairpin</span> on " + target.name, 'find']);
			setLog.delay(3000, this, ["You find a <span>handful of paint brushes</span> on " + target.name, 'find']);
			setLog.delay(3000, this, ["You find <span>4 " + target.name + " skins</span> on " + target.name, 'find']);
		}
	});
	
});