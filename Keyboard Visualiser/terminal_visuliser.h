#pragma once
#include <string>

#include "fftw3.h"

void dispFreqResp(fftwf_complex* fftResult, int nBins);
void writeAdditional(std::string str);

