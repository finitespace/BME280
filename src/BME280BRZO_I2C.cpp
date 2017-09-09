/*
BME280BRZO_I2C.cpp
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
Last Updated: Jan 1 2016. - Happy New year!

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the BME280BRZO_I2C environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include "brzo_i2c.h"
#include "BME280BRZO_I2C.h"

/****************************************************************/
BME280BRZO_I2C::BME280BRZO_I2C
(
  uint8_t tosr,
  uint8_t hosr,
  uint8_t posr,
  uint8_t mode,
  uint8_t st,
  uint8_t filter,
  bool spiEnable, 
  uint8_t bme_280_addr
):BME280(tosr, hosr, posr, mode, st, filter, spiEnable), 
  bme_280_addr(bme_280_addr)
{
}


/****************************************************************/
bool BME280BRZO_I2C::WriteRegister
(
  uint8_t addr,
  uint8_t data
)
{
    uint8_t bf[2];
    bf[0] = addr;
    bf[1] = data;
    brzo_i2c_start_transaction(bme_280_addr, I2C_CLOCK_RATE);
    brzo_i2c_write(bf, 2, false);
    return (brzo_i2c_end_transaction()==0);
}

/****************************************************************/
bool BME280BRZO_I2C::ReadRegister
(
  uint8_t addr,
  uint8_t data[],
  uint8_t length
)
{
    brzo_i2c_start_transaction(bme_280_addr, I2C_CLOCK_RATE);
    brzo_i2c_write(&addr, 1, true);
    brzo_i2c_read(data, length, false);
    brzo_i2c_end_transaction();
    return (brzo_i2c_end_transaction()==0);
}


/****************************************************************/
#if defined(ARDUINO_ARCH_ESP8266)
bool BME280BRZO_I2C::begin
(
  int SDA,
  int SCL
)
{
  // allow config of pins
  brzo_i2c_setup(SDA,SCL,I2C_ACK_TIMEOUT);
  return BME280::Initialize();
}


/****************************************************************/
bool BME280BRZO_I2C::begin()
{
  return Initialize();
}
#endif


/****************************************************************/
bool BME280BRZO_I2C::Initialize()
{
  brzo_i2c_setup(D2,D1,I2C_ACK_TIMEOUT);
  return BME280::Initialize();
}
