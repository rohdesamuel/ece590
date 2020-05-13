// Setup and reading taken from example.

#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

const int ACCELEROMETER_ADDR = 0x18;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

typedef struct accelerometer_t {
  float x;
  float y;
  float z;
  float l;
  float dx;
  float dy;
  float dz;
  float dl;
} accelerometer_t;

accelerometer_t prev_accelerometer = {};

void accelerometer_read(accelerometer_t* event) {
  sensors_event_t sensor_event;
  lis.getEvent(&sensor_event);

  event->x = sensor_event.acceleration.x;
  event->y = sensor_event.acceleration.y;
  event->z = sensor_event.acceleration.z;
  event->l = sqrt(sensor_event.acceleration.x * sensor_event.acceleration.x +
                  sensor_event.acceleration.y * sensor_event.acceleration.y +
                  sensor_event.acceleration.z * sensor_event.acceleration.z);

  event->dx = event->x - prev_accelerometer.x;
  event->dy = event->y - prev_accelerometer.y;
  event->dz = event->z - prev_accelerometer.z;
  event->dl = event->l - prev_accelerometer.l;
  prev_accelerometer = *event;
}

void accelerometer_setup() {
  Serial.println("Initializing accelerometer.");
  if (! lis.begin(ACCELEROMETER_ADDR)) {
    Serial.println("Could not start accelerometer.");
    for (;;);
  }

  lis.setDataRate(LIS3DH_DATARATE_100_HZ);

  accelerometer_t unused;
  accelerometer_read(&unused);
  Serial.println("Done.");
}
