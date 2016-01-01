# BME280
Provides an Arduino library for reading and interpreting Bosch BME280 data.
<snippet>
<content>
# Summary
Reads temperature, humidity, and pressure. Calculates altitude and dew point. Provides functions for english and metric. Also reads pressure in Pa, hPa, inHg, atm, bar, torr, N/m^2 and psi.

## Installation
To use this library download the zip file, uncompress it to a folder named BME280. Move the folder to {Arduino Path}/libraries.

## Usage
Include the library at the top of your Arduino script. `#include <BME280>`
Create a global or local variable. `BME280 bme;`
In your start up call `bme.begin()`.
Read the temperature, humidity, pressure, altitude and/or dew point.
`float pres, temp, hum;
bme.ReadData(pres, temp, hum);
or
temp = bme.ReadTemperature();
hum = bme.ReadHumidity();
pres = bme.ReadPressure();

float altitude, dewPoint;
altitude = bme.CalculateAltitude();
dewPoint = bme.CalculateDewPoint();`

## Methods

###### BME280(uint8_t tosr = 0x1, uint8_t hosr = 0x1, uint8_t posr = 0x1, uint8_t mode = 0x3, uint8_t st = 0x5, uint8_t filter = 0x0, bool spiEnable = false, uint8_t bme_280_addr = 0x76);  
  Info: Constructor used to create the class. All parameters have default values.
  Return: None

    *Temperature Oversampling Rate: uint8_t, default = 0x1
      values: B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16

    *Humidity Oversampling Rate: uint8_t, default = 0x1
      values: B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16

    *Pressure Oversampling Rate: uint8_t, default = 0x1
      values: B000 = Skipped, B001 = x1, B010 = x2, B011 = x4, B100 = x8, B101/other = x16

    *Mode: uint8_t, default = forced
      values: Sleep = B00, Normal = B01 and B10, Forced = B11

    *Filter: uint8_t, default = None
      values: B000 = off, B001 = 2, B010 = 4, B011 = 8, B100/other = 16

    *SPI Enable: bool, default = false
      values: true = enable, false = disable

    *BME280 Address: uint8_t, default = 0x76
      values: any uint8_t




###### bool  begin();
  Info: Method used at start up to initialize the class. Starts the I2C interface.
  Return: bool, true = success, false = failure (no device found)

###### float ReadTemperature(bool celsius = false);
  Info: Read the temperature from the BME280 and return a float.
  Return: float = temperature

    *Celsius: bool, default = false
      values: true = return temperature in degrees Celsius, false = return
      temperature in degrees Fahrenheit


###### float ReadPressure(uint8_t unit = 0x0);
  Info: Read the pressure from the BME280 and return a float with the specified unit.
  Return: float = pressure

    *Unit: uint8_t, default = 0x0
      values: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar,
      B101 = torr, B110 = N/m^2, B111 = psi

###### float ReadHumidity();
  Info: Read the humidity from the BME280 and return a percentage as a float.
  Return: float = percent relative humidity

###### void  ReadData(float& pressure, float& temp, float& humidity, bool metric = true, uint8_t p_unit = 0x0);
  Info: Read the data from the BME280 with the specified units.
  Return: None, however, pressure, temp and humidity are changed.

    *Pressure: float, reference
      values: reference to storage float for pressure

    *Temperature: float, reference
      values: reference to storage float for temperature

    *Humidity: float, reference
      values: reference to storage float for humidity

    *Metric: bool, default = true
      values: true = meters, false = feet

    *Pressure Unit: uint8_t, default = 0x0
        values: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar,
        B101 = torr, B110 = N/m^2, B111 = psi

###### float CalculateAltitude(bool metric = true, float seaLevelPressure = 101325);
  Info: Read the data from the BME280 with the specified units and then calculate the altitude.
  Return: float = altitude

    *Metric: bool, default = true
      values: true = meters, false = feet

    *Sea Level Pressure: float, unit = Pa, default = 101325
      values:  any float


###### float CalculateAltitude(float pressure, bool metric = true, float seaLevelPressure = 101325); // Pressure given in Pa.
  Info: Calculate the altitude based on the pressure with the specified units.
  Return: float = altitude

    *Pressure: float, unit = Pa
      values: any float

    *Metric: bool, default = true
      values: true = meters, false = feet

    *Sea Level Pressure: float, unit = Pa, default = 101325
      values:  any float

###### float CalculateDewPoint(bool metric = true);
  Info: Read BME280 data and calculate the dew point with the specified units.
  Return: float = dew point

    *Metric: bool, default = true
      values: true = return temperature in degrees Celsius, false = return
      temperature in degrees Fahrenheit

###### float CalculateDewPoint(float temp, float hum, bool metric = true);
  Info: Calculate the dew point based on the temperature and humidity with the specified units.
  Return: float = dew point

    *Temperature: float, unit = Celsius if metric is true, Fahrenheit if metric is false
      values: any float

    *Humidity: float, unit = % relative humidity
      values: any float

    *Metric: bool, default = true
      values: true = return degrees Celsius, false = return degrees Fahrenheit

## Contributing
1. Fork the project.
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request.

## History
Jan 1, 2016 - Version 1.0.0 released.

## Credits
Written by Tyler Glenn, 2016.

## License
GNU GPL, see License.txt
</content>
</snippet>
