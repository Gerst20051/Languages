#include "stdafx.h" // pre-compiled headers
#include "globaldata.h" // for access to many global vars
#include "application.h" // for MsgSleep()
#include "exports.h"
#include "script.h"

LPTSTR result_to_return_dll; //HotKeyIt H2 for ahkgetvar and ahkFunction return.
// ExprTokenType aResultToken_to_return ;  // for ahkPostFunction
FuncAndToken aFuncAndTokenToReturn[10] ;    // for ahkPostFunction
int returnCount = 0 ;

#ifdef _USRDLL
#ifndef MINIDLL
//COM virtual functions
BOOL com_ahkPause(LPTSTR aChangeTo){return ahkPause(aChangeTo);}
unsigned int com_ahkFindLabel(LPTSTR aLabelName){return ahkFindLabel(aLabelName);}
// LPTSTR com_ahkgetvar(LPTSTR name,unsigned int getVar){return ahkgetvar(name,getVar);}
// unsigned int com_ahkassign(LPTSTR name, LPTSTR value){return ahkassign(name,value);}
unsigned int com_ahkExecuteLine(unsigned int line,unsigned int aMode,unsigned int wait){return ahkExecuteLine(line,aMode,wait);}
BOOL com_ahkLabel(LPTSTR aLabelName, unsigned int nowait){return ahkLabel(aLabelName,nowait);}
unsigned int com_ahkFindFunc(LPTSTR funcname){return ahkFindFunc(funcname);}
// LPTSTR com_ahkFunction(LPTSTR func, LPTSTR param1, LPTSTR param2, LPTSTR param3, LPTSTR param4, LPTSTR param5, LPTSTR param6, LPTSTR param7, LPTSTR param8, LPTSTR param9, LPTSTR param10){return ahkFunction(func,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10);}
unsigned int com_ahkPostFunction(LPTSTR func, LPTSTR param1, LPTSTR param2, LPTSTR param3, LPTSTR param4, LPTSTR param5, LPTSTR param6, LPTSTR param7, LPTSTR param8, LPTSTR param9, LPTSTR param10){return ahkPostFunction(func,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10);}
BOOL com_ahkKey(LPTSTR keys){return ahkKey(keys);}
#ifndef AUTOHOTKEYSC
unsigned int com_addScript(LPTSTR script, int aExecute){return addScript(script,aExecute);}
BOOL com_ahkExec(LPTSTR script){return ahkExec(script);}
unsigned int com_addFile(LPTSTR fileName, bool aAllowDuplicateInclude, int aIgnoreLoadFailure){return addFile(fileName,aAllowDuplicateInclude,aIgnoreLoadFailure);}
#endif
#ifdef _USRDLL
unsigned int com_ahkdll(LPTSTR fileName,LPTSTR argv,LPTSTR args){return ahkdll(fileName,argv,args);}
unsigned int com_ahktextdll(LPTSTR script,LPTSTR argv,LPTSTR args){return ahktextdll(script,argv,args);}
BOOL com_ahkTerminate(bool kill){return ahkTerminate(kill);}
BOOL com_ahkReady(){return ahkReady();}
BOOL com_ahkReload(){return ahkReload();}
#endif
#endif
#endif

EXPORT BOOL ahkPause(LPTSTR aChangeTo) //Change pause state of a running script
{

	if ( (((int)aChangeTo == 1 || (int)aChangeTo == 0) || (*aChangeTo == 'O' || *aChangeTo == 'o') && ( *(aChangeTo+1) == 'N' || *(aChangeTo+1) == 'n' ) ) || *aChangeTo == '1')
	{
#ifndef MINIDLL
		Hotkey::ResetRunAgainAfterFinished();
#endif
		if ((int)aChangeTo == 0)
		{
			g->IsPaused = false;
			--g_nPausedThreads; // For this purpose the idle thread is counted as a paused thread.
		}
		else
		{
			g->IsPaused = true;
			++g_nPausedThreads; // For this purpose the idle thread is counted as a paused thread.
		}
#ifndef MINIDLL
		g_script.UpdateTrayIcon();
#endif
	}
	else if (*aChangeTo != '\0')
	{
		g->IsPaused = false;
		--g_nPausedThreads; // For this purpose the idle thread is counted as a paused thread.
#ifndef MINIDLL
		g_script.UpdateTrayIcon();
#endif
	}
	return (int)g->IsPaused;
}


EXPORT unsigned int ahkFindFunc(LPTSTR funcname)
{
	return (unsigned int)g_script.FindFunc(funcname);
}

