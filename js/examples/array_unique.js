(function($){
	var _old = $.unique;
	$.unique = function(arr){
		// do the default behavior only if we got an array of elements
		if (!!arr[0].nodeType) {
			return _old.apply(this,arguments);
		} else {
			// reduce the array to contain no dupes via grep/inArray
			return $.grep(arr,function(v,k){
				return $.inArray(v,arr) === k;
			});
		}
	}
})(jQuery);