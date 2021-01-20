/*
BME280I2C_BRZO.cpp
This code records data from the BME280 sensor and provides an API.
This file is part of the Arduino BME280 library.

Copyright (C) 2016  Tyler Glenn
Forked by Alex Shavlovsky
to support https://github.com/pasko-zh/brzo_i2c library on ESP8266.

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
Last Updated: Oct 07 2017.

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the BME280I2C_BRZO environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
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


/****************************************************************/
bool BME280I2C_BRZO::IsConnected(void)
{
  bool connected = false;
  brzo_i2c_start_transaction(m_settings.bme280Addr, m_settings.i2cClockRate);
  if(0 == brzo_i2c_end_transaction())
    connected = true;

  return connected;
}

#endif
