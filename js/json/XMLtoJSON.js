function XHR(sURL, fncCallback /*, argumentToPass1, argumentToPass2, etc. */) {
  var oResp, aArgs = Array.prototype.slice.call(arguments, 2);
  if (window.XMLHttpRequest) { oResp = new XMLHttpRequest(); }
  else if (window.ActiveXObject) { oResp = new ActiveXObject("Microsoft.XMLHTTP"); }
  if (oResp) {
    if (fncCallback) {
      if (typeof oResp.onload !== "undefined")
        oResp.onload = function() {
          fncCallback.apply(oResp, aArgs);
          oResp = null;
        };
      else {
        oResp.onreadystatechange = function() {
          if (oResp.readyState === 4) {
            fncCallback.apply(oResp, aArgs);
            oResp = null;
          }
        };
      }
    }
    oResp.open("GET", sURL, true);
      oResp.setRequestHeader("Content-Type", "text/plain");
    oResp.setRequestHeader("If-Modified-Since", "Sat, 1 Jan 2000 00:00:00 GMT");
    oResp.send(null);
  }
}
// recursive function which converts an XML DOM to a JavaScript Object
function xml2Obj (oXMLDom) {
  var oRObj = true;
  if (oXMLDom.nodeType === 3) { // text
    oRObj = oXMLDom.nodeValue.replace(/^\s+|\s+$/g, "");
  } else {
    if (oXMLDom.nodeType === 1) { // element
      // do attributes
      if (oXMLDom.attributes.length > 0) {
        var iAttrib;
        oRObj = {};
        oRObj["@attributes"] = {};
        for (var iAttrId = 0; iAttrId < oXMLDom.attributes.length; iAttrId++) {
          iAttrib = oXMLDom.attributes.item(iAttrId);
          oRObj["@attributes"][iAttrib.nodeName] = iAttrib.nodeValue;
        }
      }
    }
    // do children
    if (oXMLDom.hasChildNodes()) {
      var iKey, iValue, iXMLNode;
      if (oRObj === true) { oRObj = {}; }
      for (var iChildId = 0; iChildId < oXMLDom.childNodes.length; iChildId++) {
        iXMLNode = oXMLDom.childNodes.item(iChildId);
        iKey = iXMLNode.nodeType === 3 ? "@content" : iXMLNode.nodeName;
        iValue = xml2Obj(iXMLNode);
        if (oRObj.hasOwnProperty(iKey)) {
          if (iXMLNode.nodeType === 3) { oRObj[iKey] += iValue; }
          else {
            if (oRObj[iKey].constructor !== Array) { oRObj[iKey] = [oRObj[iKey]]; }
            oRObj[iKey].push(iValue);
          }
        } else if (iXMLNode.nodeType !== 3 || iValue !== "") { oRObj[iKey] = iValue; }
      }
     }
  }
  return(oRObj);
};
// function called via ajax callback
function myFunction() {
  // gets the object
  var oMyObject = xml2Obj(this.responseXML);
  // converts the object to a string and display it in an alert message
  alert(JSON.stringify(oMyObject));
}