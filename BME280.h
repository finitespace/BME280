/*
BME280.h
This code records data from the BME280 sensor and provides an api.
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

Written: Dec 30 2015.
Last Updated: Jan 1 2016.
This code is licensed under the GNU LGPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor.
 */
#ifndef TG_BME_280_H
#define TG_BME_280_H

/* ==== Defines ==== */

#define CTRL_HUM_ADDR  0xF2
#define CTRL_MEAS_ADDR 0xF4
#define CONFIG_ADDR    0xF5
#define PRESS_ADDR     0xF7
#define TEMP_ADDR      0xFA
#define HUM_ADDR       0xFD
#define TEMP_DIG_ADDR  0x88
#define PRESS_DIG_ADDR 0x8E
#define HUM_DIG_ADDR1  0xA1
#define HUM_DIG_ADDR2  0xE1
/* ==== END Defines ==== */


class BME280{
protected:
  uint8_t tempOversamplingRate, humidityOversamplingRate, pressureOversamplingRate; // B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16
  uint8_t mode;                               // Sleep = B00, Normal = B01 and B10, Forced = B11
  uint8_t standbyTime;                        // B000 = 0.5ms, B001 = 62.5ms, B010 = 125ms, B011 = 250ms, B100 = 250ms, B101 = 1000ms, B110 = 10ms, B111 = 20ms
  uint8_t filter;                             // B000 = off, B001 = 2, B010 = 4, B011 = 8, B100/other = 16
  uint8_t controlHumidity;                    // ctrl_hum register. (ctrl_hum[2:0] = Humidity oversampling rate.)
  uint8_t controlMeasure;                     // ctrl_meas register. (ctrl_meas[7:5] = temperature oversampling rate, ctrl_meas[4:2] = pressure oversampling rate, ctrl_meas[1:0] = mode.)
  uint8_t config;                             // config register. (config[7:5] = standby time, config[4:2] = filter, ctrl_meas[0] = spi enable.)
  uint8_t bme_280_addr;
  uint8_t dig[32];
  bool spiEnable;

  void WriteRegister(uint8_t addr, uint8_t data);

  bool ReadTrim();
  bool ReadData(int32_t data[8]);

  float CalculateTemperature(int32_t raw, int32_t& t_fine, bool celsius = true);
  float CalculateHumidity(int32_t raw, int32_t t_fine);
  float CalculatePressure(int32_t raw, int32_t t_fine, uint8_t unit = 0x0);   // unit: B000/other = Pa, B001 = hPa, B010 = inHg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi

public:
  BME280(uint8_t tosr = 0x1, uint8_t hosr = 0x1, uint8_t posr = 0x1, uint8_t mode = 0x3,
    uint8_t st = 0x5, uint8_t filter = 0x0, bool spiEnable = false,
    uint8_t bme_280_addr = 0x76);  // Oversampling = 2, mode = normal, standby time = 125ms, filter = none.
  bool  begin();
  float ReadTemperature(bool celsius = false);
  float ReadPressure(uint8_t unit = 0x0);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
  float ReadHumidity();
  void  ReadData(float& pressure, float& temp, float& humidity, bool metric = true, uint8_t p_unit = 0x0);


  float CalculateAltitude(bool metric = true, float seaLevelPressure = 101325);
  float CalculateAltitude(float pressure, bool metric = true, float seaLevelPressure = 101325); // Pressure given in Pa.

  float CalculateDewPoint(bool metric = true);
  float CalculateDewPoint(float temp, float hum, bool metric = true);

};
#endif // TG_BME_280_H
