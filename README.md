# BME280
Provides an Arduino library for reading and interpreting Bosch BME280 data over I2C, SPI or Sw SPI. Additional environment calculation functions are provided. ESP and BRZO are now supported.

## Table of Contents

1. [BME280](#bme280)
2. [Table of Contents](#Table_of_Contents)
3. [Summary](#summary)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Enumerations](#enumerations)
7. [Methods](#methods)
      - [BME280I2C(const Settings& settings)](#methods)
      - [BME280Spi(const Settings& settings)](#methods)
      - [BME280SpiSw(const Settings& settings)](#methods)
      - [bool  begin()](#methods)
      - [void  setSettings(const Settings& settings)](#methods)
      - [const Settings& getSettings()](#methods)
      - [float temp(TempUnit unit)](#methods)
      - [float pres(PresUnit unit)](#methods)
      - [float hum()](#methods)
      - [void  read(float& pressure, float& temp, float& humidity, TempUnit tempUnit, PresUnit presUnit)](#methods)
      - [uint8_t chipID()](#methods)

8. [Environment Calculations](#environment_calculations)
      - [float Alitude(float pressure, bool metric = true, float seaLevelPressure = 101325)](#environment_calculations)
      - [float SealevelAlitude(float alitude, float temp, float pres)](#environment_calculations)
      - [float DewPoint(float temp, float hum, bool metric = true)](#environment_calculations)
9. [Contributing](#contributing)
10. [History](#history)
11. [Credits](#credits)
12. [License](#license)
<snippet>
<content>

## Summary

Reads temperature, humidity, and pressure. Calculates altitude and dew point. Provides functions for english and metric. Also reads pressure in Pa, hPa, inHg, atm, bar, torr, N/m^2 and psi.

## Installation

To use this library download the zip file, uncompress it to a folder named BME280. Move the folder to {Arduino Path}/libraries.

## Usage

Include the library at the top of your Arduino script. `#include <BME280>`
Create a global or local variable. `BME280 bme`
In your start up call `bme.begin()`.
Read the temperature, humidity, pressure, altitude and/or dew point.

`float pres, temp, hum`
`bme.read(pres, temp, hum)`

or

`temp = bme.temp()`
`hum = bme.hum()`
`pres = bme.pres()`

## Enumerations
#### TempUnit Enum
   * TempUnit_Celsius
   * TempUnit_Fahrenheit

#### PresUnit Enum
   * PresUnit_Pa
   * PresUnit_hPa
   * PresUnit_inHg
   * PresUnit_atm
   * PresUnit_bar
   * PresUnit_torr
   * PresUnit_psi

#### OSR Enum
   * OSR_X1
   * OSR_X2
   * OSR_X4
   * OSR_X8
   * OSR_X16

#### Mode Enum
   * Mode_Sleep
   * Mode_Forced
   * Mode_Normal

#### StandbyTime Enum
   * StandbyTime_500us
   * StandbyTime_62500us
   * StandbyTime_125ms
   * StandbyTime_250ms
   * StandbyTime_50ms
   * StandbyTime_1000ms
   * StandbyTime_10ms
   * StandbyTime_20ms

#### Filter Enum
   * Filter_Off
   * Filter_1
   * Filter_2
   * Filter_4
   * Filter_8
   * Filter_16

## Settings

#### BME280 Settings Struct
    * Temperature Oversampling Rate (tempOSR): OSR Enum, default = OSR_X1

    * Humidity Oversampling Rate (humOSR): OSR Enum, default = OSR_X1

    * Pressure Oversampling Rate (presOSR): OSR Enum, default = OSR_X1

    * Mode (mode): Mode Enum, default = Mode_Forced

    * Standby Time (standbyTime): StandbyTime Enum, default = StandbyTime_1000ms

    * Filter (filter): Filter Enum, default = Filter_Off

    * SPI Enable: SpiEnable Enum, default = false
      values: true = enable, false = disable


#### BME280I2C Settings Struct
   * Includes all fields in BME280 settings.

   * BME 280 Address (bme280Addr): uint8_t, default = 0x76

#### BME280Spi Settings Struct
   * Includes all fields in BME280 settings.

   * SPI Chip Select Pin (spiCsPin): uint8_t
      values: Any pin 0-31

#### BME280Spi Settings Struct
   * Includes all fields in BME280 settings.

   * SPI Chip Select Pin (spiCsPin): uint8_t
     values: Any pin 0-31

   * SPI Master Out Slave In Pin (spiMosiPin): uint8_t
     values: Any pin 0-31

   * SPI Master In Slave Out Pin (spiMisoPin): uint8_t
     values: Any pin 0-31

   * SPI Serial Clock Pin (spiSckPin): uint8_t
     values: Any pin 0-31

## Methods


#### BME280I2C(const Settings& settings)

  Constructor used to create the I2C Bme class. All parameters have default values.
  Return: None



#### BME280Spi(const Settings& settings)

  Constructor used to create the Spi Bme class. All parameters have default values except chip select.
  Return: None


#### BME280SpiSw(const Settings& settings)

  Constructor used to create the software Spi Bme class. All parameters have default values except chip select, mosi, miso and sck.
  Return: None

#### bool  begin()

  Method used at start up to initialize the class. Starts the I2C or SPI interface. Can be called again to re-initialize the mode settings.
  Return: bool, true = success, false = failure (no device found)

#### void setSettings(const Settings& settings)

  Method to set the sensor settings.
  
  
#### const Settings& getSettings()

  Method to get the sensor settings.

#### float temp(TempUnit unit)

  Read the temperature from the BME280 and return a float.
  Return: float = temperature

    * unit: tempUnit, default = TempUnit_Celsius

#### float pres(PresUnit unit)

  Read the pressure from the BME280 and return a float with the specified unit.
  Return: float = pressure

    * unit: uint8_t, default = PresUnit_Pa

#### float hum()

  Read the humidity from the BME280 and return a percentage as a float.
  Return: float = percent relative humidity

#### void  read(float& pressure, float& temp, float& humidity, TempUnit tempUnit, PresUnit presUnit)

  Read the data from the BME280 with the specified units.
  Return: None, however, pressure, temp and humidity are changed.

    * Pressure: float, reference
      values: reference to storage float for pressure

    * Temperature: float, reference
      values: reference to storage float for temperature

    * Humidity: float, reference
      values: reference to storage float for humidity

    * tempUnit: tempUnit, default = TempUnit_Celsius

    * presUnit: uint8_t, default = PresUnit_Pa

#### uint8_t chipID()
   Returns the chip identification number.
   Return: uint8_t 0x60 = BME ID
                   0x58 = BMP ID


## Environment Calculations

#### float Alitude(float pressure, bool metric = true, float seaLevelPressure = 101325)

  Calculate the altitude based on the pressure with the specified units.
  Return: float = altitude

    * Pressure: float, unit = Pa
      values: any float

    * Metric: bool, default = true
      values: true = meters, false = feet

    * Sea Level Pressure: float, unit = Pa, default = 101325
      values:  any float

#### float SealevelAlitude(float alitude, float temp, float pres)

  Convert current pressure to sea-level pressure, returns
  Altitude (in meters), temperature in Celsius

  Return: The equivalent pressure at sea level.

    * alitude: float
      values: meters

    * temp: float
      values: celsius

    * hum: float

#### float DewPoint(float temp, float hum, bool metric = true)

  Calculate the dew point based on the temperature and humidity with the specified units.
  Return: float = dew point

    * Temperature: float, unit = Celsius if metric is true, Fahrenheit if metric is false
      values: any float

    * Humidity: float, unit = % relative humidity
      values: any float

    * Metric: bool, default = true
      values: true = return degrees Celsius, false = return degrees Fahrenheit



## Contributing

1. Fork the project.
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request.

## History

- Jan  1, 2016   - Version 1.0.0 released
- Sep  19, 2016  - Version 2.0.0 released (Restructure for I2C and SPI)
- Nov 21,  2016  - Version 2.0.1 released (Set mode support)
- Dec  19, 2016  - Version 2.1.0 released (Support for SPI)
- Dec  21, 2016  - Version 2.1.1 released (Fixing compilation errors)
- Feb  17, 2017  - Version 2.1.2 released (Docs)
- Sept  9, 2017  - Version 2.1.3 released (Formatting, reorg)
- Sept 13, 2017  - Version 2.1.4 released (Examples update, bug fixes)
- Oct   7, 2017  - Version 2.2.0 released (Enums, begin restructure)
- Oct  10, 2017  - Version 2.2.1 released (Bug fixes)

## Credits

Written by Tyler Glenn, 2016.

Special thanks to Mike Glenn for editing and reviewing the code.

## License

GNU GPL, see License.txt
</content>
</snippet>
