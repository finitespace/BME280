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

/* Recommended Modes -
   Based on Bosch BME280I2C environmental sensor data sheet.

Weather Monitoring :
   forced mode, 1 sample/minute
   pressure ×1, temperature ×1, humidity ×1, filter off
   Current Consumption =  0.16 μA
   RMS Noise = 3.3 Pa/30 cm, 0.07 %RH
   Data Output Rate 1/60 Hz

Humidity Sensing :
   forced mode, 1 sample/second
   pressure ×0, temperature ×1, humidity ×1, filter off
   Current Consumption = 2.9 μA
   RMS Noise = 0.07 %RH
   Data Output Rate =  1 Hz

Indoor Navigation :
   normal mode, standby time = 0.5ms
   pressure ×16, temperature ×2, humidity ×1, filter = x16
   Current Consumption = 633 μA
   RMS Noise = 0.2 Pa/1.7 cm
   Data Output Rate = 25Hz
   Filter Bandwidth = 0.53 Hz
   Response Time (75%) = 0.9 s


Gaming :
   normal mode, standby time = 0.5ms
   pressure ×4, temperature ×1, humidity ×0, filter = x16
   Current Consumption = 581 μA
   RMS Noise = 0.3 Pa/2.5 cm
   Data Output Rate = 83 Hz
   Filter Bandwidth = 1.75 Hz
   Response Time (75%) = 0.3 s

*/

BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C bme(settings);

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(SERIAL_BAUD);

  while(!Serial) {} // Wait

  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Could not find BME280I2C sensor!");
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

   // Change some settings before using.
   settings.tempOSR = BME280::OSR_X4;

   bme.setSettings(settings);
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
   client->println("Pa");

   delay(1000);
}
