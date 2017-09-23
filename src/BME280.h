/*

BME280.h

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
This code is licensed under the GNU LGPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

 */

#ifndef TG_BME_280_H
#define TG_BME_280_H

#include "Arduino.h"


//////////////////////////////////////////////////////////////////
/// BME280 - Driver class for Bosch Bme280 sensor
/// 
/// Based on the data sheet provided by Bosch for 
/// the Bme280 environmental sensor.
/// 
class BME280
{
public:
/*****************************************************************/
/* INIT FUNCTIONS                                                */
/*****************************************************************/

  /////////////////////////////////////////////////////////////////
  /// Constructor used to create the class.
  /// All parameters have default values.
  BME280(
    uint8_t tempOversamplingRate        = 0x1,    // temp oversampling = 1
    uint8_t humidityOversamplingRate    = 0x1,    // hum oversampling = 1
    uint8_t pressureOversamplingRate    = 0x1,    // pres oversampling = 1
    uint8_t mode                        = 0x1,    // mode = forced
    uint8_t standbyTime                 = 0x5,    // standby time = 1000ms
    uint8_t filter                      = 0x0,    // filter = none.
    bool spiEnable                      = false);

  /////////////////////////////////////////////////////////////////
  void setMode(
    uint8_t mode);

/*****************************************************************/
/* ENVIRONMENTAL FUNCTIONS                                       */
/*****************************************************************/

  //////////////////////////////////////////////////
  /// Read the temperature from the BME280 and return a float.
  float temp(
    bool celsius = true);

  /////////////////////////////////////////////////////////////////
  /// Read the pressure from the BME280 and return a float with the 
  /// specified unit. 
  float pres(
    /// x00 = Pa, x01 = hPa, x02 = Hg, x03 = atm, 
    /// x04 = bar, x05 = torr,  x06 = N/m^2, x07 = psi
    uint8_t unit = 0x0);   

  /////////////////////////////////////////////////////////////////
  /// Read the humidity from the BME280 and return a percentage
  /// as a float.
  float hum();

  /////////////////////////////////////////////////////////////////
  /// Read the data from the BME280 in the specified unit.
  void  read(
    float&  pressure,
    float&  temperature,
    float&  humidity,
    bool    metric        = true, 
    uint8_t p_unit        = 0x0);


/*****************************************************************/
/* CALULATION FUNCTIONS                                          */
/*****************************************************************/

  /////////////////////////////////////////////////////////////////
  /// Read the data from the BME280 with the specified units and
  /// then calculate the altitude. 
  float alt(
    bool metric = true,
    float seaLevelPressure = 101325);

  /////////////////////////////////////////////////////////////////
  /// Calculate the altitude based on the pressure with the
  /// specified units.
  float alt(
    float pressure,
    bool metric = true,
    float seaLevelPressure = 101325); // Pressure given in Pa.

  /////////////////////////////////////////////////////////////////
  /// Convert current pressure to sea-level pressure, returns
  /// sealevel pressure in mbar
  float sealevel(
    float A);  // A: current altitude (meters).

  /////////////////////////////////////////////////////////////////
  /// Read BME280 data and calculate the dew point with the
  /// specified units.
  float dew(
    bool metric = true);

  /////////////////////////////////////////////////////////////////
  /// Calculate the dew point based on the temperature and
  /// humidity with the specified units.
  float dew(
    float temp,
    float hum,
    bool metric = true);

/*****************************************************************/
/* INITIATION FUNCTIONS                                       */
/*****************************************************************/

  /////////////////////////////////////////////////////////////////
  /// Method used to initialize the class.
  bool begin();

  ////////////////////////////////////////////////////////////////
  /// Method used to return CHIP_ID.
  uint8_t chipID();
  
protected:

/*****************************************************************/
/* CONSTRUCTOR INIT FUNCTIONS                                    */
/*****************************************************************/

  //////////////////////////////////////////////////////////////////
  /// Write configuration to BME280, return true if successful.
  virtual bool Initialize();

/*****************************************************************/
/* WORKER FUNCTIONS                                              */
/*****************************************************************/

  /////////////////////////////////////////////////////////////////
  /// Read the the trim data from the BME280, return true if 
  /// successful.
  bool ReadTrim();

  /////////////////////////////////////////////////////////////////
  /// Read the raw data from the BME280 into an array and return
  /// true if successful. 
  bool ReadData(
    int32_t data[8]);

