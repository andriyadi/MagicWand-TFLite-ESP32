# Magic Wand with Machine Learning

Magic Wand using [ESPectro32](https://shop.makestro.com/product/espectro32-v2/) board, powered by TensorFlow Lite for Microcontrollers and PlatformIO. Previously I made a [similar project](https://github.com/andriyadi/MagicWand-TFLite-Arduino) but using Arduino Nano 33 BLE Sense board.

As ESPectro32 is based on ESP32, this project should be easily adapted for other ESP32 board. You can change this to `#define ESPECTRO32 0` in `output_handler.cpp`. But you need to have some kind of display to let user know what is the predicted gesture. ESPectro32 has on-board LED Matrix to display the predicted gesture.

## Demo Video

The video is quite similar to this:

[![Demo video thumbnail](http://i3.ytimg.com/vi/Lfv3WJnYhX0/hqdefault.jpg)](https://www.youtube.com/watch?v=Lfv3WJnYhX0)

## Prerequisites

* [PlatformIO](http://platformio.org/)
* PlatformIO's [platform-espressif32](https://github.com/platformio/platform-espressif32). Should be installed automatically

## Credit

* [Magic Wand original sample code]([exercises/intro.md](https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/micro/examples/magic_wand))
* Some code is inspired from [here](https://blog.boochow.com/article/m5stack-tflite-magic-wand.html)
* [SparkFun_LSM6DS3_Arduino_Library](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library). Need to include here, as its PlatformIO library is failed to compile.