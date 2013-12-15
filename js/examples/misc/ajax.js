	
  var ajaxArray = new Array(); 	

  function validate(check_item)
  {
	  return true;
  }
  
  function submit_form(form,strURL,id) 
  { 
	 var str = getFormValues(form); 
     xmlReq = xmlhttpFormPost(strURL,id,str); 
  } 
  
 function getFormValues(fobj) 
 { 

   var str = ""; 

   for(var i = 0;i < fobj.elements.length;i++) 
   { 
		 if(fobj.elements[i]!=null)
		 {
			 if (fobj.elements[i].type == "checkbox") {
                if (fobj.elements[i].checked) {
                  	str += fobj.elements[i].name + "=" + escape(fobj.elements[i].value) + "&";
               	} //end if (fobj.elements[i].checked) {
			 } //end if (fobj.elements[i].type == "checkbox") {
			 else str += fobj.elements[i].name + "=" + escape(fobj.elements[i].value) + "&"; 
		 }
   } 
   str = str.substr(0,(str.length - 1)); 
   return str; 
}


	function xmlhttpFormPost(strURL,id,str) {
		
		document.getElementById(id).innerHTML = '<img src="http://www.liveleak.com/i/processing.gif" width="16" height="16"> <strong>Processing request..please hold on...</strong>';
		
		var index = 0; 
        // Find an empty slot in the AJAX array, 10 ajax connections at same time max for now
        while (ajaxArray[index] && index < 10)index++; 
      
		ajaxArray[index] = false;
	    var self = this;
	    // Mozilla/Safari
		
		if (window.XMLHttpRequest) {
        ajaxArray[index] = new XMLHttpRequest();
	    }
	    // IE
	    else if (window.ActiveXObject) {
	        ajaxArray[index] = new ActiveXObject("Microsoft.XMLHTTP");
 	   }

		ajaxArray[index].open('POST', strURL, true);
        ajaxArray[index].setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		 
 	    ajaxArray[index].onreadystatechange = function() {
			
        if (ajaxArray[index].readyState == 4) {
				document.getElementById(id).innerHTML = ajaxArray[index].responseText;
				ajaxArray[index] = null;
				display_main_table();
			}
		}
		ajaxArray[index].send(str);
		return false;
	}
	
	function setAndExecute(divId, innerHTML){ 
	  var div = document.getElementById(divId); 
	  div.innerHTML = innerHTML; 
	  var x = div.getElementsByTagName("script"); 
	  for(var i=0;i<x.length;i++)   {       eval(x[i].text);   }
	}
	
	function xmlhttpPost(strURL,id) {
		
		document.getElementById(id).innerHTML = '<p><img src="http://www.liveleak.com/i/processing.gif" width="16" height="16"> <strong>Processing request..please hold on...</strong></p>';
		
		var index = 0; 
        // Find an empty slot in the AJAX array, 10 ajax connections at same time max for now
        while (ajaxArray[index] && index < 10)index++; 
      
		ajaxArray[index] = false;
		
	    var self = this;
	    // Mozilla/Safari
	    if (window.XMLHttpRequest) {
        ajaxArray[index] = new XMLHttpRequest();
	    }
	    // IE
	    else if (window.ActiveXObject) {
	        ajaxArray[index] = new ActiveXObject("Microsoft.XMLHTTP");
 	   }

		ajaxArray[index].open('POST', strURL, true);
	    ajaxArray[index].setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
 	    ajaxArray[index].onreadystatechange = function() {
        if (ajaxArray[index].readyState == 4) {
				setAndExecute(id,ajaxArray[index].responseText);
				ajaxArray[index] = null;
			}
		}
		ajaxArray[index].send('i=1');
		return false;
	}
	
	function return_html(strURL) {
		
	    var xmlHttpReq = false;
	    var self = this;
	    // Mozilla/Safari
	    if (window.XMLHttpRequest) {
        self.xmlHttpReq = new XMLHttpRequest();
	    }
	    // IE
	    else if (window.ActiveXObject) {
	        self.xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
 	   }
		self.xmlHttpReq.open('POST', strURL, true);
	    self.xmlHttpReq.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
 	    self.xmlHttpReq.onreadystatechange = function() {
        if (self.xmlHttpReq.readyState == 4) {
		   return self.xmlHttpReq.responseText;
		  }
		}
		self.xmlHttpReq.send('i=1');
	} //end function return_html(strURL) {


		 

	 