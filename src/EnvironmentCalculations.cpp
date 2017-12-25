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
Last Updated: Dec 23 2017. 

This header must be included in any derived code or copies of the code.

 */
#include "EnvironmentCalculations.h"

#include <Arduino.h>
#include <math.h>

#define hi_coeff1 -42.379
#define hi_coeff2   2.04901523
#define hi_coeff3  10.14333127
#define hi_coeff4  -0.22475541
#define hi_coeff5  -0.00683783
#define hi_coeff6  -0.05481717
#define hi_coeff7   0.00122874
#define hi_coeff8   0.00085282
#define hi_coeff9  -0.00000199
/****************************************************************/
float EnvironmentCalculations::Altitude
(
  float pressure,
  AltitudeUnit altUnit,
  float referencePressure,
  float outdoorTemp,
  TempUnit tempUnit
)
{
  // Equation inverse to EquivalentSeaLevelPressure calculation.
  float altitude = NAN;
  if (!isnan(pressure) && !isnan(referencePressure) && !isnan(outdoorTemp))
  {
      if(tempUnit != TempUnit_Celsius)
          outdoorTemp = (outdoorTemp - 32.0) * (5.0 / 9.0); /*conversion to [�C]*/

      altitude = pow(referencePressure / pressure, 0.190234) - 1;
      altitude *= ((outdoorTemp + 273.15) / 0.0065);
      if(altUnit != AltitudeUnit_Meters)  altitude *= 3.28084;
  }
  return altitude;
}


/****************************************************************/
float EnvironmentCalculations::AbsoluteHumidity
(
  float temperature, 
  float humidity,
  TempUnit tempUnit
)
{
  //taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  //precision is about 0.1°C in range -30 to 35°C
  //August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
  //Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
  //reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html
  float temp = NAN;
  const float mw = 18.01534; 	// molar mass of water g/mol
  const float r = 8.31447215; 	// Universal gas constant J/mol/K

  if (isnan(temperature) || isnan(humidity) )
  {
    return NAN;
  }
  
  if(tempUnit != TempUnit_Celsius)
  {
        temperature = (temperature - 32.0) * (5.0 / 9.0); /*conversion to [°C]*/
  }
          
  temp = pow(2.718281828, (17.67 * temperature) / (temperature + 243.5));
  
  //return (6.112 * temp * humidity * 2.1674) / (273.15 + temperature); 		//simplified version
  return (6.112 * temp * humidity * 2.1674 * mw) / ((273.15 + temperature) * r); 	//long version
}

/****************************************************************/
float EnvironmentCalculations::HeatIndex
(
  float temperature,
  float humidity,
  TempUnit tempUnit
)
{
  if ( !isnan(temperature) && !isnan(humidity) ) {
    if (tempUnit == TempUnit_Celsius) {
      temperature = (temperature * (9.0 / 5.0) + 32.0); /*conversion to [°F]*/
    }
    // Using both Rothfusz and Steadman's equations
    // http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
    float heatindex(NAN);
    if (temperature <= 40) {
      if (tempUnit == TempUnit_Celsius) {
        return (temperature - 32.0) * (5.0 / 9.0); /*conversion back to [°C]*/
      }
      else {
        return temperature;
      }
    }
    heatindex = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (humidity * 0.094));

    if (heatindex > 79) {
      float tempQ, humQ;
      tempQ = temperature * temperature;
      humQ = humidity * humidity;
      heatindex =  hi_coeff1 +
                  hi_coeff2 * temperature +
                  hi_coeff3 * humidity +
                  hi_coeff4 * temperature * humidity +
                  hi_coeff5 * tempQ +
                  hi_coeff6 * humQ +
                  hi_coeff7 * tempQ * humidity +
                  hi_coeff8 * temperature * humQ +
                  hi_coeff9 * tempQ * humQ;

      if ((humidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
        heatindex -= ((13.0 - humidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

      else if ((humidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
        heatindex += (0.02 * (humidity - 85.0) * (87.0 - temperature));
    }

    if (tempUnit == TempUnit_Celsius) {
      return (heatindex - 32.0) * (5.0 / 9.0); /*conversion back to [°C]*/
    }
    else {
      return heatindex;
    }
  }
  // fallback if the parameter are not useful
  else {
	  return NAN;
  }
}

/****************************************************************/
float EnvironmentCalculations::EquivalentSeaLevelPressure
(
  float altitude,
  float temp,
  float pres,
  AltitudeUnit altUnit,
  TempUnit tempUnit
)
{
    float seaPress = NAN;
    if(!isnan(altitude) && !isnan(temp) && !isnan(pres))
    {
        if(tempUnit != TempUnit_Celsius)
            temp = (temp - 32.0) * (5.0 / 9.0); /*conversion to [�C]*/

        if(altUnit != AltitudeUnit_Meters)
            altitude *= 0.3048; /*conversion to meters*/

        seaPress = (pres / pow(1 - ((0.0065 *altitude) / (temp + (0.0065 *altitude) + 273.15)), 5.257));
    }
    return seaPress;
}


/****************************************************************/
float EnvironmentCalculations::DewPoint
(
  float temp,
  float hum,
  TempUnit tempUnit
)
{
  // Equations courtesy of Brian McNoldy from http://andrew.rsmas.miami.edu;
  float dewPoint = NAN;

  if(!isnan(temp) && !isnan(hum))
  {
     if (tempUnit == TempUnit_Celsius)
     {
       dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * temp)/(243.04 + temp)))
       /(17.625 - log(hum/100.0) - ((17.625 * temp)/(243.04 + temp)));
     }
     else
     {
       float ctemp = (temp - 32.0) * 5.0/9.0;

       dewPoint = 243.04 * (log(hum/100.0) + ((17.625 * ctemp)/(243.04 + ctemp)))
       /(17.625 - log(hum/100.0) - ((17.625 * ctemp)/(243.04 + ctemp)));

       dewPoint = dewPoint * 9.0/5.0 + 32.0;
     }
  }

  return dewPoint;
}
