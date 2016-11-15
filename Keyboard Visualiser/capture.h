#pragma once

#define REFTIMES_PER_SEC  10000000 //REFERENCE_TIME is in 100's of ns
#define REFTIMES_PER_MILLISEC  10000

class CaptureThreadContext {
public:
	ScopedIMMDevice* pScopeddevice;

};