//Title: Meat Cooker - Digital BBQ Thermometer

// Part 1: This initializes the LCD using the I2C converter and the global variables that we will need:

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <driver/adc.h>
#include <SPI.h>
#include <arduino-timer.h>
#include <math.h>

// Ensure bluetooth settings are correct
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif   

// Rename BluetoothSerial
BluetoothSerial SerialBT;

auto t = timer_create_default(); // create a timer with default settings

float meat;                     // Meat temperature
float air;                      // BBQ air temperature
float meat_ = 20;               // Meat temperature, for rate of change function
float air_ = 20;                // BBQ air temperature, for rate of change function
float meat_r;                   // Rate change meat temperature
float air_r;                    // Rate change BBQ air temperature
float update_r = 3000;          // Update rate for sensors, milliseconds
float vin = 3.3;                // Reference voltage of ESP32 ADC

// Meat probe
float r_0 = 25000;                         // Resistance in ohms of your fixed resistor
float A_0 = 0.8975771660 * pow(10, -3);    // "A" Coeffecient in Steinhart-Hart Equation
float B_0 = 2.067767238 * pow(10, -4);     // "B"
float C_0 = 1.742614938 * pow(10, -7);     // "C"

// Air aligator clip
float r_1 = 1000;                        
float A_1 = 0.9482846445 * pow(10, -3);    
float B_1 = 1.952744345 * pow(10, -4);    
float C_1 = 2.570293116 * pow(10, -7);    

// Functions to get the probe temperatures when required.
float T_meat(){

  // Calculate temperature from raw input                                
  int a0 = analogRead(35);
  float v0 = a0 * 3.3 / 4096;                                  // Converts analogue value to voltage
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
  int a1 = analogRead(34);                                       // Same code as above. Repeat for as many sensors as you need to connect.
  float v1 = a1 * 3.3 / 4096;
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


// Part Update temperatures for dsiplay
bool updateTemp(void *){
  meat = T_meat();
  air = T_air();
  return true;
}

// Part Update rate of temperature change
bool updateTempRate(void *){
  float meat_n = T_meat();          // Get temperature now
  float air_n = T_air();
  
  meat_r = (meat_n - meat_) * 10;     // for 10, can't get ((1000.0*60.0)/(update_r*2.0)) working
  air_r = (air_n - air_) * 10;
  meat_ = meat_n;
  air_ = air_n;

  // Serial
  Serial.print("Air @ ");
  Serial.print(air_r, 2);
  Serial.print(" C/min, ");
  Serial.print("Prb1 @ ");
  Serial.print(meat_r, 2);
  Serial.println(" C/min");

  // Bluetooth serial
  SerialBT.print("Air @ ");
  SerialBT.print(air_r, 2);
  SerialBT.print(" C/min, ");
  SerialBT.print("Prb1 @ ");
  SerialBT.print(meat_r, 2);
  SerialBT.println(" C/min");

  return true;
}

// Update the serial & bluetooth serial output:

bool updateSerial(void *){
  // Serial
  Serial.print("Air : ");
  Serial.print(air, 1);
  Serial.print(" C, ");
  Serial.print("Prb1: ");
  Serial.print(meat, 1);
  Serial.println(" C");

  // Bluetooth serial
  SerialBT.print("Air : ");
  SerialBT.print(air, 1);
  SerialBT.print(" C, ");
  SerialBT.print("Prb1: ");
  SerialBT.print(meat, 1);
  SerialBT.println(" C");

  return true;
}

// The "Setup" function begins serial communication and setups update rates of timers:
void setup()
{
  Serial.begin(115200);
  SerialBT.begin("AussieMeatCooker");   // Bluetooth device name
  delay(200);                           // Time delay so serial prints properly
  Serial.println("The device started, now you can pair it with bluetooth!");
  t.every(update_r, updateTemp);
  t.every(update_r*2, updateTempRate);
  t.every(update_r, updateSerial);  
}                      

// The main loop just ticks over the timer:
void loop()
{
  t.tick(); 
}