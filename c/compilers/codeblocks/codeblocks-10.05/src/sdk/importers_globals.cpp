/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 4909 $
 * $Id: importers_globals.cpp 4909 2008-02-27 13:15:26Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/importers_globals.cpp $
 */

#include "sdk_precomp.h"
#include "importers_globals.h"

// default: ask for compiler to use
bool ImportersGlobals::UseDefaultCompiler = false;

// default: ask which targets to import
bool ImportersGlobals::ImportAllTargets = false;

// Do not forget to update this function, when new flags are added!
void ImportersGlobals::ResetDefaults()
{
    UseDefaultCompiler = false;
    ImportAllTargets = false;
}
