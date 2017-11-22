/*
BME280 BRZO I2C Test.ino

This code shows how to record data from the BME280 environmental sensor
using I2C interface and https://github.com/pasko-zh/brzo_i2c library
on ESP8266.

This file is an example file, part of the Arduino BME280 library.

Copyright (C) 2016  Tyler Glenn
Forked by Alex Shavlovsky
to support https://github.com/pasko-zh/brzo_i2c library on ESP8266.

GNU General Public License

Written: Dec 30 2015.
Last Updated: Oct 07 2017.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  D2 on ESP8266
SCK (Serial Clock)  ->  D1 on ESP8266

 */
#include "Arduino.h"

#include "brzo_i2c.h"
#define USING_BRZO 1
#include "BME280I2C_BRZO.h"

#define SERIAL_BAUD 115200

const uint32_t I2C_ACK_TIMEOUT = 2000;

BME280I2C_BRZO bme;   // Default : forced mode, standby time = 1000 ms
                      // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

bool metric = true;

//////////////////////////////////////////////////////////////////
void setup()
{
   Serial.begin(SERIAL_BAUD);

   while(!Serial) {} // Wait

   brzo_i2c_setup(SDA,SCL,I2C_ACK_TIMEOUT);

   while(!bme.begin())
   {
      Serial.println("Could not find BME280 sensor!");
      delay(1000);
   }

   switch(bme.chipModel())
   {
      case BME280::ChipModel_BME280:
        Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
   }
}

//////////////////////////////////////////////////////////////////
void loop()
{
   printBME280Data(&Serial);
   delay(500);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);
   
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->print(" Pa");
}
