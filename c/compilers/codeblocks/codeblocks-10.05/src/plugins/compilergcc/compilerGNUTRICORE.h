/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef COMPILER_GNUTRICORE_H
#define COMPILER_GNUTRICORE_H
#include <compiler.h>
class CompilerGNUTRICORE : public Compiler
{
	public:
		CompilerGNUTRICORE();
		virtual ~CompilerGNUTRICORE();
		virtual void Reset();
		virtual void LoadDefaultRegExArray();
		virtual AutoDetectResult AutoDetectInstallationDir();
	protected:
		virtual Compiler* CreateCopy();
};
#endif // COMPILER_GNUTRICORE_H
