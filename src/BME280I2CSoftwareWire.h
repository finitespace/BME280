/*
BME280I2CSoftwareWire.h
This code records data from the BME280 sensor and provides an API.
This file is part of the Arduino BME280 library.
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

Written: Mar 13 2018.

This code is licensed under the GNU LGPL and is open for distrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.
 */

#ifndef TG_BME_280_I2C_SOFTWARE_WIRE_H
#define TG_BME_280_I2C_SOFTWARE_WIRE_H

#include "BME280I2C.h"

class SoftwareWire;

//////////////////////////////////////////////////////////////////
/// BME280I2CSoftwareWire - SoftwareWire I2C Implementation of BME280.
class BME280I2CSoftwareWire: public BME280I2C
{

public:

   ///////////////////////////////////////////////////////////////
   /// Constructor used to create the class. All parameters have
   /// default values.
   BME280I2CSoftwareWire(
      SoftwareWire& softwareWire,
      const BME280I2C::Settings& settings = BME280I2C::Settings());


protected:
private:
   SoftwareWire& m_softwareWire;

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
