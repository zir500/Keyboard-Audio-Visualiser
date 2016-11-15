#include "common.h"
#include "capture.h"


//Capture Thread - Begins and handles actual capture of audio data before sending it to the fourier and display thread
void beginCaptureThread(LPVOID pContext) {
	HRESULT hr = S_OK;
	WAVEFORMATEX* pwfex;
	REFERENCE_TIME hnsDesiredDuration = REFTIMES_PER_SEC;//(Requested buffer length)
	REFERENCE_TIME hnsActualDuration; //Actual Buffer Length assigned
	bool isDone = false;

	CaptureThreadContext* pArgs = (CaptureThreadContext*)pContext;

	//Initialize OCM Library
	ScopedCoInitializeEx captureThreadCoInit();

	//Get an audioClient from the device
	ScopedIAudioClient scopedAudioClient(pArgs->pScopeddevice);

	hr = scopedAudioClient.pAudioClient->GetMixFormat(&pwfex);
	EXIT_ON_ERROR("GetMixFormat", hr);

	//Initialize the audio client in loopback mode 
	hr = scopedAudioClient.pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_LOOPBACK,
		hnsDesiredDuration,
		0,
		pwfex,
		NULL
	);
	EXIT_ON_ERROR("Initialize", hr);

	//Get the actual buffer size 
	UINT32 numBufferFrames;
	hr = scopedAudioClient.pAudioClient->GetBufferSize(&numBufferFrames);
	EXIT_ON_ERROR("GetBufferSize() after initialization", hr);

	hnsActualDuration = (double)REFTIMES_PER_SEC * numBufferFrames / pwfex->nSamplesPerSec;


	//Get a Capture client
	ScopedIAudioCaptureClient scopedAudioCaptureClient(&scopedAudioClient);

	hr = scopedAudioClient.Start();
	EXIT_ON_ERROR("AudioCapture Start()", hr);


	UINT32 packetLength = 0;//Number of frames in a packet
	BYTE* pFrameData;
	UINT32 numFramesAvail;
	DWORD flags;
	UINT32 blockAlign = pwfex->nBlockAlign; //The number of bytes for each frame (each sample) this includes both channels
	while (!isDone) {
		//Main Capture Loop

		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);//Allow the buffer to partially fill before we process and empty it.

		hr = scopedAudioCaptureClient.pCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR("GetNextPacketSize()", hr)

			while (packetLength != 0) {
				hr = scopedAudioCaptureClient.pCaptureClient->GetBuffer(&pFrameData, &numFramesAvail, &flags, NULL, NULL);
				EXIT_ON_ERROR("GetBuffer()", hr)

					if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
						pFrameData = NULL;
					}

				//Finally we will send the data to the fourier module.
				long packetSizeBytes = numFramesAvail * blockAlign;
				//Copy 'packetSizeBytes' bytes to the fourier thread input  
				//Handle any error in copying the data

				hr = scopedAudioCaptureClient.pCaptureClient->ReleaseBuffer(numBufferFrames);
				EXIT_ON_ERROR("ReleaseBuffer()", hr)

				hr = scopedAudioCaptureClient.pCaptureClient->GetNextPacketSize(&packetLength);
				EXIT_ON_ERROR("GetNextPacketSize() after aquiring buffer data", hr)
			}
	}
}