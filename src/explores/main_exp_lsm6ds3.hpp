#include <Arduino.h>

#include <Arduino_LSM6DS3.h>

void setup()
{
  Serial.begin(115200);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  float x, y, z;
  IMU.readAcceleration(x, y, z);
}


void loop()
{
  float x, y, z;

  // if (IMU.accelerationAvailable()) {
  
  // uint16_t tempUnsigned;

  // tempUnsigned = IMU.fifoGetStatus();
  // Serial.print("\nFifo Status 1 and 2 (16 bits): 0x");
  // Serial.println(tempUnsigned, HEX);
  // Serial.print("\n");   

  while( ( IMU.fifoGetStatus() & 0x8000 ) == 0 ) {};  //Wait for watermark
 
  //Now loop until FIFO is empty.  NOTE:  As the FIFO is only 8 bits wide,
  //the channels must be synchronized to a known position for the data to align
  //properly.  Emptying the fifo is one way of doing this (this example)
  while( ( IMU.fifoGetStatus() & 0x1000 ) == 0 ) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }

  // tempUnsigned = IMU.fifoGetStatus();
  // Serial.print("\nFifo Status 1 and 2 (16 bits): 0x");
  // Serial.println(tempUnsigned, HEX);
  // Serial.print("\n");  
}