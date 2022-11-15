/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/superProbe/src/superProbe.ino"
/*
 * Project superProbe
 * Description: Super LoRa sensor node
 * Author: Christian Chavez
 * Date: 1665096473 UNIX
 */

#include <math.h>


void setup();
void loop();
void takeSamples();
void sendData();
void radioSetup();
void createBuffer(int _sendID, int _bufferLength, float _tempF, float _batteryVoltage);
void atTest();
void softwareReset();
void setMode(int modeParameter);
void setIPR(int _baudRate);
void setParameter(int radioParameter);
void setBand(int bandParameter);
void setAddress(int address);
void setNetworkID(int networkID);
void setCPIN();
void setCRFOP(int outputPower);
void atSend(char _buffer[50]);
void askVER();
void askUID();
void FACTORY();
void reyaxOn();
void reyaxOff();
#line 11 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/superProbe/src/superProbe.ino"
int probeNumber = 2;                 // identify devices
const int THERMISTORNOMINAL = 10000; // resistance at 25 degrees C
const int TEMPERATURENOMINAL = 25;   // temp for nominal resistance (almost always 25 C)
const int SERIESRESISTOR = 10000;    // value of 'other' resistor
const int NUMSAMPLES = 100;          // number of samples to be taken
const int BCOEFFICIENT = 3950;       // beta coeficient of thermistor

const int THERMISTORPIN = A0;

float samples[NUMSAMPLES]; // temperature samples
float tempF;
float tempMap;
float average;
float averageResistance;
float tempC;

float batteryVoltage;

int counter;

char buffer[50];

bool serialPrint, particlePublish, askSetting;

int address = 42;
int networkID = 6;
int sendID = 23;
int bufferLength = 55;
int bandParameter = 0;  // 0 for 915 and 1 for 868
int radioParameter = 0; // 0 for within 3km 1 for over 3km
int baudRate = 115200;
int outputPower = 15; // 0-15

SystemSleepConfiguration config;
int lowPowerTime = 30; // number of minutes

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  RGB.control(TRUE);
  RGB.brightness(10);
  switch (probeNumber)
  {
  case 1:
    RGB.color(0, 0, 255); // blue!
    break;
  case 2:
    RGB.color(25, 255, 88); // green!
    break;
  case 3:
    RGB.color(255, 69, 0); // orange!
    break;
  case 4:
    RGB.color(160, 32, 240); // purple!
    break;
  case 5:
    RGB.color(255, 0, 0); // red!
    break;
  case 6:
    RGB.color(115, 147, 179); // grey!
    break;
  case 7:
    RGB.color(255, 255, 0); // yellow
    break;
  default:
    RGB.control(FALSE);
    break;
  }
  Serial.begin(9600);
  Serial1.begin(115200);
  delay(1000);
  pinMode(THERMISTORPIN, INPUT);

  serialPrint = true;     // serial print to screen
  particlePublish = false; // publish to particle cloud
  askSetting = false;      // ask lora setting values

  // serialPrint = true;     // serial print to screen
  // particlePublish = false; // publish to particle cloud
  // askSetting = true;      // ask lora setting values

  // FACTORY(); // set lora module to factory defaults
  atTest();
  radioSetup(); // start lora module

  config.mode(SystemSleepMode::ULTRA_LOW_POWER)
      .duration(lowPowerTime * 60000); // minutes to milliseconds
  counter = 0;
}

void loop()
{
  // setMode(0);
  setMode(0);
  delay(500);
  takeSamples();
  createBuffer(sendID, bufferLength, tempF, batteryVoltage);
  sendData();
  delay(1000);
  setMode(1);
  delay(500);
  System.sleep(config);
  System.reset();
}

void takeSamples()
{
  uint8_t i;

  // take N samples in a row, with a slight delay
  for (i = 0; i < NUMSAMPLES; i++)
  {
    samples[i] = analogRead(THERMISTORPIN);
    delay(10);
  }

  // average all the samples out
  average = 0;
  for (i = 0; i < NUMSAMPLES; i++)
  {
    average += samples[i];
  }
  average /= NUMSAMPLES;

  // convert the value to resistance
  averageResistance = 4095 / average - 1;
  averageResistance = SERIESRESISTOR / averageResistance;

  tempC = averageResistance / THERMISTORNOMINAL; // (R/Ro)
  tempC = log(tempC);                            // ln(R/Ro)
  tempC /= BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  tempC += 1.0 / (TEMPERATURENOMINAL + 273.15);  // + (1/To)
  tempC = 1.0 / tempC;                           // Invert
  tempC -= 273.15;                               // convert absolute temp to C

  tempF = tempC * 1.8 + 32;

  // tempF = random(0,23000)/100.0;

  Serial.printf("tempC: %f tempF: %f counter: %i deviceID: %s\n", tempC, tempF, counter, System.deviceID().c_str());

  batteryVoltage = analogRead(BATT) * 0.0011224;
  Serial.printf("voltage: %f\n", batteryVoltage);

  // delay(1000);
}

