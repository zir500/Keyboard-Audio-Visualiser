#include "common.h"
#include "capture.h"

void beginCapture(CComPtr<IMMDevice>& pIMMDevice) {
	HRESULT hr = S_OK;
	WAVEFORMATEX* pwfex;
	REFERENCE_TIME hnsDesiredDuration = REFTIMES_PER_SEC; //(Requested buffer length)
	REFERENCE_TIME hnsActualDuration; //Actual Buffer Length assigned
	UINT32 numBufferFrames;

	CComPtr<IAudioClient> pAudioClient;
	CComPtr<IAudioCaptureClient> pAudioCaptureClient;
	
	// Get AudioClient Object
	hr = pIMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&pAudioClient));
	EXIT_ON_ERROR("Activate IMMDevice", hr);

	hr = pAudioClient->GetMixFormat(&pwfex);
	EXIT_ON_ERROR("GetMixFormat", hr);

	//Init AudioClient in Loopback mode
	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsDesiredDuration, 0, pwfex, NULL);
	EXIT_ON_ERROR("AudioClient Initialize", hr);

	// Find out how much buffer was actually assigned (May be different to desired size)
	hr = pAudioClient->GetBufferSize(&numBufferFrames);
	EXIT_ON_ERROR("GetBufferSize", hr);
	hnsActualDuration = REFTIMES_PER_SEC * numBufferFrames / pwfex->nSamplesPerSec;

	hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), reinterpret_cast<void**>(&pAudioCaptureClient));
	EXIT_ON_ERROR("GetService (AudioCaptureClient)", hr);

	hr = pAudioClient->Start();
	EXIT_ON_ERROR("AudioClient Start", hr);

	readAudioBuffer(pwfex, hnsActualDuration, numBufferFrames, pAudioCaptureClient);
}

// blockAlign: The number of bytes for each frame (each sample) this includes both channels
void readAudioBuffer(WAVEFORMATEX* pwfex, REFERENCE_TIME& hnsActualDuration, UINT32& numBufferFrames, CComPtr<IAudioCaptureClient>& pAudioCaptureClient) {
	HRESULT hr = S_OK;
	bool isDone = false;
	UINT32 packetLength = 0;//Number of frames in a packet
	BYTE* pFrameData;
	UINT32 numFramesAvail;
	UINT32 blockAlign = pwfex->nBlockAlign;
	DWORD flags;
	while (!isDone) {
		//Main Capture Loop
		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2.0);//Allow the buffer to partially fill before we process and empty it.

		hr = pAudioCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR("GetNextPacketSize", hr);

			while (packetLength != 0) {
				Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2.0);//Allow the buffer to partially fill before we process and empty it.
				hr = pAudioCaptureClient->GetBuffer(&pFrameData, &numFramesAvail, &flags, NULL, NULL);
				EXIT_ON_ERROR("GetBuffer", hr);

				if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
					pFrameData = NULL;
				}

				//Finally we will send the data to the fourier module.
				long packetSizeBytes = numFramesAvail * blockAlign;
				//Copy 'packetSizeBytes' bytes from the pFrameBuffer to the fourier thread input  
				//Handle any error in copying the data

				hr = pAudioCaptureClient->ReleaseBuffer(numFramesAvail);
				EXIT_ON_ERROR("ReleaseBuffer", hr);

				hr = pAudioCaptureClient->GetNextPacketSize(&packetLength);
				EXIT_ON_ERROR("GetNextPacketSize", hr);
				isDone = true;
			}
	}
}
