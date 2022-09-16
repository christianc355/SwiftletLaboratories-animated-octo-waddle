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
#include <JsonParserGeneratorRK.h>

void setup();
void loop();
void plumePublish();
void plumeConnect();
#line 11 "/home/sun/Documents/SwiftletLaboratories-animated-octo-waddle/helloThingsBoard/src/helloThingsBoard.ino"
#define TOKEN "cglClS3UqPK1FGzY8Z8I"
#define MQTTPORT 1883

char thingsboardServer[] = "thingsboard.cloud";
char versions[] = "{\"firmware_version\":\"0.0.0\",\"software_version\":\"0.0\"}";
char mqttBuf[128];
char timeBuf[128];
char mqttSprint;
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

    if (millis() - lastTime > 10000)
    {
        plumePublish();
        lastTime = millis();
    }
    plume.loop();
}

void plumePublish()
{
    float t1 = random(80000, 170000) / 1000.0;
    float t2 = random(80000, 170000) / 1000.0;
    float t3 = random(80000, 170000) / 1000.0;
    float t4 = random(80000, 170000) / 1000.0;
    float t5 = random(80000, 170000) / 1000.0;
    float t6 = random(80000, 170000) / 1000.0;
    float t7 = random(80000, 170000) / 1000.0;
    float t8 = random(80000, 170000) / 1000.0;
    float t9 = random(80000, 170000) / 1000.0;
    float t10 = random(80000, 170000) / 1000.0;
    float v1 = random(32000, 47000) / 1000.0;
    float v2 = random(32000, 47000) / 1000.0;
    float v3 = random(32000, 47000) / 1000.0;
    float v4 = random(32000, 47000) / 1000.0;
    float v5 = random(32000, 47000) / 1000.0;
    float v6 = random(32000, 47000) / 1000.0;
    float v7 = random(32000, 47000) / 1000.0;
    float v8 = random(32000, 47000) / 1000.0;
    float v9 = random(32000, 47000) / 1000.0;
    float v10 = random(32000, 47000) / 1000.0;
    long long timeNow = Time.now();
    // String timeNow = String(Time.now() * 1000);
    // Serial.println(timeNow);
    sprintf(timeBuf,"%i000",Time.now());
    Serial.printf("timeBuf: %s\n",timeBuf);
    Serial.printf("timeNow: %i000\n",timeNow);
    Serial.println(Time.now());
    Serial.println(Time.now()*1000);
    // {"ts":1451649600512, "values":{"key1":"value1", "key2":"value2"}}
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values\":{\"t1\":%.2f,\"v1\":%.2f}}", timeBuf, t1, v1);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f, \"v1\":%.2f}", t1, v1);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t2\":%.2f,\"v2\":%.2f}", t2, v2);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t3\":%.2f,\"v3\":%.2f}", t3, v3);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t4\":%.2f,\"v4\":%.2f}", t4, v4);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t5\":%.2f,\"v5\":%.2f}", t5, v5);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t6\":%.2f,\"v6\":%.2f}", t6, v6);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t7\":%.2f,\"v7\":%.2f}", t7, v7);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t8\":%.2f,\"v8\":%.2f}", t8, v8);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t9\":%.2f,\"v9\":%.2f}", t9, v9);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t10\":%.2f,\"v10\":%.2f}", t10, v10);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f,\"t2\":%.2f\"t3\":%.2f,\"t4\":%.2f\"t5\":%.2f,\"t6\":%.2f\"t7\":%.2f,\"t8\":%.2f\"t9\":%.2f,\"t10\":%.2f}", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
    //  sprintf(mqttBuf, "{\"t1\":%.2f,\"t2\":%.2f\"t3\":%.2f,\"t4\":%.2f\"t5\":%.2f,\"t6\":%.2f\"t7\":%.2f,\"t8\":%.2f\"t9\":%.2f,\"t10\":%.2f}", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
    // Serial.printf("mqttBuf: %s\nsizeOf(mqttBuf): %i\n", mqttBuf, sizeof(mqttBuf));
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
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