EXPORT unsigned int ahkFindLabel(LPTSTR aLabelName)
{
	Label *aLabel = g_script.FindLabel(aLabelName) ;
	return (unsigned int)aLabel->mJumpToLine;
}

EXPORT int ximportfunc(ahkx_int_str func1, ahkx_int_str func2, ahkx_int_str_str func3) // Naveen ahkx N11
{
    g_script.xifwinactive = func1 ;
    g_script.xwingetid  = func2 ;
    g_script.xsend = func3;
    return 0;
}


// Naveen: v1. ahkgetvar()
EXPORT LPTSTR ahkgetvar(LPTSTR name,unsigned int getVar)
{
	Var *ahkvar = g_script.FindOrAddVar(name);
	if (getVar != NULL)
	{
		if (ahkvar->mType == VAR_BUILTIN)
			return _T("");
		result_to_return_dll = (LPTSTR )realloc((LPTSTR )result_to_return_dll,MAX_INTEGER_LENGTH);
		return ITOA64((int)ahkvar,result_to_return_dll);
	}
	if (!ahkvar->HasContents() && ahkvar->mType != VAR_BUILTIN )
		return _T("");
	if (*ahkvar->mCharContents == '\0')
	{
		result_to_return_dll = (LPTSTR )realloc((LPTSTR )result_to_return_dll,(ahkvar->mByteCapacity ? ahkvar->mByteCapacity : ahkvar->mByteLength) + MAX_NUMBER_LENGTH + 1);
		if ( ahkvar->mType == VAR_BUILTIN )
			ahkvar->mBIV(result_to_return_dll,name); //Hotkeyit 
		else if ( ahkvar->mType == VAR_ALIAS )
			ITOA64(ahkvar->mAliasFor->mContentsInt64,result_to_return_dll);
		else if ( ahkvar->mType == VAR_NORMAL )
			ITOA64(ahkvar->mContentsInt64,result_to_return_dll);//Hotkeyit
	}
	else
	{
		result_to_return_dll = (LPTSTR )realloc((LPTSTR )result_to_return_dll,ahkvar->mByteLength+1);
		if ( ahkvar->mType == VAR_ALIAS )
			ahkvar->mAliasFor->Get(result_to_return_dll); //Hotkeyit removed ebiv.cpp and made ahkgetvar return all vars
 		else if ( ahkvar->mType == VAR_NORMAL )
			ahkvar->Get(result_to_return_dll);  // var.getText() added in V1.
		else if ( ahkvar->mType == VAR_BUILTIN )
			ahkvar->mBIV(result_to_return_dll,name); //Hotkeyit 
	}
	return result_to_return_dll;
}	

EXPORT unsigned int ahkassign(LPTSTR name, LPTSTR value) // ahkwine 0.1
{
	Var *var;
	if (   !(var = g_script.FindOrAddVar(name, _tcslen(name)))   )
		return -1;  // Realistically should never happen.
	var->Assign(value); 
	return 0; // success
}
//HotKeyIt ahkExecuteLine()
EXPORT unsigned int ahkExecuteLine(unsigned int line,unsigned int aMode,unsigned int wait)
{
	Line *templine = (Line *)line;
	if (templine == NULL)
		return (unsigned int)g_script.mFirstLine;
	if (aMode)
	{
		if (wait)
			SendMessage(g_hWnd, AHK_EXECUTE, (WPARAM)templine, (LPARAM)aMode);
		else
			PostMessage(g_hWnd, AHK_EXECUTE, (WPARAM)templine, (LPARAM)aMode);
	}
	if (templine = templine->mNextLine)
	if (templine->mActionType == ACT_BLOCK_BEGIN && templine->mAttribute)
	{
		for(;!(templine->mActionType == ACT_BLOCK_END && templine->mAttribute);)
			templine = templine->mNextLine;
		templine = templine->mNextLine;
	} 
	return (unsigned int) templine;
}

EXPORT BOOL ahkLabel(LPTSTR aLabelName, unsigned int nowait) // 0 = wait = default
{
	Label *aLabel = g_script.FindLabel(aLabelName) ;
	if (aLabel)
	{
		if (nowait)
			PostMessage(g_hWnd, AHK_EXECUTE_LABEL, (LPARAM)aLabel, (LPARAM)aLabel);
		else
			SendMessage(g_hWnd, AHK_EXECUTE_LABEL, (LPARAM)aLabel, (LPARAM)aLabel);
		return 1;
	}
	else
		return 0;
}

