#include "Temperature.h"
#include <Arduino.h>
#include <math.h>
#include <driver/adc.h>

float meat;                     // Meat temperature
float air;                      // BBQ air temperature
float meat_ = 20;               // Meat temperature, for rate of change function
float air_ = 20;                // BBQ air temperature, for rate of change function
int pin_air = 34;               // ESP32 pin number for input
int pin_meat = 35;
float vin = 3.28;                // Reference voltage of ESP32 ADC

// Meat probe
float R_m = 25000;                         // Resistance in ohms of your fixed resistor
float A_m = 0.8975771660 * pow(10, -3);    // "A" Coeffecient in Steinhart-Hart Equation
float B_m = 2.067767238 * pow(10, -4);     // "B"
float C_m = 1.742614938 * pow(10, -7);     // "C"

// Air aligator clip
float R_a = 1000;                        
float A_a = 0.9482846445 * pow(10, -3);    
float B_a = 1.952744345 * pow(10, -4);    
float C_a = 2.570293116 * pow(10, -7);    

// Value initialisation
float meat_r;                   // Rate change function initialisation
float air_r;                    
unsigned long time_m_ = 0;
unsigned long time_a_ = 0;


float adc_64(int pin_no) {
  // ADC average of 64 readings
  float a = 0;  // Add ADC values to this variable
  for (int x = 0; x<64; x++) {
    // Reading potentiometer value 64 times (~3ms), taking average
    a += analogRead(pin_no)/64;
    }
  return a;
}


float adc_cal(float a) {
  // ADC to V with calibration, it takes into account the limits of the ESP32 ADC converter
  float b = 0;  // Volts (V)
  if (a<18){
    b = -2;
  } else if (a>4076) {
    b = -1;
  } else if (a<2700) {
    b = (0.8194542611 * pow(a,1) + 142.7653067728)/1000;
  } else if (a>=2700) {
    b = (-0.0001385263 * pow(a,2) + 1.4919918635 * pow(a,1) - 630.5586748105)/1000;
  }
  return b;
}



float Temp(int pin_no, float R, float A, float B, float C){
  // Functions to get the probe temperatures when required.
  // Calculate temperature from raw input                                
  float v0_raw = adc_64(pin_no);
  float v0 = adc_cal(v0_raw);                                        // Converts analogue value to voltage
  //SerialBT.print("[Debug ");
  //SerialBT.print(v0_raw, 1);
  //SerialBT.print(" raw, ");
  //SerialBT.print(v0, 2);
  //SerialBT.print(" V] ");
  if (v0 == -1) {
    return -1;
  } else if (v0 == -2) {
    return -2;
  } else if ((v0 != -1) || (v0 != -2)) {
    float r0 = (((R * vin) / v0) - R);                           // Calculates resistance value of thermistor based on fixed resistor value and measured voltage
    float logr0 = log(r0);                                       // Natural log of thermistor resistance used in Steinhart-Hart Equation
    float logcubed0 = logr0 * logr0 * logr0;                     // The cube of the above value
    float k0 = 1.0 / (A + (B * logr0) + (C * logcubed0));        // Steinhart-Hart Equation to calculate temperature in Kelvin
    float c0 = k0 - 273.15;                                      // Convert temperature K to C
    return c0;
  } else {
    return -3;
  }
}


bool printTemp(float temp) {
  // Print temp with errors
  if (temp == -2) {
    Serial.print("LOW");
  } else if (temp == -1) {
    Serial.print("HIGH");
  } else if (temp == -3) {
    Serial.print("ERR");
  } else {
    Serial.print(temp, 1);
  }
  return true;
}


bool updateTemp(void *) {
  // Updates temperature readings   
  meat = Temp(pin_meat, R_m, A_m, B_m, C_m);
  air = Temp(pin_air, R_a, A_a, B_a, C_a);

  Serial.print("Air : ");
  Serial.print(air, 1);
  Serial.print(" C, Meat : ");
  Serial.print(meat, 1);
  Serial.println(" C.");

  return true;
}


bool updateTempRate(void *) {
    // Updates rate of change of temperature
    unsigned long currentTime = millis();
    
    // Get current temperatures
    float meatCurrent = Temp(pin_meat, R_m, A_m, B_m, C_m);
    float airCurrent = Temp(pin_air, R_a, A_a, B_a, C_a);

    // Calculate elapsed time in seconds
    float elapsedSeconds = (currentTime - time_m_) / 1000.0;

    // Avoid division by zero
    if (elapsedSeconds > 0) {
        // Calculate rate of change in °C/min
        meat_r = (meatCurrent - meat_) * 60 / elapsedSeconds;
        air_r = (airCurrent - air_) * 60 / elapsedSeconds;
    } else {
        meat_r = 0.0;
        air_r = 0.0;
    }

    // Update previous values for next iteration
    meat_ = meatCurrent;
    air_ = airCurrent;
    time_m_ = currentTime;

    // Debug output
    Serial.print("Air @ ");
    Serial.print(air_r, 1);
    Serial.print(" C/min, ");
    Serial.print("Meat @ ");
    Serial.print(meat_r, 1);
    Serial.println(" C/min.");

    return true;
}
