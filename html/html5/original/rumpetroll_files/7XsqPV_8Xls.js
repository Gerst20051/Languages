/*1293561867,169776066*/

if (window.CavalryLogger) { CavalryLogger.start_js(["KsJXN"]); }

onloadRegister(function(){copy_properties(AsyncRequest.prototype,{setNectarModuleData:function(c){if(this.method=='POST'){var d=Env.module;if(c&&d===undefined){var b={fbpage_fan_confirm:1};var e=null;for(var a=c;a&&a!=document.body;a=a.parentNode){if(!a.id||typeof a.id!=='string')continue;if(a.id.startsWith('pagelet_')){d=a.id;break;}if(!e&&b[a.id])e=a.id;}if(d===undefined&&e)d=e;}if(d!==undefined){if(this.data.nctr===undefined)this.data.nctr={};this.data.nctr._mod=d;}}},setNectarImpressionId:function(){if(this.method=='POST'){var a=env_get('impid');if(a!==undefined){if(this.data.nctr===undefined)this.data.nctr={};this.data.nctr._impid=a;}}}});});
function htmlspecialchars(a){if(typeof(a)=='undefined'||a===null||!a.toString)return '';if(a===false){return '0';}else if(a===true)return '1';return a.toString().replace(/&/g,'&amp;').replace(/"/g,'&quot;').replace(/'/g,'&#039;').replace(/</g,'&lt;').replace(/>/g,'&gt;');}function htmlize(a){return htmlspecialchars(a).replace(/\n/g,'<br />');}function escape_js_quotes(a){if(typeof(a)=='undefined'||!a.toString)return '';return a.toString().replace(/\\/g,'\\\\').replace(/\n/g,'\\n').replace(/\r/g,'\\r').replace(/"/g,'\\x22').replace(/'/g,'\\\'').replace(/</g,'\\x3c').replace(/>/g,'\\x3e').replace(/&/g,'\\x26');}
window.__UIControllerRegistry=window.__UIControllerRegistry||{};
function UIPagelet(c,d,a,b){this._id=c||null;this._element=ge(c||$N('div'));this._src=d||null;this._context_data=a||{};this._data=b||{};this._handler=bagofholding;this._request=null;this._use_ajaxpipe=false;this._is_bundle=true;this._allow_cross_page_transition=false;this._append=false;return this;}UIPagelet.loadFromEndpoint=function(b,e,a,c){c=c||{};var d=('/pagelet/generic.php/'+b).replace(/\/+/g,'/');new UIPagelet(e,d,a).setUseAjaxPipe(c.usePipe).setBundleOption(b.substring(0,8)!='/intern/'&&c.bundle!==false).setReplayable(c.replayable).setAppend(c.append).setJSNonBlock(c.jsNonblock).setAllowCrossPageTransition(c.crossPage).go();};copy_properties(UIPagelet.prototype,{getElement:function(a){a=a||false;if(a)this._element=ge(this._id);return this._element;},setHandler:function(a){this._handler=a;return this;},go:function(b,a){if(arguments.length>=2||typeof b=='string'){this._src=b;this._data=a||{};}else if(arguments.length==1)this._data=b;this.refresh();return this;},setAllowCrossPageTransition:function(a){this._allow_cross_page_transition=a;return this;},setBundleOption:function(a){this._is_bundle=a;return this;},refresh:function(b){var a=function(d){this._request=null;if(b&&this._id)this._element=ge(this._id);var c=HTML(d.getPayload());if(this._append){DOM.appendContent(this._element,c);}else DOM.setContent(this._element,c);this._handler();}.bind(this);if(this._use_ajaxpipe){this._request=new AjaxPipeRequest();this._request.setCanvasId(this._id).setAppend(this._append).setJSNonBlock(this._jsNonblock);}else this._request=new AsyncRequest().setMethod('GET').setReadOnly(true).setOption('bundle',this._is_bundle).setHandler(a);this._request.setURI(this._src).setReplayable(this._replayable).setAllowCrossPageTransition(this._allow_cross_page_transition).setData({data:JSON.encode(merge(this._context_data,this._data))}).send();return this;},cancel:function(){if(this._request)this._request.abort();},setUseAjaxPipe:function(a){this._use_ajaxpipe=!!a;return this;},setReplayable:function(a){this._replayable=!!a;return this;},setAppend:function(a){this._append=!!a;return this;},setJSNonBlock:function(a){this._jsNonblock=!!a;return this;}});

if (window.Bootloader) { Bootloader.done(["KsJXN"]); }