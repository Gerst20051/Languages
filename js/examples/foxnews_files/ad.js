(function($) { 
    $.ad = {
		_tile: 0,
		ord: Math.floor(999999999*Math.random()),
		_meta: {},
		_data: {},
		dc: {
			_svr: "http://ad.doubleclick.net",
			_method: "adj",
			_url: "",
			_kw: "",
			_meta: "",
			_reg: "",
			_rs: "",
			_adv: "",
			_ref: "",
			_sid: "",
			_fmt: "",
			_u: [],
			_bk: "",
			_qc: "",
			tile: function() {
				return ++$.ad._tile;
			},
		    tag: function(data, tile, id) {
				var ie  ='\v'=='v';var b1='',b2='';if(ie){b1=String.fromCharCode(60,98,111,100,121,62);b2=String.fromCharCode(60,47,98,111,100,121,62);}
				var s3  = (data.dc.s3)?";s3="+data.dc.s3:"";
				var s4  = (data.dc.s4)?";s4="+data.dc.s4:"";
				var hub = (data.dc.hub)?";hub="+data.dc.hub:"";
				var col = (data.col)?";col="+data.col:"";
				var pageid = "";//";pageid=" + data.pageid
				var cus = "";//";" + data.site + "=ad"
				var idd = id.split('-');
				var tag = this._svr + "/" + this._method + "/" + data.dc.site + this.dcopt(tile) + ";comp=" + this.adv(window.location, true) + ";s1=" + (data.dc.s1||"") + ";s2=" + (data.dc.s2||"") + s3 + s4 + hub + ";pos=" + idd[0] + ";ptype=" + data.ptype + this.fmt(true) + pageid + col + ";url=" + this.url(window.location.pathname) + this.kw(window.location, true) + this.ref(true) + this.meta(5,'tags') + cus;
				tag += this.revsci.get() + this.bk.get() + this.quant.get() + this.sid(true) + this.reginfo() + ";sz=" + idd[1].replace(/_/g,",") + ";tile=" + tile + this.u(data, id) + ";!c=;ord=" + $.ad.ord + "?";
		        	//return (this._method=='adj')?"<scr" + "ipt src=\"" + tag + "\" type=\"text/javascript\"></scr" + "ipt>":tag;

				if (false) {
					//if (window.console==undefined){window.console={log: function(s){alert(s);}};}
					var szs=idd[1].split('_');
					var nw = 0;var nh = 0;var sz = '';
					for (var i=0;i<szs.length;i++) {
						sz = szs[i].split('x');
						nw = sz[0]/1>nw?sz[0]/1:nw;
						nh = sz[1]/1>nh?sz[1]/1:nh;
					}
					//b1="<body onload=\"parent.jQuery.ad.util.iframe.resize('ifr-"+id+"',document.body.scrollWidth,document.body.scrollHeight);var ns=document.getElementsByTagName('noscript');if(ns.length>0&&document.body.scrollWidth==1&&document.body.scrollHeight==1){if(typeof console=='undefined'){console={};console.log=function(s){alert(s)};};console.log('noscripts:'+ns.length+' width:'+document.body.scrollWidth+' height:'+document.body.scrollHeight+' html:'+ns[0].innerHTML.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>'));document.getElementsByTagName('body')[0].innerHTML=ns[0].innerHTML.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>');parent.jQuery.ad.util.iframe.resize('ifr-"+id+"',"+nw+","+nh+");}\">";b2=String.fromCharCode(60,47,98,111,100,121,62);
					b1="<body onload=\"parent.jQuery.ad.util.iframe.resize('ifr-"+id+"',document.body.scrollWidth,document.body.scrollHeight);var ns=document.getElementsByTagName('noscript');if(ns.length>0&&document.body.scrollWidth==1&&document.body.scrollHeight==1){if(typeof console!='undefined'){console.log('noscripts:'+ns.length+' width:'+document.body.scrollWidth+' height:'+document.body.scrollHeight+' html:'+ns[0].innerHTML.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>'));}document.getElementsByTagName('body')[0].innerHTML=ns[0].innerHTML.replace(/\&lt;/g, '<').replace(/\&gt;/g, '>');parent.jQuery.ad.util.iframe.resize('ifr-"+id+"',"+nw+","+nh+");}\">";b2=String.fromCharCode(60,47,98,111,100,121,62);
			       	return b1+"<scr" + "ipt src=\"" + tag + "\" type=\"text/javascript\"></scr" + "ipt><style type=\"text/css\">body,div,img,a{margin:0;padding:0;}img{border:none;}</style>"+b2;
				}
		        	return (this._method=='adj')?b1+"<scr" + "ipt type=\"text/javascript\">window.onload = function(){parent.jQuery.ad.util.iframe.resize('ifr-"+id+"', document.body.scrollWidth, document.body.scrollHeight);};</scr" + "ipt>"+"<scr" + "ipt src=\"" + tag + "\" type=\"text/javascript\"></scr" + "ipt><style type=\"text/css\">body{margin:0;}img{border:none;}</style>"+b2:tag;
			},
			inline: function(id) {
				var d = $.ad.meta();
				$.ad.pre();
				if (window.sid != undefined && $.ad.util.cookie('surround') != "") {
					$.ad.util.cookie('surround', window.sid, {path: '/'});
				}
				if ($.ad.dc._bk == "") {
					$.ad.dc.bk.init();
				}
	
				var ie='\v'=='v';
				$.ad.render = function(d) {
					$('#'+id).removeClass("dc");
					var tile = $.ad.dc.tile();
					var t = $.ad.dc.tag(d, tile, id);
					var s = (168+id.length);
					var l = t.length;
					if(ie){s=s+6;l=t.length-7;}
					document.write(t.substring(s,l));
				}
	
				var col = "";
				if (d.ptype==="column"||d.ctype==="column"){var col = "-" + d.channel.split('/').slice(-1)[0];}
				var url = "http://ads.foxnews.com/api/"+$.ad.util.enc(d.channel)+"-"+d.ptype.toLowerCase()+col+"-data_inline.js";
				document.write("<scr"+"ipt src=\""+url+"\"></scr"+"ipt>");
			},
			url: function(u) {
		        if (this._url=="") {
					this._url = u.substr(1).split("/").join("_").split(".")[0].replace(/[^a-z0-9_-]/g,'');
				}
				return this._url;
			},
			dcopt: function(t) {
				return (t==1)?';dcopt=ist':'';
			},
			fmt: function(l) {
				var p = ";format=";
				if (this._fmt==""&&this._fmt!="-") {
					this._fmt = $.ad.meta().raw.originalPlatform||'-';
				}
				if (this._fmt=="-") {return "";}
				return l?p+this._fmt:this._fmt;
			},
		    kw: function(u, l) {
		        var p = ";kw=";
				if (this._kw==""&&this._kw!="-") {
		        	this._kw = $.ad.util.param(u, 'q')||$.ad.util.param(u, 'searchString')||'-';
				}
				if (this._kw=="-") {return "";}
				return l?p+this._kw:this._kw;
			},
			meta: function(n,m) {
				if (this._meta==""&&this._meta!="-") {
					var keys = '';
			        var t = document.getElementsByTagName('meta');
			        for(var i=0;i<t.length;i++){
			            if(t[i].name == 'keywords'){
			                keys = t[i].content;
			                break;
			            }
			        }
			        //this._meta = keys==''?'-':keys.split(',').slice(0,n).join(',').replace(/ *, */g,",").replace(/ /g,"-").replace(/[^a-z,-]/g,"");
					if (keys=='') {
						this._meta = '-';
					} else {
			        	if (m=='full') {
							this._meta = keys.split(',').slice(0,n).join(',').replace(/ *, */g,",").replace(/ /g,"-").replace(/[^a-z,-]/g,"");
						}
			       		if (m=='tags') {
							var karr = keys.split(',').slice(0,n);
							var len = karr.length;
							var x = '', k = '';
							for (var t=0;t<len;t++) {
								x = karr[t].replace(/^ /g,"").toLowerCase().replace(/ /g,"-").replace(/[^a-z-]/g,"");
		        				k += ";m"+(t+1)+"="+x;
							}
							this._meta = k;
						}
					}
				}
				if (this._meta=="-") {return "";}
				return this._meta;
			},
			reginfo: function() {
		        //r8=c&r7=c&r6=m&r5=e&r4=c&r3=a&r2=11235&r1=&
				if (this._reg==""&&this._reg!="-") {
					var pad = $.ad.util.cookie("p_AD");
					this._reg = (pad)?';'+pad.substring(0,pad.length-1)+"united_states".replace(/&/g,";"):'-';
				}
				if (this._reg=="-") {return "";}
				return this._reg;
			},
			revsci: {
				get: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return "";}
		        		if ($.ad.dc._rs=="") {
		        			var p = ";rs=";
						var rsi = $.ad.util.cookie("rsi_segs")||"";
						if (rsi) {
							$.ad.dc._rs = p + rsi.split('|').slice(0,6).join(p).replace(/E05510_/g,"");
						} else {
							$.ad.dc._rs = "";
						}
					}
					return $.ad.dc._rs;
				},
				pre: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return;}
					$.ad.util.inject("http://js.revsci.net/gateway/gw.js?csid=E05510",'DM_tag',dm_tag);
					
					function dm_tag(){
						var data = $.ad.meta();
						DM_addToLoc("section", escape(data.channel));
						DM_addToLoc("sectionType", escape(data.ctype));
						DM_addToLoc("pageType", escape(data.ptype));
						DM_tag();
					};
				}
			},
		    sid: function(l) {
		        var p = ";sid=";
		        if (this._sid==""&&this._sid!="-") {
		        	this._sid = $.ad.util.cookie('surround')||'-';
				}
				if (this._sid=="-") {return "";}
				return l?p+this._sid:this._sid;
			},
		    adv: function(u, l) {
		        var p = ";adv=";
				if (this._adv==""&&this._adv!="-") {
		        	this._adv = $.ad.util.param(u, 'test')||$.ad.util.param(u, 'adv')||'-';
				}
				if (this._adv=="-") {return "";}
				return l?p+this._adv:this._adv;
			},
			ref: function(l) {
		        var p = ";ref=";
				if (this._ref==""&&this._ref!="-") {
					var ref_map = {
						"google": "q",
						"bing":   "q",
						"yahoo":  "p",
						"ask":    "q",
						"aol":    "query"
					};
					var dr = document.referrer;
					if (dr) {
						var ref = dr.split('/')[2].split('.');
						ref = ref[ref.length-2];
						var q = ref_map[ref];
						if (q!=undefined) {
				        	this._ref = ref+':'+$.ad.util.param(dr, q)||'-';
						} else {
							this._ref = '-';
						}
					} else {
						this._ref = '-';
					}
				}
				if (this._ref=="-") {return "";}
				return l?p+this._ref:this._ref;
			},
		    u: function(data, id) {
				//u=segs|sid|adv|topic/hub|col|ptype|pos|cat|chan|pers|show|end|format|sec|test|stream|playlist|pname|r1|r2|r3|r4|r5|r6|r7|r8|bk|kw
				var s = ',';
				var idd = id.split('-')[0];
				var metainfo = this.meta(5,'tags').replace(/;m[0-9]=/g,'|').substr(1).split('|');
				var i = 5;
				var meta = [];
				while (i--) {
					meta.push(metainfo[i]||"");
				}
				meta = meta.reverse().join('|');
				var rs = this.revsci.get().replace(/;rs=/g,s).substring(1);
				var reginfo = this.reginfo(false);
				var reg = '';
				if (reginfo=='') {
					reg = "|||||||";
				} else {
					reg = "united_states"+reginfo.substring(0,reginfo.length-1).replace(/r[0-9]=/g,'').split("&").reverse().join("|");//+"||";
				}
				//undefined
				var cat  = "";
				var chan = "";
				var pers = "";
				var show = "";
				var end  = "";
				var fmat = "";
				var sec  = "";
				var test = "";
				var strm = "";
				var play = "";
				var pnam = "";
				
				var bk = this.bk.get().replace(/;bk=/g,s).substring(1);
				var a = [rs,this.sid(false),this.adv(window.location, false),data.dc.hub,data.col,data.ptype,idd,meta,cat,chan,pers,show,end,fmat,sec,test,strm,play,pnam,reg,bk,this.kw(window.location, false)];

		        var p = ";u=";
				if (this._u[idd]==undefined) {
					this._u[idd] = a.join('|');
				}
				return p + this._u[idd];
			},
			bk: {
				get: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return "";}
		        		var p = ";bk=";
					if ($.ad.dc._bk==""&&$.ad.dc._bk!="-") {

						/*
						//first see if bluekai is passing anything
						var bk_a = [];
						//console.log("BK Campaigns: "+bk_results.campaigns.length);
						var bk_results = window.bk_results;
						if (bk_results&&bk_results.campaigns.length) {
							var camp;
							for (var i=0;i<bk_results.campaigns.length;i++) {
								camp = bk_results.campaigns[i];
								//console.log("Categories: "+camp.categories.length);
								for (var j=0;j<camp.categories.length;j++) {
									//console.log("Category: "+camp.categories[j].categoryID);
									bk_a.push(camp.categories[j].categoryID);
								}
							}
						}
						//second check the cookie
						var bk_cookie = $.ad.util.cookie('ad_bk');
						
						//if there was already something in the cookie add that to the array
						if (bk_cookie) {
							//console.log("BK Cookie: "+bk_cookie);
							var bk_ca = bk_cookie.split('|');
							for (var i=0;i<bk_ca.length;i++) {
								bk_a.push(bk_ca[i]);
							}
						}
						//add back into cookie
						if (bk_a.length) {
							//eliminate duplicates
							bk_a.sort();
							for (var i=0;i<bk_a.length;i++) {
								if (bk_a[i-1] == bk_a[i]) {
									bk_a.splice(i, 1);
									i--;
								}
							}
							$.ad.util.cookie('ad_bk', bk_a.join('|'), {path: '/', expires: 30});
						}
						//$.ad.util.cookie('ad_bk');
						*/

						if ($.ad.util.cookie('ad_bk')) {
							var bkc = $.ad.util.cookie('ad_bk').split('|');
							if (bkc.length) {
								$.ad.dc._bk = p + bkc.join(p);
							} else {
								$.ad.dc._bk = '-';
							}
						} else {
							$.ad.dc._bk = '-';
						}
					}
					if ($.ad.dc._bk=="-") {return "";}
					return $.ad.dc._bk;
				},
				pre: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return;}
					//listener pixel
					document.write("<img height=\"1\" width=\"1\" src=\"http://tags.bluekai.com/site/668\" />");

					$.ad.util.include("http://tags.bluekai.com/site/668?ret=js",1);
				},
				init: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return;}
					//first see if bluekai is passing anything
					var bk_a = [];
					//console.log("BK Campaigns: "+bk_results.campaigns.length);
					var bk_results = window.bk_results;
					if (bk_results&&bk_results.campaigns.length) {
						var camp;
						for (var i=0;i<bk_results.campaigns.length;i++) {
							camp = bk_results.campaigns[i];
							//console.log("Categories: "+camp.categories.length);
							for (var j=0;j<camp.categories.length;j++) {
								//console.log("Category: "+camp.categories[j].categoryID);
								bk_a.push(camp.categories[j].categoryID);
							}
						}
					}
					//second check the cookie
					var bk_cookie = $.ad.util.cookie('ad_bk');
					
					//if there was already something in the cookie add that to the array
					if (bk_cookie) {
						//console.log("BK Cookie: "+bk_cookie);
						var bk_ca = bk_cookie.split('|');
						for (var i=0;i<bk_ca.length;i++) {
							bk_a.push(bk_ca[i]);
						}
					}
					//add back into cookie
					if (bk_a.length) {
						//eliminate duplicates
						bk_a.sort();
						for (var i=0;i<bk_a.length;i++) {
							if (bk_a[i-1] == bk_a[i]) {
								bk_a.splice(i, 1);
								i--;
							}
						}
						$.ad.util.cookie('ad_bk', bk_a.join('|'), {path: '/', expires: 30});
					}
					//$.ad.util.cookie('ad_bk');
				}
			},
			quant: {
				get: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return "";}
		        		var p = ";qc=";
					if ($.ad.dc._qc==""&&$.ad.dc._qc!="-") {
						if ($.ad.util.cookie('__qseg')) {
							var qcc = $.ad.util.cookie('__qseg').split('|');
							var len = qcc.length;
							var i = (len>=8)?8:len;
							if (len>0) {
								var qc_a = [];
								var qc_s = "";
								for (j=0;j<i;j++) {
									qc_a = qcc[j].split("_");
									if (qc_a.length>1) {
										qc_s += p + qc_a[1];
									}
								}
								if (qc_s=="") { qc_s = "-"; }
								$.ad.dc._qc = qc_s;
							} else {
								$.ad.dc._qc = '-';
							}
						} else {
							$.ad.dc._qc = '-';
						}
					}
					if ($.ad.dc._qc=="-") {return "";}
					return $.ad.dc._qc;
				},
				pre: function() {
					//FOX-10744
					if ($.ad.meta().ptype=="slideshow") {return;}
					//listener pixel
					window._qoptions={
						qacct:"p-ddEiIs2qFSY46"
					};
					$.ad.util.include("http://edge.quantserve.com/quant.js",1);
					
					$.ad.util.include("http://pixel.quantserve.com/seg/"+window._qoptions.qacct+".js",1);
				}
			},
			init: function(data) {
				$('.ad.dc').each(function(){
					$(this).empty();
					var id=$(this).attr("id");
					var ifr = $.ad.util.iframe.create(1, 1, id);
					$(this).append(ifr);
					var tile = $.ad.dc.tile();
					if ($.ad.dc._method=="adi") {
						ifr.src=($.ad.dc.tag(data, tile, id));
					}
					if ($.ad.dc._method=="adj") {
						var doc;
						doc = $.ad.util.iframe.doc(ifr);
						doc.open();
						doc.write($.ad.dc.tag(data, tile, id));
						//doc.close();
						$.ad.util.iframe.closeDoc(doc, 0);
						//console.log("tag: " + id + "\n" + $.ad.dc.tag(data, tile, id));
					}
				});
			},
			load: function() {
				$.ad.dc.init($.ad._data);
			},
			pix: function(data) {
				if (typeof isie=="undefined" || !isie) {
					var id="pixel-1x1";
					var ifr = $.ad.util.iframe.create(1, 1, id);
					$('body').append(ifr);
					var tile = $.ad.dc.tile();
					var doc;
					doc = $.ad.util.iframe.doc(ifr);
					doc.open();
					doc.write($.ad.dc.tag(data, tile, id));
					$.ad.util.iframe.closeDoc(doc, 0);
				}
			}
	    },
	    qu: {
	        _svr: "ads.adsonar.com",
			_path: "http://js.adsonar.com/js/adsonar.js",
		    getSize: function(data, id) {
				var qu = data.qu[id];
				return {width: qu.width, height: qu.height};
			},
		    tag: function(data, id) {
				var qu = data.qu[id];
				if (qu.ps==undefined) { qu.ps = "-1"; }
		        var tag = "<style type=\"text/css\">html,body{border:0;margin:0;padding:0;}</style><scr" + "ipt type=\"text/javascript\">var adsonar_placementId="+qu.placeid+",adsonar_pid="+qu.pid+",adsonar_ps="+qu.ps+",adsonar_zw="+qu.width+" ,adsonar_rfu=\""+window.document.location+"\";adsonar_zh="+qu.height+",adsonar_jv=\""+$.ad.qu._svr+"\";</scr"+ "ipt>";//document.write(\"<iframe src='http://www.google.com'></iframe>\");
		        tag += "<scr" + "ipt src=\"" + $.ad.qu._path + "\" type=\"text/javascript\"></scr"+ "ipt>";
		        return tag;
			},
			init: function(data) {
				$('.ad.qu').each(function(){
					var id=$(this).attr("id");
					var sz = $.ad.qu.getSize(data, id);
					var ifr = $.ad.util.iframe.create(sz.width, sz.height, id);
					$(this).append(ifr);
					var doc;
					doc = $.ad.util.iframe.doc(ifr);
					doc.open();
					doc.write($.ad.qu.tag(data, $(this).attr("id")));
					$.ad.util.iframe.closeDoc(doc, 0);
				});
			}
	    },
		vib: {
			pre: function(d) {
				var channel = d.channel;
				var ptype = d.ptype;
				var vib_map_full = {
					"fnc": {
						"fnc/entertainment": 2696,
						"fnc/health": 6591,
						"fnc/politics": 4656,
						"fnc/scitech": 2699,
						"fnc/sports": 9213,
						"fnc/us": 4656,
						"fnc/weather": 4226,
						"fnc/world": 4656
					},
					"fbn": 3300,
					"imag": 4656
				};
				var vib_map = {
					"fnc/entertainment": 2696,
					"fnc/entertainment/gossip": 2696,
					"fnc/entertainment/movies": 2696,
					"fnc/entertainment/style": 2696,
					"fnc/entertainment/tv": 2696,
					"fnc/entertainment/music": 2696,
					"fnc/entertainment/gossip/poptarts": 2696,
					"fnc/entertainment/tv/realitycheck": 2696
				};
				var id = vib_map[channel];
				if (id!=undefined&&(ptype=="article"||ptype=="story")) {
					$.ad.util.include("http://foxnews.us.intellitxt.com/intellitxt/front.asp?ipid="+id,1);
				}
			}
		},
		kona: {
			pre: function(d) {
				var channel = d.raw.channel;
				var ptype = d.ptype;
				var dom = document.domain.split('.');
				dom = dom[dom.length-2];
				if (channel==undefined||channel=="") {
					if (dom=="foxnews") {channel="fnc";}
					if (dom=="foxbusiness") {channel="fbn";}
				}
				var kontera_map = {
					"fnc": 131821,
					"fbn": 134803
				};
				var id = kontera_map[channel];
				if (id!=undefined&&(ptype=="article"||ptype=="story"||ptype=="column")&&$.ad.dc.fmt()!="print"&&channel!="fnl") {
					window.dc_AdLinkColor = "blue";
					window.dc_isBoldActive= "no";
					window.dc_PublisherID = id;
					$.ad.util.include("http://kona.kontera.com/javascript/lib/KonaLibInline.js",1);
					$.ad.util.include("http://ad.doubleclick.net/adj/"+channel+";pos=kontera;sz=1x1;ord=" + $.ad.ord + "?",1);
				}
			}
		},
		spot: {
			pre: function(d) {
				var channel = d.channel;
				var ptype = d.ptype;
				var spot_map = {
					"fnc/root":                 {"type": "homep152", "cat": "foxne823"},
					"fnc/politics":             {"type": "polit789", "cat": "foxne212"},
					"fnc/politics/president":   {"type": "polit789", "cat": "foxne086"},
					"fnc/politics/senate":      {"type": "polit789", "cat": "foxne558"},
					"fnc/politics/house":       {"type": "polit789", "cat": "foxne164"},
					"fnc/politics/state-local": {"type": "polit789", "cat": "foxne497"},
					"fnc/politics/courts":      {"type": "polit789", "cat": "foxne596"},
					"fnc/politics/pentagon":    {"type": "polit789", "cat": "foxne948"},
					"fnc/politics/elections":   {"type": "polit789", "cat": "foxne200"}
				};
				var id = spot_map[channel];
				if (id!=undefined&&ptype=="channel") {
					var a = Math.ceil(Math.random()*10000000000000);
					document.write("<iframe src=\"http://fls.doubleclick.net/activityi;src=2852600;type="+id.type+";cat="+id.cat+";ord="+a+"?\" width=\"1\" height=\"1\" frameborder=\"0\"></iframe>");
				}
			}
		},
		hbx: {
			init: function() {
				//stub
			},
			strip: function(a) {
				a = a.split("|").join("");
				a = a.split("&").join("");
				a = a.split("'").join("");
				a = a.split("#").join("");
				a = a.split("$").join("");
				a = a.split("%").join("");
				a = a.split("^").join("");
				a = a.split("*").join("");
				a = a.split(":").join("");
				a = a.split("!").join("");
				a = a.split("<").join("");
				a = a.split(">").join("");
				a = a.split("~").join("");
				a = a.split(";").join("");
				a = a.split(" ").join("+");
				return a;
			},
			evt: function(a,b) {
				b=_hbE[_hbEC++]={};
				b._N=a;
				b._C=0;
				return b;
			},
			load: function() {
				if (document.domain=="www.foxnews.com") {
					$('#hbx-load').remove();
					$('body').append("<iframe id=\"hbx-load\" src=\"http://www.foxnews.com/js/hbx-load.html\" style=\"display:none;width:1px;height:1px;\"></iframe>");
				}
			}
		},
		loom: {
			pre: function(d) {
				window.L_VARS = {};
				var channel = d.raw.channel;
				if (channel==undefined||channel=="") {
					var dom = document.domain.split('.');
					dom = dom[dom.length-2];
					if (dom=="foxnews") {channel="fnc";}
					if (dom=="foxbusiness") {channel="fbn";}
					if (dom=="foxsmallbusinesscenter") {channel="sbc";}
				}
				var ptype = d.ptype;
				var loom_map = {
					"fnc": 1566965288,
					"fbn": 4068804847,
					"sbc": 4068804847
				};
				var id = loom_map[channel];
				if (id!=undefined&&(ptype=="article"||ptype=="story"||ptype=="column")) {
					window.L_VARS.publisher_key=id;
					window.L_VARS.guid=window.location.href;
					if (id==1566965288) {
						window.L_VARS.mapset ="foxnews_videos";
						window.L_VARS.zone=6;
					}
					window.L_VARS.anchor="loomia_display";
					$.ad.util.include("http://widget-cache.loomia.com/js/onewidget_clix.js",2);			
				}
			}
		},
		bay: {
			init: function(d) {
				var ptype = d.ptype;
				var bay_map = {
					"article":"",
					"column":"",
					"post":"",
					"slideshow":"",
					"video":""
				};
				if (ptype in bay_map&&$.ad.dc.fmt()!="print") {
					var script_url = "baynote.js";
					var path = d.raw.channel+"/"+d.raw.section;
					var baynote_script_url_map = {
						"fnl/.+": "fn-latino-baynote.js"
					};
					script_url = (function(o,p) {
						for (var i in o) {
							if (p.match(new RegExp(i))) {
								return o[i];
							}
						}
					})(baynote_script_url_map,path)||script_url;
					$.ad.util.include("http://ads.foxnews.com/js/"+script_url,2);
				}
			}
		},
		niel: {
			pre: function(d) {
				var channel = d.raw.channel;
				var niel_map = {
					"fnc": "us-801796h",
					"fnl": "us-801796h",
					"fbn": "us-201978h",
					"fsb": "us-201978h"
				};
				var id = niel_map[channel];
				if (id!=undefined) {
					var im = new Image(1, 1);
					im.onerror = im.onload = function() {
						im.onerror = im.onload = null;
					};
					im.src = ["//secure-us.imrworldwide.com/cgi-bin/m?ci=", id, "&cg=0&cc=1&si=", escape(window.location.href), "&rp=", escape(document.referrer), "&ts=compact&rnd=", (new Date()).getTime()].join('');
				}
			}
		},
		goog: {
			pre: function(d) {
			}
		},
		omni: {
			pre: function(d) {
				var s_account = "foxnewsexception";
				var path = d.raw.channel+"/"+d.raw.section;
				var omniture_rsid_map = {
					"fnc/nation": "foxnewsfoxnation",
					"fnc/.+": "foxnews",
					"fnl/.+": "foxnewsfoxlatino",
					"fsb/.+": "foxnewssmallbusinesscenter",
					"fbn/.+": "foxnewsbusinessprod"
				};
				s_account = (function(o,p) {
					for (var i in o) {
						if (p.match(new RegExp(i))) {
							return omniture_rsid_map[i];
						}
					}
				})(omniture_rsid_map,path)||s_account;
				window.s_account = s_account;
				$.ad.util.include("http://ads.foxnews.com/js/omtr_code.js",1);					
			},
			init: function(d) {
				var omtr = window.omtr;
				var hier = [d.raw.channel, d.raw.section, d.raw.subsection1, d.raw.subsection2, d.raw.subsection3, d.raw.subsection4];
				
				var channel = clean(0,6,':') || 'undefined';
				
				omtr.pageName = channel+':'+d.ctype+':'+d.ptype;
				omtr.channel = d.raw.section;
				omtr.server = ""; /////////////////////////////////////////////////////////////////////////////////////////////
				omtr.prop1 = d.raw.section;
				omtr.prop2 = clean(1,3,'/');
				omtr.prop3 = clean(1,4,'/');
				omtr.prop4 = clean(1,5,'/');
				
				//auto:
				omtr.prop5 = "";
				omtr.prop6 = "";
				omtr.prop7 = "";
				omtr.prop8 = "";
				
				//////////////////////// add other detail ptypes //////////////////////////
				var omniture_c_map = {
					"article": "",
					"bio": "",
					"column": "",
					"post": "",
					"slideshow": "",
					"story": "",
					"transcript": ""
				};
				if (d.ptype in omniture_c_map) {
					var def = "N/A";
					omtr.prop11 = d.pageid.split(':')[2]||def;
					omtr.prop12 = d.raw.title||def;
					omtr.prop13 = d.ptype||def;
					omtr.prop14 = d.raw.creator||def;
					omtr.prop15 = d.raw.source||def;
					omtr.prop16 = d.raw.subsection4||d.raw.subsection3||d.raw.subsection2||d.raw.subsection1||d.raw.section||def;
				} else {
					omtr.prop11 = "";
					omtr.prop12 = "";
					omtr.prop13 = "";
					omtr.prop14 = "";
					omtr.prop15 = "";
					omtr.prop16 = "";
				}
				
				//auto:
				omtr.prop22 = "";
				omtr.prop23 = "";
				omtr.prop24 = "";
				omtr.prop25 = "";
				omtr.prop26 = "";
				omtr.prop27 = "";
				omtr.prop41 = "";
				omtr.prop42 = "";
				
				if (d.ptype=='slideshow') {
					omtr.prop49 = d.raw.title;
					omtr.eVar50 = $.ad.util.param(window.location.href, 'slide');
				}
				
				omtr.hier1 = clean(0,6,',');
				
				//auto:
				omtr.hier2 = "";
				omtr.hier3 = "";
				
				//event1
				//event26
				
				function clean(a,b,d) {
					var r = new RegExp(d+"+", "g");
					var s = hier.slice(a,b).join(d).replace(r, d);
					if (s.substr(s.length-1,s.length-1)==d) {
						s = s.substr(0,s.length-1);
					}
					return s;
				}
				/************* DO NOT ALTER ANYTHING BELOW THIS LINE ! **************/
				//window.s = omtr;
				//window.s.t();
				omtr.t();
			},
			load: function(d) {
				var omtr = window.omtr;
				if (d!=undefined) {
					for (key in d) {
						var f = $.ad.omni.map[key];
						if (d.hasOwnProperty(key)&&f!=undefined) {
							//omtr[f]=d[key];
							f(d[key]);
						}
					}
				}
				omtr.t();
				//console.log(omtr);
			},
			map: {
				"slide": function(x){window.omtr.eVar50=x;window.omtr.events="event26,event1";},
				"print": function(){window.omtr.linkTrackVars="eVar11,eVar12,eVar13,eVar14,eVar15,events";window.omtr.linkTrackEvents="event27";window.omtr.events="event27";window.omtr.tl('','o','print');},
				"print-open": function(){window.omtr.linkTrackVars="eVar11,eVar12,eVar13,eVar14,eVar15,events";window.omtr.linkTrackEvents="event32";window.omtr.events="event32";window.omtr.tl(window.location.href,'o','print-open');},
				"video-playlist": function(d){window.omtr.eVar39=d.title||"N/A";window.omtr.linkTrackVars="eVar39,events";window.omtr.linkTrackEvents="event20";window.omtr.events="event20";window.omtr.tl('','o','video-playlist');},
				"video-thumbnail": function(d){window.omtr.eVar31=d.guid||"N/A";window.omtr.eVar32=d.show||"N/A";window.omtr.eVar33=d.category||"N/A";window.omtr.eVar34=d.personality||"N/A";window.omtr.eVar39=d.playlistTitle||"N/A";window.omtr.eVar52=d.title||"N/A";window.omtr.linkTrackVars="eVar31,eVar32,eVar33,eVar34,eVar39,eVar52,events";window.omtr.linkTrackEvents="event35";window.omtr.events="event35";window.omtr.tl(d.url,'o','video-thumbnail');}
			}
		},
		chart: {
			load: function(url) {
				if (typeof pSUPERFLY=="object") {
					pSUPERFLY.virtualPage(url);
				}
			},
			start: function() {
				window._sf_startpt=(new Date()).getTime();
			},
			init: function(id, dom) {
				window._sf_endpt=(new Date()).getTime();
				window._sf_async_config = {uid:id, domain:dom, pingServer:"pfox.chartbeat.net", title:$.ad.meta().raw.title, path:window.location.pathname};
				var authors = $.grep([$.ad.meta().raw.creator, $.ad.meta().raw.contributor, $.ad.meta().raw.source], function(n,i){return(n);}).join(',');
				var sections = $.ad.meta().raw.section;
				if (authors) { _sf_async_config.authors = authors; }
				if (sections) { _sf_async_config.sections = sections; }
				$.ad.util.include(document.location.protocol+"//static.chartbeat.com/js/chartbeat.js",2);
			},
			pre: function(d) {
				var channel = d.raw.channel;
				var id = 8971;
				if (id!=undefined) {
					var dom = document.domain.split('.').slice(-2).join('.');
					var path = d.raw.channel+"/"+d.raw.section;
					var domain_exceptions_map = {
						"fnl/.+": "latino.foxnews.com",
						"fnc/nation": "nation.foxnews.com",
						"fsb/.+": "smallbusiness.foxbusiness.com"
					};
					dom = (function(o,p) {
						for (var i in o) {
							if (p.match(new RegExp(i))) {
								return o[i];
							}
						}
					})(domain_exceptions_map,path)||dom;
					var w_onload = window.onload;
					window.onload = (typeof window.onload!="function")?function(){$.ad.chart.init(id,dom);}:function(){w_onload();$.ad.chart.init(id,dom);};
				}
			}
		},
		coms: {
			pre: function(d) {
				var src = ("https:"==document.location.protocol?"https://sb":"http://b")+'.scorecardresearch.com/beacon.js';
				$.ad.util.inject(src,'COMSCORE',function(){window.COMSCORE.beacon({c1:2,c2:"9013181",c3:"",c4:window.location.protocol+"//"+window.location.host+window.location.pathname,c5:"",c6:"",c15:""});});
			}
		},
		fim: {
			pre: function() {
				window._fanpid="314-000262";
				if ("undefined"==typeof _fan) {
					var src = ("https:"==document.location.protocol?"https://":"http://")+'trgj.opt.fimserve.com/fp.js';
					$.ad.util.include(src, 2);
				}
			}
		},
		track: function(d) {
			var track_map = {
				"view": function(d){$.ad.omni.init(d);$.ad.hbx.load();$.ad.niel.pre(d);$.ad.chart.load('/');},
				"slide": function(x){$.ad.omni.load({"slide": x});$.ad.hbx.load();$.ad.niel.pre($.ad.meta());$.ad.chart.load('/'+x);},
				"email": function(d){$.ad.omni.load({"email": d});},
				"print": function(d){$.ad.omni.load({"print": d});},
				"print-open": function(d){$.ad.omni.load({"print-open": d});},
				"video-playlist": function(d){},
				"video-thumbnail": function(d){$.ad.omni.load({"video-thumbnail": d});$.ad.niel.pre($.ad.meta());$.ad.chart.load(d.url);}
			};
			if (d!=undefined) {
				for (key in d) {
					var f = track_map[key];
					if (d.hasOwnProperty(key)&&f!=undefined) {
						f(d[key]);
					}
				}
			}
		},
		prt: {
			pre: function(d) {
				var channel = d.raw.channel;
				if (channel==undefined||channel=="") {
					var dom = document.domain.split('.');
					dom = dom[dom.length-2];
					if (dom=="foxnews") {channel="fnc";}
					if (dom=="foxbusiness") {channel="fbn";}
					if (dom=="foxsmallbusinesscenter") {channel="sbc";}
				}
				var format_dynamics_map = {
					"fnc": 2640,
					"fbn": 2641,
					"sbc": 2641
				};
				var id = format_dynamics_map[channel];
				if (id!=undefined) {
					$.ad.util.include("http://cache-01.cleanprint.net/cp/ccg?divId="+id,2);			
				}
			}
		},
		tynt: {
			init: function(d) {
				var ptype = d.ptype;
				var tynt_map = {
					"article":"",
					"bio":"",
					"column":"",
					"post":"",
					"slideshow":"",
					"transcript":""
				};
				if (ptype in tynt_map&&document.location.protocol=='http:') {
					window.Tynt = window.Tynt||[];
					var tynt_id = "";
					var path = d.raw.channel+"/"+d.raw.section;
					var tynt_id_map = {
						"fnc/.+": "bYSjoEJmOr3Qsfab7jrHcU",
						"fnl/.+": "aK7Hk8XCCr34e2adbi-bpO",
						"fsb/.+": "bQcl_G_lKr363qadbi-bnq",
						"fbn/.+": "aZzN--W0Gr3OhQab7jrHcU"
					};
					tynt_id = (function(o,p) {
						for (var i in o) {
							if (p.match(new RegExp(i))) {
								return o[i];
							}
						}
					})(tynt_id_map,path)||tynt_id;
					Tynt.push(tynt_id);
					$.ad.util.include("http://tcr.tynt.com/ti.js",2);
				}
			}
		},
		util: {
			getLRC: function(s) {
		        if (s=="") { return 0; }
		        var len = s.length;
		        var lrc = 0;
		        for (var i = 0;i < len;i++) {
		            lrc += lrc ^ s.charCodeAt(i);
		        }
		        return lrc;
		    },
			enc: function(s) {
			    //return $.ad.util.base_convert_range($.ad.util.md5(s), 16, 10).substring(0, 16).replace(/\./, "");
			    return $.ad.util.md5(s);
			},
			iframe: {
				create: function(w,h,id) {
					var ifr = document.createElement('iframe');
					ifr.setAttribute('id','ifr-'+id);
					ifr.setAttribute('border', 0);
					ifr.setAttribute('scrolling', 'no');
					ifr.setAttribute('frameBorder', 0);
					ifr.setAttribute('width', w);
					ifr.setAttribute('height', h);
					return ifr;
		    	},
				doc: function(ifr) {
					if (ifr.contentDocument) {
						// For NS6
						return ifr.contentDocument;
					} else if (ifr.contentWindow) {
						// For IE5.5 and IE6
						return ifr.contentWindow.document;
					} else if (ifr.document) {
						// For IE5
						return ifr.document;
					} else {
						return;
					}
				},
				closeDoc: function(doc, i) {
					if (i==10) {
						doc.close();
					} else {
						setTimeout(function(){$.ad.util.iframe.closeDoc(doc,i+1);},20);
					}
				},
				resize: function(id, w, h) {
					$('#'+id).css({width:w, height:h});
					//console.log('iframe ad: ' + id + ' w:' + w + ' h:' + h);
				}
			},
			param: function(u, name) {
				u = unescape(u);
				var match=(new RegExp("[\\?&#]"+name+"=([^&#]*)")).exec(u);
				if (!match) {
					return '';
				} else {
					return match[1].replace(/ /g, "-").replace(/[^a-zA-Z0-9-]/g, "").toLowerCase();
				}
			},
			cookie: $.cookie || function(name, value, options) {
				if (typeof value != 'undefined') {
					options = options || {};
					if (value === null) {
						value = '';
						options.expires = -1;
					}
					var expires = '';
					if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
						var date;
						if (typeof options.expires == 'number') {
							date = new Date();
							date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
						} else {
							date = options.expires;
						}
						expires = '; expires=' + date.toUTCString();
					}
					var path = options.path ? '; path=' + (options.path) : '';
					var domain = options.domain ? '; domain=' + (options.domain) : '';
					var secure = options.secure ? '; secure' : '';
					document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
				} else {
					var cookieValue = null;
					if (document.cookie && document.cookie != '') {
						var cookies = document.cookie.split(';');
						for (var i = 0; i < cookies.length; i++) {
							var cookie = $.ad.util.trim(cookies[i]);
							if (cookie.substring(0, name.length + 1) == (name + '=')) {
								cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
								break;
							}
						}
					}
					return cookieValue;
				}
				return "";
			},
			trim: $.trim || function(s) {
				return s.replace(/^\s+|\s+$/g,"");
			},
			md5: function(str) {
			    var xl;
			    var rotateLeft = function(lValue, iShiftBits) {
			        return (lValue<<iShiftBits) | (lValue>>>(32-iShiftBits));
			    };
			    var addUnsigned = function(lX,lY) {
			        var lX4,lY4,lX8,lY8,lResult;
			        lX8 = (lX & 0x80000000);
			        lY8 = (lY & 0x80000000);
			        lX4 = (lX & 0x40000000);
			        lY4 = (lY & 0x40000000);
			        lResult = (lX & 0x3FFFFFFF)+(lY & 0x3FFFFFFF);
			        if (lX4 & lY4) {
			            return (lResult ^ 0x80000000 ^ lX8 ^ lY8);
			        }
			        if (lX4 | lY4) {
			            if (lResult & 0x40000000) {
			                return (lResult ^ 0xC0000000 ^ lX8 ^ lY8);
			            } else {
			                return (lResult ^ 0x40000000 ^ lX8 ^ lY8);
			            }
			        } else {
			            return (lResult ^ lX8 ^ lY8);
			        }
			    };
			    var _F = function(x,y,z) { return (x & y) | ((~x) & z); };
			    var _G = function(x,y,z) { return (x & z) | (y & (~z)); };
			    var _H = function(x,y,z) { return (x ^ y ^ z); };
			    var _I = function(x,y,z) { return (y ^ (x | (~z))); };
			 
			    var _FF = function(a,b,c,d,x,s,ac) {
			        a = addUnsigned(a, addUnsigned(addUnsigned(_F(b, c, d), x), ac));
			        return addUnsigned(rotateLeft(a, s), b);
			    };
			    var _GG = function(a,b,c,d,x,s,ac) {
			        a = addUnsigned(a, addUnsigned(addUnsigned(_G(b, c, d), x), ac));
			        return addUnsigned(rotateLeft(a, s), b);
			    };
			    var _HH = function(a,b,c,d,x,s,ac) {
			        a = addUnsigned(a, addUnsigned(addUnsigned(_H(b, c, d), x), ac));
			        return addUnsigned(rotateLeft(a, s), b);
			    };
			    var _II = function(a,b,c,d,x,s,ac) {
			        a = addUnsigned(a, addUnsigned(addUnsigned(_I(b, c, d), x), ac));
			        return addUnsigned(rotateLeft(a, s), b);
			    };
			    var convertToWordArray = function(str) {
			        var lWordCount;
			        var lMessageLength = str.length;
			        var lNumberOfWords_temp1=lMessageLength + 8;
			        var lNumberOfWords_temp2=(lNumberOfWords_temp1-(lNumberOfWords_temp1 % 64))/64;
			        var lNumberOfWords = (lNumberOfWords_temp2+1)*16;
			        var lWordArray=new Array(lNumberOfWords-1);
			        var lBytePosition = 0;
			        var lByteCount = 0;
			        while ( lByteCount < lMessageLength ) {
			            lWordCount = (lByteCount-(lByteCount % 4))/4;
			            lBytePosition = (lByteCount % 4)*8;
			            lWordArray[lWordCount] = (lWordArray[lWordCount] | (str.charCodeAt(lByteCount)<<lBytePosition));
			            lByteCount++;
			        }
			        lWordCount = (lByteCount-(lByteCount % 4))/4;
			        lBytePosition = (lByteCount % 4)*8;
			        lWordArray[lWordCount] = lWordArray[lWordCount] | (0x80<<lBytePosition);
			        lWordArray[lNumberOfWords-2] = lMessageLength<<3;
			        lWordArray[lNumberOfWords-1] = lMessageLength>>>29;
			        return lWordArray;
			    };
			    var wordToHex = function(lValue) {
			        var wordToHexValue="",wordToHexValue_temp="",lByte,lCount;
			        for (lCount = 0;lCount<=3;lCount++) {
			            lByte = (lValue>>>(lCount*8)) & 255;
			            wordToHexValue_temp = "0" + lByte.toString(16);
			            wordToHexValue = wordToHexValue + wordToHexValue_temp.substr(wordToHexValue_temp.length-2,2);
			        }
			        return wordToHexValue;
			    };
			    var x=[],
			        k,AA,BB,CC,DD,a,b,c,d,
			        S11=7, S12=12, S13=17, S14=22,
			        S21=5, S22=9 , S23=14, S24=20,
			        S31=4, S32=11, S33=16, S34=23,
			        S41=6, S42=10, S43=15, S44=21;
			 
			    str = $.ad.util.utf8_encode(str);
			    x = convertToWordArray(str);
			    a = 0x67452301; b = 0xEFCDAB89; c = 0x98BADCFE; d = 0x10325476;
			    
			    xl = x.length;
			    for (k=0;k<xl;k+=16) {
			        AA=a; BB=b; CC=c; DD=d;
			        a=_FF(a,b,c,d,x[k+0], S11,0xD76AA478);
			        d=_FF(d,a,b,c,x[k+1], S12,0xE8C7B756);
			        c=_FF(c,d,a,b,x[k+2], S13,0x242070DB);
			        b=_FF(b,c,d,a,x[k+3], S14,0xC1BDCEEE);
			        a=_FF(a,b,c,d,x[k+4], S11,0xF57C0FAF);
			        d=_FF(d,a,b,c,x[k+5], S12,0x4787C62A);
			        c=_FF(c,d,a,b,x[k+6], S13,0xA8304613);
			        b=_FF(b,c,d,a,x[k+7], S14,0xFD469501);
			        a=_FF(a,b,c,d,x[k+8], S11,0x698098D8);
			        d=_FF(d,a,b,c,x[k+9], S12,0x8B44F7AF);
			        c=_FF(c,d,a,b,x[k+10],S13,0xFFFF5BB1);
			        b=_FF(b,c,d,a,x[k+11],S14,0x895CD7BE);
			        a=_FF(a,b,c,d,x[k+12],S11,0x6B901122);
			        d=_FF(d,a,b,c,x[k+13],S12,0xFD987193);
			        c=_FF(c,d,a,b,x[k+14],S13,0xA679438E);
			        b=_FF(b,c,d,a,x[k+15],S14,0x49B40821);
			        a=_GG(a,b,c,d,x[k+1], S21,0xF61E2562);
			        d=_GG(d,a,b,c,x[k+6], S22,0xC040B340);
			        c=_GG(c,d,a,b,x[k+11],S23,0x265E5A51);
			        b=_GG(b,c,d,a,x[k+0], S24,0xE9B6C7AA);
			        a=_GG(a,b,c,d,x[k+5], S21,0xD62F105D);
			        d=_GG(d,a,b,c,x[k+10],S22,0x2441453);
			        c=_GG(c,d,a,b,x[k+15],S23,0xD8A1E681);
			        b=_GG(b,c,d,a,x[k+4], S24,0xE7D3FBC8);
			        a=_GG(a,b,c,d,x[k+9], S21,0x21E1CDE6);
			        d=_GG(d,a,b,c,x[k+14],S22,0xC33707D6);
			        c=_GG(c,d,a,b,x[k+3], S23,0xF4D50D87);
			        b=_GG(b,c,d,a,x[k+8], S24,0x455A14ED);
			        a=_GG(a,b,c,d,x[k+13],S21,0xA9E3E905);
			        d=_GG(d,a,b,c,x[k+2], S22,0xFCEFA3F8);
			        c=_GG(c,d,a,b,x[k+7], S23,0x676F02D9);
			        b=_GG(b,c,d,a,x[k+12],S24,0x8D2A4C8A);
			        a=_HH(a,b,c,d,x[k+5], S31,0xFFFA3942);
			        d=_HH(d,a,b,c,x[k+8], S32,0x8771F681);
			        c=_HH(c,d,a,b,x[k+11],S33,0x6D9D6122);
			        b=_HH(b,c,d,a,x[k+14],S34,0xFDE5380C);
			        a=_HH(a,b,c,d,x[k+1], S31,0xA4BEEA44);
			        d=_HH(d,a,b,c,x[k+4], S32,0x4BDECFA9);
			        c=_HH(c,d,a,b,x[k+7], S33,0xF6BB4B60);
			        b=_HH(b,c,d,a,x[k+10],S34,0xBEBFBC70);
			        a=_HH(a,b,c,d,x[k+13],S31,0x289B7EC6);
			        d=_HH(d,a,b,c,x[k+0], S32,0xEAA127FA);
			        c=_HH(c,d,a,b,x[k+3], S33,0xD4EF3085);
			        b=_HH(b,c,d,a,x[k+6], S34,0x4881D05);
			        a=_HH(a,b,c,d,x[k+9], S31,0xD9D4D039);
			        d=_HH(d,a,b,c,x[k+12],S32,0xE6DB99E5);
			        c=_HH(c,d,a,b,x[k+15],S33,0x1FA27CF8);
			        b=_HH(b,c,d,a,x[k+2], S34,0xC4AC5665);
			        a=_II(a,b,c,d,x[k+0], S41,0xF4292244);
			        d=_II(d,a,b,c,x[k+7], S42,0x432AFF97);
			        c=_II(c,d,a,b,x[k+14],S43,0xAB9423A7);
			        b=_II(b,c,d,a,x[k+5], S44,0xFC93A039);
			        a=_II(a,b,c,d,x[k+12],S41,0x655B59C3);
			        d=_II(d,a,b,c,x[k+3], S42,0x8F0CCC92);
			        c=_II(c,d,a,b,x[k+10],S43,0xFFEFF47D);
			        b=_II(b,c,d,a,x[k+1], S44,0x85845DD1);
			        a=_II(a,b,c,d,x[k+8], S41,0x6FA87E4F);
			        d=_II(d,a,b,c,x[k+15],S42,0xFE2CE6E0);
			        c=_II(c,d,a,b,x[k+6], S43,0xA3014314);
			        b=_II(b,c,d,a,x[k+13],S44,0x4E0811A1);
			        a=_II(a,b,c,d,x[k+4], S41,0xF7537E82);
			        d=_II(d,a,b,c,x[k+11],S42,0xBD3AF235);
			        c=_II(c,d,a,b,x[k+2], S43,0x2AD7D2BB);
			        b=_II(b,c,d,a,x[k+9], S44,0xEB86D391);
			        a=addUnsigned(a,AA);
			        b=addUnsigned(b,BB);
			        c=addUnsigned(c,CC);
			        d=addUnsigned(d,DD);
			    }
			    var temp = wordToHex(a)+wordToHex(b)+wordToHex(c)+wordToHex(d);
			    return temp.toLowerCase();
			},
			utf8_encode: function(argString) {
			    var string = (argString+'').replace(/\r\n/g, "\n").replace(/\r/g, "\n");
			    var utftext = "";
			    var start, end;
			    var stringl = 0;
			
			    start = end = 0;
			    stringl = string.length;
			    for (var n = 0; n < stringl; n++) {
			        var c1 = string.charCodeAt(n);
			        var enc = null;
			
			        if (c1 < 128) {
			            end++;
			        } else if((c1 > 127) && (c1 < 2048)) {
			            enc = String.fromCharCode((c1 >> 6) | 192) + String.fromCharCode((c1 & 63) | 128);
			        } else {
			            enc = String.fromCharCode((c1 >> 12) | 224) + String.fromCharCode(((c1 >> 6) & 63) | 128) + String.fromCharCode((c1 & 63) | 128);
			        }
			        if (enc !== null) {
			            if (end > start) {
			                utftext += string.substring(start, end);
			            }
			            utftext += enc;
			            start = end = n+1;
			        }
			    }
			    if (end > start) {
			        utftext += string.substring(start, string.length);
			    }
			    return utftext;
			},
			base_convert: function(number, frombase, tobase) {
			    return parseInt(number+'', frombase+0).toString(tobase+0);
			},
			base_convert_range: function(number, frombase, tobase) {
			    return (parseInt(number+'', frombase+0)*10).toString(tobase+0);
			},
			include: function(url, m) {
				//var ifr = $.ad.util.iframe.create(0, 0, 'sc-'+$.ad.util.enc(url));
				//$('body').append(ifr);
				//var doc;
				//doc = $.ad.util.iframe.doc(ifr);
				//doc.open();
				//doc.write(unescape("%3Cscript src=\"" + url + "\" type=\"text/javascript\"%3E%3C/script%3E"));
				//$.ad.util.iframe.closeDoc(doc, 0);

				//if (m===1) {document.write(unescape("%3Cscript src=\"" + url + "\" type=\"text/javascript\"%3E%3C/script%3E"));}
				//if (m===2)   {$('head:first').append("%3Cscript src=\"" + url + "\" type=\"text/javascript\"%3E%3C/script%3E");}

				//var scr = unescape("%3Cscript src=\""+url+"\" type=\"text/javascript\"%3E%3C/script%3E");
				//var f = m===1?function(scr){document.write(scr);}:function(scr){$('head:first').append(scr);};
				//f(scr);

				var f = m===1?function(url){var scr = unescape("%3Cscript src=\""+url+"\" type=\"text/javascript\"%3E%3C/script%3E");document.write(scr);}:function(url){var s=document.createElement("script");s.src=url;document.getElementsByTagName("head")[0].appendChild(s);};
				f(url);
			},
			inject: function(url, vr, cb) {
				var scr = unescape("%3Cscript src=\""+url+"\" type=\"text/javascript\"%3E%3C/script%3E");
				document.write(scr);
				var i = 0;
				if (window[vr]==undefined) {
					var _timer = setInterval(check, 50);
				}
				function check() {
					if (window[vr]!=undefined) {
						clearInterval(_timer);
						_timer = null;
						cb();
						return 1;
					} else {
						i++;
							if (i>20) {
							clearInterval(_timer);
							_timer = null;
							//alert(i +' timed out');
							return 0;
						}
					}
				}
			},
			obj: {
				empty: function(obj) {
				    for (var p in obj) {
				        if (obj.hasOwnProperty(p)) {
				            return false;
				        }
				    }
				    return true;
				},
				clone: function(obj) {
				    //will work with arrays too:
				    var temp = (obj instanceof Array) ? [] : {};
				    for (myvar in obj) {
				        if (obj[i] && typeof obj[myvar] == "object") {
				            temp[myvar] = $.util.obj.clone(obj[myvar]);
				        } else {
				            temp[myvar] = obj[myvar];
				        }
				    }
				    return temp;
				}
			}
		},
		pre: function() {
			if (window._ad_pre_called==undefined) {
				var d = $.ad.meta();

				//comscore
				$.ad.coms.pre(d);

				//spot
				//$.ad.spot.pre(d);
				//rev sci
				$.ad.dc.revsci.pre();
				//bluekai
				$.ad.dc.bk.pre();
				//quantcast
				$.ad.dc.quant.pre();
				//vibrantmedia
				//$.ad.vib.pre(d);
				//kontera
				$.ad.kona.pre(d);
				
				//loomia
				//$.ad.loom.pre(d);
				//nielsen
				$.ad.niel.pre(d);
	 			//google
				$.ad.goog.pre(d);
				//chartbeat
				$.ad.chart.pre(d);
				//omniture				
				$.ad.omni.pre(d);
				window._ad_pre_called=true;

				//fim
				//$.ad.fim.pre();
				//format dynamics
				//$.ad.prt.pre(d);
			}
		},
		init: function() {
			var d = $.ad.meta();
			var m = "server";

			$.ad.util.cookie('surround', window.sid, {path: '/'});

			//bluekai
			$.ad.dc.bk.init();

			//baynote
			$.ad.bay.init(d);

			//omniture
			$.ad.omni.init(d);

			//tynt
			$.ad.tynt.init(d);

			var col = "";
			if (d.ptype==="column"||d.ctype==="column") { var col = "-" + d.channel.split('/').slice(-1)[0]; }
			var url = "http://ads.foxnews.com/api/"+$.ad.util.enc(d.channel)+"-"+d.ptype.toLowerCase()+col+"-data.js?callback=jQuery.ad.callback&jsonp=?";
			if (m=="server") {
				$.getJSON(url);
			}
			if (m=="client") {
				var data = $.ad.lookup(d);
				$.ad.invoke([$.ad.dc.init, $.ad.qu.init, $.ad.hbx.init], data);
			}
		},
		callback: function(data) {
			$.ad.invoke([$.ad.dc.init, $.ad.qu.init, $.ad.hbx.init], data);
			$.ad._data = data;
		},
		invoke: function(fs, data) {
		    //for (f in fs) {
			for (f=0;f<fs.length;f++) {
			    fs[f](data);
			}
		},
		meta: function() {
			if ($.ad.util.obj.empty($.ad._meta)) {
				var channel = "", ptype = "", ctype = "", pageid = "", categories = [], d = {};
				var arr = ['dc', 'prism'];
				var t = {};
				var m = document.getElementsByTagName('meta');
				for (var i = 0;i<m.length;i++){
				  	var n = m[i].name;
				  	if(n&&arr.indexOf(n.split('.')[0])>-1){
				    	t[n.split('.')[1]] = m[i].content;
				  	}
				}
				if (t.channel==undefined && window.pdata!=undefined) {
					d = window.pdata;
					channel    = d.breadcrumb;
					ptype      = d.page_type;
					ctype      = d.channel_type;
					pageid     = d.page_id;
					categories = d.categories;
				} else if (t.channel==undefined && window.pd!=undefined) {
					d = window.pd;
					channel    = d.breadcrumb;
					ptype      = d.page_type;
					ctype      = d.channel_type;
					pageid     = d.vcmid;
					categories = d.categories;
				} else {
					channel    = (t.channel+"/"+t.section+(t.subsection1?"/"+t.subsection1:"")+(t.subsection2?"/"+t.subsection2:"")+(t.subsection3?"/"+t.subsection3:"")+(t.subsection4?"/"+t.subsection4:"")).replace(/&/g,'').replace(/ +/g,' ').replace(/ /g,"-").toLowerCase();
					ptype      = type(t.type, t.genre, t.aggregationType);
					ctype      = t.aggregationType; 
					pageid     = t.identifier||0;
					categories = t.subject?t.subject.split(','):"";
				}
				//return {channel: channel, ptype: ptype, ctype: ctype, pageid: pageid, categories: categories, raw: t};
				$.ad._meta = {channel: channel, ptype: ptype, ctype: ctype, pageid: pageid, categories: categories, raw: t};
				//alert("channel: "+channel+", ptype: "+ptype+", ctype: "+ctype+", pageid: "+pageid);
			}
			return $.ad._meta;
			//return {channel: channel, ptype: ptype, ctype: ctype, pageid: pageid, categories: categories, raw: t};
			
			function type(t, g, a) {
				//t = dc.type, g = prism.genre, a = prism.aggregationType
				if (t=="Text.Article"&&g=="")  { return a=="column"?"column":"article"; }
				if (t=="Text.Article"&&g=="column")  { return "column"; }
				if (t=="Text"&&g=="biography") { return "bio"; }
				if (t=="Collection"&&g=="homePage") { if (a=="topic") { return a; } return "channel"; }// return a;
				if (t=="InteractiveResource"&&g=="interactiveContent") { return "interactive"; }
				if (t=="Text.Article"&&g=="blogEntry") { return "post"; }
				if (t=="PhysicalObject"&&g=="productDescription") { return "product"; }
				if (t=="Data.Statistical"&&g=="stockQuote") { return "quote"; }
				if (t=="InteractiveResource"&&g=="photoEssay") { return "slideshow"; }
				if (t=="Text"&&g=="transcript") { return "transcript"; }
				if (t=="MovingImage"&&g=="") { return "video"; }// (possibly use prism.presentationtype as a scheme on the dc.type element - "video")
				return "other";
			}
		},
		lookup: function(d) {
			//use with /Home
			//var key = d.path.split('/').slice(1).join('_').toLowerCase();
			//new with fnc
			var key = d.path.split('/').join('_').toLowerCase();
			var data = d2009_client_data[key]||d2009_client_data.generic;
			if (!data.dc.site) {
				var p = d.path.split('/');
				data.dc.site=d.site+'/'+p.slice(2).join('/').toLowerCase();
				data.dc.s1=p.slice(2)[0].toLowerCase();
				data.dc.s2=p.slice(3)[0].toLowerCase();
			}
			if (d.ctype=='front') {
				data.dc.site = data.dc.site+'/index';
				data.dc.s2 = 'index';
			}
			if (d.ctype=='person') {
				var p = d.path.replace(/\/columnists/, '').split('/');
				//var s1 = p.slice(2)[0].toLowerCase();
				//var s2 = p.slice(4)[0].toLowerCase();
				//data.dc.s1 = s1
				//data.dc.s2 = s2
				//data.dc.site = d.site+'/'+s1+'/'+s2;
				data.dc.site=d.site+'/'+p.slice(2).join('/').toLowerCase();
				data.dc.s1=p.slice(2)[0].toLowerCase();
				data.dc.s2=p.slice(3)[0].toLowerCase();
			}
			if (d.leaf=='Classification') {
				var category = "";
				var p = "";
				var s1, s2, s3;
				if (d.categories.length > 0) {
					category = d.categories[0];
					p = category.toLowerCase().replace(/ /g,"-").replace(/[^a-zA-Z\/-]/g,"").split('/');
					data.dc.s1 = s1 = p.slice(1)[0];
					data.dc.s2 = s2 = p.slice(2)[0];
					data.dc.s3 = s3 = p.slice(3)[0];
					p[1]="category";
					data.dc.site=d.site+p.join('/');
				}
			}
			if (d.ptype=='slideshow') {
				data.dc.site = data.dc.site+'/slideshow';
			}
			data.site = d.site;
			data.ptype = d.ptype;
			data.pageid = d.pageid;
			data.col = (d.path.indexOf('/columns/')!=-1)?d.leaf:'';
			return data;
		}
    };
})(jQuery);

