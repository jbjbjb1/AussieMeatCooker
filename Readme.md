# Readme

The purpose of this project is to make a device to monitor bbq air and meat temperature to cook theperfect meal!

## Main Tutorial Following
https://forum.arduino.cc/index.php?topic=231655.0

## Steps completed (top most recent)
* Arduino screen is working and showing temperatures using potentiometer/resistor as input 1 and 2
* Souldering iron used to connect the I2C to the 1602 LCD

## Next steps
* Then run tests/liquid_crystal to see the 'hello world' example

## Items (all prices $AUD)
* 1 x meat sensor replacement Inkbird, $11.99 + $1.99, https://www.ebay.com.au/itm/PROBE-for-INKBIRD-IBT-4X-meat-thermometer-replacement-Accessories-REPLACE-sensor/222779014478?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2648
* 1 x bbq clip sensor replacement Inkbird, $10.99 + $1.99, https://www.ebay.com.au/itm/PROBE-Inkbird-cooking-thermometer-sensor-replacement-grill-clip-cable-replace/322770203997?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2648
* ? 2 x 2.5MM Stereo Chassis Socket, $1.85, https://www.jaycar.com.au/2-5mm-stereo-chassis-socket/p/PS0107
* ? 2 x 3.5mm Stereo Plug to 2.5mm Stereo Socket, $3.95, https://www.jaycar.com.au/3-5mm-stereo-plug-to-2-5mm-stereo-socket/p/PA3536
* 1 x I2C
* 1 x 1602 LCD

## How to use
* Install app 'Serial USB terminal' on mobile.
* In settings, set Baud Rate to 9600
* Connect sensors by 2.5mm jack, connect usb from Arduino to mobile and in the app click the connect button

## Next steps
* Add rate of change of temperature per minute to project. Useful to track rate of cooking.
* 3D print own case for enclosure: https://hackaday.com/2017/08/21/a-detailed-guide-for-3d-printing-enclosures/ (or try to replicate https://core-electronics.com.au/clear-enclosure-for-arduino-electronics-enclosure-1-0.html)
* Use an ESP board with inbuilt bluietooth (no screen), and have a re-chargeable battery using something like this: https://www.jaycar.com.au/arduino-compatible-lithium-battery-usb-charger-module/p/XC4502 - here is a complete project: https://github.com/dereulenspiegel/ibbq-gateway

## Notes during setup
* A lot of problems can come from not having the correct libraries for the Ardunio code