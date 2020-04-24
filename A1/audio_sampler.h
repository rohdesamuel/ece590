/* FFT code from https://www.norwegiancreations.com/2017/08/what-is-fft-and-how-can-you-implement-it-on-an-arduino/ */
#include "arduinoFFT.h"

#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 8000 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

/* PINS */
int AUDIO_PIN;

void setup_audio_sampling(int audio_pin) {
  AUDIO_PIN = audio_pin;
  pinMode(AUDIO_PIN, INPUT);

  sampling_period_us = round(1000*(1.0/SAMPLING_FREQUENCY));
}

void run_audio_sampler() {
  /*SAMPLING*/
  for(int i=0; i<SAMPLES; i++) {
    microseconds = micros();    //Overflows after around 70 minutes!
    vReal[i] = analogRead(AUDIO_PIN);
    vImag[i] = 0;

    while(micros() < (microseconds + sampling_period_us)){}
  }

  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
}
