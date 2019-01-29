#include "common.h"
#include "fourier.h"
#include "terminal_visuliser.h"
#include <sstream>
#include <iomanip>

#include "fftw3.h"

#define SAMPLES 128
#define BINS SAMPLES/2

static float* in;
static fftwf_complex* out;
static fftwf_plan plan;

void setupFFT() {
	//int n, double* in, fftw_complex *out, unsigned flags
	in = (float*)fftwf_malloc(sizeof(float)*SAMPLES);
	out = (fftwf_complex*)fftwf_alloc_complex(sizeof(fftwf_complex)*BINS);

	plan = fftwf_plan_dft_r2c_1d(SAMPLES, in, out, NULL);
}

void doFFT(BYTE* pFrameData, UINT32 numFramesAvaliable, WAVEFORMATEX* pwfex) {
	std::ostringstream infoStream;
	infoStream << "Frames: " << std::setfill('0') << std::setw(4) << numFramesAvaliable << "  Sample Rate: " << pwfex->nSamplesPerSec;
	writeAdditional(infoStream.str());

	// audioData[Frame][Channel]
	UINT32* audioData = reinterpret_cast<UINT32*>(pFrameData);

	// CAN EASILY GO OUT OF BOUNDS. IF AUDIO DATA INS TBIG ENOUGH FOR SAMPLE.  (FREQUENTLY HAPPENS AT THEN DO AUDIO PLAYBACK)
	if (numFramesAvaliable >= SAMPLES) {
		for (int i = 0; i < SAMPLES; ++i) {
			in[i] = static_cast<float>(audioData[pwfex->nChannels*i]);
		}
	} else {
		// Not enough data for fft this time.
	}

	fftwf_execute(plan);

	dispFreqResp(out, BINS);
}	