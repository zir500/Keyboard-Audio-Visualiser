#pragma once
#include "common.h"


//RAII Class for COM Initialization
class ScopedCoInitializeEx {
public:
	ScopedCoInitializeEx() {
		HRESULT hr = S_OK;
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		EXIT_ON_ERROR("CoInitialise Failed", hr)
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

		EXIT_ON_ERROR("CoCreateInstance for Device Enumerator", hr)
	}

	~ScopedIMMDeviceEnumerator() {
		pEnumerator->Release();
	}
};

class ScopedIAudioClient {
public:
	IAudioClient* pAudioClient;

	ScopedIAudioClient(ScopedIMMDevice* pScopedDevice) {
		HRESULT hr = S_OK;
		hr = pScopedDevice->device->Activate(
			__uuidof(IAudioClient),
			CLSCTX_ALL,
			NULL,
			(void**)pAudioClient
		);
		EXIT_ON_ERROR("Activating IAudioClient", hr);
	}

	HRESULT Start() {
		isStarted = true;
		return pAudioClient->Start();
	}

	HRESULT Stop() {
		isStarted = false;
		return pAudioClient->Stop();
	}

	~ScopedIAudioClient() {
		if (isStarted) {
			Stop();
		}
		pAudioClient->Release();
	}
private:
	bool isStarted = false;
};