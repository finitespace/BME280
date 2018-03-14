/*
BME280I2CSoftwareWireI2C.cpp
This code records data from the BME280I2CSoftwareWire sensor and provides an API.
This file is part of the Arduino BME280I2CSoftwareWire library.
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
Last Updated: Jan 1 2016. - Happy New year!

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the BME280I2CSoftwareWire environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include "config.h"

#ifdef USING_SOFTWARE_WIRE

#include "SoftwareWire.h"

#include "BME280I2CSoftwareWire.h"


/****************************************************************/
BME280I2CSoftwareWire::BME280I2CSoftwareWire
(
  SoftwareWire& softwareWire,
  const Settings& settings
):BME280I2C(settings),
  m_softwareWire(softwareWire)
{
}

/****************************************************************/
bool BME280I2CSoftwareWire::WriteRegister
(
  uint8_t addr,
  uint8_t data
)
{
  m_softwareWire.beginTransmission(getSettings().bme280Addr);
  m_softwareWire.write(addr);
  m_softwareWire.write(data);
  m_softwareWire.endTransmission();

  return true; // TODO: Chech return values from wire calls.
}


/****************************************************************/
bool BME280I2CSoftwareWire::ReadRegister
(
  uint8_t addr,
  uint8_t data[],
  uint8_t length
)
{
  uint8_t ord(0);

  m_softwareWire.beginTransmission(getSettings().bme280Addr);
  m_softwareWire.write(addr);
  m_softwareWire.endTransmission();

  m_softwareWire.requestFrom(static_cast<uint8_t>(getSettings().bme280Addr), length);

  while(m_softwareWire.available())
  {
    data[ord++] = m_softwareWire.read();
  }

  return ord == length;
}

#endif
