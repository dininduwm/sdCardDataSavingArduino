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

File root;

// convert to char(bytes) to store
String convertToChar(float floatVal)
{
  byte *b = (byte *)&floatVal;
  String result = (String) char(b[3])+char(b[2])+char(b[1])+char(b[0]);
  return result;
}

void printDirectory(File dir, int numTabs)
{
  while (true)
  {

    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void setup()
{
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  //Serial.println("done!");
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
    Serial.println("error opening data.csv");
  }

  root = SD.open("/");

  printDirectory(root, 0);
  
}

void loop()
{
  // nothing happens after setup finishes.
}
