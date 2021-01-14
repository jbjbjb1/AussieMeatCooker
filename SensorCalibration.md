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

For the sensors I expect they will need to work in ranges:
* Most accurate: 62 or 74 C
* Usually: 20 - 80 C
* Sometimes: -10 - 110 C

Working out notes (see file VoltageDividerCalc.py for calculations):
* Ardunio is 10 bit so has range of 1024 values (0-1023).
* Let: 
Temp (C), Res (kOhm), Volt
-10, 221.3, 5
110, 1.870, 0
* Each step is 0.12 C -> OK
* Mid point (2.5V) is 60 C -> OK



# Sensor calibration (bbq air)

# Refinement
* I did not connect the leads of the multimeter together to measure the resistance when measuring "nothing"