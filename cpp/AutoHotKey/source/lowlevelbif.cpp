#include "stdafx.h" // pre-compiled headers
#include "globaldata.h" // for access to many global vars
#include "application.h" // for MsgSleep()
#include "exports.h"
#include "script.h"

#define BIF(fun) void fun(ExprTokenType &aResultToken, ExprTokenType *aParam[], int aParamCount)

BIF(BIF_FindFunc) // Added in Nv8.
{
	// Set default return value in case of early return.
	aResultToken.symbol = SYM_INTEGER ;
	aResultToken.marker = _T("");
	// Get the first arg, which is the string used as the source of the extraction. Call it "findfunc" for clarity.
	TCHAR funcname_buf[MAX_NUMBER_SIZE]; // A separate buf because aResultToken.buf is sometimes used to store the result.
	LPTSTR funcname = TokenToString(*aParam[0], funcname_buf); // Remember that aResultToken.buf is part of a union, though in this case there's no danger of overwriting it since our result will always be of STRING type (not int or float).
	int funcname_length = (int)EXPR_TOKEN_LENGTH(aParam[0], funcname);
	aResultToken.value_int64 = (__int64)ahkFindFunc(funcname);
	return;
}


BIF(BIF_Getvar)
{
	int i = 0;
	if (aParam[0]->symbol == SYM_VAR)
		i = (int)aParam[0]->var;
	aResultToken.value_int64 = i;
}


BIF(BIF_Static)
{
	if (aParam[0]->symbol == SYM_VAR)
	{
		Var *var = aParam[0]->var;
		if (var->mType == VAR_ALIAS)
			var = var->mAliasFor;
		var->mAttrib |= VAR_ATTRIB_STATIC;
	}
}

BIF(BIF_Alias)
{
	ExprTokenType &aParam0 = *aParam[0];
	ExprTokenType &aParam1 = *aParam[1];
	if (aParam0.symbol == SYM_VAR)
	{
		Var &var = *aParam0.var;

		UINT len = 0;
		switch (aParam1.symbol)
		{
		case SYM_VAR:
		case SYM_INTEGER:
			len = (UINT)aParam1.var;
		break;
		// HotKeyIt H10 added to accept dynamic text and also when value is returned by ahkgetvar in AutoHotkey.dll
		case SYM_OPERAND:
			len = (UINT)ATOI64(aParam1.marker);
		}
		var.mType = len ? VAR_ALIAS : VAR_NORMAL;
		var.mByteLength = len;
	}
}
BIF(BIF_CacheEnable)
{
	if (aParam[0]->symbol == SYM_VAR)
	{
		(aParam[0]->var->mType == VAR_ALIAS ? aParam[0]->var->mAliasFor : aParam[0]->var)
			->mAttrib &= ~VAR_ATTRIB_CACHE_DISABLED;
	}
}

BIF(BIF_getTokenValue)
{
   ExprTokenType *token = aParam[0];
   
   if (token->symbol != SYM_INTEGER)
      return;
   
   token = (ExprTokenType*) token->value_int64;

    if (token->symbol == SYM_VAR)
    {
      Var &var = *token->var;

      VarAttribType cache_attrib = var.mAttrib & (VAR_ATTRIB_HAS_VALID_INT64 | VAR_ATTRIB_HAS_VALID_DOUBLE);
      if (cache_attrib)
      {
         aResultToken.symbol = (SymbolType) (cache_attrib >> 4);
         aResultToken.value_int64 = var.mContentsInt64;
      }

      else if (var.mAttrib & VAR_ATTRIB_OBJECT)
      {
         aResultToken.symbol = SYM_OBJECT;
         aResultToken.object = var.mObject;
      }
      else
      {
         aResultToken.symbol = SYM_OPERAND;
         aResultToken.marker = var.mCharContents;
      }
    }
    else
    {
        aResultToken.symbol = token->symbol;
        aResultToken.value_int64 = token->value_int64;
    }
    if (aResultToken.symbol == SYM_OBJECT)
        aResultToken.object->AddRef();
}