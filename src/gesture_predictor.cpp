/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#include "gesture_predictor.h"

#include "constants.h"
#include "Arduino.h"

// Set to 1 to display each inference results
#define DEBUG_INF_RES 1

String LABELS[4] = {"W", "O", "L", "U"};

namespace {
// State for the averaging algorithm we're using.
float prediction_history[kGestureCount][kPredictionHistoryLength] = {};
int prediction_history_index = 0;
int prediction_suppression_count = 0;
}  // namespace

// Return the result of the last prediction
// 0: wing("W"), 1: ring("O"), 2: slope("angle"), 3: unknown
int PredictGesture(float* output, tflite::ErrorReporter* error_reporter) {
  
  // Record the latest predictions in our rolling history buffer.
  for (int i = 0; i < kGestureCount; ++i) {

#if DEBUG_INF_RES
    //Using percentage to visualize inference result
    // Serial.print(LABELS[i]); Serial.print(": "); Serial.print(output[i]*100); Serial.print("%,\t");
    
    //Using bar graph to visualize inference result
    Serial.printf("%s: ", LABELS[i].c_str()); 
    int barNum = static_cast<int>(roundf(output[i]*10));
    for(int k = 0; k < barNum; k++) {
      Serial.print("█"); 
    }   
    for(int k=barNum-1; k < 10; k++) {
      Serial.print(" "); 
    } 
    Serial.print("\t");
#endif

    prediction_history[i][prediction_history_index] = output[i];
  }

#if DEBUG_INF_RES
  error_reporter->Report("\r\n\r\n");
#endif

  // Figure out which slot to put the next predictions into.
  ++prediction_history_index;
  if (prediction_history_index >= kPredictionHistoryLength) {
    prediction_history_index = 0;
  }

  // Average the last n predictions for each gesture, and find which has the
  // highest score.
  int max_predict_index = -1;
  float max_predict_score = 0.0f;
  for (int i = 0; i < kGestureCount; i++) {
    float prediction_sum = 0.0f;
    for (int j = 0; j < kPredictionHistoryLength; ++j) {
      prediction_sum += prediction_history[i][j];
    }
    const float prediction_average = prediction_sum / kPredictionHistoryLength;
    if ((max_predict_index == -1) || (prediction_average > max_predict_score)) {
      max_predict_index = i;
      max_predict_score = prediction_average;
    }
  }

  // If there's been a recent prediction, don't trigger a new one too soon.
  if (prediction_suppression_count > 0) {
    --prediction_suppression_count;
  }
  // If we're predicting no gesture, or the average score is too low, or there's
  // been a gesture recognised too recently, return no gesture.
  if ((max_predict_index == kNoGesture) ||
      (max_predict_score < kDetectionThreshold) ||
      (prediction_suppression_count > 0)) {
    return kNoGesture;
  } else {
    // Reset the suppression counter so we don't come up with another prediction
    // too soon.
    prediction_suppression_count = kPredictionSuppressionDuration;
    return max_predict_index;
  }
}
