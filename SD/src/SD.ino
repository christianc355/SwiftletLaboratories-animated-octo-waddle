/*
 * Project SD
 * Description:
 * Author:
 * Date:
 */
#include <SdFat.h>
#include <SPI.h>

File dataFile;

const int chipSelect = 4;
int temp;
int humid;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  Serial.println("About to collect make believe data");
  delay(2000);
  temp = random(65,75);
  humid = random(20,50);

  print2SD(temp,humid);
  delay(2000);
  read2SD();
}

void print2SD(int print_temp, int print_humid) {

  dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.printf("%i, %i \n",temp,humid);
    dataFile.close();
    Serial.printf("%i, %i \n",temp,humid);
  }  
  else {
    Serial.println("error opening datalog.csv");
  }
  return;
} 

void read2SD(){

    // re-open the file for reading:
  dataFile = SD.open("datalog.csv");
  if (dataFile) {
    Serial.println("datalog.csv: ");

    // read from the file until there's nothing else in it:
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("error opening datalog.txt");
  }
  return;
}