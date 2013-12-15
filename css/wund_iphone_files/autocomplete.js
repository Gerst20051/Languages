jQuery(document).ready(function($) {
	var input = $("#wuSearch");
	var select = $("#wuSelect");
	var hasSelect = (select && select.length > 0);
	input.autocomplete({
		source: function(request, response) {
			$.ajax({
				url: "http://autocomplete.wunderground.com/aq",
				dataType: "jsonp",
				jsonp: "cb",
				data: {
					query: request.term
				},
				success: function(data) {
					if (!data || !data.RESULTS) {
						// alert("error: null or empty ajax response");
						return;
					}
					response($.map(data.RESULTS, function(item) {
						var l = request.term.length;
						var name = item.name;
						var type = item.type;
						var value;
						if (!hasSelect && type == "pws") {
							// filter out PWS items if there is no select menu to submit a search by PWS
							return null;
						}
						if (type == "pws") {
							value = name.split(",",1)[0];
						} else {
							value = name;
						}
						return {
							label: '<span class="ui-autocomplete-term">'+name.substr(0,l)+'</span>' +name.substr(l),
							value: value,
							type: type
						}
					}));
				}
			});
		},
		search: function(event, ui) {
			var disabled = $(this).autocomplete("option", "disabled");
			if (disabled) {
				return false;
			}
		},
		select: function(event, ui) {
			if (ui.item) {
				this.value = ui.item.value;
				updateSelectMenu(ui.item.type);
				// submit the form automatically (city pages have a custom submit function)
				if (window.SubmitFastForecastSearch) {
					SubmitFastForecastSearch();
				} else {
					this.form.submit();
				}
			}
		},
		minLength: 1
	});
	if (hasSelect) {
		function updateEnabledState()
		{
			var sel = select.val().toLowerCase();
			if (sel == "weather" || sel == "pws") {
				input.autocomplete("option", "disabled", false);
				setNativeAutocomplete(input, false);
			} else {
				input.autocomplete("option", "disabled", true);
				setNativeAutocomplete(input, true);
			}
			// console.log("disabled? " + input.autocomplete("option", "disabled"));
		}
		select.change(updateEnabledState);
		updateEnabledState();
	}
	function updateSelectMenu(type) {
		if (!hasSelect) return;
		if (type == "city") {
			select.val("WEATHER");
		} else if (type == "pws") {
			select.val("PWS");
		}
	}
});

function setNativeAutocomplete(jquery_elt, active_state) {
	if (active_state === false) {
		jquery_elt.addClass( "ui-autocomplete-input" )
			.attr({
			"autocomplete": "off",
			"role": "textbox",
			"aria-autocomplete": "list",
			"aria-haspopup": "true"
			});
	} else if (active_state === true) {
		jquery_elt.removeClass( "ui-autocomplete-input" )
			.removeAttr( "autocomplete" )
			.removeAttr( "role" )
			.removeAttr( "aria-autocomplete" )
			.removeAttr( "aria-haspopup" );
	}
}
