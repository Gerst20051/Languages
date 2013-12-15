/*! jQuery Countdown Plugin
* Copyright Tom Ellis http://www.webmuse.co.uk
* Licensed under MIT License
*/
(function($) {
	
	jQuery.fn.countdown = function(options) {   
		var defaults = {
			date: "December 11, 2010",
			updateTime:1000,
			minus: false
		};
	  
	  	  
		var options = $.extend({}, defaults, options);
		
		return this.each(function() {
		  
			CountTime(this,options.date, options.updateTime, options.minus);
			
		});
	};  

})(jQuery);

	//var CountTime = this;
	CountTime = function(elem,date, time, minus)
	{	
		return new CountTime.fn.init(elem,date,time, minus);
	}
	
  
	CountTime.fn = CountTime.prototype = {
		  
		CountTime: "1.1",
		countDownDate: "",
		countDownInterval: 0,
		countDownBox : null,
		cancel : false,
		intval: null,
		minus: false,
		
		init: function(elem, date, time, minus)
		{
			CountTime.fn.countDownBox = $(elem);
			CountTime.fn.countDownDate = date;
			CountTime.fn.countDownInterval = time;
			CountTime.fn.minus = minus;

			CountTime.fn.intval = setInterval(CountTime.fn.update, CountTime.fn.countDownInterval);
	
		},
		update : function(time, minus){
		

				TodaysDate = new Date();
				
				CountdownDate = new Date( CountTime.fn.countDownDate );
											
				msPerDay = 24 * 60 * 60 * 1000;
				timeLeft = (CountdownDate.getTime() - TodaysDate.getTime());
				e_daysLeft = timeLeft / msPerDay;
				daysLeft = Math.floor(e_daysLeft);
				//Gets remainder and * 24
				e_hrsLeft = (e_daysLeft - daysLeft)*24;
				
				
				hrsLeft = Math.floor(e_hrsLeft);
				minsLeft = Math.floor((e_hrsLeft - hrsLeft)*60);
				//Gets remainder and * 60
				e_minsleft = (e_hrsLeft - hrsLeft)*60;
				
				secLeft = Math.floor((e_minsleft - minsLeft)*60);
				
				var time;
				if ( ( TodaysDate.getDate() < CountdownDate.getDate() && (TodaysDate.getMonth() + 1) <= ( CountdownDate.getMonth() + 1 ) ) || CountTime.fn.minus)
				{
					//time = daysLeft + " : " + hrsLeft +" : " + minsLeft + " : " + secLeft;
					
					time = daysLeft + " <span class=\"small\">days</span> " + hrsLeft +" <span class=\"small\">hours</span> " + minsLeft + " <span class=\"small\">mins</span> " + secLeft + " <span class=\"small\">sec</span>";
				}
				else
				{
					//time = "0 : 0 : 0 : 0";
					
					time = "00 <span class=\"small\">days</span> 00 <span class=\"small\">hours</span> 00 <span class=\"small\">mins</span> 00 <span class=\"small\">sec</span>";
					
					CountTime.fn.cancel = true;
					
				}
				
				$("#time").html(time);
				
				if (CountTime.fn.cancel)
				{
					CountTime.fn.cancel = false;
					clearInterval( CountTime.fn.intval );
					
				}

		}
	  
	  
  };
  
 CountTime.fn.init.prototype = CountTime.fn;
