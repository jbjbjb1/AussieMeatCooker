//Title: Meat Cooker - Digital BBQ Thermometer

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <arduino-timer.h>
#include "Temperature.h"

const char* ssid = "AussieMeatCooker1";
const char* password = "12345678";

AsyncWebServer server(80);
auto t = timer_create_default();

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Serve temperature data
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "{\"meat\":" + String(meat) + ", \"air\":" + String(air) + "}";
        request->send(200, "application/json", json);
    });

    server.begin();

    t.every(update_r, updateTemp);
    t.every(update_r * update_rr, updateTempRate);
}

void loop() {
    t.tick();
}