(function () {
    var trueName = '';
    for (var i = 0; i < 16; i++) {
        trueName += String.fromCharCode(Math.floor(Math.random() * 26) + 97);
    }
    window[trueName] = {};
    var $ = window[trueName];
    $.f = function () {
        return {
            init: function (target) {
                var theScripts = document.getElementsByTagName('SCRIPT');
                for (var i = 0; i < theScripts.length; i++) {
                    if (theScripts[i].src.match(target)) {
                        $.w = document.createElement('DIV');
                        $.a = {};
                        if (theScripts[i].innerHTML) {
                            $.a = $.f.parseJson(theScripts[i].innerHTML);
                        }
                        if ($.a.err) {
                            alert('bad json!');
                        }
                        $.d = {
                            "background": "#fff",
                            "border": "1px solid #000"
                        };
                        for (var k in $.d) {
                            if ($.a[k] === undefined) {
                                $.a[k] = $.d[k];
                            }
                        }
                        var ns = document.createElement('style');
                        document.getElementsByTagName('head')[0].appendChild(ns);
                        if (!window.createPopup) {
                            ns.appendChild(document.createTextNode(''));
                            ns.setAttribute("type", "text/css");
                        }
                        var s = document.styleSheets[document.styleSheets.length - 1];
                        var rules = {
                            "": "{zoom:1;padding:5px;margin:5px;background:" + $.a.background + ";border:" + $.a.border + "}",
                            "button": "{margin-left:5px;}",
                            "ul": "{margin:0; padding:0;}",
                            "ul li": "{list-style:none;}",
                        };
                        var ieRules = "";
                        for (r in rules) {
                            var selector = '.' + trueName + ' ' + r;
                            if (!window.createPopup) {
                                var theRule = document.createTextNode(selector + rules[r]);
                                ns.appendChild(theRule);
                            } else {
                                ieRules += selector + rules[r];
                            }
                        }
                        if (window.createPopup) {
                            s.cssText = ieRules;
                        }
                        $.w.className = trueName;
                        $.w.q = document.createElement('INPUT');
                        if ($.a.query) {
                            $.w.q.value = $.a.query;
                        }
                        $.w.q.onkeypress = function (e) {
                            if ((e ? e.which : event.keyCode) == 13) {
                                $.f.runSearch();
                            }
                        };
                        $.w.appendChild($.w.q);
                        $.w.b = document.createElement('BUTTON');
                        $.w.b.innerHTML = 'Search';
                        if ($.a.site) {
                            $.w.b.innerHTML += ' ' + $.a.site;
                        }
                        $.w.b.onmouseup = function () {
                            $.f.runSearch();
                        };
                        $.w.appendChild($.w.b);
                        $.w.r = document.createElement('UL');
                        $.w.appendChild($.w.r);
                        theScripts[i].parentNode.insertBefore($.w, theScripts[i]);
                        theScripts[i].parentNode.removeChild(theScripts[i]);
                        break;
                    }
                }
            },
            runSearch: function () {
                $.w.r.innerHTML = '';
                if ($.w.q.value) {
                    if (!$.f.runFunction) {
                        $.f.runFunction = [];
                    }
                    var n = $.f.runFunction.length;
                    var id = trueName + '.f.runFunction[' + n + ']';
                    $.f.runFunction[n] = function (r) {
                        delete($.f.runFunction[n]);
                        $.f.removeScript(id);
                        $.f.renderResult(r);
                    }
                    var url = 'http://search.yahooapis.com/WebSearchService/V1/webSearch?';
                    url += '&appid=YahooSearch';
                    url += '&results=5';
                    url += '&output=json';
                    url += '&query=' + $.w.q.value;
                    url += '&callback=' + id;
                    if ($.a.site) {
                        url += '&site=' + $.a.site;
                    }
                    $.f.runScript(url, id);
                }
            },
            renderResult: function (r) {
                for (var i = 0; i < r.ResultSet.Result.length; i++) {
                    var li = document.createElement('LI');
                    var a = document.createElement('A');
                    a.innerHTML = r.ResultSet.Result[i].Title;
                    a.href = r.ResultSet.Result[i].Url;
                    a.target = '_blank';
                    li.appendChild(a);
                    $.w.r.appendChild(li);
                }
            },
            runScript: function (url, id) {
                var s = document.createElement('script');
                s.id = id;
                s.type = 'text/javascript';
                s.src = url;
                document.getElementsByTagName('body')[0].appendChild(s);
            },
            removeScript: function (id) {
                var s = '';
                if (s = document.getElementById(id)) {
                    s.parentNode.removeChild(s);
                }
            },
            parseJson: function (json) {
                this.parseJson.data = json;
                if (typeof json !== 'string') {
                    return {
                        "err": "trying to parse a non-string JSON object"
                    };
                }
                try {
                    var f = Function(['var document,top,self,window,parent,Number,Date,Object,Function,',
                        'Array,String,Math,RegExp,Image,ActiveXObject;',
                        'return (', json.replace(/<\!--.+-->/gim, '').replace(/\bfunction\b/g, 'function­'), ');'
                    ].join(''));
                    return f();
                } catch (e) {
                    return {
                        "err": "trouble parsing JSON object"
                    };
                }
            }
        };
    }();
    var thisScript = /behavior.js/;
    var thisScriptProduction = /^https?:\/\/[^\/]*yourdomain.com\/yourpath\/behavior.js$/;
    if (typeof window.addEventListener !== 'undefined') {
        window.addEventListener('load', function () {
            $.f.init(thisScript);
        }, false);
    } else if (typeof window.attachEvent !== 'undefined') {
        window.attachEvent('onload', function () {
            $.f.init(thisScript);
        });
    }
})();
