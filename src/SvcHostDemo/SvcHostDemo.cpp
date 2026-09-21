#include <windows.h>

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    return TRUE;
}

SERVICE_STATUS_HANDLE g_serviceStatusHandle = nullptr;

SERVICE_STATUS g_serviceStatus =
{
    SERVICE_WIN32_SHARE_PROCESS,
    SERVICE_START_PENDING,
    SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN
};

DWORD WINAPI HandlerEx(
    DWORD dwControl,
    DWORD dwEventType,
    LPVOID lpEventData,
    LPVOID lpContext
)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        return NO_ERROR;

    case SERVICE_CONTROL_INTERROGATE:
        SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus);
        return NO_ERROR;

    default:
        return NO_ERROR;
    }
}

extern "C" __declspec(dllexport) VOID WINAPI ServiceMain(
    DWORD dwArgc,
    LPCWSTR* lpszArgv
)
{
    g_serviceStatusHandle =
        RegisterServiceCtrlHandlerExW(
            lpszArgv[0],
            HandlerEx,
            nullptr
        );

    if (!g_serviceStatusHandle)
    {
        return;
    }

    g_serviceStatus.dwCurrentState = SERVICE_RUNNING;
    g_serviceStatus.dwWin32ExitCode = ERROR_SUCCESS;
    g_serviceStatus.dwServiceSpecificExitCode = 0;
    g_serviceStatus.dwCheckPoint = 0;
    g_serviceStatus.dwWaitHint = 0;

    if (!SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus))
    {
        return;
    }

    Sleep(30000);

    g_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
    g_serviceStatus.dwWin32ExitCode = ERROR_SUCCESS;
    g_serviceStatus.dwServiceSpecificExitCode = 0;
    g_serviceStatus.dwCheckPoint = 0;
    g_serviceStatus.dwWaitHint = 0;

    SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus);

    g_serviceStatus.dwCurrentState = SERVICE_STOPPED;
    g_serviceStatus.dwWin32ExitCode = ERROR_SUCCESS;
    g_serviceStatus.dwServiceSpecificExitCode = 0;
    g_serviceStatus.dwCheckPoint = 0;
    g_serviceStatus.dwWaitHint = 0;

    SetServiceStatus(g_serviceStatusHandle, &g_serviceStatus);
}
