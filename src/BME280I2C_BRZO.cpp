/*
BME280I2C_BRZO.cpp
This code records data from the BME280 sensor and provides an API.
This file is part of the Arduino BME280 library.

Based on the data sheet provided by Bosch for the BME280I2C_BRZO environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.


Copyright (c) 2016 Tyler Glenn, Alex Shavlovsky, et al.

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

#include "BME280I2C_BRZO.h"

#ifdef USING_BRZO

#include "brzo_i2c.h"

/****************************************************************/
BME280I2C_BRZO::BME280I2C_BRZO
(
  const Settings& settings
):BME280I2C(settings),
  m_settings(settings)
{
}

/****************************************************************/
void BME280I2C_BRZO::setSettings
(
   const Settings& settings
)
{
   m_settings = settings;
   BME280::setSettings(settings);
}


/****************************************************************/
const BME280I2C_BRZO::Settings& BME280I2C_BRZO::getSettings() const
{
   return m_settings;
}


/****************************************************************/
bool BME280I2C_BRZO::WriteRegister
(
  uint8_t addr,
  uint8_t data
)
{
    uint8_t bf[2];
    bf[0] = addr;
    bf[1] = data;
    brzo_i2c_start_transaction(m_settings.bme280Addr, m_settings.i2cClockRate);
    brzo_i2c_write(bf, 2, false);
    return (brzo_i2c_end_transaction()==0);
}

/****************************************************************/
bool BME280I2C_BRZO::ReadRegister
(
  uint8_t addr,
  uint8_t data[],
  uint8_t length
)
{
    brzo_i2c_start_transaction(m_settings.bme280Addr, m_settings.i2cClockRate);
    brzo_i2c_write(&addr, 1, true);
    brzo_i2c_read(data, length, false);
    brzo_i2c_end_transaction();
    return (brzo_i2c_end_transaction()==0);
}

#endif
