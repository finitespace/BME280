/*
BME280SpiSw.h
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

Written: Dec 18 2016. - Happy Holidays!
Last Updated: Dec 18 2016. - Happy Holidays!

This code is licensed under the GNU LGPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.
 */
#ifndef TG_BME_280_SPI_H
#define TG_BME_280_SPI_H

/* ==== Includes ==== */
#include "BME280.h"
/* ==== END Includes ==== */

/* ==== Defines ==== */
#define BME280_SPI_WRITE 0x7F
#define BME280_SPI_READ  0x80

/* ==== END Defines ==== */


class BME280SpiSw: public BME280{
  uint8_t csPin;
  int8_t mosiPin;
  int8_t misoPin;
  int8_t sckPin;

  /* ==== Write configuration to BME280, return true if successful. ==== */
  bool Initialize();

  /* ==== Does a sw spi transfer. ==== */
  uint8_t SpiTransferSw(uint8_t data);

  /* ==== Read the data from the BME280 addr into an array and return true if successful. ==== */
  bool ReadAddr(uint8_t addr, uint8_t array[], uint8_t len);

  /* ==== Write values to BME280 registers. ==== */
  virtual void WriteRegister(uint8_t addr, uint8_t data);

  /* ==== Read the the trim data from the BME280, return true if successful. ==== */
  virtual bool ReadTrim();

  /* ==== Read the raw data from the BME280 into an array and return true if successful. ==== */
  virtual bool ReadData(int32_t data[8]);

public:
  /* ==== Constructor for software spi ==== */
  BME280SpiSw(uint8_t spiCsPin, uint8_t spiMosiPin, uint8_t spiMisoPin, uint8_t spiSckPin, uint8_t tosr = 0x1,
    uint8_t hosr = 0x1, uint8_t posr = 0x1, uint8_t mode = 0x1, uint8_t st = 0x5, uint8_t filter = 0x0);  // Oversampling = 1, mode = forced, standby time = 1000ms, filter = none.

  /* ==== Method used at start up to initialize the class. Starts the I2C interface. ==== */
  virtual bool begin();

};
#endif // TG_BME_280_SPI_H
