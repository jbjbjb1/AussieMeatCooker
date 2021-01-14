//Title: Meat Cooker - Digital BBQ Thermometer

// Part 1: This initializes the LCD using the I2C converter and the global variables that we will need:

#include <SPI.h>
#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
                       
LiquidCrystal_I2C lcd(0x27,16,2);

Timer t;

float air;                      // BBQ air temperature
float meat;                     // Meat temperature
float vin = 4.98;               // DC Voltage as measured with DMM between +5V and GND
float r2 = 4700;                // Resistance in ohms of your fixed resistor

float A = 0.-1.247037951 * pow(10, -3);     // "A" Coeffecient in Steinhart-Hart Equation
float B = 5.155847023 * pow(10, -4);        // "B"
float C = 3.997113292 * pow(10, -7);        // "C"

// Part 2: The "Setup" function displays connection info on the LCD, and calls our other functions (readSensors and updateDisplay) on a fixed interval based on Timer "t":

void setup()
{
    Serial.begin(9600);
    analogReference(DEFAULT);                    // Use default 5V reference
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.home();
    lcd.print("Meat Cooker!");
    Serial.println("Meat Cooker!");             // Output to serial too
    delay(1000); 

    t.every(4000, readSensors);
    t.every(4000, updateDisplay);
    t.every(4000, updateSerial);  
}                      

// Part 3: The main loop essentially just updates the timer:

void loop()
{
    t.update(); 
}

// Part 4: The "readSensors" function does exactly that; it reads the sensor and calculates the temperature:

void readSensors()
{
    float a0 = analogRead(A0);                                   // This reads the "voltage" value on A0. Value is actually divided into 1024 steps from 0-1023.
    float v0 = a0 * .0049;                                       // Converts A0 value to an actual voltage (5.0V / 1024 steps = .0049V/step.
    float r0 = (((r2 * vin) / v0) - r2);                         // Calculates resistance value of thermistor based on fixed resistor value and measured voltage
    float logr0 = log(r0);                                       // Natural log of thermistor resistance used in Steinhart-Hart Equation
    float logcubed0 = logr0 * logr0 * logr0;                     // The cube of the above value
    float k0 = 1.0 / (A + (B * logr0) + (C * logcubed0));        // Steinhart-Hart Equation to calculate temperature in Kelvin
    float c0 = k0 - 273.15;                                      // Convert temperature K to C

if (isnan(c0))                                                   // If value is not a number, assign an arbitrary value
{
   air = int(1);
}
else
{
    air = c0;                                                    // Otherwise use the calculated value
}

float a1 = analogRead(A1);                                       // Same code as above. Repeat for as many sensors as you need to connect.
float v1 = a1 * .0049;
float r1 = (((r2 * vin) / v1) - r2);
float logr1 = log(r1);
float logcubed1 = logr1 * logr1 * logr1;
float k1 = 1.0 / (A + (B * logr1) + (C * logcubed1));
float c1 = k1 - 273.15;  

if (isnan(c1))
{
    meat = int(1);
}
else
{
    meat = c1;
}
}

// Part 5: The "updateDisplay" function displays the temperature values on the LCD:

void updateDisplay()
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("BBQ :");
    lcd.print(air, 1);
    lcd.print(" C");
    
    lcd.setCursor(0,1);
    lcd.print("Meat:");
    lcd.print(meat, 1);
    lcd.print(" C");
}

// Part 6: Update the serial output:

void updateSerial()
{
    
    Serial.print("BBQ: ");
    Serial.print(air, 1);
    Serial.print(" C, ");
    Serial.print("Meat: ");
    Serial.print(meat, 1);
    Serial.println(" C");
}
