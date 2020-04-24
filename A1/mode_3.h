/* PINS */
int LOFI_PIN;
int SET_PIN;

void setup_mode_3(int lofi_pin, int set_pin) {
  LOFI_PIN = lofi_pin;
  SET_PIN = set_pin;
  
  pinMode(LOFI_PIN, INPUT);
  pinMode(SET_PIN, INPUT_PULLUP);
}

#define SAMPLE_SIZE 25
int samples[SAMPLE_SIZE] = { 0 };
int sample_index = 0;
int set_sample = 0;

int deltas[SAMPLE_SIZE] = { 0 };
int delta_index = 0;

int sample_at(int index) {
  return samples[index % SAMPLE_SIZE];
}

void run_mode_3(RGBConverter* converter, byte rgb[]) {
  samples[sample_index] = map(analogRead(LOFI_PIN), 0, 670, 0, 1000);

  float avg = 0.0f;
  for (int i = 0; i < SAMPLE_SIZE; ++i) {
    avg += samples[i];
  }
  avg /= (float)SAMPLE_SIZE;

  float delta = sample_at(sample_index) - sample_at(sample_index - 1);
  for (int i = 0; i < 0; ++i) {
    delta += sample_at(sample_index - i) - sample_at(sample_index - i - 1);
  }
  delta /= (float)2;
  
  if (!digitalRead(SET_PIN)) {
    set_sample = avg;
  }
  rgb[0] = map(set_sample, 0, 1000, 0, 255);
  rgb[1] = rgb[2] = 0;
  
  converter->hslToRgb(set_sample / 1000.0f, 1.0, 0.5f, rgb);
  sample_index = (sample_index + 1) % SAMPLE_SIZE;
}
