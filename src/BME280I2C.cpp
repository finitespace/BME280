/*
BME280I2CI2C.cpp
This code records data from the BME280I2C sensor and provides an API.
This file is part of the Arduino BME280I2C library.

Based on the data sheet provided by Bosch for the BME280I2C environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.


Copyright (c) 2016 Tyler Glenn, et al.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */

#include <Wire.h>

#include "BME280I2C.h"


/****************************************************************/
BME280I2C::BME280I2C
(
  const Settings& settings
):BME280(settings),
  m_settings(settings)
{
}


/****************************************************************/
void BME280I2C::setSettings
(
   const Settings& settings
)
{
   m_settings = settings;
   BME280::setSettings(settings);
}


/****************************************************************/
const BME280I2C::Settings& BME280I2C::getSettings() const
{
   return m_settings;
}


/****************************************************************/
bool BME280I2C::WriteRegister
(
  uint8_t addr,
  uint8_t data
)
{
  Wire.beginTransmission(m_settings.bme280Addr);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();

  return true; // TODO: Check return values from wire calls.
}


/****************************************************************/
bool BME280I2C::ReadRegister
(
  uint8_t addr,
  uint8_t data[],
  uint8_t length
)
{
  uint8_t ord(0);

  Wire.beginTransmission(m_settings.bme280Addr);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(static_cast<uint8_t>(m_settings.bme280Addr), length);

  while(Wire.available())
  {
    data[ord++] = Wire.read();
  }

  return ord == length;
}
