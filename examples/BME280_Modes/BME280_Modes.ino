/*
BME280I2C Modes.ino

This code shows how to use predefined recommended settings from Bosch for
the BME280I2C environmental sensor.

GNU General Public License

Written: Dec 30 2015.
Last Updated: Sep 23 2017.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

#include <BME280I2C.h>
#include <Wire.h>             // Needed for legacy versions of Arduino.

#define SERIAL_BAUD 115200

BME280I2C bme;                  // Default : forced mode, standby time = 1000 ms
                                // pressure ×1, temperature ×1, humidity ×1, filter off

/* Based on Bosch BME280I2C environmental sensor data sheet. */

//BME280I2C bme;                // Weather Monitoring : forced mode, 1 sample/minute
                                // pressure ×1, temperature ×1, humidity ×1, filter off
                                // Current Consumption =  0.16 μA
                                // RMS Noise = 3.3 Pa/30 cm, 0.07 %RH
                                // Data Output Rate 1/60 Hz

//BME280I2C bme(1, 1, 0);       // Humidity Sensing : forced mode, 1 sample/second
                                // pressure ×0, temperature ×1, humidity ×1, filter off
                                // Current Consumption = 2.9 μA
                                // RMS Noise = 0.07 %RH
                                // Data Output Rate =  1 Hz

//BME280I2C bme(2, 1, 5, 3, 0, 4); // Indoor Navigation : normal mode, standby time = 0.5ms
                                // pressure ×16, temperature ×2, humidity ×1, filter = x16
                                // Current Consumption = 633 μA
                                // RMS Noise = 0.2 Pa/1.7 cm
                                // Data Output Rate = 25Hz
                                // Filter Bandwidth = 0.53 Hz
                                // Response Time (75%) = 0.9 s


//BME280I2C bme(1, 0, 4, 3, 0, 4); // Gaming : normal mode, standby time = 0.5ms
                                // pressure ×4, temperature ×1, humidity ×0, filter = x16
                                // Current Consumption = 581 μA
                                // RMS Noise = 0.3 Pa/2.5 cm
                                // Data Output Rate = 83 Hz
                                // Filter Bandwidth = 1.75 Hz
                                // Response Time (75%) = 0.3 s

bool metric = false;

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(SERIAL_BAUD);

  while(!Serial) {} // Wait
  
  while(!bme.begin())
  {
    Serial.println("Could not find BME280I2C sensor!");
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

   bme.read(pres, temp, hum, pressureUnit, metric);   // Parameters: (float& pressure, float& temp, float& humidity, bool hPa = true, bool celsius = false)

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

   client->print("\t\tAltitude: ");
   client->print(altitude);
   client->print((metric ? "m" : "ft"));
   client->println();
}
