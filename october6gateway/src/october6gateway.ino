/*
 * Project october6gateway
 * Description: IoT Gateway
 * Author: Christian Chavez !
 * Date: 1665096473 UNIX
 */

#include <Particle.h>
#include <Ubidots.h>
#include <PubSubClient.h>

// PRODUCT_ID(17507);   // for module
PRODUCT_VERSION(1); 
// #define TOKEN "WYAarNcBsj6qVc0NKg1c" // catalyst compost
String TOKEN = "WYAarNcBsj6qVc0NKg1c"; // catalyst compost
String ID = System.deviceID();
#define MQTTPORT 1883

char thingsboardServer[] = "thingsboard.cloud";

char mqttBuf[128];
char timeBuf[128];

TCPClient pubsub;
PubSubClient plume((Client &)pubsub); // mqtt server named after plume-toed swiftlet

FuelGauge fuel;

float tempC;
float tempF, tempBlue, tempGreen, tempOrange, temp004, temp005, temp006, temp007;
float voltage, voltageBlue, voltageGreen, voltageOrange, voltage004, voltage005, voltage006, voltage007;

float t1, t2, t3, t4, t5, t6, t7, t8;
float v1, v2, v3, v4, v5, v6, v7, v8;

bool x1 = false;
bool x2 = false;
bool x3 = false;
bool x4 = false;
bool x5 = false;
bool x6 = false;
bool x7 = false;
bool x8 = false;

String deviceID;
String incomingString;
String incomingString2;
int probeNumber;

String WEBHOOK_NAME = "customer01";
Ubidots ubidots("webhook", UBI_PARTICLE);

String parse0, parse1, parse2, parse3, parse4;

bool dataSent;
bool earlyBird = false; // used to go online early for more consistant publishes
bool onlineNow = false;

unsigned int lastSend;
unsigned int safetyTimer = 3600000;

unsigned int counter;

SystemSleepConfiguration config;

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  RGB.brightness(255);
  Serial.begin(9600);
  Serial1.begin(115200);
  delay(3000);
  reyaxSetup();

  Particle.setDisconnectOptions(CloudDisconnectOptions().graceful(true).timeout(120000));

  goOnline();
  Particle.syncTime();
  Particle.publishVitals();
  plume.setServer(thingsboardServer, MQTTPORT);
  plumeConnect();
  plumePublish();
  delay(3000);
  goOffline();
  RGB.brightness(3);
  lastSend = millis();
  counter = 0;
  dataSent = false;
  Serial.printf("deviceID: %s\n", ID.c_str());
}

