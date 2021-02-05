//Title: Meat Cooker - Digital BBQ Thermometer

// Part 1: This initializes the LCD using the I2C converter and the global variables that we will need:

#include <SPI.h>
#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
                       
LiquidCrystal_I2C lcd(0x27,16,2);

Timer t;

float meat;                     // Meat temperature
float air;                      // BBQ air temperature
float meat_ = 20;               // Meat temperature, for rate of change function
float air_ = 20;                // BBQ air temperature, for rate of change function
float meat_r;                   // Rate change meat temperature
float air_r;                    // Rate change BBQ air temperature
float update_r = 3000;          // Update rate for sensors, milliseconds
float vin = 5;                  // DC Voltage as measured with DMM between +5V and GND

// Meat sharp probe
float r_0 = 19690;                         // Resistance in ohms of your fixed resistor
float A_0 = 0.8975771660 * pow(10, -3);    // "A" Coeffecient in Steinhart-Hart Equation
float B_0 = 2.067767238 * pow(10, -4);     // "B"
float C_0 = 1.742614938 * pow(10, -7);     // "C"

// Air aligator clip
float r_1 = 995;                        
float A_1 = 0.9482846445 * pow(10, -3);    
float B_1 = 1.952744345 * pow(10, -4);    
float C_1 = 2.570293116 * pow(10, -7);    


// Part 2: The "Setup" function displays connection info on the LCD, and calls our other functions (readSensors and updateDisplay) on a fixed interval based on Timer "t":

void setup()
{
    Serial.begin(9600);
    analogReference(DEFAULT);                    // Use default 5V reference
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("Meat Cooker!");
    lcd.setCursor(0,1);
    lcd.print("Units: C @ C/min");
    Serial.println("Meat Cooker!");             // Output to serial too
    delay(5000); 

    t.every(update_r, updateTemp);
    t.every(update_r*2, updateTempRate);
    t.every(update_r, updateDisplay);
    t.every(update_r, updateSerial);  
}                      

// Part 3: The main loop essentially just updates the timer:

void loop()
{
    t.update(); 
}

// Part 4: Functions to get the probe temperatures when required.

float T_meat(){

    // Read meat sharp probe
    float a0 = analogRead(A0);                                   // This reads the "voltage" value on A0. Value is actually divided into 1024 steps from 0-1023.
    float v0 = a0 * 0.0048828125;                                // Converts A0 value to an actual voltage (5.0V / 1024 steps)
    float r0 = (((r_0 * vin) / v0) - r_0);                       // Calculates resistance value of thermistor based on fixed resistor value and measured voltage
    float logr0 = log(r0);                                       // Natural log of thermistor resistance used in Steinhart-Hart Equation
    float logcubed0 = logr0 * logr0 * logr0;                     // The cube of the above value
    float k0 = 1.0 / (A_0 + (B_0 * logr0) + (C_0 * logcubed0));  // Steinhart-Hart Equation to calculate temperature in Kelvin
    float c0 = k0 - 273.15;                                      // Convert temperature K to C

    if (isnan(c0)){                                              // If value is not a number, assign an arbitrary value
        meat = int(1);
    }
    else{ 
        meat = c0;                                                    // Otherwise use the calculated value
    }
    return meat;
}

float T_air(){

    // Read air aligator clip
    float a1 = analogRead(A1);                                       // Same code as above. Repeat for as many sensors as you need to connect.
    float v1 = a1 * 0.0048828125;
    float r1 = (((r_1 * vin) / v1) - r_1);
    float logr1 = log(r1);
    float logcubed1 = logr1 * logr1 * logr1;
    float k1 = 1.0 / (A_1 + (B_1 * logr1) + (C_1 * logcubed1));
    float c1 = k1 - 273.15;  

    if (isnan(c1)){
        air = int(1);
    }
    else{
        air = c1;
    }
    return air;
}


// Part 4.1 Update temperatures for dsiplay
void updateTemp(){
    meat = T_meat();
    air = T_air();
}

// Part 4.2 Update rate of temperature change
void updateTempRate(){
    float meat_n = T_meat();
    float air_n = T_air();
    
    Serial.print(meat_n, 1);
    Serial.print("-");
    Serial.print(meat_, 1);
    Serial.print("=");
    Serial.print(meat_n - meat_, 1);
    Serial.print(", const: ");
    Serial.print(10, 1);
    Serial.print(", @: ");
    
    meat_r = (meat_n - meat_) * 10;     // for 10, can't get ((1000.0*60.0)/(update_r*2.0)) working
    air_r = (air_n - air_) * 10;
    meat_ = meat_n;
    air_ = air_n;
    
    Serial.println(meat_r, 1);
}

// Part 5: The "updateDisplay" function displays the temperature values on the LCD:

void updateDisplay(){

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Air :");
    lcd.print(air, 0);
    lcd.setCursor(9,0);         //  Set to appear at fixed point so below text does not move
    lcd.print("@ ");
    lcd.print(air_r, 1);
    
    lcd.setCursor(0,1);
    lcd.print("Prb1:");
    lcd.print(meat, 0);
    lcd.setCursor(9,1);
    lcd.print("@ ");
    lcd.print(meat_r, 1);
}

// Part 6: Update the serial output:

void updateSerial(){

    Serial.print("Air : ");
    Serial.print(air, 1);
    Serial.print(" C, ");
    Serial.print("Prb1: ");
    Serial.print(meat, 1);
    Serial.println(" C");
}
