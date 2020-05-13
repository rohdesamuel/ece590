// https://www.xtronical.com/esp32monitorownbatteryvoltage/


const int VOLTAGE_PIN = 13;

sprite_t battery_s = {};

void battery_setup() {
  pinMode(VOLTAGE_PIN, INPUT);
  battery_s.bmp[0] = (unsigned char*)batt_75_100;
  battery_s.bmp[1] = (unsigned char*)batt_50_75;
  battery_s.bmp[2] = (unsigned char*)batt_25_50;
  battery_s.bmp[3] = (unsigned char*)batt_0_25;
  battery_s.num_frames = 4;
  battery_s.w = 8;
  battery_s.h = 5;
  battery_s.speed = 0;
}

int battery_read() {
  return analogRead(VOLTAGE_PIN);
}

void battery_draw() {
  float percent = (100.f * battery_read() / 2048.f);
  if (75 < percent <= 100) {
    battery_s.cur_frame = 0;
    draw_sprite(&battery_s, 119, 1);
  } else if (50 < percent <= 75) {
    battery_s.cur_frame = 1;
    draw_sprite(&battery_s, 119, 1);
  } else if (25 < percent <= 50) {
    battery_s.cur_frame = 2;
    draw_sprite(&battery_s, 119, 1);
  } else {
    battery_s.cur_frame = 3;
    draw_sprite(&battery_s, 119, 1);
  }
}
