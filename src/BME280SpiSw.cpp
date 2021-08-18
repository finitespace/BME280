/*
BME280SpiSw.cpp
This code records data from the BME280SpiSw sensor and provides an API.
This file is part of the Arduino BME280SpiSw library.

Based on the data sheet provided by Bosch for the BME280SpiSw environmental sensor,
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

