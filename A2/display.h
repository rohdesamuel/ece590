// Setup and reading taken from example.

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

typedef Adafruit_SSD1306* display_handle_t;

const int DISPLAY_ADDR = 0x3D;

void display_setup() {
  Serial.println("Initializing display.");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) { // Address 0x3D for 128x64
    Serial.println(F("Could not start display."));
    for(;;); // Don't proceed, loop forever
  }

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  // Clear the buffer
  display.clearDisplay();
  display.display();

  
  Serial.println("Done.");
}

display_handle_t display_handle() {
  return &display;
}

void display_present(display_handle_t d) {
  d->display();
  d->clearDisplay();
}

int display_writestr(display_handle_t d, const char* c, int max_size=-1) {
  int written = 0;
  if (max_size < 0) {
    max_size = 64;
  }

  while(*c && max_size > 0) {
    d->write(*c);
    --max_size;
    ++c;
    ++written;
  }
  return written;
}
