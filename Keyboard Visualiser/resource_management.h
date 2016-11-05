#pragma once
#include "common.h"


//RAII Class for COM Initialization
class ScopedCoInitializeEx {
public:
	ScopedCoInitializeEx() {
		HRESULT hr = S_OK;
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		EXIT_IF_FAIL("CoInitialise Failed", hr)
	}

	~ScopedCoInitializeEx() {
		CoUninitialize();
	}
};

class ScopedIMMDevice {
public:
	IMMDevice* device;
	ScopedIMMDevice():device(NULL){}

	~ScopedIMMDevice() {
		if (device != NULL) {
			device->Release();
		}
	}
};

class ScopedIMMDeviceEnumerator {
public:
	IMMDeviceEnumerator* pEnumerator;

	ScopedIMMDeviceEnumerator() {
		HRESULT hr = S_OK;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), 
			NULL, 
			CLSCTX_ALL, 
			__uuidof(IMMDeviceEnumerator), 
			(void**) &pEnumerator);

		EXIT_IF_FAIL("CoCreateInstance for Device Enumerator", hr)
	}

	~ScopedIMMDeviceEnumerator() {
		pEnumerator->Release();
	}
};