function TextToXML(strXML) {
 var xmlDoc = null;
 try {
  xmlDoc = (document.all)?new ActiveXObject("Microsoft.XMLDOM"):new DOMParser();
  xmlDoc.async = false;
 } catch(e) {throw new Error("XML Parser could not be instantiated");}
 var out;
 try {
  if(document.all) {
   out = (xmlDoc.loadXML(strXML))?xmlDoc:false;
  } else {  
   out = xmlDoc.parseFromString(strXML, "text/xml");
  }
 } catch(e) { throw new Error("Error parsing XML string"); }
 return out;
} 