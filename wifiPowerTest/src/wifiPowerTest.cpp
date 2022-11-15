/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/wifiPowerTest/src/wifiPowerTest.ino"
/*
 * Project wifiPowerTest
 * Description: testing power consumption of particle argon
 * Author: Christian Chavez
 * Date: 1666904161 UNIX
 */

void setup();
void loop();
#line 8 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/wifiPowerTest/src/wifiPowerTest.ino"
bool dataSent = false;

void setup()
{
  // RGB.control(true);
  RGB.brightness(255);
  Particle.syncTime();
  Particle.publishVitals();
}

void loop()
{
  if (((Time.minute() == 0) || (Time.minute() == 30)) && dataSent == false)
  {
    Particle.syncTime();
    Particle.publishVitals();
    Particle.publish("Hello!");
    dataSent = true;
  }

  if (((Time.minute() == 59) || (Time.minute() == 29)) && dataSent == true)
  {
    Particle.publish("Reset time");
    dataSent = false;
  }
}