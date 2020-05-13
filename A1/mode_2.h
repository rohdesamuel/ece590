void setup_mode_2(int audio_pin) {}

void run_mode_2(RGBConverter* converter, byte rgb[]) {
  /* Not the real total energy -- calculated as the MSE -- but close enough. */
  double tot_energy = 0.0;
  for(int i=0; i<(SAMPLES/2); i++)
  {
    tot_energy += vReal[i];
  }

  /*PRINT RESULTS*/
  if (tot_energy > 34000) {
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    converter->hslToRgb(1.0f, 1.0f, 0.5f, rgb);

    float brightness = map(tot_energy, 34000, 100000, 100, 1000) / 1000.0f;
    
    rgb[0] *= brightness;
    rgb[1] *= brightness;
    rgb[2] *= brightness;
  } else {
    rgb[0] = 10;
    rgb[1] = rgb[2] = 0;
  }
}
