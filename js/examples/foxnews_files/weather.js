// Weather Service Application. updated: 06/10/2010
// Dependencies: jQuery
// ** Wrapped to use $ for jQuery instead (v5 conflict resolution)

(function($){
	
	function WeatherService() {
		var initVars = {
			weatherCityData: [],
			ACListener: {},
			currentLocId: "",
			currentCityString: "",
			serviceTriggered: false,

			UPDATE_HEADER_ACTION: 'UPDATE_HEADER',
			SERVICE_URL_WEATHER_JSONP: '/weather/feed/getWeatherJsonP?callback=',
			SERVICE_URL_LOCATION_JSONP: '/weather/feed/getLocationIdJsonP?callback=',
			SERVICE_URL_WEATHER_HTML: '/weather/feed/getWeatherHtml',
			SERVICE_URL_LOCATION_REGION_JSON: '/weather/feed/getLocationRegionCodeJson',
			SERVICE_URL_CITY_DATA: '/weather/feed/getWeatherCityData',
			SERVICE_URL_MARINE_JSON: '/weather/feed/getMarineLocationsJson',
			SERVICE_URL_SKI_RESORTS_JSON: '/weather/feed/getSkiResortsJson',
			SERVICE_URL_SKI_COND_JSON: '/weather/feed/getSkiConditionJson',
			SERVICE_URL_AIRPORT_LOCATION_JSON: '/weather/feed/getAirportLocationIdJson',
			SERVICE_URL_AIRPORT_DELAY_JSON: '/weather/feed/getAirportDelayJson',
			PAGE_URL_YOUR_WEATHER: '/weather/your-weather/index.html',
			PAGE_URL_SEARCH_RESULTS: '/weather/search-results/index.html',
			PATH_WEATHER_ICON_IMAGE: '/static/fn2/ws/weather/img/icons_60x60/',
			WEATHER_ICON_IMAGE_EXT: '.png',
			WEATHER_DEFAULT_LOCATION: 'USNY0996',
			WEATHER_COOKIE_DOMAIN: 'foxnews.com',

			MSG_DEFAULT_ERROR: 'Data unavailable. Please check back again soon.',
			MSG_AIRPORT_CODE_ERROR: 'Please enter a valid 3- or 4-digit airport code.',
			MSG_AIRPORT_NOT_FOUND_ERROR: 'No airport results found.  Please try again.',
			TXT_DEFAULT_SEARCH_FIELD: 'Enter city or zip code',
			TXT_HEADER_SEARCH_FIELD: 'Enter city or zip',
			TXT_AIRPORT_SEARCH_FIELD: 'Enter airport code (IATA or ICAO)',

			LIST_SKI_CONDITIONS_ENABLED: 'AK|AL|AZ|CA|CO|CT|IA|ID|IL|IN|MA|MD|ME|MI|MN|MO|MT|NC|ND|NH|NJ|NM|NV|NY|OH|OR|PA|RI|SD|TN|UT|VA|VT|WA|WI|WV|WY',
			LIST_MARINE_FORECAST_ENABLED: 'AK|AL|AS|AZ|CA|CT|DC|DE|FL|FM|GA|GU|HI|LA|MA|MD|ME|MH|MS|NC|NJ|NY|OR|PA|PR|PW|RI|SC|TX|VA|VI|VT|WA'
		}
		for (i in initVars) {
			this[i] = initVars[i];
		}
	};

	WeatherService.prototype = {
		init: function() {
			// this is to make sure that first time visitor has the weatherloc cookie created
			var initData='{"location":[{"loc_id":"USNY0996","zip":"10108","city":"New York","state":"NY"}]}';
			
			if(this.weatherCookie('weatherloc')){
				var weatherlocObj = this.createObjFromJSON(this.weatherCookie('weatherloc'));
				if(!weatherlocObj || !weatherlocObj.location || !weatherlocObj.location[0].loc_id){
					this.createWeatherLocationCookie(initData);
				}
			}else{
				this.createWeatherLocationCookie(initData);
			}
		},
		// from jquery.cookie.js
		weatherCookie: function(name,value,options) {
		    if(typeof value != 'undefined'){
		        options=options || {};
		        if(value===null){
		            value='';
		            options.expires=-1;
		        }
		        var expires='';
		        if(options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)){
		            var date;
		            if(typeof options.expires == 'number'){
		                date=new Date();
		                date.setTime(date.getTime()+(options.expires*24*60*60*1000));
		            }else{
		                date=options.expires;
		            }
		            expires='; expires='+date.toUTCString();
		        }
		        var path=options.path ? '; path='+(options.path) : '';
		        var domain=options.domain ? '; domain='+(options.domain) : '';
		        var secure=options.secure ? '; secure' : '';
		        document.cookie=[name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
		    }else{
		        var cookieValue=null;
		        if(document.cookie && document.cookie != ''){
		            var cookies=document.cookie.split(';');
		            for(var i=0; i < cookies.length; i++){
		                var cookie = $.trim(cookies[i]);
		                if(cookie.substring(0, name.length+1) == (name+'=')){
		                    cookieValue=decodeURIComponent(cookie.substring(name.length+1));
		                    break;
		                }
		            }
		        }
		        return cookieValue;
		    }
		},
		createWeatherDataCookie: function(locId,jsonString) {				
			var validateWeatherObj = this.createObjFromJSON(jsonString);
			var weatherJSObj=validateWeatherObj.weatherData;
			var city=weatherJSObj.cityName;
			var country=weatherJSObj.country;
			var tempHi=weatherJSObj.days[0].hi;
			var tempLo=weatherJSObj.days[0].lo;	
			if(validateWeatherObj && city!='' && country!=''){
				var expDate=new Date();
				if (tempHi=='' || tempLo=='') {
					expDate.setTime(expDate.getTime()+(60*1000));
				} else {
					expDate.setTime(expDate.getTime()+(2*60*60*1000));
				}
				this.weatherCookie('weatherdata_'+locId, jsonString, {expires: expDate, path: '/', domain:this.WEATHER_COOKIE_DOMAIN});
			}
		},
		createWeatherLocationCookie: function(jsonString) {
			var expDate=new Date();
			expDate.setTime(expDate.getTime()+(1000*24*60*60*1000));
			this.weatherCookie('weatherloc', jsonString, {expires: expDate, path: '/', domain:this.WEATHER_COOKIE_DOMAIN});
		},
	 	createWeatherFavoritesCookie: function(favArray) {
			var favLocCookieString='';
			for(var favIndex=0; favIndex<favArray.length; favIndex++){
				favLocCookieString += favArray[favIndex]+'|';
			}
			// trim the last pipe
			favLocCookieString=favLocCookieString.slice(0, -1);	
			if(favLocCookieString!=''){
				var expDate=new Date();
				expDate.setTime(expDate.getTime()+(1000*24*60*60*1000));
				this.weatherCookie('weatherfavlocs', favLocCookieString, {expires: expDate, path: '/', domain:this.WEATHER_COOKIE_DOMAIN});
			}else{
				this.weatherCookie('weatherfavlocs', '', {expires: -1, path: '/', domain:this.WEATHER_COOKIE_DOMAIN});
			}
		},
		getWeatherDataCookie: function(locId) {
			return this.weatherCookie('weatherdata_'+locId);
		},
		createObjFromJSON: function(jsonString){
			if(jsonString!=''){
				return eval('('+unescape(jsonString).replace(/\+/g, " ")+')');
			}else{
				return null;
			}
		},
		createLocJSObj: function(jsonObj) {
			var locationObj={};
			var wCityString=jsonObj.city.replace('`','\'');
			if(jsonObj.loc_id!='undefined'&&jsonObj.loc_id!=null){locationObj.loc_id=jsonObj.loc_id;}else{return null;}
			if(jsonObj.city!='undefined'&&jsonObj.city!=null){locationObj.city=jsonObj.city.replace('`','\'');}else{locationObj.city='';}
			if(jsonObj.zip!='undefined'&&jsonObj.zip!=null){locationObj.zip=jsonObj.zip;}else{locationObj.zip='';}
			if(jsonObj.state!='undefined'&&jsonObj.state!=null){locationObj.state=jsonObj.state;}else{locationObj.state='';};
			if(jsonObj.country!='undefined'&&jsonObj.country!=null){locationObj.country=jsonObj.country;}else{locationObj.country='';}
			if(jsonObj.loc_id.substr(0,2)!='US'){wCityString+=', '+jsonObj.country;}else{wCityString+=', '+jsonObj.state;}	
			locationObj.fullCityString=wCityString;
			return locationObj;
		},
		displayHeader: function(serviceDomain) {
			if(this.serviceTriggered){return false;}
			else {this.serviceTriggered=true;}
			
			if(serviceDomain!='undefined'&&serviceDomain!=null) {
				this.PAGE_URL_SEARCH_RESULTS = serviceDomain + this.PAGE_URL_SEARCH_RESULTS;
				this.PAGE_URL_YOUR_WEATHER = serviceDomain + this.PAGE_URL_YOUR_WEATHER;
				this.PATH_WEATHER_ICON_IMAGE = serviceDomain + this.PATH_WEATHER_ICON_IMAGE;		
				this.SERVICE_URL_LOCATION_JSONP = serviceDomain + this.SERVICE_URL_LOCATION_JSONP;
				this.SERVICE_URL_WEATHER_JSONP = serviceDomain + this.SERVICE_URL_WEATHER_JSONP;
				if(document.location.hostname.lastIndexOf(this.WEATHER_COOKIE_DOMAIN)==-1){
					this.WEATHER_COOKIE_DOMAIN = document.location.hostname;
				}
			}
			this.init();
			this.renderWidget();
		},
		renderWidget: function() {
			$("body").data("weather-rendered",true); // log initialization
			var ws = this;
			var t=setTimeout(renderer=function(){
				ws.renderHeaderWeather();
				ws.renderHeaderLocationChange();
			},500);
		},
		renderHeaderWeather: function(){
			var ws = this;
			// if weatherloc exist, pass the location ID from cookie
			if(this.weatherCookie('weatherloc')){
				var weatherlocObj = this.createObjFromJSON(this.weatherCookie('weatherloc'));
				var locObject = this.createLocJSObj(weatherlocObj.location[0]);
				if(this.getWeatherDataCookie(locObject.loc_id)){ 
					this.headerDisplayAction(locObject.loc_id);
					$(document).ready(function(){
						ws.renderWeatherWidget(locObject.loc_id); // new widget
					});
				}else{
					// requested location is not in cookie yet, fetch data and store in cookie then display
					this.fetchAndSaveWeatherJSONCookie(locObject.loc_id);
				}
			}else{
				// render Default (Manhattan)
				this.init();
				var weatherlocObj = this.createObjFromJSON(this.weatherCookie('weatherloc'));
				this.fetchAndSaveWeatherJSONCookie(weatherlocObj.location[0].loc_id);
			}

			var requestedLocationObj = this.createObjFromJSON(this.weatherCookie('weatherloc'));
			// send a request to update the current conditions
			$('.portlet .weather.current-conditions').each(function(){
				ws.renderYourWeather(this,requestedLocationObj.location[0].loc_id,'weather_current');
			});
		},
		renderRightRailSearch: function(){
			var topRightRailHtml='<ul>'+this.generateRightRailSearch('yourweather')+'</ul>';
			document.write(topRightRailHtml);
		},
		generateRightRailSearch: function(ac_results_css_appendix) {
			var searchField='weatherInput_'+ac_results_css_appendix;
			var rightRailHTML='<li><form method="get" action="'+ this.PAGE_URL_SEARCH_RESULTS+'" id="weather_search_form" class="form_1"'
				+' onsubmit="weather_service.locationSearchAction(this,\'#'+searchField+'\');"><fieldset class="text_box">'
				+'<div class="search-input-holder"><div class="preloader-img"></div><input type="text" name="searchTerm" id="'+searchField+'" onfocus="weather_service.searchFieldOnFocus(this);"'
				+' onblur="weather_service.searchFieldOnBlur(this, \''+ this.TXT_DEFAULT_SEARCH_FIELD+'\');"'
				+' class="weather_city_autocomplete_'+ac_results_css_appendix+' search_input no_spon" size="50" value="'
				+ this.TXT_DEFAULT_SEARCH_FIELD+'" name="keys"/></div>'
				+'<input type="submit" class="go" value=""/></fieldset></form></li>';
			return rightRailHTML;
		},
		
		// new weather module 
		renderWeatherWidget: function(locationId) {
			var widgetElm = $("#weather-widget");
			if (widgetElm.size()===0) { return false; } // if no widget, don't run
			
			var ws = this;
			var headerDataCookie = this.getWeatherDataCookie(locationId);
			if(headerDataCookie) {
				var headerDataObj = this.createObjFromJSON(headerDataCookie);
				var wdObj = headerDataObj.weatherData;
				
				// location
				var form = widgetElm.children("form");
				form.find("label, p.state").html(wdObj.cityName + ((wdObj.stateShort.length>0) ? ", " + wdObj.stateShort : "")).unbind("click").click(function(){
					ws.gotoYourWeatherPage(locationId,wdObj.cityName,wdObj.stateShort);
				});
				
				// days
				var listElm = widgetElm.children("ul:first");
				
				listElm.children("li.section").each(function(i){
					var item = $(this);
					
					if (wdObj.days[i]) {
						
						var dataSource = wdObj.days[i];
						var imgSrc = ws.PATH_WEATHER_ICON_IMAGE+dataSource.image+ws.WEATHER_ICON_IMAGE_EXT;
						var hiTemp = dataSource.hi;
						var lowTemp = dataSource.lo;

						// today - current temp
						if (i===0) { item.find(".current").html(wdObj.currentTemp+"&#176;"); }
						
						item.find(".media").html("<img />");
						item.find(".media img").each(function(){
							var elm = $(this);
							elm.css({ opacity:"0", display:"inline" });
							if (!$.browser.opera) {
								var img = new Image();
								img.onload = function() { 
									elm.attr("src",imgSrc); 
									animateFn.show(elm,{ speed:300,type:"swing" });
								}
								img.src = imgSrc;
							} else {
								elm.css({ backgroundImage:"none", display:"inline", opacity:"0" });
								elm.attr({ src:imgSrc });
								animateFn.show(elm,{ speed:300,type:"linear" });
							}
						});
						
						var hiLow = "Hi " + ((hiTemp.length>0) ? hiTemp + "&#176;" : "-") + " | Lo " + ((lowTemp.length>0) ? lowTemp + "&#176;" : "-") ;
						item.children("div:first").children("span").html(hiLow);
						item.unbind("click").click(function(){ ws.gotoYourWeatherPage(locationId,wdObj.cityName,wdObj.stateShort); });
					}
					
				});
				
			}
			
			// search portion
			widgetElm.children("form").each(function(){
				var form = $(this);
				var search = form.find("#weather_widgetInput");
				var txt = search.attr("value");
				
				search.focus(function(){
					var elm = $(this);
					var val = elm.attr("value");
					elm.attr("value",(val===txt)?"":val);	
				}).blur(function(){
					var elm = $(this);
					var val = elm.attr("value");
					elm.attr("value",(val==="")?txt:val);
				});
				
				// on submit
				form.submit(function(){
					ws.locationSearchAction(this,"#weather_widgetInput", ws.UPDATE_HEADER_ACTION);
					return false;
				});
				
			});
			
		},
		
		searchFieldOnFocus: function(inputField) {
			if(inputField.value==this.TXT_DEFAULT_SEARCH_FIELD || inputField.value==this.TXT_HEADER_SEARCH_FIELD){
				inputField.value='';
			}
		},
		searchFieldOnBlur: function(inputField, message) {
			if(inputField.value==''){
				inputField.value=message;
			}
		},
		airportSearchAction: function(airportForm, airportLocId) {
			var ws = this;
			var airportCodeEx=new RegExp('^[a-zA-Z]{3,4}$');
			var requestedAirportCode=$('#airportCode').attr('value');
			if(airportCodeEx.exec(requestedAirportCode)){
				$.ajax({
					type:"GET",
					async:false,
					url:ws.SERVICE_URL_AIRPORT_LOCATION_JSON,
					data:({airportCode:requestedAirportCode}),
					success:function(data,textStatus){
						var locationJson = ws.createObjFromJSON(data);
						if(locationJson){
							if(locationJson.location.length>0){
								var airportLocObj = ws.createLocJSObj(locationJson.location[0]);
								$(airportForm).attr('action',ws.PAGE_URL_YOUR_WEATHER);
								$(airportLocId).attr('value',airportLocObj.loc_id);
							}else{
								showToConsole(ws.MSG_AIRPORT_NOT_FOUND_ERROR);
								return false;
							}
						}else{
							showToConsole(ws.MSG_DEFAULT_ERROR);
							return false;
						}
					}
				});
			}else{
				showToConsole(ws.MSG_AIRPORT_CODE_ERROR);
				return false;
			}
		},
		locationSearchAction: function(locationForm,searchInputId,action) {
			var ws = this;
			var locIdEx=new RegExp('^[a-zA-Z]{4}[0-9]{4}$');
			var upToCommaEx=new RegExp('[^,]*');

			if(locIdEx.exec(this.currentLocId)){
				$(locationForm).attr('action',this.PAGE_URL_YOUR_WEATHER);
				$(searchInputId).attr('name','locId');
				$(searchInputId).attr('value',this.currentLocId);
				if(action==this.UPDATE_HEADER_ACTION){
					this.fetchLocationJSON(this.currentLocId);
					this.renderWidget();
				}
			}else{ // including zip code (should return exact match)
				var locSearchTerm=upToCommaEx.exec($(searchInputId).attr('value'));
				
				ws.location_search_callback = function(data){
					$(locationForm).attr('action',ws.PAGE_URL_SEARCH_RESULTS);
					var locationJson = ws.createObjFromJSON(data);

					if(locationJson){
						if(locationJson.location.length==1){
							var exactMatchObj = ws.createLocJSObj(locationJson.location[0]);
							if(action==ws.UPDATE_HEADER_ACTION){
								ws.fetchLocationJSON(exactMatchObj.loc_id);
								ws.renderWidget();
							}else{
								$(locationForm).attr('action',ws.PAGE_URL_YOUR_WEATHER);
								$(searchInputId).attr('name','locId');
								$(searchInputId).attr('value',exactMatchObj.loc_id);
							}
						}else{
							var qStr = "?" + $(searchInputId).attr("name") + "=" + locSearchTerm[0].replace(/\s+/g,"+");
							window.location.href = ws.PAGE_URL_SEARCH_RESULTS + qStr;
							//$(searchInputId).attr('value',locSearchTerm);
							//$(locationForm).submit();
						}
					}else{
						var qStr = "?" + $(searchInputId).attr("name") + "=" + locSearchTerm[0].replace(/\s+/g,"+");
						window.location.href = ws.PAGE_URL_SEARCH_RESULTS + qStr;
						//$(searchInputId).attr('value',locSearchTerm);
						//$(locationForm).submit();
					}
				}
				
				$.ajax({
					url:ws.SERVICE_URL_LOCATION_JSONP+'weather_service.location_search_callback&jsonp=?',
					dataType:'jsonp',
					async:false,
					data:({zipOrLocation:locSearchTerm})
				});	
			}
		},
		gotoYourWeatherPage: function(locId,city,state,country) {
			var ws = this;
			var yourWeatherURL = ws.PAGE_URL_YOUR_WEATHER+'?locId='+locId;
			window.location.href=yourWeatherURL;
		},
		renderYourWeather: function(divClass,locationId,weatherView) {
			var ws = this;
			if(locationId==null || locationId==''){
				locationId = ws.WEATHER_DEFAULT_LOCATION;
			}
			$.get(
				ws.SERVICE_URL_WEATHER_HTML,
				{locationId:locationId,weatherView:weatherView},
				function(data){
					$(divClass).html(data);
				}
			);
		},
		renderSearchResults: function(divClass,searchTerm) {
			var ws = this;
			
			ws.location_searchresults_callback = function(data) {
				var searchResultsHtml='';
				if(data){
					var locationJson=ws.createObjFromJSON(data);
					if(locationJson){
						if(locationJson.location.length>1){
							searchResultsHtml+='<p class="results">Your search for <span class="city">'
							+ searchTerm+'</span> returned multiple locations</p>'
							+ '<p>Please click on the location you are looking for:</p>'
							+ '<p class="sorted">Sorted alphabetically by city, state, country<span/></p>'
							+ '<table>';
							for(var resultsIndex=0; resultsIndex<locationJson.location.length; resultsIndex++){
								var matchingLocObj = ws.createLocJSObj(locationJson.location[resultsIndex]);
								var locationLink = ws.PAGE_URL_YOUR_WEATHER+'?locId='+matchingLocObj.loc_id;					
								if(resultsIndex%2==0){searchResultsHtml+='<tr class="even">';}
								else {searchResultsHtml+='<tr>';}							
								searchResultsHtml+='<td><a href="'+locationLink+'">'+matchingLocObj.city+'</a></td>'
								+ '<td>'+matchingLocObj.state+'</td><td>'+matchingLocObj.country+'</td></tr>';
							}
							searchResultsHtml+='</table>';
						} else if(locationJson.location.length==1){
							var locObj=ws.createLocJSObj(locationJson.location[0]);
							ws.gotoYourWeatherPage(locObj.loc_id,locObj.city,locObj.state,locObj.country);
						}else{
							// display no matches
							searchResultsHtml += '<p class="results">Sorry there are no results for <span class="city">'
							+ searchTerm+'</span></p>'
							+ '<p>Please try another search</p>';
						}
					}else{
						searchResultsHtml += ws.MSG_DEFAULT_ERROR;
					}
				}else{
					searchResultsHtml += ws.MSG_DEFAULT_ERROR;
				}
				$(divClass).html(searchResultsHtml);
			}
			
			$.ajax({
				url:weather_service.SERVICE_URL_LOCATION_JSONP+'weather_service.location_searchresults_callback&jsonp=?',
				dataType:'jsonp',
				async:false,
				data:({zipOrLocation:searchTerm})
			});
		},
		renderAirportDelayInfo: function(divClass, airportCode) {
			var ws = this;
			var aCode=airportCode.toUpperCase();
			$.get(
				ws.SERVICE_URL_AIRPORT_DELAY_JSON,
				{airportCode:aCode},
				function(data){
					if(data){
						var delayJson=ws.createObjFromJSON(data);
						var airportDelayHtml='';
						if(delayJson.delayList.length){						
							airportDelayHtml+='<div class="delay">';
							for(var delayIndex=0; delayIndex<delayJson.delayList.length; delayIndex++){
								var delayInfo=delayJson.delayList[delayIndex].delayInfo;
								var delayReason=delayJson.delayList[delayIndex].delayReason;
								airportDelayHtml+='<p><img alt="Exclamation" src="/static/all/img/exclamation.png"/>Delay Information</p>'
								+ '<p class="information">'+delayInfo+'</p>'
								+ '<p class="reason">Delay Reason</p><p class="information">'+delayReason+'</p>';
							}
							airportDelayHtml += '</div>';						
						} else {
							airportDelayHtml+='<div class="no-delay">'
								+'<p><img alt="Exclamation" src="/static/all/img/exclamation.png"/>Delay Information</p>'
								+ '<p class="information">No delays at '+aCode+ ' reported.</p></div>';
						}
						$(divClass).html(airportDelayHtml);
					}
				}
			);
		},
		renderMarineForecast: function(divClass) {
			var ws = this;
			var marineHtml='';
			marineHtml+='<div class="marine-search"><h1><strong>Marine</strong> Forecast</h1>'
				+'<div class="location"><form action="" class="form_1" id="search_form4" method="get">'
				+'<div id="marine-state-dropdown" class="list-type-dropdown"><p class="text title">Choose Your State</p>'
				+'<div class="drop_container holder">'
				+'<input type="hidden" name="marineState" value="" /><p class="weather_dropdown current-value">- select -</p>'
				+'<ul>'+this.generateStatesList(this.LIST_MARINE_FORECAST_ENABLED)+'</ul></div><div class="clear"></div></div>'
				+'<div id="marine-cities-dropdown" class="list-type-dropdown"><p class="text title">Choose Marine Location</p>'
				+'<div class="drop_container holder"><input type="hidden" name="marineLocation" value="" />'
				+'<p id="tide-cities-p" class="weather_dropdown current-value disabled">- select -</p>'
				+'<ul id="tide-cities-list"><li></li></ul></div><div class="clear"></div></div>'
				+'</form></div></div>'
				+'<div class="marine-result"></div>';

			$(divClass).html(marineHtml);
			$('.weather .marine-result').each(function(){
				ws.renderYourWeather(this,'USCA0638','weather_marine');
			});
		},
		renderTideLocationsDropdown: function(stateAbbr) {
			var ws = this;
			if(stateAbbr!=""){
				$.ajax({
					type:"GET",
					async:false,
					url:ws.SERVICE_URL_MARINE_JSON,
					data:({state:stateAbbr}),
					success:function(data,textStatus){
						var marineLocJson = ws.createObjFromJSON(data);
						if(marineLocJson){
							if(marineLocJson.marineLocations.length>0){
								ws.generateTideLocationsList(marineLocJson.marineLocations);
							}else{
								showToConsole(ws.MSG_DEFAULT_ERROR);
								return false;
							}
						}else{
							showToConsole(ws.MSG_DEFAULT_ERROR);
							return false;
						}
					}
				});
			}
		},
		generateTideLocationsList: function(locArrayObj) {
			var tideLocDropdown='';
			if(locArrayObj != null){
				for(var i=0; i<locArrayObj.length; i++){
					var marineLocId=locArrayObj[i].loc_id;
					var marineCity=locArrayObj[i].city;
					tideLocDropdown += '<li lang="'+marineLocId+'">'+marineCity+'</li>';
				}
				$('#tide-cities-list').html(tideLocDropdown);
				$('#tide-cities-p').removeClass("disabled");
			}else{
				tideLocDropdown += '<li lang=""></li>';
				$('#tide-cities-list').html(tideLocDropdown);
				$('#tide-cities-p').html('- select -');
				$('#tide-cities-p').addClass("disabled");
			}
			fox.site.channel.listDropdown.init();
		},
		generateStatesList: function(statesList){
			var listHtml='';
			var states=statesList.split("|");
			for(var i=0; i<states.length; i++){
				listHtml += '<li lang="'+states[i]+'">'+eval('weather_service.us_states.'+states[i])+'</li>';
			}
			return listHtml;
		},
		renderSkiConditions: function(divClass) {
			var ws = this;
			var skiHtml='';	
			skiHtml+='<div class="ski-search"><h1><strong>Ski</strong> Conditions</h1>'
				+'<div class="location"><form action="" class="form_1" id="search_form5" method="get">'
				+'<div id="ski-state-dropdown" class="list-type-dropdown"><p class="text title">Choose Your State</p>'
				+'<div class="drop_container holder">'
				+'<input type="hidden" name="skiState" value="" /><p class="weather_dropdown current-value">- select -</p>'
				+'<ul>'+this.generateStatesList(this.LIST_SKI_CONDITIONS_ENABLED)+'</ul></div><div class="clear"></div></div>'
				+'<div id="ski-resorts-dropdown" class="list-type-dropdown"><p class="text title">Choose Ski Location</p>'
				+'<div class="drop_container holder"><input type="hidden" name="skiLocation" value="" />'
				+'<p id="ski-resorts-p" class="weather_dropdown current-value disabled">- select -</p>'
				+'<ul id="ski-resorts-list"><li></li></ul></div><div class="clear"></div></div>'
				+'</form></div></div>'
				+'<div class="ski-result"></div>';

			$(divClass).html(skiHtml);
			$('.weather .ski-result').each(function(){
				ws.renderSkiCondResult(this, '303003');
			});
		},
		renderSkiResortsDropdown: function(stateAbbr) {
			var ws = this;
			if(stateAbbr!=""){
				$.ajax({
					type:"GET",
					async:false,
					url:ws.SERVICE_URL_SKI_RESORTS_JSON,
					data:({state:stateAbbr}),
					success:function(data,textStatus){
						var skiResortJson = ws.createObjFromJSON(data);
						if(skiResortJson){
							if(skiResortJson.skiResorts!=null && skiResortJson.skiResorts.length>0){
								ws.generateSkiResortsList(skiResortJson.skiResorts);
							}else{
								return false;
							}
						}else{
							showToConsole(ws.MSG_DEFAULT_ERROR);
							return false;
						}
					}
				});
			}
		},
		generateSkiResortsList: function(locArrayObj) {
			var skiResortsDropdown='';
			if(locArrayObj != null){
				for(var i=0; i<locArrayObj.length; i++){
					var skiResortId=locArrayObj[i].resort_id;
					var skiResortName=locArrayObj[i].resort_name;
					skiResortsDropdown += '<li lang="'+skiResortId+'">'+skiResortName+'</li>';
				}
				$('#ski-resorts-list').html(skiResortsDropdown);
				$('#ski-resorts-p').removeClass("disabled");
			}else{
				skiResortsDropdown += '<li lang=""></li>';
				$('#ski-resorts-list').html(skiResortsDropdown);
				$('#ski-resorts-p').html('- select -');
				$('#ski-resorts-p').addClass("disabled");
			}
			fox.site.channel.listDropdown.init();
		},
		renderSkiCondResult: function(divClass,resortId){
			var ws = this;
			$.get(
				ws.SERVICE_URL_SKI_COND_JSON,
				{resortId:resortId},
				function(data){
					var skiCondJson=ws.createObjFromJSON(data);
					var skiResultHtml=ws.generateSkiCondHtml(skiCondJson);
					$(divClass).html(skiResultHtml);
				}
			);
		},
		generateSkiCondHtml: function(skiCondJson){
			var skiObj=skiCondJson.skiConditions;
			var snowMin=skiObj.new_snow_min?skiObj.new_snow_min+' in':'N/A';
			var surface=skiObj.surface_condition?skiObj.surface_condition:'N/A';
			var lastSnowfall=skiObj.last_snow?skiObj.last_snow+'in':'N/A';
			var prevSnowfall=skiObj.prev_snow?skiObj.prev_snow:'N/A';
			var baseDepth = 'N/A';
			if (skiObj.base_min && skiObj.base_max){
				baseDepth = skiObj.base_min + ' of ' + skiObj.base_max;
			}			
			var skiHtml='<ul><li><p class="city-name">'+skiObj.resort_name+', '+skiObj.state+' Ski Report'
				+'</p><div class="four-half"><table><tbody>'
				+'<tr><td class="first">Operating Status</td><td>'+skiObj.open_status+'</td></tr>'
				+'<tr class="even"><td class="first">Surface</td><td>'+surface+'</td></tr>'
				+'<tr><td class="first">Temperature</td><td>'+skiObj.temperature+' F</td></tr>'
				+'<tr class="even"><td class="first">New Snowfall</td><td>'+snowMin+'</td></tr>'
				+'<tr><td class="first">Last Snowfall</td><td>'+lastSnowfall+'</td></tr>'
				+'<tr class="even"><td class="first">Previous Snowfall</td><td>'+prevSnowfall+'</td></tr>'
				+'<tr><td class="first">Lifts Open</td><td>'+skiObj.lifts_open+' of '+skiObj.lifts_total+'</td></tr>'
				+'<tr class="even"><td class="first">Trails Open</td><td>'+skiObj.trails_open+' of '+skiObj.trails_total+'</td></tr>'
				+'<tr><td class="first">Grooming Trails</td><td>'+skiObj.groom_trails+' of '+skiObj.trails_total+'</td></tr>'
				+'<tr class="even"><td class="first">Base Depth</td><td>'+baseDepth+'</td></tr>'
				+'<tr><td class="first">Terrain</td><td>'+skiObj.terrain+'&#37;</td></tr>'
				+'<tr class="even"><td class="first">Snow Made</td><td>'+skiObj.snow_made+'&#37;</td></tr>'
				+'<tr><td>&nbsp;</td><td>&nbsp;</td></tr>'
				+'<tr class="even"><td>&nbsp;</td><td>&nbsp;</td></tr>'
				+'<tr><td>&nbsp;</td><td>&nbsp;</td></tr></tbody></table></div></li></ul>';
			return skiHtml;
		},
		addToFavorites: function(locId) {
			if(this.weatherCookie('weatherfavlocs')){
				var favLocationArray=(this.weatherCookie('weatherfavlocs')).split('|');

				// Make sure that there are less than 5 favorites
				if(favLocationArray.length < 5){
					// loop to see if it's in favorite already
					var saveFavorite=true;
					for(var favIndex=0; favIndex<favLocationArray.length; favIndex++){
						if(favLocationArray[favIndex]==locId){
							alert('You have already saved this location to My Favorites.');
							saveFavorite=false;
						}
					}
					if(saveFavorite){
						favLocationArray[favLocationArray.length]=locId;
						this.createWeatherFavoritesCookie(favLocationArray);
						this.renderFavorites();
					}
				}else{
					alert('You have 5 saved locations in My Favorites. Please remove one from My Favorites to save this location.');
				}

			}else{
				var newFavCookieArray=new Array(locId);
				this.createWeatherFavoritesCookie(newFavCookieArray);
				this.renderFavorites();
			}
		},
		removeFavorite: function(locId) {
			if(this.weatherCookie('weatherfavlocs')){
				var favLocationArray=(this.weatherCookie('weatherfavlocs')).split('|');
				for(var favIndex=0; favIndex<favLocationArray.length; favIndex++){
					if(favLocationArray[favIndex] == locId){
						favLocationArray.splice(favIndex, 1);
						break;
					}
				}
				this.createWeatherFavoritesCookie(favLocationArray);
			}
			$("#li_"+locId).remove();
		},
		generateFavoritesHtml: function(weatherObj) {
			var dataObj=weatherObj.weatherData;
			var stateToDisplay=dataObj.stateShort;
			if(dataObj.locationId.substr(0,2)!='US'){stateToDisplay=dataObj.country};
			var favHtml='<li id="li_'+dataObj.locationId+'"><div class="weather-img">'
				+'<a href="#" onclick="weather_service.gotoYourWeatherPage(\''+dataObj.locationId+'\',\''
				+dataObj.cityName+'\',\''+dataObj.stateShort+'\',\''+dataObj.country+'\');return false;">'
				+'<img alt="Weather" src="'+weather_service.PATH_WEATHER_ICON_IMAGE+dataObj.days[0].image+weather_service.WEATHER_ICON_IMAGE_EXT+'"/></a>'
				+'</div><div class="city-container"><h3>'+unescape(dataObj.cityName)+', '+stateToDisplay+'</h3>'
				+'<div class="desc"><p>'+dataObj.forecast+'</p></div></div><div class="degrees">'
				+'<p>'+dataObj.days[0].hi+'&deg;&nbsp;<span class="low">'+dataObj.days[0].lo+'&deg;</span></p>'
				+'<a class="close" href="#" onclick="weather_service.removeFavorite(\''+dataObj.locationId+'\');return false;"/></div></li>';
			return favHtml;
		},
		renderFavorites: function() {
			var ws = this;
			var favoritesHtml='<ul>';
			if(this.weatherCookie('weatherfavlocs')){
				var favLocationArray=(this.weatherCookie('weatherfavlocs')).split('|');

				favoritesHtml += '<li><ul class="favorites">';
				for(var favIndex=0; favIndex<favLocationArray.length; favIndex++){
					// see if data is already in cookie
					var favLocationId=favLocationArray[favIndex];
					var favDataCookie = this.getWeatherDataCookie(favLocationId);

					if(favDataCookie){
						// get the cookie content and turn the json into object
						var favDataObj = this.createObjFromJSON(favDataCookie);
						favoritesHtml += this.generateFavoritesHtml(favDataObj);
					}else{
						// otherwise fetch it and save in cookie
						ws.location_renderfavorites_callback = function(data) {
							if(data){
								ws.createWeatherDataCookie(favLocationId,data);
								var favDataObj = ws.createObjFromJSON(data);
								favoritesHtml += ws.generateFavoritesHtml(favDataObj);
				            }
						}
						
						$.ajax({
							url:weather_service.SERVICE_URL_WEATHER_JSONP+'weather_service.location_renderfavorites_callback&jsonp=?',
							dataType:'jsonp',
							async:false,
							data:({locationId:favLocationId})
						});
					}
				}
				favoritesHtml+='</ul></li>'
				+ '<li>&nbsp;</li>';
			}
			favoritesHtml+=this.generateRightRailSearch('myfav')
			+ '</ul>';

			var favoritesObj = $(".component .weather.favorites");
			favoritesObj.html(favoritesHtml);
			$('ul.favorites li:last-child',favoritesObj).addClass("last");	
			$('input[class*=weather_city_autocomplete_myfav]',favoritesObj).keyup(function(){
		        ws.weatherCityAutoComplete(this);
		    });
		},
		renderAirportSearch: function() {
			var airportHtml='<div class="weather airport-search"><h1><strong>Airport</strong> Delays</h1><ul><li><div>'
				+'<span class="text">'+this.TXT_AIRPORT_SEARCH_FIELD+'</span>'
				+'<form method="get" id="aiport_search" class="form_1" action=""'
				+' onsubmit="return weather_service.airportSearchAction(this,\'#airportLocId\');">'
				+'<fieldset class="text_box">'
				+'<input type="text" id="airportCode" class="airport_input no_airport" size="50" value="" name="airportCode"/>'
				+'<input type="hidden" id="airportLocId" value="" name="locId"/>'
				+'<input type="submit" class="airport" value=""/>'
				+'</fieldset></form></div></li></ul></div>';
			document.write(airportHtml);
		},
		fetchAndSaveWeatherJSONCookie: function(locId){
			var ws=this;
			
			ws.location_weatherjsoncookie_callback = function(data) {
				ws.createWeatherDataCookie(locId,data);
				ws.renderWidget();
			}
			$.ajax({
				url:ws.SERVICE_URL_WEATHER_JSONP+'weather_service.location_weatherjsoncookie_callback&jsonp=?',
				dataType:'jsonp',
				async:false,
				data:({locationId:locId})
			});	
		},
		fetchLocationJSON: function(locId){
			var ws = this;
			ws.location_fetchlocationjson_callback=function(data){
				ws.createWeatherLocationCookie(data);
			}
			$.ajax({
				url:ws.SERVICE_URL_LOCATION_JSONP+'weather_service.location_fetchlocationjson_callback&jsonp=?',
				dataType:'jsonp',
				async:false,
				data:({zipOrLocation:locId})
			});
		},
		fetchLocationRegionCode: function(locId) {
			var ws = this;
			var regionCode='';
			$.ajax({
				type:"GET",
				async:false,
				url:ws.SERVICE_URL_LOCATION_REGION_JSON, 
				data:({locationId:locId}),
				success:function(data,textStatus){
					if(data){
						var regionJson=ws.createObjFromJSON(data);
						regionCode=regionJson.regionCode;
		            }
				},
				error:function(XMLHttpRequest,textStatus,errorThrown){  
					showToConsole(ws.MSG_DEFAULT_ERROR);
				}
			});
			return regionCode;
		},
		weatherCityAutoComplete: function(element) {
			var ws = this;
			//this.currentLocId="";
		   	this.currentCityString="";
		   	var ACElementId = $(element).attr("id");
			var preloaderObj = $(".preloader-img:first",$(element).parent()); 
			
		    var inputValue = $.trim($(element).val());
		    if(inputValue.length<1 ){
		        return;
		    }
		    var cssValue = $(element).attr('class');
		    var prefix='weather_city_autocomplete_';
		    var tempIndex=cssValue.indexOf(prefix)+prefix.length;
		    var appendix=cssValue.substr(tempIndex);
		    tempIndex=appendix.indexOf(' ');
		    if(tempIndex>0){
		    	appendix=appendix.substr(0, tempIndex);
		    }

			var initial=inputValue.charAt(0).toLowerCase();
			if (initial.match(/[a-z]/)==null) { // match a-z characters only
				return;
			}

			if (typeof ws.weatherCityData[initial]==='undefined') { // check if object data exists
				
				if (typeof ClientStorageManager==='undefined') { // check if CSM exists
					triggerAjaxCall(false); 
				} else {
					var restrict = ['cookie','msiestorage']; // do not use these storage mechanisms
					if (restrict.join('|').indexOf(ClientStorageManager.type.toLowerCase())===-1) {
						// Using Client Storage Manager
						ClientStorageManager.getItem(prefix+initial,function(storageData){
							if (storageData) {
								var cities = eval(storageData);
								ws.weatherCityData[initial] = cities;
								triggerAutoComplete(cities);
							} else {
								triggerAjaxCall(true);
							}
						});
					} else {
						triggerAjaxCall(false);
					}
				}

			} else {
				triggerAutoComplete(ws.weatherCityData[initial]);
			}

			function triggerAjaxCall(useCSM) {

				if (typeof ws.ACListener[ACElementId]==='undefined') {
					ws.ACListener[ACElementId] = { // initialize listener
						init: false,
						fchar: initial,
						ajaxReady: true
					}
				} 

				if (!ws.ACListener[ACElementId].ajaxReady) { return; } // cancel a request if there is still an ajax call ongoing

				ws.ACListener[ACElementId].ajaxReady = false; // initialize to not ready in preparation for an ajax call
				$(preloaderObj).show(); // show preloader img
				$.ajax({
		            type: "GET",
		            url: ws.SERVICE_URL_CITY_DATA,
		            data: ({ cityNameInitial:initial }),
		            dataType: "text",
		            success: function(data,textStatus) {
						if (data) {

							var cities = data.replace(/var cities\=/,"");
							if (useCSM) {
								ClientStorageManager.saveItem({
									key: prefix+initial,
									value: cities,
									expire: 7, // 7 day - test
									success: function(returnObj) {
										ws.weatherCityData[initial] = eval(returnObj.value);
									},
									error: function(err) { 
										// fail silently
									}
								});
								if (typeof ws.weatherCityData[initial]==='undefined') {
									ws.weatherCityData[initial] = eval(cities);
								}
								triggerAutoComplete(ws.weatherCityData[initial]);

							} else {
								ws.weatherCityData[initial] = eval(cities);
								triggerAutoComplete(ws.weatherCityData[initial]);
							}

							$(preloaderObj).fadeOut("slow"); // hide preloader
							ws.ACListener[ACElementId].ajaxReady = true; // sucess

						} else {

							$(preloaderObj).hide(); // hide preloader
							ws.ACListener[ACElementId].ajaxReady = true; // ajax call finished
							showToConsole(ws.MSG_DEFAULT_ERROR);
						}

					},
					error: function(XMLHttpRequest,textStatus,errorThrown) {

						$(preloaderObj).hide(); // hide preloader
						ws.ACListener[ACElementId].ajaxReady = true; // ajax call finished
		                showToConsole(ws.MSG_DEFAULT_ERROR);

		            }
				});
			}

			function triggerAutoComplete(cities) {

				if (typeof ws.ACListener[ACElementId]==='undefined') {
					ws.ACListener[ACElementId] = { // initialize listener
						init: false,
						fchar: initial,
						ajaxReady: true
					}
				} else {
					if (ws.ACListener[ACElementId].fchar !== initial) { // if initial not same as stored
						ws.ACListener[ACElementId].init = false;
					}
				}

				if (!ws.ACListener[ACElementId].init) { // apply autocomplete trigger
					$(element).autocomplete(cities,{
						max:20,
						width: 193,
						selectFirst: false,					
		            	resultsClass: 'ac_results ac_results_'+appendix,
		                formatItem: function(item) { return item.t; }
		            }).result(function(event, item){
						ws.currentLocId = item.id;
						ws.currentCityString = item.t;
					});
					// set initial true state
					ws.ACListener[ACElementId].init = true;
					ws.ACListener[ACElementId].fchar = initial;
				}

			}
		},
		renderHeaderLocationChange: function() {
			var ws = this;
			var htmlArr = [];
			var formTimeout = false;
			
			htmlArr.push('<div id="weather_loc_change" style="display:none;"><form action="#" method="get"><fieldset><legend>Weather</legend>');
			htmlArr.push('<input id="weather_headInput" type="text" name="searchTerm" value="Enter city or zip" />&nbsp;<input type="submit" name="submit" class="btn-smll" value="Go" />');
			htmlArr.push('</fieldset></form></div>');
			var htmlElm = $(htmlArr.join(""));
			
			htmlElm.find("form").each(function(){
				var elm = $(this);
				var search = elm.find("#weather_headInput");
				
				search.focus(function(){
					ws.searchFieldOnFocus(this);
					clearTimeout(formTimeout);
				}).blur(function(){
					var elm = $(this);
					var val = elm.attr("value");
					elm.attr("value",(val==="")?ws.TXT_HEADER_SEARCH_FIELD:val);
					formTimeout = setTimeout(function(){ ws.changeHeaderLocation(); },3000);
				});
				
				// on submit
				elm.submit(function(){
					clearTimeout(formTimeout);
					ws.locationSearchAction(elm,"#weather_headInput", ws.UPDATE_HEADER_ACTION);
					return false;
				});
			});
			
			$('.weather-search, #weather_corner').append(htmlElm);
		},
		changeHeaderLocation: function(){
			var info = $("#weather_cur_loc");
			var search = $("#weather_loc_change");
			
			if (info.is(":hidden")) {
				search.css("display","none");
				info.css({ opacity:"0",display:"inline" });
				animateFn.show(info,{ speed:300,type:"swing" });
			} else {
				info.css("display","none");
				search.css({ opacity:"0",display:"inline" });
				animateFn.show(search,{ speed:300,type:"swing" });
			}
			
		},
		
		// Display cookie data in header
		headerDisplayAction: function(locationId) {
			var ws = this;
			var headerDataCookie = this.getWeatherDataCookie(locationId);
			if(headerDataCookie){
				var headerDataObj = this.createObjFromJSON(headerDataCookie);
				var wdObj=headerDataObj.weatherData;
				
				var htmlArr = [];
				var imgSrc = this.PATH_WEATHER_ICON_IMAGE+wdObj.days[0].image+this.WEATHER_ICON_IMAGE_EXT;
				htmlArr.push('<div id="weather_cur_loc"><p><a href="#">Forecast:</a> <span class="degree">'+ wdObj.currentTemp + '&#176; <img src="http://www.foxnews.com/static/all/img/clear.gif" style="width:24px; height:24px;"/></span> ');
				htmlArr.push('<span class="location">'+wdObj.cityName+'</span> <a href="#">edit</a></p></div>');
				var htmlElm = $(htmlArr.join(""));

				// image preloader for smooth transition
				htmlElm.find("span.degree img").each(function(){
					var elm = $(this);
					elm.css({ opacity:"0", display:"inline" });
					if (wdObj.days[0].image.length===0) { 
						elm.css("display","none"); 
					} else {
						if (!$.browser.opera) {
							var img = new Image();
							img.onload = function() { 
								elm.attr("src",imgSrc); 
								animateFn.show(elm,{ speed:300,type:"swing" });
							}
							img.src = imgSrc;
						} else {
							elm.css({ backgroundImage:"none", display:"inline", opacity:"0" });
							elm.attr({ src:imgSrc });
							animateFn.show(elm,{ speed:300,type:"linear" });
						}
					}
				});
				
				htmlElm.find("a").each(function(){
					var aTag = $(this);
					var text = aTag.text().toLowerCase();
					
					aTag.click(function(){
						// forecast link
						if (text.indexOf("forecast")>-1) { ws.gotoYourWeatherPage(locationId,wdObj.cityName,wdObj.stateShort); }
						
						// edit link
						if (text.indexOf("edit")>-1) { ws.changeHeaderLocation(); }
						
						return false;
					});
				});
				
				$(".weather-search, #weather_corner").html(htmlElm);
				
			}
		}
	};

	function showToConsole(str){
		if(typeof(console)==='object'){console.log(str);}
	}
	
	WeatherService.prototype.us_states = {
		'AK':'Alaska',
		'AL':'Alabama',
		'AR':'Arkansas',
		'AS':'American Samoa',
		'AZ':'Arizona',
		'CA':'California',
		'CO':'Colorado',
		'CT':'Connecticut',
		'DC':'District of Columbia',
		'DE':'Delaware',
		'FL':'Florida',
		'FM':'Federated States of Micronesia',
		'GA':'Georgia',
		'GU':'Guam',
		'HI':'Hawaii',
		'IA':'Iowa',
		'ID':'Idaho',
		'IL':'Illinois',
		'IN':'Indiana',
		'KS':'Kansas',
		'KY':'Kentucky',
		'LA':'Louisiana',
		'MA':'Massachusetts',
		'MD':'Maryland',
		'ME':'Maine',
		'MH':'Marshall Islands',
		'MI':'Michigan',
		'MN':'Minnesota',
		'MO':'Missouri',
		'MP':'Northern Mariana Islands',
		'MS':'Mississippi',
		'MT':'Montana',
		'NC':'North Carolina',
		'ND':'North Dakota',
		'NE':'Nebraska',
		'NH':'New Hampshire',
		'NJ':'New Jersey',
		'NM':'New Mexico',
		'NV':'Nevada',
		'NY':'New York',
		'OH':'Ohio',
		'OK':'Oklahoma',
		'OR':'Oregon',
		'PA':'Pennsylvania',
		'PR':'Puerto Rico',
		'PW':'Palau',
		'RI':'Rhode Island',
		'SC':'South Carolina',
		'SD':'South Dakota',
		'TN':'Tennessee',
		'TX':'Texas',
		'UT':'Utah',
		'VA':'Virginia',
		'VI':'Virgin Islands',
		'VT':'Vermont',
		'WA':'Washington',
		'WI':'Wisconsin',
		'WV':'West Virginia',
		'WY':'Wyoming'
	}

	var weather_service = window.weather_service = new WeatherService();
	
	var animateFn = {
		isIE: function() {
			return $.browser.msie;
		},
		animate: function(elm,cfg) {
			if (!this.isIE()) {
				if (!cfg.speed) { cfg.speed = 300; }
				if (!cfg.type) { cfg.type = "swing"; }

				elm.animate(cfg.css,cfg.speed,cfg.type,function(){
					if (cfg.callback) { cfg.callback(); }
				});
			} else {
				elm.css(cfg.css);
				elm.css("opacity","");
				if (cfg.callback) { cfg.callback(); }
			}
		},
		hide: function(elm,cfg) {
			if (!elm) { return false; }
			var cssObj = { opacity:"0" };
			if (cfg.css) {
				for (i in cfg.css) { cssObj[i] = cfg.css[i]; }
			}
			cfg.css = cssObj;
			this.animate(elm,cfg);
		},
		show: function(elm,cfg) {
			if (!elm) { return false; }
			var cssObj = { opacity:"1" };
			if (cfg.css) {
				for (i in cfg.css) { cssObj[i] = cfg.css[i]; }
			}
			cfg.css = cssObj;
			this.animate(elm,cfg);
		}
	};
	
})(jQuery);
