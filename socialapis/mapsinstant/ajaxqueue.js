/*
 * Queued Ajax requests.
 * A new Ajax request won't be started until the previous queued 
 * request has finished.
 */
$.ajaxQueue = function(o) {
	var _old = o.complete;
	o.complete = function() {
		if (_old) _old.apply(this, arguments);
		jQuery.dequeue($.ajaxQueue, "ajax");
	};
	$([$.ajaxQueue]).queue("ajax", function() {
		$.ajax(o);
	});
};

/*
 * Synced Ajax requests.
 * The Ajax request will happen as soon as you call this method, but
 * the callbacks (success/error/complete) won't fire until all previous
 * synced requests have been completed.
 */
$.ajaxSync = function(o) {
	var fn = $.ajaxSync.fn, data = $.ajaxSync.data, pos = fn.length;
	
	fn[pos] = {
		error: o.error,
		success: o.success,
		complete: o.complete,
		done: false
	};

	data[pos] = {
		error: [],
		success: [],
		complete: []
	};

	o.error = function() { data[pos].error = arguments; };
	o.success = function() { data[pos].success = arguments; };
	o.complete = function() {
		data[ pos ].complete = arguments;
		fn[ pos ].done = true;

		if (pos == 0 || !fn[pos-1])
			for (var i = pos; i < fn.length && fn[i].done; i++) {
				if (fn[i].error) fn[i].error.apply($, data[i].error);
				if (fn[i].success) fn[i].success.apply($, data[i].success);
				if (fn[i].complete) fn[i].complete.apply($, data[i].complete);
				fn[i] = null;
				data[i] = null;
			}
	};

	return $.ajax(o);
};

$.ajaxSync.fn = [];
$.ajaxSync.data = [];