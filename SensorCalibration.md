# Sensor calibration (meat)

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
Following this tutorial (https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html) to understand fixed resistance for voltage divider. VoltageDividerCalc.py has calculations testing paramaters. Ardunio is 10 bit so has range of 1024 values (0-1023). Then, following this paper (http://unsworks.unsw.edu.au/fapi/datastream/unsworks:56332/bin32ff80f2-1c1e-4bb2-9e86-8843f5423fbb?view=true) to find the best relationship for the fixed resistance for voltage dividers is:

R_o = (R_min + R_max)^(1/2)

### Meat sensor
For the sensor I expect they will need to work in ranges:
* Most accurate: 62 C or 74 C
* Usually: 20 C - 80 C
* Sometimes: -10 C - 110 C (248.1k - 2.62k)

So, sqrt(248.1 * 2.62) = 25 kOhm (fixed resistor value).

### BBQ air sensor
(NOTE: Need to check this sensor has the same Steinhart-Hart model coefficients as the above)
For the sensor I expect they will need to work in ranges:
* Most accurate: 150 to 250 C
* Usually: 100 - 300 C
* Sometimes: 50 - 300 C (17.42k -  0.054k)

Calculating sqrt(17.42 * 0.054) = 0.97 kOhm
So, try 1 kOhm fixed resistor value.

# Refinement
* Use a higher quality multimeter.
* Calibrate at temperature of 250 or 300 C.
* I did not connect the leads of the multimeter together to measure the resistance when measuring "nothing"