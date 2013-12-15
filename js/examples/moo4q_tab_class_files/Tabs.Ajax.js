Tabs.Ajax = new Class({
	
	Extends: Tabs, // makes this a sub class of `Tabs`
	
		// can add some new options
		options: {
			loadingHTML: '<p>Loading...</p>'
		},
	
	// give it a new jQuery prototype method
	jQuery: 'ajaxTabs',

	// change just a single a method to add wildly different functionality
	show: function(index){
		var href = jQuery(this.tabs[index]).attr('href');
		// detach tabs and prevent default browser behavior
		this.detach().tabs.bind('click.stop',function(event){
			event.preventDefault();
		});
		// this.parent calls the `show` method of the parent class
		this.parent(index);
		var self = this; // for use in ajax callback
		this.currentTab.html(this.options.loadingHTML).load(href, function(){
			self.attach().fireEvent('ajaxComplete').tabs.unbind('click.stop');
			// could have used $.proxy, but storing `this` in `self` is easy enough
		});
		return this;
	}

});

