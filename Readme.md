# Readme

The purpose of this project is to make a device to monitor bbq air and meat temperature to cook the perfect steak! The image below shows the current progress of the project - when a perfect steak is shown, this project is functional!

<img src="progress_pic.jpg" width="600">

This project is springboarading off some great work done here: https://forum.arduino.cc/index.php?topic=231655.0

## Steps completed (top oldest)
* Got code working, used potentiometer to simulate different temperatures (as resitances)
* Arduino screen working (I2C to the 1602 LCD) and showing temperatures
* Sensors arrived, did calibration and calculated Steinhart-Hart model coefficients
* Added functionality for rate of change of temperature

## Next steps
* Put on ESP32 board with bluetooth (similar to https://github.com/dereulenspiegel/ibbq-gateway)

## Items (all prices $AUD)
* 1 x meat sensor replacement Inkbird, $11.99 + $1.99, https://www.ebay.com.au/itm/PROBE-for-INKBIRD-IBT-4X-meat-thermometer-replacement-Accessories-REPLACE-sensor/222779014478?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2648
* 1 x bbq clip sensor replacement Inkbird, $10.99 + $1.99, https://www.ebay.com.au/itm/PROBE-Inkbird-cooking-thermometer-sensor-replacement-grill-clip-cable-replace/322770203997?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2648
* 2 x 2.5MM Stereo Chassis Socket, $1.85, https://www.jaycar.com.au/2-5mm-stereo-chassis-socket/p/PS0107
* 1 x I2C   (from Arduino kit)
* 1 x 1602 LCD  (from Arduino kit)

## How to use
* Setup Arduino as shown in schematic. Load on program meat_cooker\meat_cooker.ino
* Install app 'Serial USB terminal' on mobile, in settings, set Baud Rate to 9600
* Connect sensors by 2.5mm jacks, connect usb from Arduino to mobile and in the app click the connect button
* Start cooking!
* Use