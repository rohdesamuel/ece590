int mode = 1;

const int MODE_PIN_BUTTON = 2;
int mode_pin_button_pressed = 0;

const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 3;
const int RGB_BLUE_PIN = 9;

#include "audio_sampler.h"
#include "src/RGBConverter/RGBConverter.h"
RGBConverter rgb_converter;

#include "mode_1.h"
#include "mode_2.h"
#include "mode_3.h"

const double clap[] = {24923.44, 11112.54, 285.00, 1032.03, 1625.96, 720.45, 171.27, 866.91, 954.00, 763.26, 1023.60, 1722.15, 2173.83, 2403.09, 759.94, 255.07, 432.01, 1545.17, 2467.38, 2731.28, 2304.67, 2356.65, 2313.06, 2752.14, 2073.23, 978.31, 3371.22, 4121.28, 2510.42, 2349.71, 2236.47, 597.99, 1091.87, 2035.56, 2235.19, 2184.98, 1937.48, 1074.82, 2725.65, 2485.65, 1449.52, 2641.78, 2396.81, 569.02, 934.91, 957.38, 230.79, 421.63, 562.96, 1008.17, 1019.16, 596.86, 757.68, 928.87, 641.95, 409.23, 353.18, 781.66, 871.65, 505.70, 231.08, 586.33, 993.53, 887.70};
unsigned long clap_timestamps[2] = { 0 };
int clap_index = 0;
int is_on = 1;

void setup() {
  pinMode(MODE_PIN_BUTTON, INPUT_PULLUP);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  setup_audio_sampling(
    A1  /* audio_pin */
    );
  setup_mode_1(
    A0  /* pr_pin */
    );

  setup_mode_2(
    A1  /* audio_pin */
    );

  setup_mode_3(
    A2, /* lofi_pin */
    11 /* set_pin */
    );

  Serial.begin(9600);
}

void switch_mode() {
  // Debouncing logic.
  if (!mode_pin_button_pressed) {
    if (!digitalRead(MODE_PIN_BUTTON)) {
      mode = mode % 3 + 1;
    }
    mode_pin_button_pressed = 1;
  }

  if (digitalRead(MODE_PIN_BUTTON)) {
    mode_pin_button_pressed = 0;
  }
}

void set_color(int red, int green, int blue) {
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);
}

int clap_on_clap_off() {
  int is_clap = 1;
  for(int i=0; i<(SAMPLES/2); i++)
  {
    double diff = abs(vReal[i] - clap[i]) / clap[i];
    is_clap &= !(diff >= 0.99 && diff <= 1.01);
  }

  unsigned long now = micros();
  if (is_clap) {
    if (now - clap_timestamps[clap_index] > 5 * 1000000) {
      clap_timestamps[clap_index] = now;
    }
    clap_index = (clap_index + 1) % 2;

    if (now - clap_timestamps[0] <= 5 * 1000000 &&
      now - clap_timestamps[1] <= 5 * 1000000) {
      is_on = !is_on;
    }
    Serial.println("clap");
  }
   
  return !is_on;
}

void loop() {
  switch_mode();
  run_audio_sampler();
  if (clap_on_clap_off()) {
    set_color(0, 0, 0);
    return;
  }
  
  byte rgb[3];
  switch(mode) {
    case 1: run_mode_1(&rgb_converter, rgb); break;
    case 2: run_mode_2(&rgb_converter, rgb); break;
    case 3: run_mode_3(&rgb_converter, rgb); break;
  }

  set_color(rgb[0], rgb[1], rgb[2]);
}
