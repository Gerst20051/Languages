@echo off
rem
rem This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
rem http://www.gnu.org/licenses/gpl-3.0.html
rem
rem Copyright: 2008 Jens Lody
rem
rem $Revision: 5277 $
rem $Id: update.bat 5277 2008-10-19 19:08:32Z jenslody $
rem $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/IncrementalSearch/update.bat $
rem

md ..\..\..\devel\share\CodeBlocks\images\settings > nul 2>&1
md ..\..\..\output\share\CodeBlocks\images\settings > nul 2>&1
copy .\*.png ..\..\..\devel\share\CodeBlocks\images\settings\ > nul 2>&1
copy .\*.png ..\..\..\output\share\CodeBlocks\images\settings\ > nul 2>&1
exit 0

