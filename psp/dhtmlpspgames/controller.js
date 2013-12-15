// PSPNG:C:1.0 :: PSP NetGames (Controller Script v1.0)
// *****************************************************
// DOM scripting by brothercake -- http://www.brothercake.com/
// GNU Lesser General Public License -- http://www.gnu.org/licenses/lgpl.html
//******************************************************

//start a new domReady watcher to construct a controller object
//as soon as the "gameform" form exists
new domFunction(function()
{
	var control = new controller();

}, {'gameform':'id'});



//controller object
function controller()
{
	//bind a submit handler to the game form
	document.getElementById('gameform').onsubmit = function()
	{
		//reload the page and don't submit
		//because submitting doesn't work for local pages
		//and all this form actually needs to do
		//is reload the list page, so reloading the master page is just as good
		location.reload();
		return false;
	}
}
