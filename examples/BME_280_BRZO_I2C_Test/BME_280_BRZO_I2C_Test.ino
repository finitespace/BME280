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
Last Updated: Sep 23 2017.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  D2 on ESP8266
SCK (Serial Clock)  ->  D1 on ESP8266

 */

#include "brzo_i2c.h"
#include <BME280I2C_BRZO.h>

#define SERIAL_BAUD 115200

#define SDA D2
#define SCL D1

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
}

//////////////////////////////////////////////////////////////////
void loop()
{
   printBME280Data(&Serial);
   printBME280CalculatedData(&Serial);
   delay(500);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);
   uint8_t pressureUnit(3);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi

   bme.read(pres, temp, hum, metric, pressureUnit);   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(metric ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->print(" atm");
}

//////////////////////////////////////////////////////////////////
void printBME280CalculatedData
(
   Stream* client
)
{
   float altitude = bme.alt(metric);
   float dewPoint = bme.dew(metric);

   client->print("\t\tAltitude: ");
   client->print(altitude);
   client->print((metric ? "m" : "ft"));
   client->print("\t\tDew point: ");
   client->print(dewPoint);
   client->println("°"+ String(metric ? 'C' :'F'));
}
