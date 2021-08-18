/*
BME280Spi.cpp
This code records data from the BME280Spi sensor and provides an API.
This file is part of the Arduino BME280Spi library.

Based on the data sheet provided by Bosch for the BME280Spi environmental sensor,
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
