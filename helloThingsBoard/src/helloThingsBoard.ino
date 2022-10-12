/*
 * Project helloThingsBoard
 * Description: ThingsBoard IoT demo
 * Author: Christian Chavez
 * Date: 1662244738 UNIX
 */

#include <PubSubClient.h>
#include <JsonParserGeneratorRK.h>

// #define TOKEN "qIiTpvhvsuslQzkCxVhH" //caseys solar base
#define TOKEN "WYAarNcBsj6qVc0NKg1c"
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

bool dataSent = false;
bool onlineNow = false;
bool earlyBird = false;

String parse0, parse1, parse2, parse3, parse4;

String incomingString;
String incomingString2;
int probeNumber;

float tempF;
float voltage;

float t1;
float t2;
float t3;
float t4;
float t5;
float t6;
float t7;
float t8;
float t9;
float t10;
float v1;
float v2;
float v3;
float v4;
float v5;
float v6;
float v7;
float v8;
float v9;
float v10;

bool x1 = false;
bool x2 = false;
bool x3 = false;
bool x4 = false;
bool x5 = false;

bool startConnecting = false;
bool connect = false;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    pinMode(D7, OUTPUT);
    reyaxSetup();
    goOnline();
    Particle.setDisconnectOptions(CloudDisconnectOptions().graceful(true).timeout(120000));
    plume.setServer(thingsboardServer, MQTTPORT);
    plumeConnect();
    plumePublish();
    snprintf(mqttBuf, sizeof(mqttBuf), "%s", versions);
    Serial.printf("mqttBuf: %s\nsizeOf(mqttBuf): %i/n", mqttBuf, sizeof(mqttBuf));
    goOffline();
}

void loop()
{
    if (onlineNow)
    {
        plume.loop();
    }

    if ((earlyBird == false) && (((Time.minute() == 22) || (Time.minute() == 29)) && !Particle.connected()))
    {
        startConnecting = true;
    }

    if (startConnecting)
    {
        RGB.brightness(3);
        onlineNow = true;
        goOnline();
        Particle.publishVitals();
        earlyBird = true; // we are early
        if (!plume.connected())
        {
            plumeConnect();
        }
        startConnecting = false;
    }

    if (((Time.minute() == 23) || (Time.minute() == 30)) && (dataSent == false))
    {
        connect = true;
    }
    if (connect)
    {
        Particle.syncTime();
        sprintf(timeBuf, "%i000", Time.now());
        plumePublish();
        // sendData();
        earlyBird = false; // we are too early
        connect = false;
    }
    if (Serial1.available())
    {
        parse0 = Serial1.readStringUntil(',');
        parse1 = Serial1.readStringUntil(',');
        tempF = Serial1.readStringUntil(',').toFloat();
        voltage = Serial1.readStringUntil(',').toFloat();
        probeNumber = Serial1.readStringUntil('\n').toInt();
        incomingString = Serial1.readString();
        Serial.printf("incomingString: %s\n", incomingString.c_str());
        Serial.printf("tempF: %.02f, probeNumber: %i\n", tempF, probeNumber);

        if (probeNumber == 1)
        {
            t1 = tempF;
            v1 = voltage;
            x1 = true;
        }
        else if (probeNumber == 2)
        {
            t2 = tempF;
            v2 = voltage;
            x2 = true;
        }
        else if (probeNumber == 3)
        {
            t3 = tempF;
            v3 = voltage;
            x3 = true;
        }
        else if (probeNumber == 4)
        {
            t4 = tempF;
            v4 = voltage;
            x4 = true;
        }

        else if (probeNumber == 5)
        {
            t5 = tempF;
            v5 = voltage;
            x5 = true;
        }
        else if (probeNumber == 6)
        {
            t6 = tempF;
            v6 = voltage;
            // x6 = true;
        }
        else if (probeNumber == 7)
        {
            t7 = tempF;
            v7 = voltage;
        }
        else
        {
            reyaxReset();
        }
        // Serial.printf("tempC: %.02f\ntempF: %.02f\ndeviceID: %s\n---\n", tempC, tempF, deviceID.c_str());
        incomingString2 = Serial1.readString();
        Serial.printf("incomingString2: %s\n", incomingString2.c_str());
        Serial.flush();
        // addData(tempF);
        dataSent = false;
    }
}

