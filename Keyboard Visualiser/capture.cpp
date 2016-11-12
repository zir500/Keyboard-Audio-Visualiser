#include "common.h"
#include "capture.h"


void beginCaptureThread(LPVOID pContext) {
	HRESULT hr = S_OK;
	WAVEFORMATEX* pwfex;
	REFERENCE_TIME hnsDesiredDuration = 10000000;//One Second

	CaptureThreadContext* pArgs = (CaptureThreadContext*)pContext;

	ScopedCoInitializeEx captureThreadCoInit();

	ScopedIAudioClient scopedAudioClient(pArgs->pScopeddevice);

	hr = scopedAudioClient.pAudioClient->GetMixFormat(&pwfex);
	EXIT_ON_ERROR("GetMixFormat", hr);

	hr = scopedAudioClient.pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		hnsDesiredDuration,
		0,
		pwfex,
		NULL
	);
	EXIT_ON_ERROR("Initialize", hr);

	hr = scopedAudioClient.pAudioClient->GetService(
		
	);
}