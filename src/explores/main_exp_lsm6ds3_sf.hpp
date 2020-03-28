#include "Arduino.h"
#include "SparkFunLSM6DS3.h"
#include "Wire.h"

LSM6DS3 my_IMU( I2C_MODE );

void setup()
{
  
  //Over-ride default settings if desired
  my_IMU.settings.gyroEnabled = 1;  //Can be 0 or 1
  my_IMU.settings.gyroRange = 2000;   //Max deg/s.  Can be: 125, 245, 500, 1000, 2000
  my_IMU.settings.gyroSampleRate = 833;   //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666
  my_IMU.settings.gyroBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;
  my_IMU.settings.gyroFifoEnabled = 1;  //Set to include gyro in FIFO
  my_IMU.settings.gyroFifoDecimation = 1;  //set 1 for on /1

  my_IMU.settings.accelEnabled = 1;
  my_IMU.settings.accelRange = 16;      //Max G force readable.  Can be: 2, 4, 8, 16
  my_IMU.settings.accelSampleRate = 833;  //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  my_IMU.settings.accelBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;
  my_IMU.settings.accelFifoEnabled = 1;  //Set to include accelerometer in the FIFO
  my_IMU.settings.accelFifoDecimation = 1;  //set 1 for on /1
  my_IMU.settings.tempEnabled = 1;
  
    //Non-basic mode settings
  my_IMU.settings.commMode = 1;

  //FIFO control settings
  my_IMU.settings.fifoThreshold = 100;  //Can be 0 to 4096 (16 bit bytes)
  my_IMU.settings.fifoSampleRate = 50;  //Hz.  Can be: 10, 25, 50, 100, 200, 400, 800, 1600, 3300, 6600
  my_IMU.settings.fifoModeWord = 6;  //FIFO mode.
  //FIFO mode.  Can be:
  //  0 (Bypass mode, FIFO off)
  //  1 (Stop when full)
  //  3 (Continuous during trigger)
  //  4 (Bypass until trigger)
  //  6 (Continous mode)
  

  Serial.begin(115200);  // start serial for output
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMUs
  if( my_IMU.begin() != 0 )
  {
	  Serial.println("Problem starting the sensor with CS @ Pin 10.");
  }
  else
  {
	  Serial.println("Sensor with CS @ Pin 10 started.");
  }
  
  Serial.print("Configuring FIFO with no error checking...");
  my_IMU.fifoBegin();
  Serial.print("Done!\n");
  
  Serial.print("Clearing out the FIFO...");
  my_IMU.fifoClear();
  Serial.print("Done!\n");
}


void loop()
{
  float temp;  //This is to hold read data
  uint16_t tempUnsigned;
  
  while( ( my_IMU.fifoGetStatus() & 0x8000 ) == 0 ) {};  //Wait for watermark
 
  tempUnsigned = my_IMU.fifoGetStatus();
  Serial.print("\nWatermark set. Fifo status: 0x");
  Serial.println(tempUnsigned, HEX);
  Serial.print("\n");  

  //Now loop until FIFO is empty.  NOTE:  As the FIFO is only 8 bits wide,
  //the channels must be synchronized to a known position for the data to align
  //properly.  Emptying the fifo is one way of doing this (this example)
  while( ( my_IMU.fifoGetStatus() & 0x1000 ) == 0 ) {

    temp = my_IMU.calcGyro(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print(", ");

    temp = my_IMU.calcGyro(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print(", ");

    temp = my_IMU.calcGyro(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print(", ");

    temp = my_IMU.calcAccel(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print(", ");

    temp = my_IMU.calcAccel(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print(", ");

    temp = my_IMU.calcAccel(my_IMU.fifoRead());
    Serial.print(temp);
    Serial.print("\n");
    
    // delay(10); //Wait for the serial buffer to clear (~50 bytes worth of time @ 57600baud)
  
  }

  tempUnsigned = my_IMU.fifoGetStatus();
  Serial.print("\nFifo Status 1 and 2 (16 bits): 0x");
  Serial.println(tempUnsigned, HEX);
  Serial.print("\n");  
}