void loop()
{
  if (onlineNow) ///////////////
  {
    plume.loop();
  }

  if ((earlyBird == false) && (((Time.minute() == 59) || (Time.minute() == 29)) && !Particle.connected()))
  {

    RGB.brightness(3);
    onlineNow = true;
    goOnline();
    Particle.syncTime();
    Particle.publishVitals();
    earlyBird = true; // we are early
    if (!plume.connected())
    {
      plumeConnect();
    }
  }
  if ((Time.minute() == 0) || (Time.minute() == 30))
  // if (((Time.minute() == 0) || (Time.minute() == 30)) && (dataSent == false))
  {
    //sprintf(timeBuf, "%i000", Time.now());
    plumePublish();
    sendData();
    earlyBird = false; // we are too early
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

    // new since Thailand

    if (probeNumber == 1)
    {
      ubidots.add("tempBlue", tempF);
      ubidots.add("voltageBlue", voltage);
      tempBlue = tempF;
      voltageBlue = voltage;
      t1 = tempF;
      v1 = voltage;
      x1 = true;
    }
    else if (probeNumber == 2)
    {
      ubidots.add("tempGreen", tempF);
      ubidots.add("voltageGreen", voltage);
      tempGreen = tempF;
      voltageGreen = voltage;
      t2 = tempF;
      v2 = voltage;
      x2 = true;
    }
    else if (probeNumber == 3)
    {
      ubidots.add("tempOrange", tempF);
      ubidots.add("voltageOrange", voltage);
      tempOrange = tempF;
      voltageOrange = voltage;
      t3 = tempF;
      v3 = voltage;
      x3 = true;
    }
    else if (probeNumber == 4)
    {
      ubidots.add("004", tempF);
      ubidots.add("004Voltage", voltage);
      temp004 = tempF;
      voltage004 = voltage;
      t4 = tempF;
      v4 = voltage;
      x4 = true;
    }
    else if (probeNumber == 5)
    {
      ubidots.add("005", tempF);
      ubidots.add("005Voltage", voltage);
      temp005 = tempF;
      voltage005 = voltage;
      t5 = tempF;
      v5 = voltage;
      x5 = true;
    }
    else if (probeNumber == 6)
    {
      ubidots.add("006", tempF);
      ubidots.add("006Voltage", voltage);
      temp006 = tempF;
      voltage006 = voltage;
      t6 = tempF;
      v6 = voltage;
      x6 = true;
    }
    else if (probeNumber == 7)
    {
      ubidots.add("007", tempF);
      ubidots.add("007Voltage", voltage);
      temp007 = tempF;
      voltage007 = voltage;
      t7 = tempF;
      v7 = voltage;
      x7 = true;
    }
    else
    {
      ubidots.add("extraProbe", tempF);
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
  // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values\":{\"t1\":%.2f,\"v1\":%.2f}}", timeBuf, tempBlue, voltageBlue);
  // plume.publish("v1/devices/me/telemetry", mqttBuf);
  // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values\":{\"t2\":%.2f,\"v2\":%.2f}}", timeBuf, tempGreen, voltageGreen);
  // plume.publish("v1/devices/me/telemetry", mqttBuf);
  float batterySoc = System.batteryCharge();
  snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values\":{\"b\":%.2f}}", timeBuf, batterySoc);
  plume.publish("v1/devices/me/telemetry", mqttBuf);

  plume.loop();
  if (x1)
  {
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values1\":{\"t1\":%.2f,\"v1\":%.2f}}", timeBuf, t1, v1);
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t1\":%.2f, \"v1\":%.2f}", t1, v1);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x1 = false;
  }

  if (x2)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t2\":%.2f,\"v2\":%.2f}", t2, v2);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values2\":{\"t2\":%.2f,\"v2\":%.2f}}", timeBuf, t2, v2);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x2 = false;
  }

  if (x3)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t3\":%.2f,\"v3\":%.2f}", t3, v3);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values3\":{\"t3\":%.2f,\"v3\":%.2f}}", timeBuf, t3, v3);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x3 = false;
  }

  if (x4)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t4\":%.2f,\"v4\":%.2f}", t4, v4);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values4\":{\"t4\":%.2f,\"v4\":%.2f}}", timeBuf, t4, v4);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x4 = false;
  }

  if (x5)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t5\":%.2f,\"v5\":%.2f}", t5, v5);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values5\":{\"t5\":%.2f,\"v5\":%.2f}}", timeBuf, t5, v5);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x5 = false;
  }

  if (x6)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t6\":%.2f,\"v6\":%.2f}", t6, v6);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values4\":{\"t4\":%.2f,\"v4\":%.2f}}", timeBuf, t6, v6);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x6 = false;
  }

  if (x7)
  {
    snprintf(mqttBuf, sizeof(mqttBuf), "{\"t7\":%.2f,\"v7\":%.2f}", t7, v7);
    // snprintf(mqttBuf, sizeof(mqttBuf), "{\"ts\":%s, \"values5\":{\"t5\":%.2f,\"v5\":%.2f}}", timeBuf, t7, v7);
    plume.publish("v1/devices/me/telemetry", mqttBuf);
    x7 = false;
  }
  plume.loop();
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

void goOnline()
{
  Particle.connect();Particle.syncTime();
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

void sendData()
{
  goOnline();
  // Particle.publishVitals();
  delay(1200);
  // String superComp = String(tempBlue) + String(tempGreen) + String(tempOrange) + String(voltageBlue) + String(voltageGreen) + String(voltageOrange);
  // Particle.publish("temps", superComp);
  // Particle.publish("voltages", superComp);
  float batterySoc = System.batteryCharge();
  ubidots.add("counter", counter);
  ubidots.add("homeBattery", batterySoc);
  bool bufferSent = false;
  bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);
  // Particle.publish("bufferSent:", String(bufferSent));

  if (bufferSent)
  {
    // Serial.printf("Ubidots data sent...\n");
    Particle.publish("Ubidots data sent...");
    delay(1200);
    dataSent = true;
    checkUpdates();
    delay(1000);
    goOffline();
    RGB.brightness(3);
    onlineNow = false;
    lastSend = millis();
    // System.sleep(config);
  }
  else
  {
    // Serial.printf("Error...Ubidots data not sent...\n");
    Particle.publish("Error...ubidots data not sent...");
    delay(1200);
    // dataSent = true;
  }
  counter++;
  // System.sleep(config);
}

void checkUpdates()
{
  if (System.updatesPending())
  {
    System.enableUpdates();
    unsigned int start = millis();
    while (millis() - start < 120000)
    {
      Particle.process();
    }
    System.reset();
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
