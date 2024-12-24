//Title: Meat Cooker - Digital BBQ Thermometer

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
int pin_air = 34;               // ESP32 pin number for input
int pin_meat = 35;
float update_r = 5000;          // Update rate for sensors, milliseconds
int update_rr = 2;              // Factor slower to update temperature change rate
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


// ADC average of 64 readings
float adc_64(int pin_no) {
  float a = 0;  // Add ADC values to this variable
  for (int x = 0; x<64; x++) {
    // Reading potentiometer value 64 times (~3ms), taking average
    a += analogRead(pin_no)/64;
    }
  return a;
}

// ADC to V with calibration, it takes into account the limits of the ESP32 ADC converter
float adc_cal(float a) {
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


// Functions to get the probe temperatures when required.
float Temp(int pin_no, float R, float A, float B, float C){
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
    float r0 = (((R * vin) / v0) - R);                       // Calculates resistance value of thermistor based on fixed resistor value and measured voltage
    float logr0 = log(r0);                                       // Natural log of thermistor resistance used in Steinhart-Hart Equation
    float logcubed0 = logr0 * logr0 * logr0;                     // The cube of the above value
    float k0 = 1.0 / (A + (B * logr0) + (C * logcubed0));  // Steinhart-Hart Equation to calculate temperature in Kelvin
    float c0 = k0 - 273.15;                                      // Convert temperature K to C
    return c0;
  } else {
    return -3;
  }
}


// Print temp with errors
bool printTemp(float temp) {
  if (temp == -2) {
    SerialBT.print("LOW");
  } else if (temp == -1) {
    SerialBT.print("HIGH");
  } else if (temp == -3) {
    SerialBT.print("ERR");
  } else {
    SerialBT.print(temp, 1);
  }
  return true;
}


// Part Update temperatures for dsiplay
bool updateTemp(void *){
  SerialBT.print("Air : ");
  printTemp(Temp(pin_air, R_a, A_a, B_a, C_a));
  SerialBT.print(" C, ");
  
  SerialBT.print("Meat : ");
  printTemp(Temp(pin_meat, R_m, A_m, B_m, C_m));
  SerialBT.println(" C."); 
  return true;
}


// Part Update rate of temperature change
bool updateTempRate(void *){
  unsigned long time_m = millis();
  float meat_n = Temp(pin_meat, R_m, A_m, B_m, C_m);          // Get temperature now
  unsigned long time_a = millis();
  float air_n = Temp(pin_air, R_a, A_a, B_a, C_a);

  float meat_r = (meat_n - meat_) * 60 / ((time_m - time_m_) / 1000);                   // Rate of change meat, using clock for time
  float air_r = (air_n - air_) * 60 / ((time_a - time_a_) / 1000);                      // Rate of change air, using clock for time
  meat_ = meat_n;
  air_ = air_n;
  time_m_ = time_m;
  time_a_ = time_a;

  SerialBT.print("Air @ ");
  SerialBT.print(air_r, 1);
  SerialBT.print(" C/min, ");
  SerialBT.print("Meat @ ");
  SerialBT.print(meat_r, 1);
  SerialBT.println(" C/min.");

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
  t.every(update_r*update_rr, updateTempRate);
}                      

// The main loop just ticks over the timer:
void loop()
{
  t.tick(); 
}