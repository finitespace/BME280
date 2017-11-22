/*
EnvironmentCalculations.cpp

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

Some unit conversations courtesy
of www.endmemo.com, altitude equation courtesy of NOAA, and dew point equation
courtesy of Brian McNoldy at http://andrew.rsmas.miami.edu.
 */

#include <Arduino.h>

#include "EnvironmentCalculations.h"


/****************************************************************/
float EnvironmentCalculations::Altitude
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
float EnvironmentCalculations::EquivalentSeaLevelPressure
(
  float alitude,
  float temp,
  float pres
)
{
   return(pres / pow(1-((0.0065 *alitude) / (temp + (0.0065 *alitude) + 273.15)),5.257));
}


/****************************************************************/
float EnvironmentCalculations::DewPoint
(
  float temp,
  float hum,
  bool metric
)
{
  // Equations courtesy of Brian McNoldy from http://andrew.rsmas.miami.edu;
  float dewPoint = NAN;

  if (metric && !isnan(temp) && !isnan(hum))
  {
    dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * temp)/(243.04 + temp)))
    /(17.625 - log(hum/100.0) - ((17.625 * temp)/(243.04 + temp)));
  }
  else if (!isnan(temp) && !isnan(hum))
  {
    float ctemp = (temp - 32.0) * 5.0/9.0;

    dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * ctemp)/(243.04 + ctemp)))
    /(17.625 - log(hum/100.0) - ((17.625 * ctemp)/(243.04 + ctemp)));

    dewPoint = dewPoint * 9.0/5.0 + 32.0;
  }
  return dewPoint;
}
