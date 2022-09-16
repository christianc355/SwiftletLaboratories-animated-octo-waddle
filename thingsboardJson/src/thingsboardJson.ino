/*
 * Project thingsboardJson
 * Description: sending json strings to thingsboard
 * Author: Christian Chavez
 * Date: 1662331217 UNIX
 */

#include <PubSubClient.h>
#include <JsonParserGeneratorRK.h>

#define TOKEN "jwxt8v5sSO831PsU7B0V"
#define MQTTPORT 1883

char thingsboardServer[] = "thingsboard.cloud";

TCPClient pubsub;
PubSubClient plume((Client &)pubsub); // mqtt server named after plume-toed swiftlet

unsigned int lastTime;

const int LEDPIN = D7;

void setup()
{
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  plume.setServer(thingsboardServer, MQTTPORT);
  plumeConnect();
  lastTime = 5000;
}

void loop()
{
  if (!plume.connected())
  {
    plumeConnect();
  }
  if (millis() - lastTime > 5000)
  {
    createJsonPayload();
    lastTime = millis();
  }
  plume.loop();
}

void createJsonPayload()
{
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);
    jw.insertKeyValue("t1", random(75000, 175000) / 1000);
    // jw.insertKeyValue("t2", random(75000, 175000) / 1000);
    // jw.insertKeyValue("t3", random(75000, 175000) / 1000);
    // jw.insertKeyValue("t4", random(75000, 175000) / 1000);
    // jw.insertKeyValue("t5", random(75000, 175000) / 1000);
    plume.publish("v1/devices/me/telemetry", jw.getBuffer());
    static bool onOff = true;
    digitalWrite(LEDPIN, onOff);
    onOff = !onOff;
  }
}

void plumeConnect()
{
  while (!plume.connected())
  {
    Serial.println("plumeConnect(): Attempting to connect to Thingsboard server");
    if (plume.connect("Argon", TOKEN, NULL)) // ClientID, User, PW
    {
      Serial.println("plumeConnect(): Connected");
      return;
    }
    else
      Serial.println("plumeConnect(): Connection failed, retry in 3 seconds");
    delay(3000);
  }
}