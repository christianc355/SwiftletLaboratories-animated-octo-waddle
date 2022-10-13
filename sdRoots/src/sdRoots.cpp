/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/sdRoots/src/sdRoots.ino"
/*
 * Project sdRoots
 * Description: Back to roots two years ago sd card code
 * Author: Christian Chavez
 * Date: 1665604957 UNIX
 */

#include <SPI.h>
#include <SdFat.h>

void setup();
void loop();
#line 11 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/sdRoots/src/sdRoots.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);

const int chipSelect = D8;
// const int SAMPLE_INTERVAL_uS = 50;

#define FILE_BASE_NAME "t"

// Create file system object.
SdFat sd;
SdFile file;

unsigned int lastTime;

const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[13] = FILE_BASE_NAME "temps.csv";

//==============================================================================
// Error messages stored in flash.
#define error(msg) sd.errorHalt(msg)
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  delay(2300);

  Serial.printf("Hello\n");

  // Particle.syncTime();
  // Time.zone(-8);

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.cc
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50)))
  {
    sd.initErrorHalt();
  }

  if (BASE_NAME_SIZE > 6)
  {
    Serial.println("FILE_BASE_NAME too long");
    while (1)
      ;
  }
}

void loop()
{

  if (millis() - lastTime > 15000)
  {
    Serial.printf("Starting Data Logging \n");
    while (sd.exists(fileName))
    {
      if (fileName[BASE_NAME_SIZE + 1] != '9')
      {
        fileName[BASE_NAME_SIZE + 1]++;
      }
      else if (fileName[BASE_NAME_SIZE] != '9')
      {
        fileName[BASE_NAME_SIZE + 1] = '0';
        fileName[BASE_NAME_SIZE]++;
      }
      else
      {
        Serial.println("Can't create file name");
        while (1)
          ;
      }
    }

    if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL))
    {
      Serial.println("file.open");
    }
    Serial.printf("Logging to: %s \n", fileName);

    float t1 = random(70000, 170000) / 1000.0;
    float t2 = random(70000, 170000) / 1000.0;
    float t3 = random(70000, 170000) / 1000.0;
    float t4 = random(70000, 170000) / 1000.0;
    float t5 = random(70000, 170000) / 1000.0;
    float t6 = random(70000, 170000) / 1000.0;
    float t7 = random(70000, 170000) / 1000.0;

    float v1 = random(3000, 4700) / 1000.0;
    float v2 = random(3000, 4700) / 1000.0;
    float v3 = random(3000, 4700) / 1000.0;
    float v4 = random(3000, 4700) / 1000.0;
    float v5 = random(3000, 4700) / 1000.0;
    float v6 = random(3000, 4700) / 1000.0;
    float v7 = random(3000, 4700) / 1000.0;

    file.printf("%f, %f, %f, %f, %f\n", t1, t2, t3, t4, t5);

    if (!file.sync() || file.getWriteError())
    {
      Serial.printf("write error");
    }

    file.close();
    Serial.printf("Done \n");
    delay(2000);
    Serial.printf("Ready for next data log \n");

    lastTime = millis();
  }
}
