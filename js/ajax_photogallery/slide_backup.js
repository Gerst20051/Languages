var slideShow=function(){
	var bxs,bxe,fxs,fxe,ys,ye,ta,ia,ie,st,ss,ft,fs,xp,yp,ci,t,tar,tarl;
	ta=document.getElementById(thumbid); ia=document.getElementById(imgid);
	t=ta.getElementsByTagName('li'); ie=document.all?true:false;
	st=3; ss=3; ft=10; fs=5; xp,yp=0;
	return{
		init:function(){
			document.onmousemove=this.pos; window.onresize=function(){setTimeout("slideShow.lim()",500)};
			ys=this.toppos(ta); ye=ys+ta.offsetHeight;
			len=t.length;tar=[];
			for(i=0;i<len;i++){
				var id=t[i].value; tar[i]=id;
				t[i].onclick=new Function("slideShow.getimg('"+id+"')");
				if(i==0){this.getimg(id)}
			}
			tarl=tar.length;
		},
		scrl:function(d){
			clearInterval(ta.timer);
			var l=(d==-1)?0:(t[tarl-1].offsetLeft-(ta.parentNode.offsetWidth-t[tarl-1].offsetWidth)+10)
			ta.timer=setInterval(function(){slideShow.mv(d,l)},st);
		},
		mv:function(d,l){
			ta.style.left=ta.style.left||'0px';
			var left=ta.style.left.replace('px','');
			if(d==1){
				if(l-Math.abs(left)<=ss){
					this.cncl(ta.id); ta.style.left='-'+l+'px';
				}else{ta.style.left=left-ss+'px'}
			}else{
				if(Math.abs(left)-l<=ss){
					this.cncl(ta.id); ta.style.left=l+'px';
				}else{ta.style.left=parseInt(left)+ss+'px'}
			}
		},
		cncl:function(){clearTimeout(ta.timer)},
		getimg:function(id){
			if(auto){clearTimeout(ia.timer)}
			if(ci!=null){
				var ts,tsl,x;
				ts=ia.getElementsByTagName('img'); tsl=ts.length;x=0;
				for(x;x<tsl;x++){
					if(ci.id!=id){var o=ts[x]; clearInterval(o.timer); o.timer=setInterval(function(){slideShow.fdout(o)},fs)}
				}
			}
			if(!document.getElementById(id)){
				var i=document.createElement('img');
				ia.appendChild(i);
				i.id=id; i.av=0; i.style.opacity=0;
				i.style.filter='alpha(opacity=0)';
				i.src=imgdir+'/'+id+imgext;
			}else{
				i=document.getElementById(id); clearInterval(i.timer);
			}
			i.timer=setInterval(function(){slideShow.fdin(i)},fs);
		},
		nav:function(d){
			var c=0;
			for(key in tar){if(tar[key]==ci.id){c=key}}
			if(tar[parseInt(c)+d]){
				this.getimg(tar[parseInt(c)+d]);
			}else{
				if(d==1){
					this.getimg(tar[0]);
				}else{this.getimg(tar[tarl-1])}
			}
		},
		auto:function(){ia.timer=setInterval(function(){slideShow.nav(1)},autodelay*1000)},
		fdin:function(i){
			if(i.complete){i.av=i.av+fs; i.style.opacity=i.av/100; i.style.filter='alpha(opacity='+i.av+')'}
			if(i.av>=100){if(auto){this.auto()}; clearInterval(i.timer); ci=i}
		},
		fdout:function(i){
			i.av=i.av-fs; i.style.opacity=i.av/100;
			i.style.filter='alpha(opacity='+i.av+')';
			if(i.av<=0){clearInterval(i.timer); if(i.parentNode){i.parentNode.removeChild(i)}}
		},
		lim:function(){
			var taw,taa,len; taw=ta.parentNode.offsetWidth; taa=taw/4; 
			bxs=slideShow.leftpos(ta); bxe=bxs+taa; fxe=bxs+taw; fxs=fxe-taa;
		},
		pos:function(e){
			xp=ie?event.clientX+document.documentElement.scrollLeft:e.pageX; 
			yp=ie?event.clientY+document.documentElement.scrollTop:e.pageY;
			if(xp>bxs&&xp<bxe&&yp>ys&&yp<ye){
				slideShow.scrl(-1);
			}else if(xp>fxs&&xp<fxe&&yp>ys&&yp<ye){
				slideShow.scrl(1);
			}else{slideShow.cncl()}
		},
		leftpos:function(t){
			var l=0;
			if(t.offsetParent){
				while(1){l+=t.offsetLeft; if(!t.offsetParent){break}; t=t.offsetParent}
			}else if(t.x){l+=t.x}
			return l;
		},
		toppos:function(t){
			var p=0;
			if(t.offsetParent){
				while(1){p+=t.offsetTop; if(!t.offsetParent){break}; t=t.offsetParent}
			}else if(t.y){p+=t.y}
			return p;
		},
		stop:function(){clearInterval(ia.timer)}
	};
}();

window.onload=function(){slideShow.init(); slideShow.lim()};