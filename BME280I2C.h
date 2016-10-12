/*
BME280I2C.h
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

Written: Sep 19 2016.
Last Updated: Sep 19 2016. - Happy Fall! <5
This code is licensed under the GNU LGPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.
 */
#ifndef TG_BME_280_I2C_H
#define TG_BME_280_I2C_H

/* ==== Includes ==== */
#include "BME280.h"
/* ==== END Includes ==== */

/* ==== Defines ==== */

/* ==== END Defines ==== */


class BME280I2C: public BME280{
  uint8_t bme_280_addr;

  /* ==== Write configuration to BME280, return true if successful. ==== */
  bool Initialize();

  /* ==== Write values to BME280 registers. ==== */
  virtual void WriteRegister(uint8_t addr, uint8_t data);

  /* ==== Read the the trim data from the BME280, return true if successful. ==== */
  virtual bool ReadTrim();

  /* ==== Read the raw data from the BME280 into an array and return true if successful. ==== */
  virtual bool ReadData(int32_t data[8]);

public:
  /* ==== Constructor used to create the class. All parameters have default values. ==== */
  BME280I2C(uint8_t tosr = 0x1, uint8_t hosr = 0x1, uint8_t posr = 0x1, uint8_t mode = 0x3,
    uint8_t st = 0x5, uint8_t filter = 0x0, bool spiEnable = false,
    uint8_t bme_280_addr = 0x76);  // Oversampling = 1, mode = normal, standby time = 125ms, filter = none.

  /* ==== Method used at start up to initialize the class. Starts the I2C interface. ==== */
  virtual bool begin();
#if defined(ARDUINO_ARCH_ESP8266)
  /* ==== On esp8266 it is possible to define I2C pins ==== */
  bool begin(int SDA, int SCL);
#endif

};
#endif // TG_BME_280_I2C_H