void sendData()
{
  String reply;
  Serial1.println(buffer);
  Serial1.flush();
  delay(2300);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint == true)
    {
      Serial.printf("send reply: %s\n", reply.c_str()); //+OK
      Serial.flush();
    }
    if (particlePublish)
    {
      Particle.publish("sendData()", reply);
    }
  }
}

void radioSetup()
{
  setMode(0); // turn on transmit and receive mode
  atTest();
  setAddress(address); // between 0-65535
  setNetworkID(networkID);
  setBand(bandParameter);
  setParameter(radioParameter);
  setIPR(baudRate);
  setCRFOP(outputPower);
  setCPIN();
}

void createBuffer(int _sendID, int _bufferLength, float _tempF, float _batteryVoltage)
{
  sprintf(buffer, "AT+SEND=%i,%i,%0.2f,%0.2f,%i\r\n", _sendID, _bufferLength, _tempF, _batteryVoltage, probeNumber);
  if (serialPrint)
  {
    Serial.printf("createBuffer: %s", buffer);
  }
  if (particlePublish)
  {
    Particle.publish("createBuffer()", buffer);
  }
}

void atTest() // 1
{             // test if module responds to commands
  String reply;
  Serial1.printf("AT\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint == true)
    {
      Serial.printf("atTest reply: %s\n", reply.c_str()); //+OK
    }
    if (particlePublish == true)
      ;
    {
      Particle.publish("atTest()", reply);
    }
  }
}

void softwareReset() // 2
{                    // resets software
  String reply;
  String reply2;
  Serial1.printf("AT+RESET\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');
    reply2 = Serial1.readStringUntil('\n');

    if (serialPrint == true)
    {
      Serial.printf("atTest reply: %s\n", reply.c_str());  //+RESET
      Serial.printf("atTest reply: %s\n", reply2.c_str()); //+READY
    }
    if (particlePublish == true)
    {
      Particle.publish("softwareReset()", reply);
      Particle.publish("softwareReset()2", reply2);
    }
  }
}
//************************** TEST SLEEP MODE AND WAKE
void setMode(int modeParameter) // 3
{                               // Set the work mode between Transmit and Receive mode or Sleep mode
  String reply;
  String reply2;
  if (modeParameter == 1)
  {
    Serial1.printf("AT+MODE=1\r\n");
    Serial.printf("AT+MODE=1\r\n");
    Serial.flush();
    Serial1.flush();
  }
  else
  {
    Serial1.printf("AT+MODE=0\r\n");
    Serial.printf("AT+MODE=0\r\n");
    Serial.flush();
    Serial1.flush();
  }
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');
    reply2 = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf("atMode reply: %s reply2: %s\n", reply.c_str(), reply2.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("atMode()", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+MODE?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+MODE?: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+MODE?", reply);
      }
    }
  }
}

void setIPR(int _baudRate) // 4
{                          //  Set UART Baud rate
  String reply;
  Serial1.printf("AT+IPR=%i\r\n", _baudRate);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str()); //+OK
    }
    if (particlePublish)
    {
      Particle.publish("setIPR", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+IPR?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+IPR?: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+IPR?", reply);
      }
    }
  }
}

//************************** TEST RADIO PARAMETERS
void setParameter(int radioParameter) // 5
{                                     //  Set the Radio Frequency parameters
  String reply;
  // Serial1.printf("AT+PARAMETER=12,7,1,4\r\n"); // default working with v1 home base
  Serial1.printf("AT+PARAMETER=12,7,1,7\r\n"); // working with v1 home base
  //  if (radioParameter == 0)
  // {
  //     Serial1.printf("AT+PARAMETER=10,7,1,7\r\n"); // within 3km //not working with v1 home base
  // }
  // else if (radioParameter == 1)
  // {
  //     Serial1.printf("AT+PARAMETER=12,4,1,7\r\n"); // more than 3km //not working with v1 home base
  // }
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str()); //+OK
    }
    if (particlePublish)
    {
      Particle.publish("setRadio", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+PARAMETER?\r\n");
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+PARAMETER?: %s\n", reply.c_str()); //+PARAMETER=" "
      if (particlePublish)
      {
        Particle.publish("setParameter?", reply);
      }
    }
  }
}

