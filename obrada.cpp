#include <math.h>
#include <stdlib.h>
#include "obrada.h"
#include "sr_fft.h"


#define PI 3.1415926535

double time_buffer[FFT_SIZE];
double fft_buffer[FFT_SIZE];
double in_delay[FFT_SIZE / 2];
double out_delay[FFT_SIZE / 2];

extern double window[FFT_SIZE];

void obrada(double *in, double *out, int N)
{


	//FFT
	for (int i = 0; i < FFT_SIZE / 2; i++) {
		time_buffer[i] = in_delay[i];
	}

	for (int i = 0; i < FFT_SIZE / 2; i++) {
		time_buffer[i + FFT_SIZE / 2] = in[i];
	}

	for (int i = 0; i < FFT_SIZE / 2; i++) {
		in_delay[i] = in[i];
	}

	for (int i = 0; i < FFT_SIZE; i++) {
		time_buffer[i] *= window[i];
	}

	fft(time_buffer, fft_buffer, FFT_ORDER);

	//Obrada
/*
	for (int i = 6; i < 94; i++) {
		fft_buffer[i] = 0;
	}
	*/
	for (int i = 2; i < FFT_SIZE; i += 2) {
		double moduo = sqrt(pow(fft_buffer[i],2) + pow(fft_buffer[i + 1],2));
		int k = i/2;
		double phi = 0.25 * PI * pow(k, 2);

		fft_buffer[i] = moduo * cos(phi);
		fft_buffer[i + 1] = moduo * sin(phi);

	}

	// Inverzna FFT
	ifft(fft_buffer, time_buffer, FFT_ORDER);

	for (int i = 0; i < FFT_SIZE; i++) {
		time_buffer[i] *= window[i];
	}

	for (int i = 0; i < FFT_SIZE / 2; i++) {
		out[i] = time_buffer[i];
	}

	for (int i = 0; i < FFT_SIZE / 2; i++) {
		out[i] += out_delay[i];
	}

	for (int i = 0; i < FFT_SIZE / 2; i++) {
		out_delay[i] = time_buffer[i + FFT_SIZE / 2];
	}





}

