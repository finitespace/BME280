/*
BME280SpiI2C.cpp
This code records data from the BME280Spi sensor and provides an API.
This file is part of the Arduino BME280Spi library.
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

Based on the data sheet provided by Bosch for the BME280Spi environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */


/* ==== Includes ==== */
#include <SPI.h>
#include "BME280Spi.h"
/* ====  END Includes ==== */

/* ==== Methods ==== */

bool BME280Spi::Initialize() {
  WriteRegister(CTRL_HUM_ADDR, controlHumidity);
  WriteRegister(CTRL_MEAS_ADDR, controlMeasure);
  WriteRegister(CONFIG_ADDR, config);
  return ReadTrim();
}


bool BME280Spi::ReadAddr(uint8_t addr, uint8_t array[], uint8_t len) {

  // bme280 uses the msb to select read and write
  // combine the addr with the read/write bit
  uint8_t readAddr = addr & BME280_SPI_READ;

  //select the device
  digitalWrite(chipSelectPin, LOW);

  // transfer the addr
  SPI.transfer(readAddr);

  // read the data
  for(int i = 0; i < len; ++i)
  {
    // transfer 0x00 to get the data
    array[i] = SPI.transfer(0x00);
  }

  // de-select the device
  digitalWrite(chipSelectPin, HIGH);

  return true;
}

void BME280Spi::WriteRegister(uint8_t addr, uint8_t data)
{
  // bme280 uses the msb to select read and write
  // combine the addr with the read/write bit
  uint8_t writeAddr = addr | BME280_SPI_WRITE;

  // select the device
  digitalWrite(chipSelectPin, LOW);

  // transfer the addr and then the data to spi device
  SPI.transfer(writeAddr);
  SPI.transfer(data);

  // de-select the device
  digitalWrite(chipSelectPin, HIGH);
}

bool BME280Spi::ReadTrim()
{
  uint8_t ord(0);

  // Temp. Dig
  if(!ReadAddr(TEMP_DIG_ADDR, &dig[ord], (uint8_t)6))
  {
    return false;
  }
  ord += 6;


  // Pressure Dig
  if(!ReadAddr(PRESS_DIG_ADDR, &dig[ord], (uint8_t)18))
  {
    return false;
  }
  ord += 18;


  // Humidity Dig 1
  if(!ReadAddr(HUM_DIG_ADDR1, &dig[ord], (uint8_t)1))
  {
    return false;
  }
  ord += 1;


  // Humidity Dig 2
  if(!ReadAddr(HUM_DIG_ADDR2, &dig[ord], (uint8_t)7))
  {
    return false;
  }
  ord += 7;

  // should always return true
  return ord == 32;
}

bool BME280Spi::ReadData(int32_t data[8]){

  // read from the pressure addr out 8 bytes
  return ReadAddr(PRESS_ADDR, data, (uint8_t)8);

}


BME280Spi::BME280Spi(uint8_t tosr, uint8_t hosr, uint8_t posr, uint8_t mode, uint8_t st, uint8_t filter, uint8_t spiChipSelectPin):
    BME280(tosr, hosr, posr, mode, st, filter, true), chipSelectPin(spiChipSelectPin)
{
  pinMode(chipSelectPin, OUTPUT);

  // ctrl_hum register. (ctrl_hum[2:0] = Humidity oversampling rate.)
  controlHumidity = humidityOversamplingRate;
  // ctrl_meas register. (ctrl_meas[7:5] = temperature oversampling rate, ctrl_meas[4:2] = pressure oversampling rate, ctrl_meas[1:0] = mode.)
  controlMeasure = (tempOversamplingRate << 5) | (pressureOversamplingRate << 2) | mode;
  // config register. (config[7:5] = standby time, config[4:2] = filter, ctrl_meas[0] = spi enable.)
  config = (standbyTime << 5) | (filter << 2) | spiEnable;
}

bool BME280Spi::begin(){
  SPI.begin();
  return Initialize();
}

/* ==== END Methods ==== */
