#include <string>

#include "service.h"
//sc create MyWinService binPath= "C:\Users\solod\Desktop\MTUCI\3course\winService\cmake-build-debug\winService.exe"

int wmain(int argc, wchar_t* argv[]) {
    if (argc > 1 && std::wstring(argv[1]) == L"run") {
        ServiceMain(0, nullptr);
        return 0;
    }

    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {const_cast<LPWSTR>(L"MySimpleService"), ServiceMain},
        {nullptr, nullptr}
    };
    return StartServiceCtrlDispatcher(ServiceTable) ? 0 : 1;
}
