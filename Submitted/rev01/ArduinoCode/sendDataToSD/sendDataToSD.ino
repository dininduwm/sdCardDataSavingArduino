#include <Arduino.h>

/*
  Listfiles

  This example shows how print out the files in a
  directory on a SD card

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald

  This example code is in the public domain.

*/
#include <SPI.h>
#include <SD.h>
#include "Wire.h"

// address for the gyro
const int MPU_ADDR = 0x68;

// Flile object to save data to File
File root;

// Array to hold the values to be written to the SD card
// If you are changing the number of elements in the data Struct you should change the numer of
// Data points to the relevent data point number
#define NoOfDataPorints 6

// arya to hold the data points
float dataPoints[NoOfDataPorints];

// initializing the GYRO
void Gyrosetup()
{
  /*
    Wire.begin();
    writeRegister(0x6B, B10000000); // PWR_MGMT_1 register (p.40) - device reset
    writeRegister(0x6B, B00000010); // PWR_MGMT_1 register (p.40) - wake up; X-axis gyroscope as clock
    writeRegister(0x1A, B00000110); // Low Pass Filter
    */

  Wire.begin();                     // Initialize comunication
  Wire.beginTransmission(MPU_ADDR); // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                 // Talk to the register 6B
  Wire.write(0x00);                 // Make reset - place a 0 into the 6B register
  //Wire.write(0x1A);                 // Applying the low pass Filter
  //Wire.write(B00000110);
  Wire.endTransmission(true); //end the transmission
}

// convert to char(bytes) to store
String convertToChar(float floatVal)
{
  // converting float value to bytes
  byte *b = (byte *)&floatVal;
  // create string using bytes
  String result = (String) char(b[3]) + char(b[2]) + char(b[1]) + char(b[0]);
  return result;
}

// wite to the SD card
void writeToSDCard()
{
  // string to construct the char array
  String res = "";

  // loop through the data points and add them to the res
  for (int i = 0; i < NoOfDataPorints; i++)
  {
    res += convertToChar(dataPoints[i]);
  }

  //adding the end line bits
  //Serial.println(res);
  res += (String) char(97) + char(97) + char(97) + char(97);
  //Serial.println(res);

  // writting to the SD card
  root = SD.open("dat1_new.bin", FILE_WRITE);

  if (root)
  {
    root.print(res);
    root.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening dat1_new.bin");
  }
}

void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // initializing the SD card module
  Serial.println("Initializing SD card...");

  // check weather the SD card init success
  if (!SD.begin(4))
  {

    Serial.println("initialization failed!");
    while (1)
      ;
  }

  Serial.println("SD card Module initialization done.");

  // Initializa the Gyro
  Gyrosetup();
  delay(1000);

  /*
  // opening the sd card data saving process
  root = SD.open("dat1_new.bin", FILE_WRITE);

  if (root)
  {
    for (int i = 0; i < 20; i++) {
      root.print(convertToChar(2.35));
    }
    Serial.println("Writing Done");
    root.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening dat1_new.bin");
  }  

  */
}

void loop()
{

  // Reading Accelerometer Data
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  float AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  float AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  float AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value

  // Reading Gyro data
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);                    // Read 4 registers total, each axis value is stored in 2 registers
  float GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  float GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  float GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  // Adding values to the dataPoints array
  dataPoints[0] = AccX;
  dataPoints[1] = AccY;
  dataPoints[2] = AccZ;

  dataPoints[3] = GyroX;
  dataPoints[4] = GyroY;
  dataPoints[5] = GyroZ;

  //Serial.println(AccX);

  // Saving new Values to the Array
  writeToSDCard();

  for (int i = 0; i < 6; i++)
  {
    Serial.print(dataPoints[i]);
    Serial.print(", ");
  }
  Serial.println();

  delay(50);
}

