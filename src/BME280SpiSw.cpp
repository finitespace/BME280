/*
BME280SpiSw.cpp
This code records data from the BME280SpiSw sensor and provides an API.
This file is part of the Arduino BME280SpiSw library.
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
Last Updated: Dec 18 2016. - Happy Holidays!

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the BME280SpiSw environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include "Arduino.h"
#include "BME280SpiSw.h"



/****************************************************************/
BME280SpiSw::BME280SpiSw
(
   const Settings& settings
)
:BME280(settings),
 m_settings(settings)
{
}


/****************************************************************/
bool BME280SpiSw::Initialize(){

   digitalWrite(m_settings.spiCsPin, HIGH);
   pinMode(m_settings.spiCsPin, OUTPUT);

   pinMode(m_settings.spiSckPin, OUTPUT);
   pinMode(m_settings.spiMosiPin, OUTPUT);
   pinMode(m_settings.spiMisoPin, INPUT);

   return BME280::Initialize();
}


/****************************************************************/
void BME280SpiSw::setSettings
(
   const Settings& settings
)
{
   m_settings = settings;
   BME280::setSettings(settings);
}


/****************************************************************/
const BME280SpiSw::Settings& BME280SpiSw::getSettings() const
{
   return m_settings;
}


/****************************************************************/
uint8_t BME280SpiSw::SpiTransferSw
(
   uint8_t data
)
{
   uint8_t resp = 0;
   for (int bit = 7; bit >= 0; --bit) {
      resp <<= 1;
      digitalWrite(m_settings.spiSckPin, LOW);
      digitalWrite(m_settings.spiMosiPin, data & (1 << bit));
      digitalWrite(m_settings.spiSckPin, HIGH);
      resp |= digitalRead(m_settings.spiMisoPin);
   }
   return resp;
}


/****************************************************************/
bool BME280SpiSw::ReadRegister
(
   uint8_t addr,
   uint8_t data[],
   uint8_t length
)
{

   // bme280 uses the msb to select read and write
   // combine the addr with the read/write bit
   uint8_t readAddr = addr |   BME280_SPI_READ;

   //select the device
   digitalWrite(m_settings.spiCsPin, LOW);
   // transfer the addr
   SpiTransferSw(readAddr);

   // read the data
   for(int i = 0; i < length; ++i)
   {
      // transfer 0x00 to get the data
      data[i] = SpiTransferSw(0);
   }

   // de-select the device
   digitalWrite(m_settings.spiCsPin, HIGH);

   return true;
}


/****************************************************************/
bool BME280SpiSw::WriteRegister
(
   uint8_t addr,
   uint8_t data
)
{
   // bme280 uses the msb to select read and write
   // combine the addr with the read/write bit
   uint8_t writeAddr = addr & ~0x80;

   // select the device
   digitalWrite(m_settings.spiCsPin, LOW);

   // transfer the addr and then the data to spi device
   SpiTransferSw(writeAddr);
   SpiTransferSw(data);

   // de-select the device
   digitalWrite(m_settings.spiCsPin, HIGH);

return true;
}

