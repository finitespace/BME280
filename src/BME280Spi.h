/*
BME280Spi.h
This code records data from the BME280 sensor and provides an API.
This file is part of the Arduino BME280 library.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.


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

#ifndef TG_BME_280_SPI_H
#define TG_BME_280_SPI_H

#include "BME280.h"


class BME280Spi: public BME280
{
public:

   struct Settings : public BME280::Settings
   {
      Settings(
         uint8_t _cspin,
         OSR _tosr       = OSR_X1,
         OSR _hosr       = OSR_X1,
         OSR _posr       = OSR_X1,
         Mode _mode      = Mode_Forced,
         StandbyTime _st = StandbyTime_1000ms,
         Filter _filter  = Filter_Off,
         SpiEnable _se   = SpiEnable_False
        ): BME280::Settings(_tosr, _hosr, _posr, _mode, _st, _filter, _se),
           spiCsPin(_cspin) {}

      uint8_t spiCsPin;
   };

   ////////////////////////////////////////////////////////////////
   /// Constructor used to create the class. All parameters have
   /// default values.
   BME280Spi(
      const Settings& settings);

/*****************************************************************/
/* ACCESSOR FUNCTIONS                                            */
/*****************************************************************/

   /////////////////////////////////////////////////////////////////
   virtual void setSettings(
      const Settings& settings);

   /////////////////////////////////////////////////////////////////
   const Settings& getSettings() const;


protected:

   ////////////////////////////////////////////////////////////////
   /// Method used at start up to initialize the class. Starts the
   /// I2C interface.
   virtual bool Initialize();

private:

   static const uint8_t BME280_SPI_WRITE   = 0x7F;
   static const uint8_t BME280_SPI_READ    = 0x80;

   Settings m_settings;

   ////////////////////////////////////////////////////////////////
   /// Read the data from the BME280 addr into an array and
   /// return true if successful.
   virtual bool ReadRegister(
      uint8_t addr,
      uint8_t array[],
      uint8_t len);

   ////////////////////////////////////////////////////////////////
   /// Write values to BME280 registers.
   virtual bool WriteRegister(
      uint8_t addr,
      uint8_t data);

};
#endif // TG_BME_280_SPI_H
