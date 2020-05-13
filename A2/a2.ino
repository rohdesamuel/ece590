#include <Wire.h>

#include "display.h"
display_handle_t d;

#include "accelerometer.h"
#include "images.h"
#include "sprite.h"
#include "battery.h"
#include "mochi.h"
#include "step_detector.h"

void setup() {
  Serial.begin(115200);

  display_setup();
  accelerometer_setup();
  battery_setup();

  d = display_handle();
  mochi_setup();
}

void loop() {
  int now_us = micros();
  
  accelerometer_t event = {};
  accelerometer_read(&event);  

  int step_detected = detect_step(now_us, &event);
  mochi_loop(step_detected);

  static int next_draw_us = 0;
  static const int draw_period_us = 15 * 1000;
  if (now_us >= next_draw_us) {
    next_draw_us += draw_period_us;
    
    battery_draw();
    mochi_draw();
    display_present(d);
  }
}
