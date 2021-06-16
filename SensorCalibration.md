# ESP32 Calibration
* Used ESP32 tool espefuse.py (https://github.com/espressif/esptool/wiki/espefuse) to get the calibration value for the ESP32 as documented here (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html). On windows, with esptools installed from pip, used `python -m espefuse --port COM6 adc_info`, the  resut was: "ADC VRef calibration: 1128mV". Was not useful so far.
* There are some issues with the default ESP32 ADC (https://github.com/espressif/esp-idf/issues/164). I was able to calibrate and use within the "useable range"; see folder /ESP32_calibration.
* The A2D of the ESP32 is 12 bit (4096 different numbers). From testing done it is accurate between inputs of 18 (0.15V) - 4076 (3.15V) (with default settings).

# Sensor calibration
## Measurements
For meat sensor (sharp probe type):
Temp, Resistance (Ohm)
3, 132k
47, 20.7k
86, 5.38k

For air sensor (alagator clip type):
Temp, Resistance (Ohm)
51.1, 16.75k
94.9, 4.04k
210, 0.250k

## Steinhart-Hart model coefficients
These were calculated by the online calculator here: https://thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm

For meat sensor (sharp probe type):
A = 0.8975771660 e-3
B = 2.067767238 e-4
C = 1.742614938 e-7

For air sensor (alagator clip type):
A = 0.9482846445 e-3
B = 1.952744345 e-4
C = 2.570293116 e-7

## Determining voltage divider resistance
Following this tutorial (https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html) to understand fixed resistance for voltage divider. VoltageDividerCalc.py has calculations testing paramaters. ESP32 is 12 bit so has range of 4095 values (0-4094). Then, following this paper (http://unsworks.unsw.edu.au/fapi/datastream/unsworks:56332/bin32ff80f2-1c1e-4bb2-9e86-8843f5423fbb?view=true) to find the best relationship for the fixed resistance for voltage dividers is:

R_o = (R_min + R_max)^(1/2)

### Meat sensor
For the sensor I expect they will need to work in ranges:
* Most accurate: 62 C or 74 C
* Usually: 20 C - 80 C
* Sometimes: -10 C - 110 C (248.1k - 2.62k)

So, sqrt(248.1 * 2.62) = 25.5 kOhm
So, try 25 kOhm fixed resistor value.

### BBQ air sensor
(NOTE: Need to check this sensor has the same Steinhart-Hart model coefficients as the above)
For the sensor I expect they will need to work in ranges:
* Most accurate: 220 to 300 C
* Usually: 100 - 320 C
* Sometimes: 10 - 340 C (82.47k -  0.0313k)

Calculating sqrt(82.47 * 0.0313) = 1.6 kOhm
So, try 1 kOhm fixed resistor value.

# Improvements
* Need air sensor to work from 0 C up to 340 C. Need that low temperature range.
* Use a external A2D converter if want higher accuracy.


# Completed (newest ontop)
* 2.60V is 206C on ESP32 (but on plot, 206C is 1.78V). -> Plot was not at high enough resolution.
* Consider a ADC chip for higher accuracy. -> Not needed, ESP32 sufficient accuracy.