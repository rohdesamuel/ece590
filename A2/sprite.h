typedef struct sprite_t {
  unsigned char* bmp[8];
  size_t cur_frame;
  size_t num_frames;
  int w, h;
  int loop;
  int speed;
  int frame_percentage;
} sprite_t;

void draw_sprite(sprite_t* sprite, int x, int y) {
  display.drawBitmap(x, y, sprite->bmp[sprite->cur_frame % sprite->num_frames], sprite->w, sprite->h, SSD1306_WHITE);  

  sprite->frame_percentage += sprite->speed;
  if (sprite->frame_percentage >= 100) {
    sprite->frame_percentage = 0;
    ++sprite->cur_frame;

    if (sprite->cur_frame >= sprite->num_frames) {
      sprite->cur_frame = 0;
      if (!sprite->loop) {
        sprite->num_frames = 1;
      }
    }
  }
}
