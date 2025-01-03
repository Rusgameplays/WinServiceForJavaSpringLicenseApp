#ifndef SERVICE_H
#define SERVICE_H

#include <windows.h>

void WINAPI ServiceCtrlHandler(DWORD dwCtrlCode);
void WINAPI ServiceMain(DWORD, LPWSTR*);

#endif // SERVICE_H
