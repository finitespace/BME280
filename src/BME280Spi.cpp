/*
BME280Spi.cpp
This code records data from the BME280Spi sensor and provides an API.
This file is part of the Arduino BME280Spi library.
Copyright (C) 2016   Tyler Glenn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.   If not, see <http://www.gnu.org/licenses/>.

Written: Dec 18 2016. - Happy Holidays!
Last Updated: Oct 07 2017.

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the BME280Spi environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include "Arduino.h"
#include "BME280Spi.h"

#include <SPI.h>

/****************************************************************/
BME280Spi::BME280Spi
(
   const Settings& settings
)
:BME280(settings),
 m_settings(settings)
{
}


/****************************************************************/
bool BME280Spi::Initialize()
{
   pinMode(m_settings.spiCsPin, OUTPUT);
   digitalWrite(m_settings.spiCsPin, HIGH);

   return BME280::Initialize();
}

/****************************************************************/
void BME280Spi::setSettings
(
   const Settings& settings
)
{
   m_settings = settings;
   BME280::setSettings(settings);
}


/****************************************************************/
const BME280Spi::Settings& BME280Spi::getSettings() const
{
   return m_settings;
}

/****************************************************************/
bool BME280Spi::ReadRegister
(
   uint8_t addr,
   uint8_t data[],
   uint8_t len
)
{
   SPI.beginTransaction(SPISettings(500000,MSBFIRST,SPI_MODE0));

   // bme280 uses the msb to select read and write
   // combine the addr with the read/write bit
   uint8_t readAddr = addr |   BME280_SPI_READ;

   //select the device
   digitalWrite(m_settings.spiCsPin, LOW);
   // transfer the addr
   SPI.transfer(readAddr);

   // read the data
   for(int i = 0; i < len; ++i)
   {
      // transfer 0x00 to get the data
      data[i] = SPI.transfer(0);
   }

   // de-select the device
   digitalWrite(m_settings.spiCsPin, HIGH);

   SPI.endTransaction();

   return true;
}


/****************************************************************/
bool BME280Spi::WriteRegister
(
   uint8_t addr,
   uint8_t data
)
{
   SPI.beginTransaction(SPISettings(500000,MSBFIRST,SPI_MODE0));

   // bme280 uses the msb to select read and write
   // combine the addr with the read/write bit
   uint8_t writeAddr = addr & ~0x80;

   // select the device
   digitalWrite(m_settings.spiCsPin, LOW);

   // transfer the addr and then the data to spi device
   SPI.transfer(writeAddr);
   SPI.transfer(data);

   // de-select the device
   digitalWrite(m_settings.spiCsPin, HIGH);

   SPI.endTransaction();

   return true;
}


/****************************************************************/
bool BME280Spi::IsConnected(void) {
  // not possible here. Connection state can be discovered by reading the
  // chip ID which is done during initalization
  return true;
}