  /////////////////////////////////////////////////////////////////
  /// Calculate the temperature from the BME280 raw data and
  /// BME280 trim, return a float.
  float CalculateTemperature(
    int32_t raw,
    int32_t& t_fine,
    bool celsius = true);

  /////////////////////////////////////////////////////////////////
  /// Calculate the humidity from the BME280 raw data and BME280
  /// trim, return a float.
  float CalculateHumidity(
    int32_t raw,
    int32_t t_fine);

  /////////////////////////////////////////////////////////////////
  /// Calculate the pressure from the BME280 raw data and BME280
  /// trim, return a float.
  float CalculatePressure(
    int32_t raw, 
    int32_t t_fine,
    /// x00 = Pa, x01 = hPa, x02 = Hg, x03 = atm, 
    /// x04 = bar, x05 = torr,  x06 = N/m^2, x07 = psi
    uint8_t unit = 0x0);


/*****************************************************************/
/* ACCESS FUNCTIONS                                              */
/*****************************************************************/

//uint8_t&  getMode();
//uint8_t*  getDig();
//uint8_t&  getControlHumidity();
//uint8_t&  getControlMeasure();
//uint8_t&  getConfig();


private:

/*****************************************************************/
/* CONSTANTS                                                     */
/*****************************************************************/

  static const uint8_t CTRL_HUM_ADDR   = 0xF2;
  static const uint8_t CTRL_MEAS_ADDR  = 0xF4;
  static const uint8_t CONFIG_ADDR     = 0xF5;
  static const uint8_t PRESS_ADDR      = 0xF7;
  static const uint8_t TEMP_ADDR       = 0xFA;
  static const uint8_t HUM_ADDR        = 0xFD;
  static const uint8_t TEMP_DIG_ADDR   = 0x88;
  static const uint8_t PRESS_DIG_ADDR  = 0x8E;
  static const uint8_t HUM_DIG_ADDR1   = 0xA1;
  static const uint8_t HUM_DIG_ADDR2   = 0xE1;
  static const uint8_t ID_ADDR         = 0xD0;
  static const uint8_t BME_ID          = 0x60;
  static const uint8_t BMP_ID          = 0x58;

  static const uint8_t TEMP_DIG_LENGTH         = 6;
  static const uint8_t PRESS_DIG_LENGTH        = 18;
  static const uint8_t HUM_DIG_ADDR1_LENGTH    = 1;
  static const uint8_t HUM_DIG_ADDR2_LENGTH    = 7;
  static const uint8_t DIG_LENGTH              = 32;
  static const uint8_t SENSOR_DATA_LENGTH      = 8;

/*****************************************************************/
/* VARIABLES                                                     */
/*****************************************************************/

  uint8_t tempOversamplingRate;               // B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16
  uint8_t humidityOversamplingRate;           // B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16
  uint8_t pressureOversamplingRate;           // B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16
  uint8_t mode;                               // Sleep = B00, Normal = B01 and B10, Forced = B11
  uint8_t standbyTime;                        // B000 = 0.5ms, B001 = 62.5ms, B010 = 125ms, B011 = 250ms, B100 = 250ms, B101 = 1000ms, B110 = 10ms, B111 = 20ms
  uint8_t filter;                             // B000 = off, B001 = 2, B010 = 4, B011 = 8, B100/other = 16
  uint8_t controlHumidity;                    // ctrl_hum register. (ctrl_hum[2:0] = Humidity oversampling rate.)
  uint8_t controlMeasure;                     // ctrl_meas register. (ctrl_meas[7:5] = temperature oversampling rate, ctrl_meas[4:2] = pressure oversampling rate, ctrl_meas[1:0] = mode.)
  uint8_t config;                             // config register. (config[7:5] = standby time, config[4:2] = filter, ctrl_meas[0] = spi enable.)
  uint8_t dig[32];
  uint8_t chip_id;

  bool spiEnable;

  bool initialized;

/*****************************************************************/
/* ABSTRACT FUNCTIONS                                            */
/*****************************************************************/

  /////////////////////////////////////////////////////////////////
  /// Write values to BME280 registers.
  virtual bool WriteRegister(
    uint8_t addr,
    uint8_t data)=0;

  /////////////////////////////////////////////////////////////////
  /// Read values from BME280 registers.
  virtual bool ReadRegister(
    uint8_t addr,
    uint8_t data[],
    uint8_t length)=0;

};

#endif // TG_BME_280_H
