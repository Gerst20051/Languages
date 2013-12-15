// PSPNG-1.0 :: PSP NetGames Version 1.0 (common functions)
// *****************************************************
// DOM scripting by brothercake -- http://www.brothercake.com/
// GNU Lesser General Public License -- http://www.gnu.org/licenses/lgpl.html
//******************************************************


//DOM-ready watcher function - http://www.brothercake.com/site/resources/scripts/domready/
function domFunction(f, a)
{
	var n = 0, p = {}, t = setInterval(function()
	{
		var c = true;
		n++;

		if(typeof document.getElementsByTagName != 'undefined'
			&& (document.getElementsByTagName('body')[0] != null || document.body != null))
		{
			c = false;

			if(typeof a == 'object')
			{
				for(var i in a)
				{
					if(a[i] == 'id' && document.getElementById(i) == null)
					{
						c = true;
						break;
					}

					else if(a[i] == 'tag')
					{
						var l = document.getElementsByTagName(i).length;

						if(typeof p[i] == 'undefined' || p[i] != l || l < 1)
						{
							p[i] = l;
							c = true;
							break;
						}
					}
				}
			}

			if(!c) { f(); clearInterval(t); }
		}

		if(n >= 30) { clearInterval(t); }

	}, 250);
};



