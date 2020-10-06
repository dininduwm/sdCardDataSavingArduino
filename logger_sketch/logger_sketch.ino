
//  Deek-Robot logging board: 5V to 5v vcc.  gnd to gnd.  sda to A4.  scl to A5.  sck to D13.  miso to D12.  mosi to D11  cs to D4.
//  chirp sensors: red to 5V vcc.  black to gnd.  green to sda (A4).  yellow to sck (A5).


// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
// for sd card
#include <SPI.h>
#include <SD.h>
#include <LowPower.h>   // lower power library to put arduino asleep during measurement intervals
#include <I2CSoilMoistureSensor.h>  // soil moisture sensor library


//_________________________________________________________________________________________________________________________________________________
// define sampling interval for the datalogger and datalogger number.   YOU WILL NEED TO CHANGE THE "dataloggerID" and "sample_intergal"
//_________________________________________________________________________________________________________________________________________________

float sample_interval = 255; // this number x8  will give the sample interval.  If you want an 80-sec interval set this number to 10). 225 will give sampling interval of 30min
String dataloggerID = "DL_1_s"; // this is the name for the datalogger - change it to something unique for each datalogger
float loop_count = 1; // don't change this.  

// soil moisture sensors
I2CSoilMoistureSensor sensor1(0x20);  // don't change - initializes the soil moisture sensor2
I2CSoilMoistureSensor sensor2(0x21);  // don't change - initializes the soil moisture sensor3
I2CSoilMoistureSensor sensor3(0x22);  // don't change - initializes the soil moisture sensor4

File myFile; // don't chagne gives name for the SD card file 
RTC_DS1307 rtc;  // rtc object

// RTC set up days of wekk
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup () {
  
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // moisture sensor setup 
  //  sensor1.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor1.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor1.getVersion(),HEX);
  Serial.println();
  
//  sensor2.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor2.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor2.getVersion(),HEX);
  Serial.println();
  
//  sensor3.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor3.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor3.getVersion(),HEX);
  Serial.println();


  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    }

//__________________ set rtc time here ________________________________________  
    // unsilence the below line to set (year, month, day, hour, min, sec:  e.g., (2018, 9, 25, 20, 38, 0) = sep 25, 2018, 20:38:00
    // after it is set you must silence the line again and reload
    // rtc.adjust(DateTime(2018, 10, 8, 10, 29, 0));
    
    // sd card        
    Serial.print("Initializing SD card...");
    if (!SD.begin(4)) {                               
      Serial.println("initialization failed!");
      return;
      }
    Serial.println("initialization done."); 
}


void loop () {
  
    // the RAM on the arduino is very small... so cannot let strings get longer than ~40 characters... or else bad things happen
    // therefore, break all string variables up into lengths not exceeding 40 chars
    String fullstring1 = "";  // create string "fullString" that contains all sensor data
    String fullstring2 = "";  // create string "fullString" that contains all clock data
    
        // call data from moisture sensor1 and add to string "fullstring"
    while (sensor1.isBusy()) delay(50); // available since FW 2.3  // make sure sensor is ready
    fullstring1 += sensor1.getCapacitance(); fullstring1 += ","; // read capacitiance sensor and add to string
    fullstring1 += sensor1.getTemperature(); fullstring1 += ","; //temperature register and add to string
    sensor1.sleep(); // available since FW 2.3
    
    // call data from moisture sensor2 and add to string "fullstring"
    while (sensor2.isBusy()) delay(50); // available since FW 2.3  // make sure sensor is ready
    fullstring1 += sensor2.getCapacitance(); fullstring1 += ","; // read capacitiance sensor and add to string
    fullstring1 += sensor2.getTemperature(); fullstring1 += ","; //temperature register and add to string
    sensor2.sleep(); // available since FW 2.3
    
    // call data from moisture sensor3 and add to string "fullstring"
    while (sensor3.isBusy()) delay(50); // available since FW 2.3  // make sure sensor is ready
    fullstring1 += sensor3.getCapacitance(); fullstring1 += ","; // read capacitiance sensor and add to string
    fullstring1 += sensor3.getTemperature(); fullstring1 += ","; //temperature register and add to string
    sensor3.sleep(); // available since FW 2.3
    
    
    // RTC code 
    DateTime now = rtc.now();
    fullstring2 += now.month(); fullstring2 += "/";
    fullstring2 += now.day(); fullstring2 += "/";
    fullstring2 += now.year(); fullstring2 += ",";
    fullstring2 += now.hour(); fullstring2 += ":";
    fullstring2 += now.minute(); fullstring2 += ":";
    fullstring2 += now.second(); // fullstring += ",";

    Serial.print(fullstring1);
    Serial.println(fullstring2);
    
    // sd card code
    myFile = SD.open("data.csv", FILE_WRITE);        
    // if the file opened okay, write to it:
    if (myFile) {
      myFile.print(fullstring1);
      myFile.println(fullstring2);
      myFile.close();
      Serial.println("done.");
      } else {
         // if the file didn't open, print an error:
         Serial.println("error opening data.csv");
         }
         
    delay(1000);  
    
   // using low power library
   while (loop_count <= sample_interval)
   {
     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // SLEEP_8s puts arduino to sleep for 8 seconds... then for another 8 and so-on ontil 'loop_count' = 'sample interval'
     loop_count = loop_count+1;
   }
   loop_count = 1; // Don't change this.  Sets loop_count back to one after coming out of deep sleep. should not be zero because LowPower will run once before adding anything

    
}











