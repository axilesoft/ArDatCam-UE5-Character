#include "UpDllUtils.h"

//VK

#if 1//defined(_DEBUG) 
#if defined(_WIN32)
#include <helpers/DebugLogLib.h>
#define DP(_X_) DbgPrint _X_ 
#elif defined(__ANDROID__)
#define DP(_X_) LOGV _X_ 
#endif
#else
#define DP(_X_) 
#endif

#include "S:/AProj/VkUpDll/VkUpDll.h"
VKDLLGETPROC; 

int dlCmd(int cmd, int pm1, int pm2)
{
	if (!vudGetProcAddress) return 0;
	//if (!vudGetProcAddress)
	//{
	//	HMODULE hm = GetModuleHandleA(VKDLL_PATH) ? GetModuleHandleA(VKDLL_PATH) : LoadLibraryA(VKDLL_PATH);
	//	vudGetProcAddress = (PFN_vudGetProcAddress)(void*)GetProcAddress(hm, "vudGetProcAddressExport");
	//}
	DeclVkUpDllFuncVarAndLoad(veCmd);

	VeCmdStruct pm;
	pm.cmd = cmd;
	pm.pm1 = pm1;
	pm.pm2 = pm2;
	return veCmd(pm);
}

int dlSendCmd(int cmd, int64_t pm1, int64_t pm2, const  wchar_t* pwc,bool sync)
{
	if (!vudGetProcAddress) return 0;
	DeclVkUpDllFuncVarAndLoad(veSendCmd);

	VeCmdStruct pm;
	pm.cmd = cmd;
	pm.pm1 = pm1;
	pm.pm2 = pm2;
	pm.pwc = (wchar_t*)pwc;
	pm.sync = sync;
	return veSendCmd(pm);
}