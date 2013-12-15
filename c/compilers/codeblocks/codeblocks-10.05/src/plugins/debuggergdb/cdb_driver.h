/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CDB_DRIVER_H
#define CDB_DRIVER_H

#include "debuggerdriver.h"

class CDB_driver : public DebuggerDriver
{
    public:
        CDB_driver(DebuggerGDB* plugin);
        virtual ~CDB_driver();

        virtual wxString GetCommandLine(const wxString& debugger, const wxString& debuggee);
        virtual wxString GetCommandLine(const wxString& debugger, int pid);
        virtual void Prepare(ProjectBuildTarget* target, bool isConsole);
        virtual void Start(bool breakOnEntry);
        virtual void Stop();

        virtual void Continue();
        virtual void Step();
        virtual void StepInstruction();
        virtual void StepIn();
        virtual void StepOut();
        virtual void Backtrace();
        virtual void Disassemble();
        virtual void CPURegisters();
        virtual void SwitchToFrame(size_t number);
        virtual void SetVarValue(const wxString& var, const wxString& value);
        virtual void MemoryDump();
        virtual void Detach();
        virtual void RunningThreads();

        void InfoFrame();
        void InfoDLL();
        void InfoFiles();
        void InfoFPU();
        void InfoSignals();

        virtual void SwitchThread(size_t threadIndex){} // not implemented

        virtual void AddBreakpoint(DebuggerBreakpoint* bp);
        virtual void RemoveBreakpoint(DebuggerBreakpoint* bp);
        virtual void EvaluateSymbol(const wxString& symbol, const wxRect& tipRect);
        virtual void UpdateWatches(bool doLocals, bool doArgs, DebuggerTree* tree);
        virtual void ParseOutput(const wxString& output);
    protected:
    private:
};

#endif // CDB_DRIVER_H
