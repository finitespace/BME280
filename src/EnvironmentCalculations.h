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

Written: Oct 7 2017.
Last Updated: Oct 7 2017.
This code is licensed under the GNU LGPL and is open for ditrbution
and copying in accordance with the license.
This header must be included in any derived code or copies of the code.

 */

#ifndef TG_ENVIRONMENT_CALCULATIONS_H
#define TG_ENVIRONMENT_CALCULATIONS_H

namespace EnvironmentCalculations
{
   enum TempUnit
   {
      TempUnit_Celsius,
      TempUnit_Fahrenheit
   };

   enum AltitudeUnit
   {
      AltitudeUnit_Meters,
      AltitudeUnit_Feet
   };

  /////////////////////////////////////////////////////////////////
  /// Calculate the altitude based on the pressure with the
  /// specified units.
  /// @param seaLevelPressure given in Pa.
  float Altitude(
    float pressure,
    AltitudeUnit altUnit = AltitudeUnit_Meters,
    float referencePressure = 1013.25,   // [hPa] ....ISA value
    float outdoorTemp = 15,              // [�C] .... ISA value
    TempUnit tempUnit = TempUnit_Celsius);

  /////////////////////////////////////////////////////////////////
  /// Convert current pressure to equivalent sea-level pressure.
  /// @param altitude in meters.
  /// @param temp in Celsius.
  /// @return the equivalent pressure at sea level.
  float EquivalentSeaLevelPressure(
    float altitude,
    float temp,
    float pres,
    AltitudeUnit altUnit = AltitudeUnit_Meters,
    TempUnit tempUnit = TempUnit_Celsius);


  /////////////////////////////////////////////////////////////////
  /// Calculate the dew point based on the temperature and
  /// humidity with the specified units.
  float DewPoint(
    float temp,
    float hum,
    TempUnit tempUnit);

}

#endif // TG_ENVIRONMENT_CALCULATIONS_H