if(!Array.indexOf){
	Array.prototype.indexOf = function(key){
		for(var i=0;i<this.length;i++){
			if(this[i]===key){
				return i;
			}
		}
		return -1;
	};
}
/*if(!Object.clone){
	Object.prototype.clone = function() {
		var newObj = (this instanceof Array) ? [] : {};
		for (i in this) {
			if (i == 'clone') {continue;}
			if (this[i] && typeof this[i] == "object") {
				newObj[i] = this[i].clone();
			} else {
				newObj[i] = this[i];
			}
		}
		return newObj;
	};
}
utils.cloneObject = function(obj){
    //change 1 - will work with arrays too:
    var temp = (this instanceof Array) ? [] : {};
    for (myvar in obj) {
        //change 2 - the recursive bit:
        if (obj[i] && typeof obj[myvar] == "object") {
            temp[myvar] = utils.cloneObject(obj[myvar]);
        } else {
            temp[myvar] = obj[myvar];
        }
    }
    return temp;
};
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////

var d2009_client_data = {
	generic: {
		"dc": {"site": "","s1": "","s2": ""},
		"qu": {
			"qu_channel_1": {"placeid": 1471746, "pid": 1369767, "ps": -1, "width": 405, "height": 220},
			"qu_story_1":   {"placeid": 1474908, "pid": 1369767, "ps": -1, "width": 328, "height": 220}
		},
		"hbx": {"pn": "Root"}
	},
	fnc_entertainment: {
		"dc": {"site": "fnc/entertainment","s1": "entertainment","s2": ""},
		"qu": {
			"qu_channel_1": {"placeid": 1471746, "pid": 1369767, "ps": -1, "width": 405, "height": 220},
			"qu_story_1":   {"placeid": 1474908, "pid": 1369767, "ps": -1, "width": 328, "height": 220}
		},
		"hbx": {"pn": "Root"}
	}
}

//chartbeat
jQuery.ad.chart.start();