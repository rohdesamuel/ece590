float food = 100;
int steps = 0;

sprite_t idle_s = {};
sprite_t jump_s = {};
sprite_t eat_s = {};
sprite_t dying_s = {};
sprite_t dead_s = {};
sprite_t heart_s = {};

enum MochiState {
  MOCHI_STATE_IDLE,
  MOCHI_STATE_EATING,
  MOCHI_STATE_DYING,
  MOCHI_STATE_DEAD,
};

MochiState mochi_state_;

void mochi_setup() {
  idle_s.bmp[0] = (unsigned char*)idle_1_data;
  idle_s.bmp[1] = (unsigned char*)idle_2_data;
  idle_s.num_frames = 1;
  idle_s.w = idle_s.h = 32;
  idle_s.speed = 50;

  dying_s.bmp[0] = (unsigned char*)dying_data;
  dying_s.num_frames = 1;
  dying_s.w = dying_s.h = 32;

  dead_s.bmp[0] = (unsigned char*)dead_data;
  dead_s.num_frames = 1;
  dead_s.w = dead_s.h = 32;

  jump_s.bmp[0] = (unsigned char*)idle_1_data;
  jump_s.bmp[1] = (unsigned char*)jump_2_data;
  jump_s.bmp[2] = (unsigned char*)jump_3_data;
  jump_s.bmp[3] = (unsigned char*)jump_4_data;
  jump_s.bmp[4] = (unsigned char*)jump_4_data;
  jump_s.bmp[5] = (unsigned char*)jump_5_data;
  jump_s.bmp[6] = (unsigned char*)jump_6_data;
  jump_s.num_frames = 7;
  jump_s.w = jump_s.h = 32;
  jump_s.speed = 50;

  heart_s.bmp[0] = (unsigned char*)heart_empty;
  heart_s.bmp[1] = (unsigned char*)heart_half;
  heart_s.bmp[2] = (unsigned char*)heart_full;
  heart_s.num_frames = 3;
  heart_s.w = heart_s.h = 7;
  heart_s.speed = 0;
}

void mochi_loop(bool step_detected) {
  int meal = 0;
  if (!step_detected && jump_s.cur_frame == 0) {
    mochi_state_ = MOCHI_STATE_IDLE;
    jump_s.loop = 0;
  } else {
    if (mochi_state_ != MOCHI_STATE_EATING) {      
      ++steps;
      meal = 5;
    }
    jump_s.loop = 1;
    mochi_state_ = MOCHI_STATE_EATING;
  }  

  food += meal;
  
  if (food >= 20.f) {    
    food -= 0.1;
  } else {
    food -= 0.05;
  }
  
  food = max(food, 0.f);
  food = min(food, 120.f);

  if (food <= 20.f and food > 0.f) {
    mochi_state_ = MOCHI_STATE_DYING;
  } else if (food == 0.f) {
    mochi_state_ = MOCHI_STATE_DEAD;
  }
}

void mochi_draw() {
  switch (mochi_state_) {
    case MOCHI_STATE_IDLE: {
      static int next_blink = micros() + rand() % 4000000 + 1000000;
      int now = micros();
      if (now > next_blink) {
        idle_s.cur_frame = 1;
        idle_s.num_frames = 2;
        next_blink = now + rand() % 4000000 + 1000000;      
      }
      draw_sprite(&idle_s, 48, 12);
      break;
    }      

    case MOCHI_STATE_EATING:
      draw_sprite(&jump_s, 48, 12);
      break;
      
    case MOCHI_STATE_DYING:
      draw_sprite(&dying_s, 48, 12);
      break;
      
    case MOCHI_STATE_DEAD:
      draw_sprite(&dead_s, 48, 12);
      break;
  }

  int start_x = 16;
  int margin = 3;
  for (int i = 0; i < 10; ++i) {
    if (i * 10.f + 5.f < food) {
      heart_s.cur_frame = 2;
    } else if (i * 10.f < food) {
      heart_s.cur_frame = 1;
    } else {
      heart_s.cur_frame = 0;
    }
    draw_sprite(&heart_s, start_x + (heart_s.w + margin) * i, 56);
  }

  char buffer[25] = {'\0'};
  d->setCursor(0, 0);
  sprintf(buffer, "%d", steps);
  display_writestr(d, buffer, 25);
  Serial.println(food);
}
