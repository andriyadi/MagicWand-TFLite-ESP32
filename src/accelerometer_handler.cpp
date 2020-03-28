/* 
Adapted by Andri Yadi.
Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "accelerometer_handler.h"

#include <Arduino.h>
#include "SparkFunLSM6DS3.h"
#include "constants.h"

#define RING_BUFFER_SIZE  600

// A buffer holding the last 200 sets of 3-channel values
float save_data[RING_BUFFER_SIZE] = {0.0};
// Most recent position in the save_data buffer
int begin_index = 0;
// True if there is not yet enough data to run inference
bool pending_initial_data = true;
// How often we should save a measurement during downsampling
int sample_every_n = 25;
// The number of measurements since we last saved one
int sample_skip_counter = 1;

LSM6DS3 myIMU( I2C_MODE );
long lastAcqMillis = 0;

int skipBeforeMs = 1000*(1/kTargetHz);

TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter) {
  
  myIMU.settings.accelEnabled = 1;
  myIMU.settings.accelRange = 4;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 104;  //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  myIMU.settings.accelBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;

  // Switch on the IMU
  if( myIMU.begin() != 0 ) {
    error_reporter->Report("Failed to initialize IMU");
    return kTfLiteError;
  }
  else {
    skipBeforeMs = static_cast<int>(1000*(1.0f/kTargetHz));
    error_reporter->Report("IMU success. Skip %d ms", skipBeforeMs);
  }
  
  return kTfLiteOk;
}

// Adapted from https://blog.boochow.com/article/m5stack-tflite-magic-wand.html
static bool AcquireData() {
  bool new_data = false;

  if ((millis() - lastAcqMillis) < 40){
    return false;
  }
  lastAcqMillis = millis();

  float x, y, z;
  x = myIMU.readFloatAccelX();
  y = myIMU.readFloatAccelY();
  z = myIMU.readFloatAccelZ();

  const float norm_x = -x;
  const float norm_y = -y;
  const float norm_z = -z;

  save_data[begin_index++] = norm_x * 1000;
  save_data[begin_index++] = norm_y * 1000;
  save_data[begin_index++] = norm_z * 1000;

  if (begin_index >= RING_BUFFER_SIZE) {
    begin_index = 0;
  }
  new_data = true;

  return new_data;
}

bool ReadAccelerometer(tflite::ErrorReporter* error_reporter, float* input,
                       int input_length) {
  
  // Skip this round if data is not ready yet
  if (!AcquireData()) {
    return false;
  }

  // Check if we are ready for prediction or still pending more initial data
  if (pending_initial_data && begin_index >= 200) {
    pending_initial_data = false;
    //Could be a sign to be ready
  }

  // Return if we don't have enough data
  if (pending_initial_data) {
    return false;
  }

  // Copy the requested number of bytes to the provided input tensor
  for (int i = 0; i < input_length; ++i) {
    int ring_array_index = begin_index + i - input_length;
    if (ring_array_index < 0) {
      ring_array_index += RING_BUFFER_SIZE;
    }
    input[i] = save_data[ring_array_index];
  }

  return true;
}
