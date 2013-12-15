/**
 * JSON to HTML Elements
 * @param {String} Root Element TagName
 * @param {Object} JSON
 */
json2HTML = function(tag, json, node) {
 
 if (!node) {
  node = document.createElement(tag);
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
      node.appendChild(json2HTML(json[x][i], document.createElement(x)));
     }
    } else {
     node.appendChild(json2HTML(json[x], document.createElement(x)));
    }
   }
  }
 }
 
 return node;
 
};