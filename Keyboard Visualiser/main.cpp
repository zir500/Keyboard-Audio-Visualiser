#include "common.h"

HRESULT getDefaultDevice(IMMDevice** ppDevice)
{
	HRESULT hr = S_OK;
	ScopedIMMDeviceEnumerator enumerator;
	hr = enumerator.pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, ppDevice);
	EXIT_ON_ERROR("GetDefaultAudioEndpoint ", hr)
	return hr;
}

int wmain(int argc, LPCWSTR argv[]) {
	HRESULT hr = S_OK; 
	ScopedIMMDevice immDevice;

	//Initialize for COM
	ScopedCoInitializeEx CoInit;

	hr = getDefaultDevice(&(immDevice.device));

}
