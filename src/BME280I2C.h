/*
BME280I2C.h
This code records data from the BME280 sensor and provides an API.
This file is part of the Arduino BME280 library.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.


Copyright (c) 2015-2021 Tyler Glenn, et al.

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

#ifndef TG_BME_280_I2C_H
#define TG_BME_280_I2C_H

#include "BME280.h"

//////////////////////////////////////////////////////////////////
/// BME280I2C - I2C Implementation of BME280.
class BME280I2C: public BME280
{

public:

   enum I2CAddr
   {
      I2CAddr_0x76 = 0x76,
      I2CAddr_0x77 = 0x77
   };


   struct Settings : public BME280::Settings
   {
      Settings(
         OSR _tosr       = OSR_X1,
         OSR _hosr       = OSR_X1,
         OSR _posr       = OSR_X1,
         Mode _mode      = Mode_Forced,
         StandbyTime _st = StandbyTime_1000ms,
         Filter _filter  = Filter_16,
         SpiEnable _se   = SpiEnable_False,
         I2CAddr _addr   = I2CAddr_0x76
        ): BME280::Settings(_tosr, _hosr, _posr, _mode, _st, _filter, _se),
           bme280Addr(_addr) {}

      I2CAddr bme280Addr;
   };

   ///////////////////////////////////////////////////////////////
   /// Constructor used to create the class. All parameters have
   /// default values.
   BME280I2C(
      const Settings& settings = Settings());


/*****************************************************************/
/* ACCESSOR FUNCTIONS                                            */
/*****************************************************************/

   /////////////////////////////////////////////////////////////////
   virtual void setSettings(
      const Settings& settings);

   /////////////////////////////////////////////////////////////////
   const Settings& getSettings() const;


protected:

private:

   Settings m_settings;

   //////////////////////////////////////////////////////////////////
   /// Write values to BME280 registers.
   virtual bool WriteRegister(
      uint8_t addr,
      uint8_t data);

   /////////////////////////////////////////////////////////////////
   /// Read values from BME280 registers.
   virtual bool ReadRegister(
      uint8_t addr,
      uint8_t data[],
      uint8_t length);

};
#endif // TG_BME_280_I2C_H
