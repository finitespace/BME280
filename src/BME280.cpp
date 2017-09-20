/*
BME280.cpp
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

Written: Dec 30 2015.
Last Updated: Jan 1 2016. - Happy New year!

This header must be included in any derived code or copies of the code.

Based on the data sheet provided by Bosch for the Bme280 environmental sensor,
calibration code based on algorithms providedBosch, some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include <Wire.h>

#include "BME280.h"

/****************************************************************/
BME280::BME280
(
  uint8_t tosr,
  uint8_t hosr,
  uint8_t posr,
  uint8_t mode,
  uint8_t st,
  uint8_t filter,
  bool spiEnable
):tempOversamplingRate(tosr),
  humidityOversamplingRate(hosr),
  pressureOversamplingRate(posr),
  mode(mode),
  standbyTime(st),
  filter(filter),
  spiEnable(spiEnable),
  initialized(false)
{
  // ctrl_hum register. (ctrl_hum[2:0] = Humidity oversampling rate.)
  controlHumidity = humidityOversamplingRate;
  // ctrl_meas register. (ctrl_meas[7:5] = temperature oversampling rate, ctrl_meas[4:2] = pressure oversampling rate, ctrl_meas[1:0] = mode.)
  controlMeasure = (tempOversamplingRate << 5) | (pressureOversamplingRate << 2) | mode;
  // config register. (config[7:5] = standby time, config[4:2] = filter, ctrl_meas[0] = spi enable.)
  config = (standbyTime << 5) | (filter << 2) | spiEnable;
}


/****************************************************************/
bool BME280::Initialize()
{
  uint8_t id[1];
  ReadRegister(ID_ADDR, &id[0], 1);

  if (id[0] != BME_ID && id[0] != BMP_ID)
  {
      return false;
  }
  chip_id = id[0];

  WriteRegister(CTRL_HUM_ADDR, controlHumidity);
  WriteRegister(CTRL_MEAS_ADDR, controlMeasure);
  WriteRegister(CONFIG_ADDR, config);

  initialized = true;
  return ReadTrim();
}


/****************************************************************/
void BME280::setMode
(
  uint8_t mode
)
{
  controlMeasure = controlMeasure | mode;
  WriteRegister(CTRL_MEAS_ADDR, controlMeasure);
}


/****************************************************************/
bool BME280::begin
(
)
{
  bool success = Initialize();
  success &= initialized;

  return success;
}


