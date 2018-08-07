//log.h Simple Logging defines
#pragma  once
#include <stdio.h>

#define LOG(FORMAT, ...) wprintf(L"[  LOG  ] " FORMAT L"\n", __VA_ARGS__)
#define ERR(FORMAT, ...) wprintf(L"[ ERROR ] " FORMAT L"\n", __VA_ARGS__)
#define EHR(MESSAGE, HRES) wprintf(L"[ ERROR ] " MESSAGE L" HRESULT = 0x%08x\n", HRES)

// If the handle indicates an error, a message is displayed and the program exits.  COM Library is uninitialized, smart pointers should clean up all other COM objects.
#define EXIT_ON_ERROR(message, hr) if(FAILED(hr)){EHR(message, hr);system("pause");CoUninitialize();exit(1);}