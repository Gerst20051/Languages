/*
 * Based on Dustin Diaz's 'ify' script, which is why I've kept his license.
 * The regular expressions are different from his though, and IMHO more complete.
 *
 * Modified by Remy Sharp / http://remysharp.com / @rem
 * Last updated: $Date$
 *
 * Dustin's Credit:
 * Twita@talinkahashifyer
 * http://www.dustindiaz.com
 * http://www.dustindiaz.com/basement/ify.html
 *
 * Copyright (c) 2009 Dustin Diaz
 * licensed under public BSD
 */
var ify = function() {
  return {
    "link": function(t) {
      return t.replace(/[a-z]+:\/\/[a-z0-9-_]+\.[a-z0-9-_:~%&\?\+#\/.=]+[^:\.,\)\s*$]/ig, function(m) {
        return '<a href="' + m + '">' + ((m.length > 25) ? m.substr(0, 24) + '...' : m) + '</a>';
      });
    },
    "at": function(t) {
      return t.replace(/(^|[^\w]+)\@([a-zA-Z0-9_]{1,15}(\/[a-zA-Z0-9-_]+)*)/g, function(m, m1, m2) {
        return m1 + '@<a href="http://twitter.com/' + m2 + '">' + m2 + '</a>';
      });
    },
    "hash": function(t) {
      return t.replace(/(^|[^&\w'"]+)\#([a-zA-Z0-9_]+)/g, function(m, m1, m2) {
        return m1 + '#<a href="http://search.twitter.com/search?q=%23' + m2 + '">' + m2 + '</a>';
      });
    },
    "clean": function(tweet) {
      return this.hash(this.at(this.link(tweet)));
    }
  };
}();