EXPORT unsigned int ahkPostFunction(LPTSTR func, LPTSTR param1, LPTSTR param2, LPTSTR param3, LPTSTR param4, LPTSTR param5, LPTSTR param6, LPTSTR param7, LPTSTR param8, LPTSTR param9, LPTSTR param10)
{
	Func *aFunc = g_script.FindFunc(func) ;
	if (aFunc)
	{	
		// g_script.mTempFunc = aFunc ;
		// ExprTokenType return_value;
		if (aFunc->mParamCount > 0 && param1 != NULL)
		{
			// Copy the appropriate values into each of the function's formal parameters.
			aFunc->mParam[0].var->Assign((LPTSTR )param1); // Assign parameter #1
			if (aFunc->mParamCount > 1  && param2 != NULL) // Assign parameter #2
			{
				// v1.0.38.01: LPARAM is now written out as a DWORD because the majority of system messages
				// use LPARAM as a pointer or other unsigned value.  This shouldn't affect most scripts because
				// of the way ATOI64() and ATOU() wrap a negative number back into the unsigned domain for
				// commands such as PostMessage/SendMessage.
				aFunc->mParam[1].var->Assign((LPTSTR )param2);
				if (aFunc->mParamCount > 2 && param3 != NULL) // Assign parameter #3
				{
					aFunc->mParam[2].var->Assign((LPTSTR )param3);
					if (aFunc->mParamCount > 3 && param4 != NULL) // Assign parameter #4
					{
						aFunc->mParam[3].var->Assign((LPTSTR )param4);
						if (aFunc->mParamCount > 4 && param5 != NULL) // Assign parameter #5
						{
							aFunc->mParam[4].var->Assign((LPTSTR )param5);
							if (aFunc->mParamCount > 5 && param6 != NULL) // Assign parameter #6
							{
								aFunc->mParam[5].var->Assign((LPTSTR )param6);
								if (aFunc->mParamCount > 6 && param7 != NULL) // Assign parameter #7
								{
									aFunc->mParam[6].var->Assign((LPTSTR )param7);
									if (aFunc->mParamCount > 7 && param8 != NULL) // Assign parameter #8
									{
										aFunc->mParam[7].var->Assign((LPTSTR )param8);
										if (aFunc->mParamCount > 8 && param9 != NULL) // Assign parameter #9
										{
											aFunc->mParam[8].var->Assign((LPTSTR )param9);
											if (aFunc->mParamCount > 9 && param10 != NULL) // Assign parameter #10
											{
												aFunc->mParam[9].var->Assign((LPTSTR )param10);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		FuncAndToken & aFuncAndToken = aFuncAndTokenToReturn[returnCount];
		aFuncAndToken.mFunc = aFunc ;
		returnCount++ ;
		if (returnCount > 9)
			returnCount = 0 ;
		PostMessage(g_hWnd, AHK_EXECUTE_FUNCTION_DLL, (WPARAM)&aFuncAndToken,NULL);
		return 0;
	}
	return -1;
}


EXPORT BOOL ahkKey(LPTSTR keys) 
{
	SendKeys(keys, false, SM_EVENT, 0, 1); // N11 sendahk
	return 0;
}

#ifndef AUTOHOTKEYSC
#ifdef _USRDLL
// Naveen: v6 addFile()
// Todo: support for #Directives, and proper treatment of mIsReadytoExecute
EXPORT unsigned int addFile(LPTSTR fileName, bool aAllowDuplicateInclude, int aIgnoreLoadFailure)
{   // dynamically include a file into a script !!
	// labels, hotkeys, functions.   
	static int filesAdded = 0  ; 
	
	Line *oldLastLine = g_script.mLastLine;
	
	if (aIgnoreLoadFailure > 1)  // if third param is > 1, reset all functions, labels, remove hotkeys
	{
		g_script.mFuncCount = 0;   
		g_script.mFirstLabel = NULL ; 
		g_script.mLastLabel = NULL ; 
		g_script.mLastFunc = NULL ; 
	    g_script.mFirstLine = NULL ; 
		g_script.mLastLine = NULL ;
		 g_script.mCurrLine = NULL ; 

		if (filesAdded == 0)
			{
			SimpleHeap::sBlockCount = 0 ;
			SimpleHeap::sFirst = NULL;
			SimpleHeap::sLast  = NULL;
			SimpleHeap::sMostRecentlyAllocated = NULL;
			}
		if (filesAdded > 0)
			{
			// Naveen v9 free simpleheap memory for late include files
			SimpleHeap *next, *curr;
			for (curr = SimpleHeap::sFirst; curr != NULL;)
				{
				next = curr->mNextBlock;  // Save this member's value prior to deleting the object.
				curr->~SimpleHeap() ;
				curr = next;
				}
			SimpleHeap::sBlockCount = 0 ;
			SimpleHeap::sFirst = NULL;
			SimpleHeap::sLast  = NULL;
			SimpleHeap::sMostRecentlyAllocated = NULL;
/*  Naveen: the following is causing a memory leak in the exe version of clearing the simple heap v10
 g_script.mVar = NULL ; 
 g_script.mVarCount = 0 ; 
 g_script.mVarCountMax = 0 ; 
 g_script.mLazyVar = NULL ; 

 g_script.mLazyVarCount = 0 ; 
*/
		}
	g_script.LoadIncludedFile(fileName, aAllowDuplicateInclude, (bool) aIgnoreLoadFailure);
	if (!g_script.PreparseBlocks(oldLastLine->mNextLine) || !g_script.PreparseIfElse(oldLastLine->mNextLine))
		return LOADING_FAILED;
	PostMessage(g_hWnd, AHK_EXECUTE, (WPARAM)g_script.mFirstLine, (LPARAM)g_script.mFirstLine);
	filesAdded += 1;
	return (unsigned int) g_script.mFirstLine;
	}
	else
	{
	g_script.LoadIncludedFile(fileName, aAllowDuplicateInclude, (bool) aIgnoreLoadFailure);
	if (!g_script.PreparseBlocks(oldLastLine->mNextLine) || !g_script.PreparseIfElse(oldLastLine->mNextLine))
		return LOADING_FAILED;
	return (unsigned int) oldLastLine->mNextLine;  // 
	}
return 0;  // never reached
}

#else
// Naveen: v6 addFile()
// Todo: support for #Directives, and proper treatment of mIsReadytoExecute
EXPORT unsigned int addFile(LPTSTR fileName, bool aAllowDuplicateInclude, int aIgnoreLoadFailure)
{   // dynamically include a file into a script !!
	// labels, hotkeys, functions.   
	Func * aFunc = NULL ; 
	int inFunc = 0 ;
	if (g->CurrentFunc)  // normally functions definitions are not allowed within functions.  But we're in a function call, not a function definition right now.
	{
		aFunc = g->CurrentFunc; 
		g->CurrentFunc = NULL ; 
		inFunc = 1 ;
	}
	Line *oldLastLine = g_script.mLastLine;
	
	if (aIgnoreLoadFailure > 1)  // if third param is > 1, reset all functions, labels, remove hotkeys
	{
		g_script.mFuncCount = 0;   
		g_script.mFirstLabel = NULL ; 
		g_script.mLastLabel = NULL ; 
		g_script.mLastFunc = NULL ; 
		g_script.LoadIncludedFile(fileName, aAllowDuplicateInclude, aIgnoreLoadFailure);
	}
	else 
	{
		g_script.LoadIncludedFile(fileName, aAllowDuplicateInclude, (bool) aIgnoreLoadFailure);
	}
	
	if (inFunc == 1 )
		g->CurrentFunc = aFunc ;

	if (!g_script.PreparseBlocks(oldLastLine->mNextLine) || !g_script.PreparseIfElse(oldLastLine->mNextLine))
		return LOADING_FAILED;
	return (unsigned int) oldLastLine->mNextLine;  // 
}


#endif // _USRDLL
#endif // AUTOHOTKEYSC

#ifndef AUTOHOTKEYSC
// HotKeyIt: addScript()
// Todo: support for #Directives, and proper treatment of mIsReadytoExecute
EXPORT unsigned int addScript(LPTSTR script, int aExecute)
{   // dynamically include a script from text!!
	// labels, hotkeys, functions.   
	Func * aFunc = NULL ; 
	int inFunc = 0 ;
	if (g->CurrentFunc)  // normally functions definitions are not allowed within functions.  But we're in a function call, not a function definition right now.
	{
		aFunc = g->CurrentFunc; 
		g->CurrentFunc = NULL ; 
		inFunc = 1 ;
	}
	Line *oldLastLine = g_script.mLastLine;

	if ((g_script.LoadIncludedText(script) != OK) || !g_script.PreparseBlocks(oldLastLine->mNextLine) || !g_script.PreparseIfElse(oldLastLine->mNextLine))
	{
	if (inFunc == 1 )
		g->CurrentFunc = aFunc ; 

		return LOADING_FAILED;
	}	
	if (aExecute > 0)
	{
		if (aExecute > 1)
			SendMessage(g_hWnd, AHK_EXECUTE, (WPARAM)oldLastLine->mNextLine, (LPARAM)oldLastLine->mNextLine);
		else
			PostMessage(g_hWnd, AHK_EXECUTE, (WPARAM)oldLastLine->mNextLine, (LPARAM)oldLastLine->mNextLine);
	}

    
	if (inFunc == 1 )
		g->CurrentFunc = aFunc ;

	return (unsigned int) oldLastLine->mNextLine;  // 
}
#endif // AUTOHOTKEYSC

#ifndef AUTOHOTKEYSC
// Todo: support for #Directives, and proper treatment of mIsReadytoExecute
EXPORT BOOL ahkExec(LPTSTR script)
{   // dynamically include a script from text!!
	// labels, hotkeys, functions.   
	Func * aFunc = NULL ; 
	int inFunc = 0 ;
	if (g->CurrentFunc)  // normally functions definitions are not allowed within functions.  But we're in a function call, not a function definition right now.
	{
		aFunc = g->CurrentFunc; 
		g->CurrentFunc = NULL ; 
		inFunc = 1 ;
	}
	Line *oldLastLine = g_script.mLastLine;

	if ((g_script.LoadIncludedText(script) != OK) || !g_script.PreparseBlocks(oldLastLine->mNextLine))
	{
		if (inFunc == 1 )
			g->CurrentFunc = aFunc;
		return LOADING_FAILED;
	}
	if (!oldLastLine->mNextLine) //H30 - if no line was added, return
		return OK;
	g_script.PreparseIfElse(oldLastLine->mNextLine);
	SendMessage(g_hWnd, AHK_EXECUTE, (WPARAM)oldLastLine->mNextLine, (LPARAM)oldLastLine->mNextLine);
	
	if (inFunc == 1 )
		g->CurrentFunc = aFunc ;
	Line *prevLine = g_script.mLastLine->mPrevLine;
	for(; prevLine != oldLastLine; prevLine = prevLine->mPrevLine)
	{
		delete prevLine->mNextLine;
	}
	oldLastLine->mNextLine = NULL;  // 
	return OK;
}
#endif // AUTOHOTKEYSC
LPTSTR FuncTokenToString(ExprTokenType &aToken, LPTSTR aBuf)
// Supports Type() VAR_NORMAL and VAR-CLIPBOARD.
// Returns "" on failure to simplify logic in callers.  Otherwise, it returns either aBuf (if aBuf was needed
// for the conversion) or the token's own string.  aBuf may be NULL, in which case the caller presumably knows
// that this token is SYM_STRING or SYM_OPERAND (or caller wants "" back for anything other than those).
// If aBuf is not NULL, caller has ensured that aBuf is at least MAX_NUMBER_SIZE in size.
{
	switch (aToken.symbol)
	{
	case SYM_VAR: // Caller has ensured that any SYM_VAR's Type() is VAR_NORMAL.
		return aToken.var->Contents(); // Contents() vs. mContents to support VAR_CLIPBOARD, and in case mContents needs to be updated by Contents().
	case SYM_STRING:
	case SYM_OPERAND:
		return aToken.marker;
	case SYM_INTEGER:
		if (aBuf)
			return ITOA64(aToken.value_int64, aBuf);
		//else continue on to return the default at the bottom.
		break;
	case SYM_FLOAT:
		if (aBuf)
		{
			sntprintf(aBuf, MAX_NUMBER_SIZE, g->FormatFloat, aToken.value_double);
			return aBuf;
		}
		//else continue on to return the default at the bottom.
		break;
	//case SYM_OBJECT: // L31: Treat objects as empty strings (or TRUE where appropriate).
	//default: // Not an operand: continue on to return the default at the bottom.
	}
	return _T("");
}

EXPORT LPTSTR ahkFunction(LPTSTR func, LPTSTR param1, LPTSTR param2, LPTSTR param3, LPTSTR param4, LPTSTR param5, LPTSTR param6, LPTSTR param7, LPTSTR param8, LPTSTR param9, LPTSTR param10)
{
	Func *aFunc = g_script.FindFunc(func) ;
	if (aFunc)
	{	
		// g_script.mTempFunc = aFunc ;
		// ExprTokenType return_value;
		if (aFunc->mParamCount > 0 && param1 != NULL)
		{
			// Copy the appropriate values into each of the function's formal parameters.
			aFunc->mParam[0].var->Assign((LPTSTR )param1); // Assign parameter #1
			if (aFunc->mParamCount > 1  && param2 != NULL) // Assign parameter #2
			{
				// v1.0.38.01: LPARAM is now written out as a DWORD because the majority of system messages
				// use LPARAM as a pointer or other unsigned value.  This shouldn't affect most scripts because
				// of the way ATOI64() and ATOU() wrap a negative number back into the unsigned domain for
				// commands such as PostMessage/SendMessage.
				aFunc->mParam[1].var->Assign((LPTSTR )param2);
				if (aFunc->mParamCount > 2 && param3 != NULL) // Assign parameter #3
				{
					aFunc->mParam[2].var->Assign((LPTSTR )param3);
					if (aFunc->mParamCount > 3 && param4 != NULL) // Assign parameter #4
					{
						aFunc->mParam[3].var->Assign((LPTSTR )param4);
						if (aFunc->mParamCount > 4 && param5 != NULL) // Assign parameter #5
						{
							aFunc->mParam[4].var->Assign((LPTSTR )param5);
							if (aFunc->mParamCount > 5 && param6 != NULL) // Assign parameter #6
							{
								aFunc->mParam[5].var->Assign((LPTSTR )param6);
								if (aFunc->mParamCount > 6 && param7 != NULL) // Assign parameter #7
								{
									aFunc->mParam[6].var->Assign((LPTSTR )param7);
									if (aFunc->mParamCount > 7 && param8 != NULL) // Assign parameter #8
									{
										aFunc->mParam[7].var->Assign((LPTSTR )param8);
										if (aFunc->mParamCount > 8 && param9 != NULL) // Assign parameter #9
										{
											aFunc->mParam[8].var->Assign((LPTSTR )param9);
											if (aFunc->mParamCount > 9 && param10 != NULL) // Assign parameter #10
											{
												aFunc->mParam[9].var->Assign((LPTSTR )param10);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		FuncAndToken & aFuncAndToken = aFuncAndTokenToReturn[returnCount];
		aFuncAndToken.mFunc = aFunc ;
		returnCount++ ;
		if (returnCount > 9)
			returnCount = 0 ;

		SendMessage(g_hWnd, AHK_EXECUTE_FUNCTION_DLL, (WPARAM)&aFuncAndToken,NULL);
		return aFuncAndToken.result_to_return_dll;
	}
	else
		return _T(""); 
}
//H30 changed to not return anything since it is not used
void callFuncDll(FuncAndToken *aFuncAndToken)
{
 	Func &func =  *(aFuncAndToken->mFunc); 
	ExprTokenType & aResultToken = aFuncAndToken->mToken ;
	// Func &func = *(Func *)g_script.mTempFunc ;
	if (!INTERRUPTIBLE_IN_EMERGENCY)
		return;
	if (g_nThreads >= g_MaxThreadsTotal)
		// Below: Only a subset of ACT_IS_ALWAYS_ALLOWED is done here because:
		// 1) The omitted action types seem too obscure to grant always-run permission for msg-monitor events.
		// 2) Reduction in code size.
		if (g_nThreads >= MAX_THREADS_EMERGENCY // To avoid array overflow, this limit must by obeyed except where otherwise documented.
			|| func.mJumpToLine->mActionType != ACT_EXITAPP && func.mJumpToLine->mActionType != ACT_RELOAD)
			return;

	// Need to check if backup is needed in case script explicitly called the function rather than using
	// it solely as a callback.  UPDATE: And now that max_instances is supported, also need it for that.
	// See ExpandExpression() for detailed comments about the following section.
	VarBkp *var_backup = NULL;   // If needed, it will hold an array of VarBkp objects.
	int var_backup_count; // The number of items in the above array.
	if (func.mInstances > 0) // Backup is needed.
		if (!Var::BackupFunctionVars(func, var_backup, var_backup_count)) // Out of memory.
			return;
			// Since we're in the middle of processing messages, and since out-of-memory is so rare,
			// it seems justifiable not to have any error reporting and instead just avoid launching
			// the new thread.

	// Since above didn't return, the launch of the new thread is now considered unavoidable.

	// See MsgSleep() for comments about the following section.
	TCHAR ErrorLevel_saved[ERRORLEVEL_SAVED_SIZE];
	tcslcpy(ErrorLevel_saved, g_ErrorLevel->Contents(), _countof(ErrorLevel_saved));
	InitNewThread(0, false, true, func.mJumpToLine->mActionType);


	// v1.0.38.04: Below was added to maximize responsiveness to incoming messages.  The reasoning
	// is similar to why the same thing is done in MsgSleep() prior to its launch of a thread, so see
	// MsgSleep for more comments:
	g_script.mLastScriptRest = g_script.mLastPeekTime = GetTickCount();


		DEBUGGER_STACK_PUSH(func.mJumpToLine, func.mName)
	// ExprTokenType aResultToken;
	// ExprTokenType &aResultToken = aResultToken_to_return ;
	func.Call(&aResultToken); // Call the UDF.

		DEBUGGER_STACK_POP()
	switch (aFuncAndToken->mToken.symbol)
	{
	case SYM_VAR: // Caller has ensured that any SYM_VAR's Type() is VAR_NORMAL.
		if (_tcslen(aFuncAndToken->mToken.var->Contents()))
		{
			aFuncAndToken->result_to_return_dll = (LPTSTR )realloc((LPTSTR )aFuncAndToken->result_to_return_dll,_tcslen(aFuncAndToken->mToken.var->Contents())*sizeof(TCHAR));
			_tcscpy(aFuncAndToken->result_to_return_dll,aFuncAndToken->mToken.var->Contents()); // Contents() vs. mContents to support VAR_CLIPBOARD, and in case mContents needs to be updated by Contents().
		}
		else if (aFuncAndToken->result_to_return_dll)
			*aFuncAndToken->result_to_return_dll = '\0';
		break;
	case SYM_STRING:
	case SYM_OPERAND:
		if (_tcslen(aFuncAndToken->mToken.marker))
		{
			aFuncAndToken->result_to_return_dll = (LPTSTR )realloc((LPTSTR )aFuncAndToken->result_to_return_dll,_tcslen(aFuncAndToken->mToken.marker)*sizeof(TCHAR));
			_tcscpy(aFuncAndToken->result_to_return_dll,aFuncAndToken->mToken.marker);
		}
		else if (aFuncAndToken->result_to_return_dll)
			*aFuncAndToken->result_to_return_dll = '\0';
		break;
	case SYM_INTEGER:
		aFuncAndToken->result_to_return_dll = (LPTSTR )realloc((LPTSTR )aFuncAndToken->result_to_return_dll,MAX_INTEGER_LENGTH);
		ITOA64(aFuncAndToken->mToken.value_int64, aFuncAndToken->result_to_return_dll);
		break;
	case SYM_FLOAT:
		result_to_return_dll = (LPTSTR )realloc((LPTSTR )aFuncAndToken->result_to_return_dll,MAX_INTEGER_LENGTH);
		sntprintf(aFuncAndToken->result_to_return_dll, MAX_NUMBER_SIZE, g->FormatFloat, aFuncAndToken->mToken.value_double);
		break;
	//case SYM_OBJECT: // L31: Treat objects as empty strings (or TRUE where appropriate).
	default: // Not an operand: continue on to return the default at the bottom.
		if (aFuncAndToken->result_to_return_dll)
			*aFuncAndToken->result_to_return_dll = '\0';
	}
	
	//Var::FreeAndRestoreFunctionVars(func, var_backup, var_backup_count);
	ResumeUnderlyingThread(ErrorLevel_saved);
	return;
}




void AssignVariant(Var &aArg, VARIANT &aVar, bool aRetainVar);
VARIANT ahkFunctionVariant(LPTSTR func, VARIANT param1,/*[in,optional]*/ VARIANT param2,/*[in,optional]*/ VARIANT param3,/*[in,optional]*/ VARIANT param4,/*[in,optional]*/ VARIANT param5,/*[in,optional]*/ VARIANT param6,/*[in,optional]*/ VARIANT param7,/*[in,optional]*/ VARIANT param8,/*[in,optional]*/ VARIANT param9,/*[in,optional]*/ VARIANT param10, int sendOrPost)
{
	Func *aFunc = g_script.FindFunc(func) ;
	if (aFunc)
	{	
		// g_script.mTempFunc = aFunc ;
		// ExprTokenType return_value;
		if (aFunc->mParamCount > 0 && &param1 != NULL)
		{
			// Copy the appropriate values into each of the function's formal parameters.
			AssignVariant(*aFunc->mParam[0].var, param1, false); // Assign parameter #1
			if (aFunc->mParamCount > 1  && &param2 != NULL) // Assign parameter #2
			{
				// v1.0.38.01: LPARAM is now written out as a DWORD because the majority of system messages
				// use LPARAM as a pointer or other unsigned value.  This shouldn't affect most scripts because
				// of the way ATOI64() and ATOU() wrap a negative number back into the unsigned domain for
				// commands such as PostMessage/SendMessage.
				AssignVariant(*aFunc->mParam[1].var, param2, false);
				if (aFunc->mParamCount > 2 && &param3 != NULL) // Assign parameter #3
				{
					AssignVariant(*aFunc->mParam[2].var, param3, false);
					if (aFunc->mParamCount > 3 && &param4 != NULL) // Assign parameter #4
					{
						AssignVariant(*aFunc->mParam[3].var, param4, false);
						if (aFunc->mParamCount > 4 && &param5 != NULL) // Assign parameter #5
						{
							AssignVariant(*aFunc->mParam[4].var, param5, false);
							if (aFunc->mParamCount > 5 && &param6 != NULL) // Assign parameter #6
							{
								AssignVariant(*aFunc->mParam[5].var, param6, false);
								if (aFunc->mParamCount > 6 && &param7 != NULL) // Assign parameter #7
								{
									AssignVariant(*aFunc->mParam[6].var, param7, false);
									if (aFunc->mParamCount > 7 && &param8 != NULL) // Assign parameter #8
									{
										AssignVariant(*aFunc->mParam[7].var, param8, false);
										if (aFunc->mParamCount > 8 && &param9 != NULL) // Assign parameter #9
										{
											AssignVariant(*aFunc->mParam[8].var, param9, false);
											if (aFunc->mParamCount > 9 && &param10 != NULL) // Assign parameter #10
											{
												AssignVariant(*aFunc->mParam[9].var, param10, false);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		FuncAndToken & aFuncAndToken = aFuncAndTokenToReturn[returnCount];
		aFuncAndToken.mFunc = aFunc ;
		returnCount++ ;
		if (returnCount > 9)
			returnCount = 0 ;

		if (sendOrPost == 1)
		{
		SendMessage(g_hWnd, AHK_EXECUTE_FUNCTION_VARIANT, (WPARAM)&aFuncAndToken,NULL);
		return aFuncAndToken.variant_to_return_dll;
		}
		else
		{
			PostMessage(g_hWnd, AHK_EXECUTE_FUNCTION_VARIANT, (WPARAM)&aFuncAndToken,NULL);
			VARIANT &r =  aFuncAndToken.variant_to_return_dll;
			r.vt = VT_NULL ;
			return r ; 
		}
		}
	FuncAndToken & aFuncAndToken = aFuncAndTokenToReturn[returnCount];
	returnCount++ ;

	VARIANT &r =  aFuncAndToken.variant_to_return_dll;
	r.vt = VT_NULL ;
	return r ; 
	// should return a blank variant
}


void TokenToVariant(ExprTokenType &aToken, VARIANT &aVar);

void callFuncDllVariant(FuncAndToken *aFuncAndToken)
{
 	Func &func =  *(aFuncAndToken->mFunc); 
	ExprTokenType & aResultToken = aFuncAndToken->mToken ;
	// Func &func = *(Func *)g_script.mTempFunc ;
	if (!INTERRUPTIBLE_IN_EMERGENCY)
		return;
	if (g_nThreads >= g_MaxThreadsTotal)
		// Below: Only a subset of ACT_IS_ALWAYS_ALLOWED is done here because:
		// 1) The omitted action types seem too obscure to grant always-run permission for msg-monitor events.
		// 2) Reduction in code size.
		if (g_nThreads >= MAX_THREADS_EMERGENCY // To avoid array overflow, this limit must by obeyed except where otherwise documented.
			|| func.mJumpToLine->mActionType != ACT_EXITAPP && func.mJumpToLine->mActionType != ACT_RELOAD)
			return;

	// Need to check if backup is needed in case script explicitly called the function rather than using
	// it solely as a callback.  UPDATE: And now that max_instances is supported, also need it for that.
	// See ExpandExpression() for detailed comments about the following section.
	VarBkp *var_backup = NULL;   // If needed, it will hold an array of VarBkp objects.
	int var_backup_count; // The number of items in the above array.
	if (func.mInstances > 0) // Backup is needed.
		if (!Var::BackupFunctionVars(func, var_backup, var_backup_count)) // Out of memory.
			return;
			// Since we're in the middle of processing messages, and since out-of-memory is so rare,
			// it seems justifiable not to have any error reporting and instead just avoid launching
			// the new thread.

	// Since above didn't return, the launch of the new thread is now considered unavoidable.

	// See MsgSleep() for comments about the following section.
	TCHAR ErrorLevel_saved[ERRORLEVEL_SAVED_SIZE];
	tcslcpy(ErrorLevel_saved, g_ErrorLevel->Contents(), _countof(ErrorLevel_saved));
	InitNewThread(0, false, true, func.mJumpToLine->mActionType);


	// v1.0.38.04: Below was added to maximize responsiveness to incoming messages.  The reasoning
	// is similar to why the same thing is done in MsgSleep() prior to its launch of a thread, so see
	// MsgSleep for more comments:
	g_script.mLastScriptRest = g_script.mLastPeekTime = GetTickCount();


		DEBUGGER_STACK_PUSH(func.mJumpToLine, func.mName)
	// ExprTokenType aResultToken;
	// ExprTokenType &aResultToken = aResultToken_to_return ;
	func.Call(&aResultToken); // Call the UDF.
	TokenToVariant(aResultToken, aFuncAndToken->variant_to_return_dll);

	DEBUGGER_STACK_POP()
	
	ResumeUnderlyingThread(ErrorLevel_saved);
	return;
}
