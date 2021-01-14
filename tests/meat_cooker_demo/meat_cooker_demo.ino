//Title: Wifi BBQ Thermometer

// Part 1: This initializes the LCD using the I2C converter, holds WiFi connection info, your Pushingbox DevID, and the global variables that we will need:

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Event.h>
#include <Timer.h>
#include <Dns.h>
#include <Dhcp.h>
#include <util.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
                       
#define I2C_ADDR    0x3F
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

char ssid[] = "YOUR NETWORK NAME";  
char pass[] = "YOUR PASSWORD";  
int status = WL_IDLE_STATUS;    

char devid[] = "YOUR DEVID FROM PUSHINGBOX";
char pbserver[] = "api.pushingbox.com";

WiFiServer server(80);
WiFiClient client;

Timer t;

int lowTemp = 200;              //  Low limit warning for smoker temp
int highTemp = 250;            // High limit warning for smoker temp
int meatDone = 201;            // Temperature at which your meat is done
int lowlim = 20;               // Values below this will considered erroneous

float air;                      //  Smoker temperature
float meat;                     //   Meat temperature
float vin = 4.999;              //  DC Voltage as measured with DMM between +5V and GND
float r2 = 9920;                // Resistance in ohms of your fixed resistor

float A = 0.00004556837992;     // "A" Coeffecient in Steinhart-Hart Equation
float B = 0.0002634285613;      // "B"
float C = 0.00000005286310982;  // "C"

// Part 2: The "Setup" function connects to WiFi, displays connection info on the LCD, and calls our other functions (readSensors, updateDisplay, and sendData) on a fixed interval based on Timer "t":

void setup()
{
 Serial.begin(9600);
 analogReference(DEFAULT);                   //use default 5V reference
 lcd.begin(16,2);                             //initialize LCD output set up the LCD's number of columns and rows:
 lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(HIGH);
 lcd.clear();
 lcd.home();
 lcd.print("WiFi Smoker");
 delay(2000);
 
 // attempt to connect using WPA2 encryption:

 Serial.println("Attempting to connect to WPA network...");
 lcd.clear();
 lcd.home();
 lcd.print("Connecting...");
 status = WiFi.begin(ssid, pass);

 // if you're not connected, stop here:
 if (status != WL_CONNECTED)
 {                                
   Serial.println("Couldn't get a wifi connection");
   lcd.clear();
   lcd.home();
   lcd.print("WiFi Error");
   while(true);
 }
 // if you are connected, print out info about the connection:
 else
 {
   server.begin();
   Serial.println("Connected to network. IP: ");
   IPAddress myAddress = WiFi.localIP();
   Serial.println(myAddress);
   lcd.clear();
   lcd.home();
   lcd.print("Connected!");
 }
 
 
 t.every(2000, readSensors);
 t.every(3000, updateDisplay);
 t.every(60000, sendData);
       
}                      

// Part 3: The main loop essentially just updates the timer and runs the built in "web page" that serves up the temperature data:

void loop()
{
  t.update();
 
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("Cook Time: ");
          client.println(float(float(millis()/1000)/60));
          client.println(" m");
          client.println("<br>");
          client.print("Smoker Temp: ");
    if (air > lowlim)
    {
      client.print(air);
    } else {
      client.print("NA "); //values below 20F are probably due to disconnected probe
    }
    client.println("<br>");
    client.print("Meat Temp: ");
    if (meat > lowlim)
    {
      client.print(meat);
    }
    else
    {
      client.print("NA "); //values below 20F are probably due to disconnected probe
    }
    client.println("<br>");
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

 
}

// Part 4: The "readSensors" function does exactly that; it reads the sensor and calculates the temperature:

void readSensors()
{
float a0 = analogRead(0);                                    // This reads the "voltage" value on A0. Value is actually divided into 1024 steps from 0-1023.
float v0 = a0 * .0049;                                       // Converts A0 value to an actual voltage (5.0V / 1024 steps = .0049V/step.
float r0 = (((r2 * vin) / v0) - r2);                         // Calculates resistance value of thermistor based on fixed resistor value and measured voltage
float logr0 = log(r0);                                       // Natural log of thermistor resistance used in Steinhart-Hart Equation
float logcubed0 = logr0 * logr0 * logr0;                     // The cube of the above value
float k0 = 1.0 / (A + (B * logr0) + (C * logcubed0));        // Steinhart-Hart Equation to calculate temperature in Kelvin
float f0 = (1.8 * (k0 - 273)) + 32;                          // Convert temperature to Fahrenheit

if (isnan(f0))                                               // If value is not a number, assign an arbitrary value
{
   air = int(1);
}
else
{
  air = f0;                                                    // Otherwise use the calculated value
}

float a1 = analogRead(1);                                    // Same code as above. Repeat for as many sensors as you need to connect.
float v1 = a1 * .0049;
float r1 = (((r2 * vin) / v1) - r2);
float logr1 = log(r1);
float logcubed1 = logr1 * logr1 * logr1;
float k1 = 1.0 / (A + (B * logr1) + (C * logcubed1));
float f1 = (1.8 * (k1 - 273)) + 32;

if (isnan(f1))
{
   meat = int(1);
}
else
{
meat = f1;
}
}

