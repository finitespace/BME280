/*
BME280I2CI2C.cpp
This code records data from the BME280I2C sensor and provides an API.
This file is part of the Arduino BME280I2C library.
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

Based on the data sheet provided by Bosch for the BME280I2C environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */


/* ==== Includes ==== */
#include <Wire.h>
#include "BME280I2C.h"
/* ====  END Includes ==== */

/* ==== Methods ==== */

bool BME280I2C::Initialize() {
  WriteRegister(CTRL_HUM_ADDR, controlHumidity);
  WriteRegister(CTRL_MEAS_ADDR, controlMeasure);
  WriteRegister(CONFIG_ADDR, config);
  return ReadTrim();
}


void BME280I2C::WriteRegister(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(bme_280_addr);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission();
}

bool BME280I2C::ReadTrim()
{
  uint8_t ord(0);

  // Temp. Dig
  Wire.beginTransmission(bme_280_addr);
  Wire.write(TEMP_DIG_ADDR);
  Wire.endTransmission();

  Wire.requestFrom(bme_280_addr, (uint8_t)6);
  while(Wire.available()){
    dig[ord++] = Wire.read();
  }

  // Pressure Dig
  Wire.beginTransmission(bme_280_addr);
  Wire.write(PRESS_DIG_ADDR);
  Wire.endTransmission();

  Wire.requestFrom(bme_280_addr, (uint8_t)18);
  while(Wire.available()){
    dig[ord++] = Wire.read();
  }

  // Humidity Dig 1
  Wire.beginTransmission(bme_280_addr);
  Wire.write(HUM_DIG_ADDR1);
  Wire.endTransmission();

  Wire.requestFrom(bme_280_addr, (uint8_t)1);
  while(Wire.available()){
    dig[ord++] = Wire.read();
  }

  // Humidity Dig 2
  Wire.beginTransmission(bme_280_addr);
  Wire.write(HUM_DIG_ADDR2);
  Wire.endTransmission();

  Wire.requestFrom(bme_280_addr, (uint8_t)7);
  while(Wire.available()){
    dig[ord++] = Wire.read();
  }
  return ord == 32;
}

bool BME280I2C::ReadData(int32_t data[8]){
  uint8_t ord = 0;

  // Registers are in order. So we can start at the pressure register and read 8 bytes.
  Wire.beginTransmission(bme_280_addr);
  Wire.write(PRESS_ADDR);
  Wire.endTransmission();

  Wire.requestFrom(bme_280_addr, (uint8_t)8);
  while(Wire.available()){
      data[ord++] = Wire.read();
  }
  return ord == 8;
}


BME280I2C::BME280I2C(uint8_t tosr, uint8_t hosr, uint8_t posr, uint8_t mode, uint8_t st, uint8_t filter,
  bool spiEnable, uint8_t bme_280_addr):
    BME280(tosr, hosr, posr, mode, st, filter, spiEnable, bme_280_addr), bme_280_addr(bme_280_addr)
{
  // ctrl_hum register. (ctrl_hum[2:0] = Humidity oversampling rate.)
  controlHumidity = humidityOversamplingRate;
  // ctrl_meas register. (ctrl_meas[7:5] = temperature oversampling rate, ctrl_meas[4:2] = pressure oversampling rate, ctrl_meas[1:0] = mode.)
  controlMeasure = (tempOversamplingRate << 5) | (pressureOversamplingRate << 2) | mode;
  // config register. (config[7:5] = standby time, config[4:2] = filter, ctrl_meas[0] = spi enable.)
  config = (standbyTime << 5) | (filter << 2) | spiEnable;
}

#if defined(ARDUINO_ARCH_ESP8266)
bool BME280I2C::begin(int SDA, int SCL) {
  // allow config of pins
  Wire.begin(SDA,SCL);
  return Initialize();
}
#endif

bool BME280I2C::begin(){
  Wire.begin();
  return Initialize();
}

/* ==== END Methods ==== */