//************************* Check band before sending to england
void setBand(int bandParameter) // 6
{                               //  Set RF Band
  String reply;
  if (bandParameter == 0)
  {
    Serial1.printf("AT+BAND=915000000\r\n"); // united states
  }
  else if (bandParameter == 1)
    ;
  {
    Serial1.printf("AT+BAND=868500000\r\n"); // europe
  }

  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str()); //+BAND=" "
    }
    if (particlePublish)
    {
      Particle.publish("setBand", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+BAND?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+BAND?: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+BAND?", reply);
      }
    }
  }
}

void setAddress(int address) // 7
{                            //  Set set module address
  String reply;
  Serial1.printf("AT+ADDRESS=%i\r\n", address);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("setAddress", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+ADDRESS?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply get address: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+ADDRESS?", reply);
      }
    }
  }
}

void setNetworkID(int networkID) // 8
{                                //  Set set module network ID
  String reply;
  Serial1.printf("AT+NETWORKID=%i\r\n", networkID);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str()); //+BAND=" "
    }
    if (particlePublish)
    {
      Particle.publish("setAddress", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+NETWORKID?\r\n");
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+NETWORKID? %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+NETWORKID?", reply);
      }
    }
  }
}

void setCPIN() // 9
{              //  set password
  String CPIN = "BA4104032968BA1114299079021594AA";
  String reply;
  Serial1.printf("AT+CPIN=%s\r\n", CPIN.c_str());
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("setCPIN", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+CPIN?\r\n");
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply CPIN: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+CPIN?", reply);
      }
    }
  }
}

void setCRFOP(int outputPower) // 10
{                              //  set RF output power
  String reply;
  Serial1.printf("AT+CRFOP=%i\r\n", outputPower);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("AT+CRFOP", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+CRFOP?\r\n");
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+CRFOP?: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+CRFOP?", reply);
      }
    }
  }
}

void atSend(char _buffer[50]) // 11
{                             //  send data
  String reply;
  Serial1.println(_buffer);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf(" reply: %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("AT+SEND", reply);
    }
  }
  if (askSetting)
  {
    Serial1.printf("AT+SEND?\r\n");
    delay(200);
    if (Serial1.available() > 0)
    {
      Serial.printf("Awaiting Reply\n");
      reply = Serial1.readStringUntil('\n');
      Serial.printf("Reply AT+SEND?: %s\n", reply.c_str());
      if (particlePublish)
      {
        Particle.publish("AT+SEND?", reply);
      }
    }
  }
}

void askVER() // 13
{             //  ask the firmware version
  String reply;
  Serial1.printf("At+VER?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf("reply askVER(): %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("askVER", reply);
    }
  }
}

void askUID() // 14
{             //  ask the UID
  String reply;
  Serial1.printf("At+UID?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf("reply askUID(): %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("askUID", reply);
    }
  }
}

void FACTORY() // 15
{              //  reset to factory defaults
  String reply;
  Serial1.printf("AT+FACTORY\r\n");
  Serial1.flush();
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');

    if (serialPrint)
    {
      Serial.printf("reply AT+FACTORY: %s\n", reply.c_str());
    }
    if (particlePublish)
    {
      Particle.publish("AT+FACTORY", reply);
    }
  }
}

void reyaxOn()
{
  String reply;

  Serial1.printf("AT+MODE=0\r\n");
  delay(2000);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from reyaxOn()\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply: %s\n", reply.c_str());
  }
  Serial1.printf("AT+MODE?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from mode?\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply: %s\n", reply.c_str());
  }
}

void reyaxOff()
{
  String reply;
  Serial1.printf("AT+MODE=1\r\n");
  Serial1.flush();
  Serial.printf("Powering down...\n");
  Serial.flush();
  delay(1000);
  // if (Serial1.available() > 0)
  // {
  //   Serial.printf("Awaiting Reply from reyaxOff()\n");
  //   reply = Serial1.readStringUntil('\n');
  //   Serial.printf("Reply: %s\n", reply.c_str());
  // }
  // Serial1.printf("AT+MODE?\r\n");
  // delay(2000);
  // if (Serial1.available() > 0)
  // {
  //   Serial.printf("Awaiting Reply from mode?\n");
  //   reply = Serial1.readStringUntil('\n');
  //   Serial.printf("Reply: %s\n", reply.c_str());
  //   delay(1000);
  // }
}