// Part 5: The "updateDisplay" function is pretty self explanatory. It displays the temperature values on the LCD:

void updateDisplay()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smoker:");
  if (air < lowlim)
  {
    lcd.print(" Error");
  }
  else if (air > lowlim & air < 100)
  {
    lcd.print(" "); //pad with a space
    lcd.print(air);
    lcd.print(" F");
  }
  else
  {
    lcd.print(air);
    lcd.print(" F");
  }
  lcd.setCursor(0,1);
  lcd.print("Meat:");
  if (meat < lowlim)
  {
    lcd.print(" Error");
  }
  else if (meat > lowlim & meat < 100)
  {
    lcd.print(" "); //pad with a space
    lcd.print(meat);
    lcd.print(" F");
  }
  else
  {
    lcd.print(meat);
    lcd.print(" F");
  }
}

// Part 6: The "sendData" function is used to send alerts via the Pushingbox API and Prowl if the smoker temperature is outside of certain values, or if the meat temperature is above the desired value:

void sendData()
{
 char event[80];
 
 int air1;                                              // We will convert the float values to int so that the Pushingbox API can handle them more cleanly
 int meat1;
 
 air1 = (int) air;
 meat1 = (int) meat;

 if (air < lowTemp)
 {
   sprintf(event, "Low+Temp:+%d*F", air1);            // The code below sends the text in the string "event" to Pushingbox which in turn sends the data to
                                                      // Prowl to push it to your iPhone/device of choice. The "+" is used in place of a space, but it is formatted
   if(client.connect(pbserver,80))                    // correctly when it shows up on my phone.
   {
     Serial.println("Connected to URL.");
     Serial.println();

     client.print("POST /pushingbox?devid=");
     client.print(devid);
     client.print("&event=");
     client.print(event);
     client.println(" HTTP/1.1");
     client.println("Host: api.pushingbox.com");
     client.println("User-Agent: arduino-ethernet");
     client.println("Connection: close");
     client.println();
   
     Serial.print("POST /pushingbox?devid=");
     Serial.print(devid);
     Serial.print("&event=");
     Serial.print(event);
     Serial.println(" HTTP/1.1");
     Serial.println("Host: api.pushingbox.com");
     Serial.println("Connection: close");
     Serial.println();
   
     delay(2000);
     client.stop();
   }
   else
   {
     client.stop();
   }  
 }
 
 if (air > highTemp)
 {
   sprintf(event, "High+Temp:+%d*F", air1);
   
   if(client.connect(pbserver,80))
   {
     Serial.println("Connected to URL.");
     Serial.println();

     client.print("POST /pushingbox?devid=");
     client.print(devid);
     client.print("&event=");
     client.print(event);
     client.println(" HTTP/1.1");
     client.println("Host: api.pushingbox.com");
     client.println("User-Agent: arduino-ethernet");
     client.println("Connection: close");
     client.println();
   
     Serial.print("POST /pushingbox?devid=");
     Serial.print(devid);
     Serial.print("&event=");
     Serial.print(event);
     Serial.println(" HTTP/1.1");
     Serial.println("Host: api.pushingbox.com");
     Serial.println("Connection: close");
     Serial.println();
   
     delay(2000);
     client.stop();
   }
   else
   {
     client.stop();
   }  
 }
 
 if (meat > meatDone)
 {
   sprintf(event, "Meat+Is+Done!+%d*F", meat1);
   
   if(client.connect(pbserver,80))
   {
     Serial.println("Connected to URL.");
     Serial.println();

     client.print("POST /pushingbox?devid=");
     client.print(devid);
     client.print("&event=");
     client.print(event);
     client.println(" HTTP/1.1");
     client.println("Host: api.pushingbox.com");
     client.println("User-Agent: arduino-ethernet");
     client.println("Connection: close");
     client.println();
   
     Serial.print("POST /pushingbox?devid=");
     Serial.print(devid);
     Serial.print("&event=");
     Serial.print(event);
     Serial.println(" HTTP/1.1");
     Serial.println("Host: api.pushingbox.com");
     Serial.println("Connection: close");
     Serial.println();
   
     delay(2000);
     client.stop();
   }
   else
   {
     client.stop();
   }  
 }
 else
  {
  }
}