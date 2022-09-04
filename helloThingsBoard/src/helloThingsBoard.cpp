/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/helloThingsBoard/src/helloThingsBoard.ino"
/*
 * Project helloThingsBoard
 * Description: ThingsBoard IoT demo
 * Author: Christian Chavez
 * Date: 1662244738 UNIX
 */

#include <PubSubClient.h>

void setup();
void loop();
void plumePublish();
void plumeConnect();
#line 10 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/helloThingsBoard/src/helloThingsBoard.ino"
#define TOKEN "ymVZkqclUjP6by3VpZeC"
#define MQTTPORT 1883

char thingsboardServer[] = "thingsboard.cloud";
char versions[] = "{\"firmware_version\":\"0.0.0\",\"software_version\":\"0.0\"}";
char mqttBuf[128];
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
    snprintf(mqttBuf, sizeof(mqttBuf), "%s", versions);
    Serial.printf("mqttBuf: %s\nsizeOf(mqttBuf): %i/n", mqttBuf, sizeof(mqttBuf));
}

void loop()
{
    if (!plume.connected())
    {
        plumeConnect();
    }

    if (millis() - lastTime > 5000)
    {
        plumePublish();
        lastTime = millis();
    }
}

void plumePublish()
{
    float t1 = random(80000, 170000) / 1000.0;
    float t2 = random(80000, 170000) / 1000.0;
    // float t3 = random(80000, 170000) / 1000.0;
    // float t4 = random(80000, 170000) / 1000.0;
    // float t5 = random(80000, 170000) / 1000.0;
    // float t6 = random(80000, 170000) / 1000.0;
    // float t7 = random(80000, 170000) / 1000.0;
    // float t8 = random(80000, 170000) / 1000.0;
    // float t9 = random(80000, 170000) / 1000.0;

    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f,\"t2\":%.2f}", t1, t2);
    Serial.printf("mqttBuf: %s\nsizeOf(mqttBuf): %i/n", mqttBuf, sizeof(mqttBuf));
    plume.publish("v1/devices/me/telemetry",mqttBuf);
    static bool onOff = true;
    digitalWrite(LEDPIN, onOff);
    onOff = !onOff;
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