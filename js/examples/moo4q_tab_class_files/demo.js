$(function(){

	$('#pane').tabs({tab: '.nav a', content: '.section'});

	$('#pane').tabs('addEvent', 'show', function(){
		$(document.body).toggleClass('toggle');
	});

	$('#pane1').ajaxTabs({
		tab: '.nav a', 
		content: '.section',
		loadingHTML: '<p class=loading>Loading<br><br><img src=loading.gif></p>',
		onShow: function(){
			this.container.css('opacity',0.5);
		},
		onAjaxComplete: function(){
			this.container.css('opacity','');
		}
	});
	
	// managing state
	$('#show').click(function(){
		$('#pane').tabs('show', 0);
	})

});