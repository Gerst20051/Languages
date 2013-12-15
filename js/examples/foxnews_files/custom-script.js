/*
 * Custom Script for Fox News. Last Updated 10-11-10. 
 * 
 */

/*
 * Customer calls this function within flash player to capture either guide click or a page event with additional attribute information. 
 */
function baynote_flash_track_video ( video_URL ){
	
	var bn_rank = "";
	var bn_guide = "";
	var bn_req = "";
	var linkText = "";
	var thumbnail_url = "";
	var video_dur = "";
	var video_url = "";
	var main_page_url = window.location.href;
	
	if (!video_URL) return;	

	bn_rank = video_URL.baynote_bnrank;
	bn_guide = video_URL.baynote_guide;
	bn_req = video_URL.baynote_req;
   	linkText = video_URL.linkText;
	thumbnail_url = video_URL.thumbnail_url;
	video_dur = video_URL.video_duration;
	video_url = video_URL.linkText;


	if ( bn_guide != null &&  bn_req != null &&  bn_rank != null ){
			
		 	var exitInfo = new Object();
			exitInfo.baynote_guide = bn_guide;
			exitInfo.baynote_req = bn_req;
			exitInfo.baynote_bnrank = bn_rank;
			exitInfo.baynote_guide_target = video_url;
			exitInfo.link = linkText;
			exitInfo.dest = video_url;
			
		if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
			exitInfo.attrs = exitInfo.attrs || {};
		}
			
		exitInfo.attrs.target = video_url;
		bnObserver.exitOccurred (exitInfo);
		return true;
	}
	else {
		if (typeof(BaynoteAPI) != "undefined" && BaynoteAPI != null){
			var param = {};
			param.d = video_url; 
			param.attrs = new Object();
			var docAttrs = new Object();
			
			param.attrs.evType = "VideoPlay";
			if( BaynoteAPI.isNotEmpty(video_url))
			param.attrs.evTarget = video_url;

		if( BaynoteAPI.isNotEmpty(thumbnail_url)&& thumbnail_url != "" )
			docAttrs.thumbnailUrl = thumbnail_url ;
		if( BaynoteAPI.isNotEmpty(video_dur) && video_dur != "" )
			docAttrs.videoDur = video_dur;
		if( BaynoteAPI.isNotEmpty(linkText) )
			docAttrs.linkText = linkText;
			
			docAttrs.medium = "video";
		//capture playlist id 
		if (typeof pageVars!= 'undefined' && BaynoteAPI.isNotEmpty(pageVars)) {  
			docAttrs.playlistId = pageVars.playlistId;;			
		}
		
		param.attrs.docAttrs = BaynoteAPI.objectToJSON(docAttrs);
		param.specialTarget = true;

		if (main_page_url.indexOf("http://video") > -1)
		BaynoteAPI.sendPageEvent(param);
		param={};
	
		return true;
		}	
	}
}

function setUrl(tag){	
	var links = document.getElementsByTagName("link");
	if (!links) return;
	
	for (var i = 0; i < links.length; i++) {
		if (!links[i]) return;
		if (links[i].rel == "canonical") {
			var tempLinkContent = links[i].href;
			if(BaynoteAPI.isNotEmpty(tempLinkContent)) {
				tag.url = tempLinkContent;
			}
		}
	}
}
/*
 * Customer calls this function to capture additional attributes for videos that don't change URLs
 */
function baynote_track_video_attribute( thumbnail_URL, video_duration, linkText, url){
	
	var thumbnailUrl = thumbnail_URL;
	var videoDuration = video_duration;
	var linkText = linkText;
	var page_url = url;
	var main_page_url = window.location.href;

	
	if (typeof(BaynoteAPI) != "undefined" && BaynoteAPI != null){
			var param = {};
			param.d = page_url;
			param.attrs = new Object();
			var docAttrs = new Object();
			param.attrs.evType = "VideoPlay";
		if( BaynoteAPI.isNotEmpty(page_url) )
			param.attrs.evTarget = page_url;

		if( BaynoteAPI.isNotEmpty(thumbnailUrl) )
			docAttrs.thumbnailUrl = thumbnailUrl ;
		if( BaynoteAPI.isNotEmpty(videoDuration) )
			docAttrs.videoDur = videoDuration;
		if( BaynoteAPI.isNotEmpty(linkText) )
			docAttrs.linkText = linkText;
	
		docAttrs.medium = "video";
		//capture playlist id 
		if (typeof pageVars!= 'undefined' && BaynoteAPI.isNotEmpty(pageVars)) {  
			docAttrs.playlistId = pageVars.playlistId;;			
		}
		
		param.attrs.docAttrs = BaynoteAPI.objectToJSON(docAttrs);
		param.specialTarget = true;

	if (main_page_url.indexOf("http://video") > -1)
	BaynoteAPI.sendPageEvent(param);
	param={};	

	return true;
		
	}	
}


function baynote_setAttrs(tag) {
	/*alert('setAttrs');*/
	
	var metas = document.getElementsByTagName("meta");
	if (!metas) return;
	
	for (var i = 0; i < metas.length; i++) {
		if (!metas[i]) return;
			if (metas[i].name == "dc.type") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.docAttrs.dctype = tempMetaContent;
				}
			} else if (metas[i].name == "prism.channel") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.docAttrs.prismchannel  = tempMetaContent;
				}
			}else if (metas[i].name == "prism.genre") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.docAttrs.prismgenre  = tempMetaContent;
				}
			}else if (metas[i].name == "prism.section") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.docAttrs.section  = tempMetaContent;
				}
			}else if (metas[i].name == "dc.title") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.title  = tempMetaContent;
				}
			}else if (metas[i].name == "dcterms.abstract") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.summary  = tempMetaContent;
				}
			}else if (metas[i].name == "medium") {
				var tempMetaContent = metas[i].content;
				if(BaynoteAPI.isNotEmpty(tempMetaContent)) {
					tag.docAttrs.medium  = tempMetaContent;
				}
			}
	} 
	
		
	//capture playlist id
	if (typeof pageVars!= 'undefined' && BaynoteAPI.isNotEmpty(pageVars)) {
		tag.docAttrs.playlistId = pageVars.playlistId;
	}  

}

function myPreHandler(tag) { 

	if (typeof tag != 'undefined' &&  typeof tag.type != 'undefined' && tag.type==bnConstants.OBSERVER_TAG)   {	
		baynote_setAttrs(tag);
		setUrl(tag);
		// Media Duration
		
	} // code that runs before the observer fires
	
	if (typeof tag != 'undefined' &&  typeof tag.type != 'undefined' && tag.type==bnConstants.GUIDE_TAG)   {	
		
		tag.userId = "DEMO";
		tag.query = "guide";
		//setUrl(tag);
	
		//do stuff before recs have loaded
	}
	
    return true;      
} 

function myPostHandler(tag) {
/*
	if (typeof tag != 'undefined' &&  typeof tag.type != 'undefined' && tag.type==bnConstants.GUIDE_TAG)   {	
		//do stuff after recs have loaded
		}
*/
	return true;
}

   // register the event handler
baynote_globals.onBeforeTagShow.push(myPreHandler);
baynote_globals.onTagShow.push(myPostHandler); 
bnResourceManager.registerResource(baynote_globals.ScriptResourceId); 