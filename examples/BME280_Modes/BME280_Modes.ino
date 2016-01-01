/*
BME280 Modes.ino
This code shows how to use predefined recommended settings from Bosch for
the BME280 environmental sensor.
This file is an example file, part of the Arduino BME280 library.
Copyright (C) 2016  Tyler Glenn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Written: Dec 30 2015.
Last Updated: Jan 1 2016.
 */

/* ==== Includes ==== */
#include <BME280.h>
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280 bme;                     // Default : forced mode, standby time = 1000 ms
                                // pressure ×1, temperature ×1, humidity ×1, filter off

/* Based on Bosch BME280 environmental sensor data sheet. */

//BME280 bme;                   // Weather Monitoring : forced mode, 1 sample/minute
                                // pressure ×1, temperature ×1, humidity ×1, filter off
                                // Current Consumption =  0.16 μA
                                // RMS Noise = 3.3 Pa/30 cm, 0.07 %RH
                                // Data Output Rate 1/60 Hz

//BME280 bme(0, 1, 1);          // Humidity Sensing : forced mode, 1 sample/second
                                // pressure ×0, temperature ×1, humidity ×1, filter off
                                // Current Consumption = 2.9 μA
                                // RMS Noise = 0.07 %RH
                                // Data Output Rate =  1 Hz

//BME280 bme(5, 2, 1, 1, 0, 4); // Indoor Navigation : normal mode, standby time = 0.5ms
                                // pressure ×16, temperature ×2, humidity ×1, filter = x16
                                // Current Consumption = 633 μA
                                // RMS Noise = 0.2 Pa/1.7 cm
                                // Data Output Rate = 25Hz
                                // Filter Bandwidth = 0.53 Hz
                                // Response Time (75%) = 0.9 s


//BME280 bme(3, 1, 0, 1, 0, 4); // Gaming : normal mode, standby time = 0.5ms
                                // pressure ×4, temperature ×1, humidity ×0, filter = x16
                                // Current Consumption = 581 μA
                                // RMS Noise = 0.3 Pa/2.5 cm
                                // Data Output Rate = 83 Hz
                                // Filter Bandwidth = 1.75 Hz
                                // Response Time (75%) = 0.3 s

bool metric = false;
/* ==== END Global Variables ==== */

/* ==== Prototypes ==== */
/* === Print a message to stream with the temp, humidity and pressure. === */
void printBME280Data(Stream * client);
/* === Print a message to stream with the altitude, dew point and. === */
void printBME280CalculatedData(Stream* client);
/* ==== END Prototypes ==== */

void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}

void loop() {
   printBME280Data(&Serial);
   printBME280CalculatedData(&Serial);
   delay(500);
}
void printBME280Data(Stream* client){
  float temp(NAN), hum(NAN), pres(NAN);
   uint8_t pressureUnit(3);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
  bme.ReadData(pres, temp, hum, pressureUnit, metric);                // Parameters: (float& pressure, float& temp, float& humidity, bool hPa = true, bool celsius = false)
  /* Alternatives to ReadData():
    float ReadTemperature(bool celsius = false);
    float ReadPressure(uint8_t unit = 0);
    float ReadHumidity();

    Keep in mind the temperature is used for humidity and
    pressure calculations. So it is more effcient to read
    temperature, humidity and pressure all together.
   */
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
void printBME280CalculatedData(Stream* client){
  float altitude = bme.CalculateAltitude(metric);
  client->print("\t\tAltitude: ");
  client->print(altitude);
  client->print((metric ? "m" : "ft"));
  client->println();

}
