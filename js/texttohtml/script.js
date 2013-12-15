
// forEach
function forEach(array, action) {
  for (var i = 0; i < array.length; i++)
    action(array[i]);
}

// map
function map(func, array) {
  var result = [];
  forEach(array, function (element) {
    result.push(func(element));
  });
  return result;
}

// splitParagraph, idiomatic
function splitParagraph(text) {
  function split() {
  var pos = 0, fragments = [];
  while (pos < text.length) {
    if (text.charAt(pos) == "*") {
      var end = findClosing("*", pos + 1);
      fragments.push({type: "emphasized", content: text.slice(pos + 1, end)});
      pos = end + 1;
    }
    else if (text.charAt(pos) == "{") {
      var end = findClosing("}", pos + 1);
      fragments.push({type: "footnote", content: text.slice(pos + 1, end)});
      pos = end + 1;
    }
    else {
      var end = findOpeningOrEnd(pos);
      fragments.push({type: "normal", content: text.slice(pos, end)});
      pos = end;
    }
  }
  return fragments;
}

  function findClosing(character, from) {
    var end = text.indexOf(character, from);
    if (end == -1) throw new Error("Missing closing '" + character + "'");
    else return end;
  }

  function findOpeningOrEnd(from) {
    function indexOrEnd(character) {
      var index = text.indexOf(character, from);
      return index == -1 ? text.length : index;
    }
    return Math.min(indexOrEnd("*"), indexOrEnd("{"));
  }

  return split(0);
}

// processParagraph
function processParagraph(paragraph) {
  var header = 0;
  while (paragraph.charAt(header) == "%")
    header++;
  if (header > 0)
    return {type: "h" + header, content: splitParagraph(paragraph.slice(header + 1))};
  else
    return {type: "p", content: splitParagraph(paragraph)};
}

// extractFootnotes
function extractFootnotes(paragraphs) {
  var footnotes = [];
  var currentNote = 0;

  function replaceFootnote(fragment) {
    if (fragment.type == "footnote") {
      currentNote++;
      footnotes.push(fragment);
      fragment.number = currentNote;
      return {type: "reference", number: currentNote};
    }
    else {
      return fragment;
    }
  }

  forEach(paragraphs, function(paragraph) {
    paragraph.content = map(replaceFootnote, paragraph.content);
  });

  return footnotes;
}

// escapeHTML
function escapeHTML(text) {
  var replacements = {"<": "&lt;", ">": "&gt;",
                      "&": "&amp;", "\"": "&quot;"};
  return text.replace(/[<>&"]/g, function(character) {
    return replacements[character];
  });
}

// HTML
var HTML = {
  tag: function(name, content, properties) {
    return {name: name, properties: properties, content: content};
  },
  link: function(target, text) {
    return HTML.tag("a", [text], {href: target});
  }
};

// tag
function tag(name, content, attributes) {
  return {name: name, attributes: attributes, content: content};
}

function link(target, text) {
  return tag("a", [text], {href: target});
}

function htmlDoc(title, bodyContent) {
  return tag("html", [tag("head", [tag("title", [title])]),
                      tag("body", bodyContent)]);
}

// book render functions
function renderFragment(fragment) {
  if (fragment.type == "reference")
	return tag("sup", [link("#footnote" + fragment.number, String(fragment.number))]);
  else if (fragment.type == "emphasized")
	return tag("em", [fragment.content]);
  else if (fragment.type == "normal")
	return fragment.content;
}

function renderParagraph(paragraph) {
  return tag(paragraph.type, map(renderFragment, paragraph.content));
}

function renderFootnote(footnote) {
  var anchor = tag("a", [], {name: "footnote" + footnote.number});
  var number = "[" + footnote.number + "] ";
  return tag("p", [tag("small", [anchor, number, footnote.content])]);
}

