//Title: Meat Cooker - Digital BBQ Thermometer

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <arduino-timer.h>
#include "Temperature.h"
#include <SPIFFS.h>

const char* ssid = "AussieMeatCooker1";
const char* password = "12345678";

float update_r = 5000;          // Update rate for sensors, milliseconds (match to index.html)

auto t = timer_create_default(); // create a timer with default settings

AsyncWebServer server(80);


void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi as Access Point
    WiFi.softAP(ssid, password); // Start the access point
    Serial.println("Access Point Started");
    Serial.print("SSID: ");
    Serial.println(ssid);

    // Print the IP address of the ESP32 AP
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    // Setup SPIFFS which hosts the index.html website
    if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
    }

    // Start the web server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    Serial.println("Server started, listening for connections...");

    
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"meat\":" + String(meat) + 
                  ", \"air\":" + String(air) + 
                  ", \"meat_r\":" + String(meat_r) + 
                  ", \"air_r\":" + String(air_r) + "}";
    request->send(200, "application/json", json);
    });


    // Start the server
    server.begin();

    // Set up periodic tasks
    t.every(update_r, updateTemp);
    t.every(update_r, updateTempRate);
}


void loop() {
    // The main loop just ticks over the timer:
    t.tick();
}