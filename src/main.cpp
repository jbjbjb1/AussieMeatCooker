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

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Serve the HTML page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Serve temperature data
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