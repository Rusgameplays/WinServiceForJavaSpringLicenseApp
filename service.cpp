#include "service.h"

#include <iostream>
#include <string>
#include <thread>
#include "named_pipe.h"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle = nullptr;
HANDLE hPipe = INVALID_HANDLE_VALUE;
bool keepRunning = true;

void WINAPI ServiceCtrlHandler(DWORD dwCtrlCode) {
    if (dwCtrlCode == SERVICE_CONTROL_STOP) {
        keepRunning = false;

        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
    }
}


void WINAPI ServiceMain(DWORD, LPWSTR*) {
    hServiceStatusHandle = RegisterServiceCtrlHandler(L"MySimpleService", ServiceCtrlHandler);

    ServiceStatus = {SERVICE_WIN32_OWN_PROCESS, SERVICE_RUNNING, SERVICE_ACCEPT_STOP};
    SetServiceStatus(hServiceStatusHandle, &ServiceStatus);

    hPipe = CreateNamedPipeInstance();
    if (hPipe == INVALID_HANDLE_VALUE) return;

    while (keepRunning) {
        if (ConnectNamedPipe(hPipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
            std::thread(HandleClient, hPipe).detach();

            hPipe = CreateNamedPipeInstance();
            if (hPipe == INVALID_HANDLE_VALUE) break;
        }
    }

    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }
}
