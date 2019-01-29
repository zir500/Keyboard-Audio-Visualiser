#pragma once

#define REFTIMES_PER_SEC  10000000.0l //REFERENCE_TIME is in 100's of ns
#define REFTIMES_PER_MILLISEC  10000.0l

void beginCapture(CComPtr<IMMDevice>& pIMMDevice);
void readAudioBuffer(WAVEFORMATEX* pwfex, REFERENCE_TIME& hnsActualDuration, UINT32& numBufferFrames, CComPtr<IAudioCaptureClient>& pAudioCaptureClient);