function renderHTML(element) {
  var pieces = [];

  function renderAttributes(attributes) {
    var result = [];
    if (attributes) {
      for (var name in attributes) 
        result.push(" " + name + "=\"" + escapeHTML(attributes[name]) + "\"");
    }
    return result.join("");
  }

  function render(element) {
    // Text node
    if (typeof element == "string") {
      pieces.push(escapeHTML(element));
    }
    // Empty tag
    else if (!element.content || element.content.length == 0) {
      pieces.push("<" + element.name + renderAttributes(element.attributes) + "/>");
    }
    // Tag with content
    else {
      pieces.push("<" + element.name + renderAttributes(element.attributes) + ">");
      forEach(element.content, render);
      pieces.push("</" + element.name + ">");
    }
  }

  render(element);
  return pieces.join("");
}

function renderFile(file, title) {
  var paragraphs = map(processParagraph, file.split("\n\n"));
  var footnotes = map(renderFootnote, extractFootnotes(paragraphs));
  var body = map(renderParagraph, paragraphs).concat(footnotes);
  return renderHTML(htmlDoc(title, body));
}

// registerEventHandler
function registerEventHandler(node, event, handler) {
  if (typeof node.addEventListener == "function")
    node.addEventListener(event, handler, false);
  else
    node.attachEvent("on" + event, handler);
}

function unregisterEventHandler(node, event, handler) {
  if (typeof node.removeEventListener == "function")
    node.removeEventListener(event, handler, false);
  else
    node.detachEvent("on" + event, handler);
}

// normalizeEvent
function normalizeEvent(event) {
  if (!event.stopPropagation) {
    event.stopPropagation = function() {this.cancelBubble = true;};
    event.preventDefault = function() {this.returnValue = false;};
  }
  if (!event.stop)
    event.stop = function() {
      this.stopPropagation();
      this.preventDefault();
    };

  if (event.srcElement && !event.target)
    event.target = event.srcElement;
  if ((event.toElement || event.fromElement) && !event.relatedTarget)
    event.relatedTarget = event.toElement || event.fromElement;
  if (event.clientX != undefined && event.pageX == undefined) {
    event.pageX = event.clientX + document.body.scrollLeft;
    event.pageY = event.clientY + document.body.scrollTop;
  }
  if (event.type == "keypress")
    event.character = String.fromCharCode(event.charCode || event.keyCode);
  return event;
}

// addHandler
function addHandler(node, type, handler) {
  function wrapHandler(event) {
    handler(normalizeEvent(event || window.event));
  }
  registerEventHandler(node, type, wrapHandler);
  return {node: node, type: type, handler: wrapHandler};
}

function removeHandler(object) {
  unregisterEventHandler(object.node, object.type, object.handler);
}

var DomLoaded =
{
	onload: [],
	loaded: function()
	{
		if (arguments.callee.done) return;
		arguments.callee.done = true;
		for (i = 0;i < DomLoaded.onload.length;i++) DomLoaded.onload[i]();
	},
	load: function(fireThis)
	{
		this.onload.push(fireThis);
		if (document.addEventListener) 
			document.addEventListener("DOMContentLoaded", DomLoaded.loaded, null);
		if (/KHTML|WebKit/i.test(navigator.userAgent))
		{ 
			var _timer = setInterval(function()
			{
				if (/loaded|complete/.test(document.readyState))
				{
					clearInterval(_timer);
					delete _timer;
					DomLoaded.loaded();
				}
			}, 10);
		}
		/*@cc_on @*/
		/*@if (@_win32)
		var proto = "src='javascript:void(0)'";
		if (location.protocol == "https:") proto = "src=//0";
		document.write("<scr"+"ipt id=__ie_onload defer " + proto + "><\/scr"+"ipt>");
		var script = document.getElementById("__ie_onload");
		script.onreadystatechange = function() {
		    if (this.readyState == "complete") {
		        DomLoaded.loaded();
		    }
		};
		/*@end @*/
	   window.onload = DomLoaded.loaded;
	}
};

DomLoaded.load(function(){
	registerEventHandler(document.getElementById("parse"), 'click', function(event){
		document.getElementById("content").innerHTML = renderFile(document.getElementById("text").value, "The Book of Programming");
	});
});