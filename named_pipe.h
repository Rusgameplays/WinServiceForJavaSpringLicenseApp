#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include <windows.h>

HANDLE CreateNamedPipeInstance();
void HandleClient(HANDLE pipe);

#endif // NAMED_PIPE_H
