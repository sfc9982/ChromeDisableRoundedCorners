#pragma warning(disable : 4996)

#define UNICODE

#include <stdio.h>
#include <windows.h>
#include "detours.h"

static BOOL (WINAPI * TrueGetVersionEx)(OSVERSIONINFO *osvi) = GetVersionEx;

BOOL WINAPI W10GetVersionEx(OSVERSIONINFO *osvi)
{
    BOOL ret = TrueGetVersionEx(osvi);

	if (ret) {
		osvi->dwMajorVersion = 10;
		osvi->dwMinorVersion = 0;
		osvi->dwBuildNumber  = 19045;
	}
	
	printf("Called W10GetVersionEx().\n");
	
    return ret;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    LONG error;
    (void)hinst;
    (void)reserved;

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();

        printf("winver_hook" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
               " Starting.\n");
        fflush(stdout);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TrueGetVersionEx, W10GetVersionEx);
        error = DetourTransactionCommit();

        if (error == NO_ERROR) {
            printf("winver_hook" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
                   " Detoured GetVersionEx().\n");
        }
        else {
            printf("winver_hook" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
                   " Error detouring GetVersionEx(): %ld\n", error);
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueGetVersionEx, W10GetVersionEx);
        error = DetourTransactionCommit();

        printf("winver_hook" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
               " Removed GetVersionEx() (result=%s).\n", error ? "Fail" : "Success");
        fflush(stdout);
    }
    return TRUE;
}
