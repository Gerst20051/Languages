<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
 <script src="/assets.php/en_US/Data,api,init.js"></script>
</head>
<body>
    <div id="fb-root"></div>
  <p>This page demo how to use Module Connect Data layer to construct queries that will be automatically built into a single fql.multiquery with dependencies. Where possible,
the data layer also perform auto merge of queries</p>
  <input type="button" value="Connect" onclick="testLogin()" />
  <p>Current Connect Status is <span id="connect_status"></span></p>
  <h1>Random 5 friends info</h1>
  <div id="infos"></div>

  <h1>Random 6 photos of these friends</h1>
  <div id="photos"></div>

 <script type="text/javascript">


  function render() {
    // Start render when user is connected
    FB.Event.monitor('auth.statusChange', function() {
      if (FB._userStatus == 'connected') {

        // Get random 5 friends ids
        var friends = FB.Data.query('select uid2 from friend where uid1={0} ORDER BY rand() limit 5', FB._session.uid);

        var count = FB.Data.waitOn([friends], 'args[0].length');
        displayFriendsCount(count);

        // Render name and pic of these friends
        renderFriendsName(friends);

        // Render photos where these friends were tagged
        renderFriendsPhoto(friends);

        // Just for illustration, you can other queries as well, they will be done a single API call
        var queryTemplate = 'select name from profile where id={0}';
        var u1 = FB.Data.query(queryTemplate, 4);
        var u2 = FB.Data.query(queryTemplate, 1160);
        FB.Data.waitOn([u1, u2], function(args) {
          log('u1 = '+ FB.JSON.stringify(args[0]));
          log('u2 = '+  FB.JSON.stringify(args[1]));
        });
      }
    });
  }


  function displayFriendsCount(count) {
    count.wait(function(result) {
       log('friends count = ' + result);
    });
  }

  // Render name and pic of these friends
  function renderFriendsName(friends) {
    var friendInfos = FB.Data.query(
     'select name, pic from user where uid in (select uid2 from {0})', friends);

    friendInfos.wait(function(data) {
      // Render info. For illustration of API, I am using any XFBML tags
      var html = '';
      FB.forEach(data, function(info) {
        html += '<p>' + info.name + '<img src="' + info.pic + '" /></p>';
      });
      FB.$('infos').innerHTML = html;
    });
  }

  // Render photos where these friends were tagged
  function renderFriendsPhoto(friends) {
    // Get pid of photos of these friends
    var pids =FB.Data.query(
     'select pid from photo_tag where subject in (select uid2 from {0}) ORDER BY rand() limit 6', friends);

    // Get photos of these pids, then render them when ready
    FB.Data.query('select src_small from photo where pid in (select pid from {0})', pids)
    .wait(function(data) {
      // Render info. For illustration of API, I am using any XFBML tags
      var html = '';
      FB.forEach(data, function(photo) {
        html += '<img src="' + photo.src_small + '" />';
      });
      FB.$('photos').innerHTML = html;
    });
  }


  function testLogin() {
    FB.login(function(result) {
      log("Connect completed");
    });
  }

  function displayConnectStatus() {
    FB.$('connect_status').innerHTML = FB._userStatus;
  }


  function log(s) {
    FB.$('result').value += s + '\n';
  }

  FB.init({
    apiKey: "4b38d3e1a6df930669e13b82e3ba8974",
    logging: true,
    status: true
  });
  render();

  // Wanna monitor connect status?
  FB.Event.monitor('auth.statusChange', displayConnectStatus);

  </script>


  <br />
  <p>Log</p>
  <textarea rows="50" style="width:100%; height:500px;" id="result"></textarea>
</body>
</html>