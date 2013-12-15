$$('.report_this').each(function(report_button) {  
   var id = report_button.id.split('_')[1];  
   report_button.observe('click', this.respondToReportButton.bind(this, id);  
});

/*
Four things are slowing this code down: 1. the usage of the $$ function, 2. the usage of each instead of a native looping construct, 3. the retrieving of the id from the id string, 4. the repeated binding of functions.
There are several possible remedies against the above code:
Give all report_this buttons a unique id (say for instance that you have 15 or less in a list)
Pre generate a list of ids using your server side language of choice and pass it to javascript
Manually traverse the DOM; $(‘container’).childNodes can do wonders
Bind once to a common parent element
Find items by name instead of class
Forget about all the initializing and fall back to old school onclick=”classinstance.respondToReportButton()”
*/

this.respondToReportButtonBound = this.respondToReportButton.bind(this);  
for(x=1;x<16;x++) {  
   button = $('report_button'+x);  
   if(!button) break;  
   button.observe('click', this.respondToReportButtonBound);  
}