void plumePublish()
{
    if (!plume.connected())
    {
        plumeConnect();
    }
    uint64_t timeNow = Time.now();
    // String timeNow = String(Time.now() * 1000);
    // Serial.println(timeNow);
    sprintf(timeBuf, "%i000", Time.now());
    Serial.printf("timeBuf: %s\n", timeBuf);
    Serial.printf("timeNow: %i000\n", timeNow);
    Serial.println(Time.now());
    Serial.println(Time.now() * 1000);
    float batterySoc = System.batteryCharge();
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"battery1\":{\"b1\":%.2f,\"v1\":%.2f}}", timeBuf, batterySoc, v1);
    // {"ts":1451649600512, "values":{"key1":"value1", "key2":"value2"}}
    if (x1)
    {
        snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values1\":{\"t1\":%.2f,\"v1\":%.2f}}", timeBuf, t1, v1);
        // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f, \"v1\":%.2f}", t1, v1);
        plume.publish("v1/devices/me/telemetry", mqttBuf);
        x1 = false;
    }

    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t2\":%.2f,\"v2\":%.2f}", t2, v2);
    if (x2)
    {
        snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values2\":{\"t2\":%.2f,\"v2\":%.2f}}", timeBuf, t2, v2);
        plume.publish("v1/devices/me/telemetry", mqttBuf);
        x2 = false;
    }

    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t3\":%.2f,\"v3\":%.2f}", t3, v3);
    if (x3)
    {
        snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values3\":{\"t3\":%.2f,\"v3\":%.2f}}", timeBuf, t3, v3);
        plume.publish("v1/devices/me/telemetry", mqttBuf);
        x3 = false;
    }

    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t4\":%.2f,\"v4\":%.2f}", t4, v4);
    if (x4)
    {
        snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values4\":{\"t4\":%.2f,\"v4\":%.2f}}", timeBuf, t4, v4);
        plume.publish("v1/devices/me/telemetry", mqttBuf);
        x4 = false;
    }

    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t5\":%.2f,\"v5\":%.2f}", t5, v5);
    if (x5)
    {
        snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values5\":{\"t5\":%.2f,\"v5\":%.2f}}", timeBuf, t5, v5);
        plume.publish("v1/devices/me/telemetry", mqttBuf);
        x5 = false;
    }
    plume.loop();

    // // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t6\":%.2f,\"v6\":%.2f}", t6, v6);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values6\":{\"t6\":%.2f,\"v6\":%.2f}}", timeBuf, t6, v6);
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
    // // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t7\":%.2f,\"v7\":%.2f}", t7, v7);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values7\":{\"t7\":%.2f,\"v7\":%.2f}}", timeBuf, t7, v7);
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
    // // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t8\":%.2f,\"v8\":%.2f}", t8, v8);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values8\":{\"t8\":%.2f,\"v8\":%.2f}}", timeBuf, t8, v8);
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
    // // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t9\":%.2f,\"v9\":%.2f}", t9, v9);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values9\":{\"t9\":%.2f,\"v9\":%.2f}}", timeBuf, t9, v9);
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
    // // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t10\":%.2f,\"v10\":%.2f}", t10, v10);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values10\":{\"t10\":%.2f,\"v10\":%.2f}}", timeBuf, t10, v10);
    // plume.publish("v1/devices/me/telemetry", mqttBuf);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f,\"t2\":%.2f\"t3\":%.2f,\"t4\":%.2f\"t5\":%.2f,\"t6\":%.2f\"t7\":%.2f,\"t8\":%.2f\"t9\":%.2f,\"t10\":%.2f}", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
    //  sprintf(mqttBuf, "{\"t1\":%.2f,\"t2\":%.2f\"t3\":%.2f,\"t4\":%.2f\"t5\":%.2f,\"t6\":%.2f\"t7\":%.2f,\"t8\":%.2f\"t9\":%.2f,\"t10\":%.2f}", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
    // Serial.printf("mqttBuf: %s\nsizeOf(mqttBuf): %i\n", mqttBuf, sizeof(mqttBuf));
    // plume.publish("v1/devices/me/telemetry", mqttBuf);

    Particle.publish("Plume data sent...");
    dataSent = true;
    plume.loop();
    delay(1200);
    goOffline();
    onlineNow = false;
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

void reyaxSetup()
{
    String password = "BA4104032968BA1114299079021594AA";
    String reply;
    int addr = 23;
    int net = 6;
    byte buf[40];

    Serial1.printf("AT+CPIN=%s\r\n", password.c_str()); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply from password\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply: %s\n", reply.c_str());
    }

    Serial1.printf("AT+ADDRESS=%i\r\n", addr); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply from address\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply address: %s\n", reply.c_str());
    }

    Serial1.printf("AT+NETWORKID=%i\r\n", net); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply from networkid\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply network: %s\n", reply.c_str());
    }

    Serial1.printf("AT+ADDRESS?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply get address: %s\n", reply.c_str());
    }

    Serial1.printf("AT+NETWORKID?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply get network: %s\n", reply.c_str());
    }

    Serial1.printf("AT+CPIN?\r\n"); // correct
    delay(200);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply\n");
        reply = Serial1.readStringUntil('\n');
        reply.getBytes(buf, 40);
        Serial.printf("Reply get password: %s\n", (char *)buf);
    }
}

void goOnline()
{
    Particle.connect();
    // Serial.printf("Particle going online...\n");
    waitFor(Particle.connected, 240000);
    // Serial.printf("Particle online...\n");
    // Particle.publish("connected");
    // delay(1200);
    Particle.syncTime();
    waitFor(Particle.syncTimeDone, 30000);
    // Serial.printf("Time synced...\n");
}

void goOffline()
{
    // Serial.printf("Going offline...\n");
    //  Particle.publish("goOffline");
    //  delay(1200);
    Particle.disconnect();
    waitUntil(Particle.disconnected);
    Cellular.off();
    waitUntil(Cellular.isOff);
    // Serial.printf("Particle disconnected and cellular off...\n");
}

void reyaxReset()
{
    String reply;
    Serial1.printf("AT+RESET\r\n");
    delay(10000);
    if (Serial1.available() > 0)
    {
        Serial.printf("Awaiting Reply from address\n");
        reply = Serial1.readStringUntil('\n');
        Serial.printf("Reply address: %s\n", reply.c_str());
    }
    System.reset();
}