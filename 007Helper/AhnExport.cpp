#include "AhnExport.h"
#include "AhnProcess.h"

#pragma comment(linker, "/EXPORT:AhnCreateProcessA=_AhnCreateProcessA@12")



extern "C"
{

	__declspec(dllexport) BOOL WINAPI AhnCreateProcessA(char* ulFileName, char* ulCmdLine, char* ulPath)
	{
		BOOL	nRet = FALSE;

		auto Process = std::make_unique<CACEProcess>();

		Process->ProcessSetToken();

		STARTUPINFOA	ulStartInfo = { 0 };

		ulStartInfo.cb = sizeof(STARTUPINFOA);

		PROCESS_INFORMATION	ulProcessInfo = { 0 };

		if (AhnProcess::extCreateProcessA(ulFileName, ulCmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE | CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, ulPath, &ulStartInfo, &ulProcessInfo))
		{
			ResumeThread(ulProcessInfo.hThread);
		}
		return nRet;
	}


}