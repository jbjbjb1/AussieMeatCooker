# Sensor calibration (meat)

## Measurements
The following measurements were made from the 'meat sensor replacement Inkbird'. The real temperature was measured with a ThermoPen:

Temp, Resistance (200k Ohm)
8.4, 103.1
38.0, 31.5
80, 6.1

Temp, Resistance (Ohm)
8.4, 103.1 e3
38.0, 31.5 e3
80, 6.1 e3

## Steinhart-Hart model coefficients
A = 1.839472271 e-3
B = 0.6807883650 e-4
C = 6.022892289 e-7

## Determining voltage divider resistance
Following this tutorial to work out optimum fixed resistance for voltage divider: https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html

### Meat sensor
For the sensor I expect they will need to work in ranges:
* Most accurate: 62 or 74 C
* Usually: 20 - 80 C
* Sometimes: -10 - 110 C

Working out notes (see file VoltageDividerCalc.py for calculations):
* Ardunio is 10 bit so has range of 1024 values (0-1023).
* A 30 kOhm resister looks to be the best for this measurement range

### BBQ air sensor
For the sensor I expect they will need to work in ranges:
* Most accurate: 150 to 250 C
* Usually: 100 - 300 C
* Sometimes: 10 - 350 C

Note: need to consider the max working temperature of the probe, and ensure it cannot overheat (or, the overheating is in the measurement range and it can tell it is overheating)

# Refinement
* I did not connect the leads of the multimeter together to measure the resistance when measuring "nothing"