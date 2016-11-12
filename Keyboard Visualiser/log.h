//log.h Simple Logging defines
#pragma  once
#include <stdio.h>

#define LOG(FORMAT, ...) wprintf(L"[  LOG  ] " FORMAT L"\n", __VA_ARGS__)
#define ERR(FORMAT, ...) wprintf(L"[ ERROR ] " FORMAT L"\n", __VA_ARGS__)
#define EHR(MESSAGE, HRES) wprintf(L"[ ERROR ] " MESSAGE L" HRESULT = 0x%08x", HRES)

#define EXIT_ON_ERROR(message, hr) if(FAILED(hr)){EHR(message, hr); exit(hr);}