/* PINS */
int PHOTORESISTOR_PIN;

/* CONFIG */
float _hue = 0; //hue varies between 0 - 1
const float _step = 0.1f;

void setup_mode_1(int pr_pin) {
  PHOTORESISTOR_PIN = pr_pin;
  
  pinMode(PHOTORESISTOR_PIN, INPUT);
}

float read_brightness() {
  return map(analogRead(PHOTORESISTOR_PIN), 0, 1023, 0, 1000) / 1000.0f;
}

void run_mode_1(RGBConverter* converter, byte rgb[]) {
  converter->hslToRgb(_hue, 1.0, 0.5f, rgb);
  _hue += _step;
  if (_hue >= 1.0f) {
    _hue = 0.0f;
  }

  float brightness = read_brightness();
  rgb[0] = rgb[0] * brightness;
  rgb[1] = rgb[1] * brightness;
  rgb[2] = rgb[2] * brightness;
}
