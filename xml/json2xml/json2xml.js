/**
 * JSON to XML
 * @param {Object} JSON
 */
json2Xml = function(json, node) {
 
 var root = false;
 if (!node) {
  node = document.createElement('root');
  root = true;
 }
 
 for (var x in json) {
  // ignore inherited properties
  if (json.hasOwnProperty(x)) {
  
   if (x == '#text') { // text
    node.appendChild(document.createTextNode(json[x]));
   } else  if (x == '@attributes') { // attributes
    for (var y in json[x]) {
     if (json[x].hasOwnProperty(y)) {
      node.setAttribute(y, json[x][y]);
     }
    }
   } else if (x == '#comment') { // comment
   // ignore
   
   } else { // elements
    if (json[x] instanceof Array) { // handle arrays
     for (var i = 0; i < json[x].length; i++) {
      node.appendChild(json2Xml(json[x][i], document.createElement(x)));
     }
    } else {
     node.appendChild(json2Xml(json[x], document.createElement(x)));
    }
   }
  }
 }
 
 if (root == true) {
  return this.textToXML(node.innerHTML);
 } else {
  return node;
 }
 
};