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

#include "output_handler.h"

#include "Arduino.h"

#define ESPECTRO32    1

#if ESPECTRO32
#include <ESPectro32_Board.h>
#endif

/*
void LightUpRGB(int kind) {
  // ESPectro32.RgbLed().clear();

  switch (kind)
  {
  case 0: //W
    ESPectro32.RgbLed().setPixel(0, 200, 0, 0);
    break;
  case 1: //O
    ESPectro32.RgbLed().setPixel(0, 0, 200, 0);
    break;
  case 2: //L
    ESPectro32.RgbLed().setPixel(0, 0, 0, 200);
    break;
  default:
    break;
  }

  ESPectro32.RgbLed().show();
}
*/

void HandleOutput(tflite::ErrorReporter* error_reporter, int kind) {
  // The first time this method runs, set up our LED
  static bool is_initialized = false;
  if (!is_initialized) {
    is_initialized = true;

    pinMode(LED_BUILTIN, OUTPUT);

#if ESPECTRO32
    ESPectro32.LedMatrix().clear();
	  ESPectro32.LedMatrix().setTextSize(1);
	  ESPectro32.LedMatrix().setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
	  ESPectro32.LedMatrix().setTextColor(100);
	  ESPectro32.LedMatrix().displayFrame(0);

    // ESPectro32.RgbLed().setBrightness(0, 150);
    // ESPectro32.RgbLed().clear();

    //Rotate LED Matrix
#endif

  }
  // Toggle the LED every time an inference is performed
  static int count = 0;
  ++count;
  if (count & 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Print some ASCII art for each gesture
  if (kind == 0) {
    // error_reporter->Report("\n\r█ Wingardium Leviosa █\n\r");
    error_reporter->Report(
        "\n\r*         *         *\n\r *       * *       "
        "*\n\r  *     *   *     *\n\r   *   *     *   *\n\r    * *       "
        "* *\n\r     *         *\n\r\n\r");

    error_reporter->Report("\n\r");
    error_reporter->Report("╔══════════════════════╗");
    error_reporter->Report("║  Wingardium Leviosa  ║");
    error_reporter->Report("╚══════════════════════╝\n\r");

#if ESPECTRO32
    ESPectro32.LedMatrix().clear();
    ESPectro32.LedMatrix().setCursor(1, 0);
    ESPectro32.LedMatrix().print("W");
#endif
    // LightUpRGB(kind);

} else if (kind == 1) {
    // error_reporter->Report("\n\r█ Obliviate █\n\r");
    error_reporter->Report(
        "\n\r          *\n\r       *     *\n\r     *         *\n\r "
        "   *           *\n\r     *         *\n\r       *     *\n\r      "
        "    *\n\r");

    error_reporter->Report("\n\r");
    error_reporter->Report("╔══════════════════════╗");
    error_reporter->Report("║       Obliviate      ║");
    error_reporter->Report("╚══════════════════════╝\n\r");

#if ESPECTRO32
    // ESPectro32.LedMatrix().drawBitmapFull((uint8_t*)LED_MATRIX_EMOTICON_SMILE);
    ESPectro32.LedMatrix().clear();
    ESPectro32.LedMatrix().setCursor(1, 0);
    ESPectro32.LedMatrix().print("O");
#endif
    // LightUpRGB(kind);

  } else if (kind == 2) {
    // error_reporter->Report("\n\r█ Lumos █\n\r");
    error_reporter->Report(
        "\n\r        *\n\r       *\n\r      *\n\r     *\n\r    "
        "*\n\r   *\n\r  *\n\r * * * * * * * *\n\r");

    error_reporter->Report("\n\r");
    error_reporter->Report("╔══════════════════════╗");
    error_reporter->Report("║         Lumos        ║");
    error_reporter->Report("╚══════════════════════╝\n\r");

#if ESPECTRO32
    ESPectro32.LedMatrix().clear();
    ESPectro32.LedMatrix().setCursor(1, 0);
    ESPectro32.LedMatrix().print("L");
#endif
    // LightUpRGB(kind);

  } else {
    // ESPectro32.LedMatrix().clear();
  }
}
