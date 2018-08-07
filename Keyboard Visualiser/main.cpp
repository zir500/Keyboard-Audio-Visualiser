#include "common.h"
#include "Functiondiscoverykeys_devpkey.h"
#include "capture.h"

int wmain(int argc, LPCWSTR argv[]) {
	HRESULT hr = S_OK;
	CComPtr<IMMDevice> pIMMDevice;
	CComPtr<IMMDeviceEnumerator> pIMMDeviceEnumerator;
	CComPtr<IPropertyStore> pDefaultDeviceProperties;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	EXIT_ON_ERROR("CoInitializeEx", hr);

	hr = pIMMDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
	EXIT_ON_ERROR("CoCreateInstance IMMDeviceEnumerator", hr);

	hr = pIMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pIMMDevice);
	EXIT_ON_ERROR("GetDefaultAudioEndpoint", hr);

	hr = pIMMDevice->OpenPropertyStore(STGM_READ, &pDefaultDeviceProperties);
	EXIT_ON_ERROR("OpenPropertyStore", hr);

	PROPVARIANT pDefaultDeviceFriendlyName;
	PropVariantInit(&pDefaultDeviceFriendlyName);
	hr = pDefaultDeviceProperties->GetValue(PKEY_Device_FriendlyName, &pDefaultDeviceFriendlyName);
	EXIT_ON_ERROR("GetValue(Friendly Name)", hr);

	printf("%S", pDefaultDeviceFriendlyName.pwszVal);

	PropVariantClear(&pDefaultDeviceFriendlyName);

	// Capture Data
	beginCapture(pIMMDevice);
	printf("\n Finished!");
	getchar();

	// Normal Exit
	CoUninitialize();
	return 0;
}
