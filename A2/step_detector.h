// The accelerometer runs at 50hz.
// 50 samples per second.
const int buffer_length = 5 * 50;
float data_buffer[buffer_length] = {0};
int data_buffer_idx = 0;

const int window_size = 5;
const int averaged_window_size = 50;
float averaged_window[averaged_window_size];
int averaged_window_idx = 0;

int sample_add(float sample) {
  data_buffer[data_buffer_idx] = sample;
}

float sample_get(int i) {
  if (i < 0) {
    i = buffer_length + i;
  }
  return data_buffer[i % buffer_length];
}

float average_get(int i) {
  if (i < 0) {
    i = averaged_window_size + i;
  }
  return averaged_window[i % averaged_window_size];
}

int detect_step(int now_us, accelerometer_t* event) {
  // Samples every 20ms.
  // New accelerometer values comes in every 10ms.
  static int next_sample_us = 0;
  static const int sample_period_us = 20 * 1000;
  static int step_detected = 0;
  if (now_us < next_sample_us) {
    return step_detected;
  }
  next_sample_us += sample_period_us;

  // Add the sample.
  sample_add(event->l - 9.8f);

  // Compute the moving average.
  float average = 0.f;
  for (int i = 0; i < window_size; ++i) {
    float sample = sample_get(data_buffer_idx - i);
    average += sample;
  }
  average *= 0.2f;
  averaged_window[averaged_window_idx] = average;

  // Detect the peaks.
  float window[averaged_window_size] = {0.f};
  {
    int index = 0;
    for (int i = averaged_window_size - 1; i >= 0; --i) {
      window[i] = average_get(averaged_window_idx - index);
      ++index;
    }
  }
  
  float peak = 0.f;
  for (int i = 0; i < averaged_window_size; ++i) {
    if ((i - 2) <= 0 || (i + 2) >= averaged_window_size) {
      continue;
    }
    
    if (window[i] < 0.f) {
      continue;
    }

    int peak_dis = averaged_window_size - i;
    if (!(window[i - 2] <= window[i - 1] &&
          window[i + 2] <= window[i + 1] &&
          window[i - 1] <= window[i] &&
          window[i + 1] <= window[i])) {
      continue;
    }

    if (peak_dis >= 5) {
      continue;
    }

    int left_dis;
    float left_height;
    {
      float val = window[i];
      int idx = i - 1;
      while (window[idx] < val && idx > 0) {
        val = window[idx];
        --idx;
      }
      left_dis = i - idx;
      left_height = val;
    }

    int right_dis;
    float right_height;
    {
      float val = window[i];
      int idx = i + 1;
      while (window[idx] < val && idx < averaged_window_size) {
        val = window[idx];
        ++idx;
      }
      right_dis = idx - i;
      right_height = val;
    }

    int width = right_dis + left_dis;
    int height = max(left_height, right_height);

    if (width > 10 && window[i] > 0.5f) {
      peak = 1.0f;
    }
  }

  step_detected = peak > 0.f;

  data_buffer_idx = (data_buffer_idx + 1) % buffer_length;
  averaged_window_idx = (averaged_window_idx + 1) % averaged_window_size;
  return step_detected;
}
