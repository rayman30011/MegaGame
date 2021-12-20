#include <MegaEngineStd.h>

#include "Initialization.h"

bool MegaEngine::Core::Initialization::IsOnlyInstance(LPCTSTR gameTitle)
{
    HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);
    if (GetLastError() != ERROR_SUCCESS)
    {
        HWND hWnd = FindWindow(gameTitle, NULL);
        if (hWnd)
        {
            ShowWindow(hWnd, SW_SHOWNORMAL);
            SetFocus(hWnd);
            SetForegroundWindow(hWnd);
            SetActiveWindow(hWnd);

            return false;
        }
    }
}

bool MegaEngine::Core::Initialization::CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	return true;
}

bool MegaEngine::Core::Initialization::CheckMemory(const DWORDLONG ramNeeded, const DWORDLONG virtualRamNeeded)
{
    MEMORYSTATUSEX status;
    GlobalMemoryStatusEx(&status);
    if (status.ullTotalPhys < ramNeeded)
    {
        _ERROR(TEXT("CheckMemory Failure: Not enough physical memory"));
        return false;
    }

    if (status.ullTotalVirtual < virtualRamNeeded)
    {
        _ERROR(TEXT("CheckMemory Failure: Not enough virtual memory"));
        return false;
    }

    char* buff = _NEW char[virtualRamNeeded];
    if (buff)
    {
        delete[] buff;
    }
    else
    {
        _ERROR(TEXT("CheckMemory Failure: Not enough virtual memory"));
        return false;
    }

    return true;
}

DWORD MegaEngine::Core::Initialization::ReadCPUSpeed()
{
    DWORD bufSize = sizeof(DWORD);
    DWORD dwMHz = 0;
    DWORD type = REG_DWORD;
    HKEY hKey;

    HRESULT error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
    if (error == ERROR_SUCCESS)
    {
        RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &bufSize);
    }

    return dwMHz;
}
