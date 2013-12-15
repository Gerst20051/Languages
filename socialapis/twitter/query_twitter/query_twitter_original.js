var value = "";

function queryTwitter (){
    if(value != ""){
      var head = document.getElementsByTagName('head')[0];
      var elem = document.createElement('script');
      elem.setAttribute('src', 'http://search.twitter.com/search.json?callback=writeToTable&q=' + value +"'");
      head.appendChild(elem);
      value = "";
    }
}

setInterval (queryTwitter, 1500);

function writeToTable (results){

  var table = $('#myTable');    
  table.children().remove();
  var tweets = results.results;
  for (var i in tweets){
        table.append('<tr style="background-color:darkgrey;"><td><img src="' + tweets[i].profile_image_url + '"/></td><td><b>' + tweets[i].from_user + ' says: </b> <i>'+ tweets[i].text + '</i></td></tr>');
  }
}

$(function (){
      $('#search').keyup(function (){
       value = $(this).val();
          });
    });