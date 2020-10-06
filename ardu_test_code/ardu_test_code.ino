//#######################################################
// Arduino Code for Arduino nano V4.0
// MPU6050 write on SD card Version 1.0
//#######################################################


#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include "defi.h"

#define SD_CARD_READER_PIN 4

File myFile;

//struct DataValue dpv;
DataValue dpv;
//DataValue* dpv = new DataValue;

void setup() {
  Serial.begin(9600);
  pinMode(SD_CARD_READER_PIN, OUTPUT);
 

  if (!SD.begin(SD_CARD_READER_PIN)) {
    Serial.print("SD not there!");
  }

  delay(1000);

  Gyrosetup();


  

}

void loop() {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers

  dpv.accel_x = Wire.read()<<8 | Wire.read(); // 16384.0; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  dpv.accel_y = Wire.read()<<8 | Wire.read(); // 16384.0; // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  dpv.accel_z = Wire.read()<<8 | Wire.read(); // 16384.0; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    // = Wire.read()<<8 | Wire.read() / 340.0 + 36.53; // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  dpv.gyro_x = Wire.read()<<8 | Wire.read(); // 131.0; // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  dpv.gyro_y = Wire.read()<<8 | Wire.read(); // 131.0; // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  dpv.gyro_z = Wire.read()<<8 | Wire.read(); // 131.0; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  

  myFile = SD.open("dat1.bin", FILE_WRITE);
  if (myFile) {
    Serial.println("write on sd...");
  myFile.write((const uint8_t *)&dpv, sizeof(dpv));

}
  myFile.close();
  //delete dpv;


}
