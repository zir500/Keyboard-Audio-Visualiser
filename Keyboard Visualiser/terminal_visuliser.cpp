#include "terminal_visuliser.h"

#include <iostream>
#include <memory>
#include <Windows.h>

#define BAR_HEIGHT 20

#define HORIZONTAL_OFFSET 0
#define VERTICAL_OFFSET 2

static std::string additional_text;

// Writes NUM_BARS bars with corresponding heights specified in barValues at the bottom left of the console.
void writeBars(HANDLE& hStdout, SHORT const * const barValues, int nBars) {
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo = {};
	SMALL_RECT window;
	COORD writePos;

	GetConsoleScreenBufferInfo(hStdout, &screenBufferInfo);
	window = screenBufferInfo.srWindow;

	for (SHORT i = 0; i < nBars; ++i) {
		for (SHORT j = 0; j <= barValues[i]; ++j) {
			writePos = { window.Left + i + HORIZONTAL_OFFSET, window.Bottom - j - VERTICAL_OFFSET};
			SetConsoleCursorPosition(hStdout, writePos);
			std::cout << "-";
		}
	}

	writePos = { window.Left, window.Bottom};
	SetConsoleCursorPosition(hStdout, writePos);
	std::cout << additional_text;
}

float mag(fftwf_complex& sample) {
	return sqrt(sample[0] * sample[0] + sample[1] * sample[1]);
}

void fftResultToBars(SHORT* bars, fftwf_complex* fftResult, int nBins) {
	// 1) Find the amplitide spectrum and total amplitude
	std::unique_ptr<float[]> amplitudeSpectrum(new float[nBins-1]);
	float totalAmplitude = 0;
	for (int i = 1; i < nBins; ++i) {
		amplitudeSpectrum[i-1] = mag(fftResult[i-1]);
		totalAmplitude += amplitudeSpectrum[i-1];
	}

	for (int i = 0; i < nBins-1; i++) {
		float bar = (totalAmplitude != 0) ? BAR_HEIGHT*(amplitudeSpectrum[i] / totalAmplitude) : 0.0f;
		bars[i] = static_cast<SHORT>(bar);
	}
}


void dispFreqResp(fftwf_complex* fftResult, int nBins) {
	HANDLE hStdout;
	SHORT* barValues = new SHORT[nBins-1]; // Will store the value of each bar to be displayed.  We will discard the DC bin, so we need one less than the bins obtained from the fft.

	fftResultToBars(barValues, fftResult, nBins);

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	writeBars(hStdout, barValues, nBins-1);

	delete barValues;
}

// A bodgy bodge
void writeAdditional(std::string str) {
	additional_text = str;
}