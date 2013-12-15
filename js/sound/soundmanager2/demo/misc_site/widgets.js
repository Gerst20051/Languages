// Utilities from prototype.js
var GsfnClass = {
  create: function() {
    return function() { this.initialize.apply(this, arguments); }
  }
}

Object.extend = function(destination, source) {
  for (var property in source) {
    destination[property] = source[property];
  }
  return destination;
}

Object.extend(String, {
  interpret: function(value) { return value == null ? '' : String(value); }
});

Object.extend(String.prototype, {
  gsub: function(pattern, replacement) {
    var result = '', source = this, match;
    replacement = arguments.callee.prepareReplacement(replacement);

    while (source.length > 0) {
      if (match = source.match(pattern)) {
        result += source.slice(0, match.index);
        result += String.interpret(replacement(match));
        source  = source.slice(match.index + match[0].length);
      } else {
        result += source, source = '';
      }
    }
    return result;
  }
});

String.prototype.gsub.prepareReplacement = function(replacement) {
  if (typeof replacement == 'function') return replacement;
  var template = new GsfnTemplate(replacement);
  return function(match) { return template.evaluate(match) };
}

var GsfnTemplate = GsfnClass.create();
GsfnTemplate.Pattern = /(^|.|\r|\n)(#\{(.*?)\})/;
GsfnTemplate.prototype = {
  initialize: function(template, pattern) {
    this.template = template.toString();
    this.pattern  = pattern || GsfnTemplate.Pattern;
  },

  evaluate: function(object) {
    return this.template.gsub(this.pattern, function(match) {
      var before = match[1];
      if (before == '\\') return match[2];
      return before + String.interpret(object[match[3]]);
    });
  }
}

var GsfnScriptAttach = GsfnClass.create();
Object.extend(GsfnScriptAttach, {
  at: function(url) {
    var element = document.createElement('script');
    element.type = 'text/javascript';
    element.src = url;
    element.setAttribute('class', 'gsfn');
    document.getElementsByTagName('head')[0].appendChild(element);
  }
});

// Satisfaction code
gsfn_topic_list = { 
  open_tag:     '<ul class="gsfn_topic_list">', 
  item:         '<li class="gsfn_#{style}"><a href="#{url}" class="gsfn_link" target="_top">#{subject}</a><span class="time">#{date}</span><div class="gsfn_summary">#{summary}</div></li>',
  result:       '<li class="gsfn_#{style}"><a href="#{url}" class="gsfn_link" target="_top">#{subject}</a></li>',
  no_results:   '<li class="gsfn_no_results">There are no topics in the community yet.</li>',
  suggested:    '<li class="gsfn_suggestion">Are any of these topics helpful?</li>',
  topic_submit: '<li class="gsfn_submit">Not quite right? <input type="submit" onclick="gsfn_submit();" value="Create a Topic" /> <span class="gsfn_or">or</span> <a href="#" onclick="gsfn_cancel(); return false;">Cancel</a></li>',
  no_results_submit: '<li class="gsfn_no_results"><input type="submit" onclick="gsfn_submit();" value="Create a Topic" /> <span class="gsfn_or">or</span> <a href="#" onclick="gsfn_cancel(); return false;">Cancel</a></li>',
  close_tag:    '</ul>'
}

function gsfn_populate(id, template) { document.getElementById(id).innerHTML = template; }
function gsfn_append(id, template) { document.getElementById(id).innerHTML += template; }
function gsfn_cancel(id) { 
  document.getElementById('gsfn_search_query').value = "";
  document.getElementById('gsfn_search_results').innerHTML = "";
}

var gsfn_searched = false;
function gsfn_search(form) {
  document.getElementById('gsfn_search_results').innerHTML = "Searching the community...";
  var url = "http://getsatisfaction.com/schillmania/searches"
  var params = []
  for (var i = form.elements.length - 1; i >= 0; i--) {
    if (form.elements[i].name) {
      if(form.elements[i].name == 'query') {
        params.push('utm_term=' + form.elements[i].value);
      }
      params.push(form.elements[i].name + '=' + form.elements[i].value);
    }
  };
  url += '?' + encodeURI(params.join('&'))
  GsfnScriptAttach.at(url);
  gsfn_searched = true;
}

function gsfn_submit() {
  form = document.getElementById('gsfn_search_form');
  if (gsfn_searched) {
    for (var i = form.elements.length - 1; i >= 0; i--){
      el = form.elements[i];
      if (el.name == 'query') {
        el.name = 'topic[subject]';
      } else if (el.name == 'tag') {
        el.name = 'topic[tag]';
      } else {
        el.disabled = 'disabled';
      }
    }
    form.submit();
    return true;
  } else {
    gsfn_search(form);
    return false;
  }
}

function gsfnResultsCallback(json) {
  var gsfn_topics = json;
  var topics_html = [];

  topics_html.push(gsfn_topic_list.open_tag);
  if (gsfn_topics.length == 0) {
    topics_html.push(gsfn_topic_list.no_results);
    topics_html.push(gsfn_topic_list.no_results_submit);
  } else {
    topics_html.push(gsfn_topic_list.suggested);
    for (var t=0; t < gsfn_topics.length; t++) {
      topics_html.push(new GsfnTemplate(gsfn_topic_list.result).evaluate(gsfn_topics[t]));
    }
    topics_html.push(gsfn_topic_list.topic_submit);
  }
  topics_html.push(gsfn_topic_list.close_tag);
  
  gsfn_populate('gsfn_search_results', topics_html.join('\n'));
}

function gsfnTopicsCallback(json) {
  var gsfn_topics = json;
  var topics_html = [];

  topics_html.push(gsfn_topic_list.open_tag);
  if (gsfn_topics.length == 0) {
    topics_html.push(gsfn_topic_list.no_results);
  } else {
    for (var t=0; t < gsfn_topics.length; t++) {
      topics_html.push(new GsfnTemplate(gsfn_topic_list.item).evaluate(gsfn_topics[t]));
    }
  }
  topics_html.push(gsfn_topic_list.close_tag);

  gsfn_populate('gsfn_content', topics_html.join('\n'));
}