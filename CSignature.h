#pragma once

#include "SDK.h"

namespace sigs {
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern);
	HMODULE GetModuleHandleSafe(const char* pszModuleName);
	DWORD GetClientSignature(char* chPattern);
	DWORD GetEngineSignature(char* chPattern);
	DWORD GetDxSignature(char* chPattern);
	DWORD GetCurSignature(char* chPattern);
	DWORD FindSig(char* szMdl, char* szPtrn); //Universal Version of Get*Signature!
}