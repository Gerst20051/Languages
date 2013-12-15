<script type="text/javascript"><!--
/*
  Date/Epoch Functions
  Author: Thomas Ballard (http://thomas.ballard.ws)
  Copr. 2002-2008, E.S.Q. Software

  Note: slightly polluted to attempt to support a GMT/non-GMT (local) timezone offset feature. 
  Hack and slash resulted in a form input having a toggle state... but these functions need 
  to know that form and input to access the property for evaluation. Need to come back 
  and make these methods more agnostic.
*/
if(!window.dDate) window.dDate = new Date()


window.getEpoch = function(){
  var GMT = document.ivnForm.GMT.checked
  if(GMT){
    return parseInt(Date.UTC(dDate.getUTCFullYear(),dDate.getUTCMonth(),dDate.getUTCDate(),dDate.getUTCHours(),dDate.getUTCMinutes(),dDate.getUTCSeconds(),dDate.getUTCMilliseconds())/1000)
  } else {
    return (dDate.getTime()-dDate.getMilliseconds())/1000
  }
}


window.getDays = function(mObj,dObj,yObj){
  // build array of days in a select container based on selected month and year
  iMonth=parseInt(mObj.options[mObj.selectedIndex].value)+1
  iYear=(yObj.value?yObj.value:1900);

  var iDays=31;
  switch(iMonth){ /* determine the number of days this month including leap years */
    case 4: case 6: case 9: case 11:  --iDays; break;
    case 2: iDays=29; if (((iYear%4)!=0) || ((iYear%100)==0)) --iDays;
  }

  dObj.options.length = 0;
  for(var i=0; i<iDays; i++){
    dObj.options[i] = new Option(i+1,i+1)
  }
}


window.toDate = function(eObj){
  var mEpoch = parseInt(eObj.value); 
  if(mEpoch<10000000000) mEpoch *= 1000; // convert to milliseconds (Epoch is usually expressed in seconds, but Javascript uses Milliseconds)

  dDate.setTime(mEpoch)
  return dDate;
}


window.toEpoch = function(mObj,dObj,yObj,hrObj,minObj,secObj){
  var month = mObj.options[mObj.selectedIndex].value
  var day = dObj.options[dObj.selectedIndex].value
  var year = yObj.value
  var hour = hrObj.options[hrObj.selectedIndex].value
  var minute = minObj.options[minObj.selectedIndex].value
  var second = secObj.options[secObj.selectedIndex].value

  //alert(month+']['+day+']['+year)
  dDate.setMonth(month,day)
  dDate.setFullYear(year)
  dDate.setHours(hour,minute,second)
  //alert(dDate)

  zObj.innerHTML = window.getEpoch()
  if(window && window.clipboardData && window.clipboardData.setData)
    bResult = window.clipboardData.setData("Text",zObj.innerHTML); //stuff the text onto the clipboard

}
//--></script>



<script type="text/javascript"><!--
var zObj = document.getElementById('dynamo')
var fobj = document.forms.ivnForm
if(fobj.GMT.checked){
  // GMT TIME
  var dDate = new Date(Date.UTC(dDate.getUTCFullYear(),dDate.getUTCMonth(),dDate.getUTCDate(),dDate.getUTCHours(),dDate.getUTCMinutes(),dDate.getUTCSeconds(),dDate.getUTCMilliseconds()));
  fobj.month.selectedIndex = dDate.getUTCMonth(); // set initial month to current
  fobj.year.value = dDate.getUTCFullYear()
  getDays(fobj.month,fobj.day,fobj.year)
  fobj.day.selectedIndex = dDate.getUTCDate()-1
  fobj.epoch.value = window.getEpoch()
  fobj.hour.selectedIndex = dDate.getUTCHours();
  fobj.minute.selectedIndex = dDate.getUTCMinutes();
  fobj.second.selectedIndex = dDate.getUTCSeconds();
} else {
  // LOCAL TIME
  var dDate = new Date();
  fobj.month.selectedIndex = dDate.getMonth(); // set initial month to current
  fobj.year.value = dDate.getFullYear()
  getDays(fobj.month,fobj.day,fobj.year)
  fobj.day.selectedIndex = dDate.getDate()-1
  fobj.epoch.value = window.getEpoch()
  fobj.hour.selectedIndex = dDate.getHours();
  fobj.minute.selectedIndex = dDate.getMinutes();
  fobj.second.selectedIndex = dDate.getSeconds();
}

//window.dDate = new Date();
var GMT = document.ivnForm.GMT.checked
if(GMT){
  var html = parseInt(Date.UTC(dDate.getUTCFullYear(),dDate.getUTCMonth(),dDate.getUTCDate(),dDate.getUTCHours(),dDate.getUTCMinutes(),dDate.getUTCSeconds(),dDate.getUTCMilliseconds())/1000)+' (Epoch)'
  + '<br>'+dDate.toUTCString()+' (Standard)'
} else {
  var html = (dDate.getTime()-dDate.getMilliseconds())/1000+' (Epoch)'
  + '<br>'+dDate+' (Standard)'
}
if(document && document.getElementById){
  obj = document.getElementById('jdate1')
  if(obj) obj.innerHTML = html
}
//--></script>

<script type="text/perlscript">
$obj = $window->document->getElementById('pdate1');
$obj->{innerHTML} = time . " (Epoch)<br>" . localtime(time) . " (Standard)";
</script>