// JavaScript Document
$(function(){
document.createElement('header');
document.createElement('footer');
if (typeof(localStorage) == 'undefined' ){
	$('body').html('Go away&hellip; No candy for you <i>or</i> get a <b>good</b> browser');
}else{
	try{
		localStorage.setItem("name", "Hello World!");
	}catch (e){
		if(e == QUOTA_EXCEEDED_ERR){
			$('body').html('Quota exceeded! Wanna clear cache?');
		}
	}	
	localStorage.removeItem("name");

$('#newitem').click(function(){
	$(this).select();	
});

$('#newitem').keyup(function(e) {
	if(e.keyCode == 13) {		
		var newitem = $(this).val();
		var itemsplit = newitem.split('#');
		var itemtxt = itemsplit[0];
		if(itemsplit[1] != undefined){
			var itemcolor = itemsplit[1];
		}
		if(newitem != 'new&hellip;' && newitem !=''){
			$('.ph').remove();
			$('#todolist').addClass('editable').append('<li class="item" style="color:'+itemcolor+'">'+itemtxt+'</li>');
			localStorage.setItem('todolist',$('#todolist').html());
			$(this).val('');
		}		
	}
});

if(localStorage.getItem('todolist')){
	$('#todolist').html(localStorage.getItem('todolist'));
	if($('#todolist').find('.item').size() > 0){
		$('#todolist').addClass('editable');	
	}
}

$('.item').live('hover', function(event) {
	if (event.type == 'mouseover') {
		$(this).append('<a class="markdone" href="#">done</a>');
	} else {
		$(this).find('a').remove();
	}
});

$('.markdone').live('click',function(){
	$(this).parent().slideUp().remove();
	localStorage.setItem('todolist',$('#todolist').html());
	ifempty()
	return false;
});

$('.editable').live('dblclick',function(){	
	$(this).addClass('editing').attr('contenteditable','true').focus();	
})

$('.editable').live('blur',function(){
	$(this).removeClass('editing').attr('contenteditable','false');	
	localStorage.setItem('todolist',$('#todolist').html());	
});

function ifempty() {	
	if($('#todolist').find('.item').size() == 0){
		$('#todolist').removeClass('editable').append('<li class="ph">w00t! I&#39;m free!!</li>');
	}
}

function supports_local_storage() {
  return ('localStorage' in window) && window['localStorage'] !== null;
}
} 
});