/****************************************************************/
bool BME280::ReadTrim()
{
   uint8_t ord(0);
   bool success = true;

  // Temp. Dig
  success &= ReadRegister(TEMP_DIG_ADDR, &dig[ord], TEMP_DIG_LENGTH);
  ord += TEMP_DIG_LENGTH;

  // Pressure Dig
  success &= ReadRegister(PRESS_DIG_ADDR, &dig[ord], PRESS_DIG_LENGTH);
  ord += PRESS_DIG_LENGTH;

  // Humidity Dig 1
  success &= ReadRegister(HUM_DIG_ADDR1, &dig[ord], HUM_DIG_ADDR1_LENGTH);
  ord += HUM_DIG_ADDR1_LENGTH;

  // Humidity Dig 2
  success &= ReadRegister(HUM_DIG_ADDR2, &dig[ord], HUM_DIG_ADDR2_LENGTH);
  ord += HUM_DIG_ADDR2_LENGTH;

#ifdef DEBUG_ON
  Serial.print("Dig: ");
  for(int i = 0; i < 32; ++i)
  {
    Serial.print(dig[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif

  return success && ord == DIG_LENGTH;
}


/****************************************************************/
bool BME280::ReadData
(
  int32_t data[SENSOR_DATA_LENGTH]
)
{
  bool success;
  uint8_t buffer[SENSOR_DATA_LENGTH];

  // for forced mode we need to write the mode to BME280 register before reading
  if ( (mode == 0x01) || (mode == 0x10) )
    setMode(mode);

  // Registers are in order. So we can start at the pressure register and read 8 bytes.
  success = ReadRegister(PRESS_ADDR, buffer, SENSOR_DATA_LENGTH);

  for(int i = 0; i < SENSOR_DATA_LENGTH; ++i)
  {
    data[i] = static_cast<int32_t>(buffer[i]);
  }

#ifdef DEBUG_ON
  Serial.print("Data: ");
  for(int i = 0; i < 8; ++i)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif

  return success;
}


/****************************************************************/
float BME280::CalculateTemperature
(
  int32_t raw,
  int32_t& t_fine,
  bool celsius
)
{
  // Code based on calibration algorthim provided by Bosch.
  int32_t var1, var2, final;
  uint16_t dig_T1 = (dig[1] << 8) | dig[0];
  int16_t  dig_T2 = (dig[3] << 8) | dig[2];
  int16_t  dig_T3 = (dig[5] << 8) | dig[4];
  var1 = ((((raw >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2 = (((((raw >> 4) - ((int32_t)dig_T1)) * ((raw >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
  t_fine = var1 + var2;
  final = (t_fine * 5 + 128) >> 8;
  return celsius ? final/100.0 : final/100.0*9.0/5.0 + 32.0;
}


/****************************************************************/
float BME280::CalculateHumidity
(
  int32_t raw,
  int32_t t_fine
)
{
  // Code based on calibration algorthim provided by Bosch.
  int32_t var1;
  uint8_t  dig_H1 =  dig[24];
  int16_t dig_H2 = (dig[26] << 8) | dig[25];
  uint8_t  dig_H3 =  dig[27];
  int16_t dig_H4 = (dig[28] << 4) | (0x0F & dig[29]);
  int16_t dig_H5 = (dig[30] << 4) | ((dig[29] >> 4) & 0x0F);
  int8_t  dig_H6 =  dig[31];

  var1 = (t_fine - ((int32_t)76800));
  var1 = (((((raw << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) +
  ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)dig_H6)) >> 10) * (((var1 *
  ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
  ((int32_t)dig_H2) + 8192) >> 14));
  var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
  var1 = (var1 < 0 ? 0 : var1);
  var1 = (var1 > 419430400 ? 419430400 : var1);
  return ((uint32_t)(var1 >> 12))/1024.0;
}


/****************************************************************/
float BME280::CalculatePressure
(
  int32_t raw,
  int32_t t_fine,
  uint8_t unit
)
{
  // Code based on calibration algorthim provided by Bosch.
  int64_t var1, var2, pressure;
  float final;

  uint16_t dig_P1 = (dig[7]  << 8) | dig[6];
  int16_t  dig_P2 = (dig[9]  << 8) | dig[8];
  int16_t  dig_P3 = (dig[11] << 8) | dig[10];
  int16_t  dig_P4 = (dig[13] << 8) | dig[12];
  int16_t  dig_P5 = (dig[15] << 8) | dig[14];
  int16_t  dig_P6 = (dig[17] << 8) | dig[16];
  int16_t  dig_P7 = (dig[19] << 8) | dig[18];
  int16_t  dig_P8 = (dig[21] << 8) | dig[20];
  int16_t  dig_P9 = (dig[23] << 8) | dig[22];

  var1 = (int64_t)t_fine - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
  var2 = var2 + (((int64_t)dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
  if (var1 == 0) { return NAN; }                                      // Don't divide by zero.
  pressure  = 1048576 - raw;
  pressure = (((pressure << 31) - var2) * 3125)/var1;
  var1 = (((int64_t)dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
  var2 = (((int64_t)dig_P8) * pressure) >> 19;
  pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

  final = ((uint32_t)pressure)/256.0;

  // Conversion units courtesy of www.endmemo.com.
  switch(unit){
    case 0x1: /* hPa */
      final /= 100.0;
      break;
    case 0x2: /* inHg */
      final /= 3386.3752577878;       /* final pa * 1inHg/3386.3752577878Pa */
      break;
    case 0x3: /* atm */
      final /= 101324.99766353; /* final pa * 1 atm/101324.99766353Pa */
      break;
    case 0x4: /* bar */
      final /= 100000.0;          /* final pa * 1 bar/100kPa */
      break;
    case 0x5: /* torr */
      final /= 133.32236534674;        /* final pa * 1 torr/133.32236534674Pa */
      break;
    case 0x6: /* N/m^2 */
      break;                      /* 1Pa / N/m^2 */
    case 0x7: /* psi */
      final /= 6894.744825494;  /* final pa * 1psi/6894.744825494Pa */
      break;
    default: /* Pa (case: 0) */
      break;
  }
  return final;
}


/****************************************************************/
float BME280::temp
(
  bool celsius
)
{
  int32_t data[8];
  int32_t t_fine;
  if(!ReadData(data)){ return NAN; }
  uint32_t rawTemp  = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
  return CalculateTemperature(rawTemp, t_fine, celsius);
}


/****************************************************************/
float BME280::pres
(
  uint8_t unit
)
{
  int32_t data[8];
  int32_t t_fine;
  if(!ReadData(data)){ return NAN; }
  uint32_t rawTemp     = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
  uint32_t rawPressure = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
  CalculateTemperature(rawTemp, t_fine);
  return CalculatePressure(rawPressure, t_fine, unit);
}


/****************************************************************/
float BME280::hum()
{
  int32_t data[8];
  int32_t t_fine;
  if(!ReadData(data)){ return NAN; }
  uint32_t rawTemp = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
  uint32_t rawHumidity = (data[6] << 8) | data[7];
  CalculateTemperature(rawTemp, t_fine);
  return CalculateHumidity(rawHumidity, t_fine);
}


/****************************************************************/
void BME280::read
(
  float& pressure,
  float& temp,
  float& humidity,
  bool metric,
  uint8_t p_unit
)
{
  int32_t data[8];
  int32_t t_fine;
  if(!ReadData(data)){
    pressure = temp = humidity = NAN;
    return;
  }
  uint32_t rawPressure = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
  uint32_t rawTemp = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
  uint32_t rawHumidity = (data[6] << 8) | data[7];
  temp = CalculateTemperature(rawTemp, t_fine, metric);
  pressure = CalculatePressure(rawPressure, t_fine, p_unit);
  humidity = CalculateHumidity(rawHumidity, t_fine);
}


/****************************************************************/
float BME280::alt
(
  bool metric,
  float seaLevelPressure
)
{
  float temp, hum, pres;
  read(pres, temp, hum, metric);
  return alt(pres, metric, seaLevelPressure);
}


/****************************************************************/
float BME280::alt
(
  float pressure,
  bool metric,
  float seaLevelPressure
)
{
  // Equations courtesy of NOAA;
  float altitude = NAN;
  if (!isnan(pressure) && !isnan(seaLevelPressure)){
    altitude = 1000.0 * ( seaLevelPressure - pressure ) / 3386.3752577878;
  }
  return metric ? altitude * 0.3048 : altitude;
}


/****************************************************************/
float BME280::sealevel
(
  float A // Altitude A (in meters), temperature T in Celsius, pressure P in mb, return the equivalent pressure (in mb) at sea level.
)
{
	float T(NAN), P(NAN);
	T = temp(true);
	P = pres(1);

	return(P / pow(1-((0.0065 *A) / (T + (0.0065 *A) + 273.15)),5.257));
}


/****************************************************************/
float BME280::dew
(
  bool metric
)
{
  float temp, hum, pres;
  read(pres, temp, hum, metric);
  return dew(temp, hum, metric);
}


/****************************************************************/
float BME280::dew
(
  float temp,
  float hum,
  bool metric
)
{
  // Equations courtesy of Brian McNoldy from http://andrew.rsmas.miami.edu;
  float dewPoint = NAN;
  if (metric && !isnan(temp) && !isnan(hum)){
    dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * temp)/(243.04 + temp)))
    /(17.625 - log(hum/100.0) - ((17.625 * temp)/(243.04 + temp)));
  }else if (!isnan(temp) && !isnan(hum)){
    float ctemp = (temp - 32.0) * 5.0/9.0;
    dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * ctemp)/(243.04 + ctemp)))
    /(17.625 - log(hum/100.0) - ((17.625 * ctemp)/(243.04 + ctemp)));
    dewPoint = dewPoint * 9.0/5.0 + 32.0;
  }
  return dewPoint;
}

/****************************************************************/
uint8_t BME280::chipID
(
)
{
  return chip_id;
}
