#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LoRa.h>
#include <ArduinoJson.h>

// Pin definitions for LilyGO T3S3
#define LORA_SCK  5
#define LORA_MISO 3
#define LORA_MOSI 6
#define LORA_SS   7
#define LORA_RST  8
#define LORA_DIO0 33

// Configuration - these should be configurable
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* API_ENDPOINT = "http://localhost:3000/api/sensor-data";

// Gateway status
bool wifiConnected = false;
unsigned long lastConnectionAttempt = 0;
const unsigned long CONNECTION_RETRY_INTERVAL = 30000; // 30 seconds

void setupLoRa() {
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    
    Serial.println("LoRa Initializing OK!");
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("WiFi connection failed!");
        wifiConnected = false;
    }
}

void forwardToAPI(const String& jsonData) {
    if (!wifiConnected) {
        Serial.println("WiFi not connected, cannot forward data");
        return;
    }
    
    HTTPClient http;
    http.begin(API_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(jsonData);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("HTTP Response: %d - %s\\n", httpResponseCode, response.c_str());
    } else {
        Serial.printf("HTTP Error: %d\\n", httpResponseCode);
    }
    
    http.end();
}

void onLoRaReceive(int packetSize) {
    if (packetSize == 0) return;
    
    String receivedData = "";
    while (LoRa.available()) {
        receivedData += (char)LoRa.read();
    }
    
    Serial.printf("Received LoRa packet: %s\\n", receivedData.c_str());
    
    // Validate JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, receivedData);
    
    if (error) {
        Serial.printf("JSON parsing error: %s\\n", error.c_str());
        return;
    }
    
    // Add gateway metadata
    doc["gateway_id"] = WiFi.macAddress();
    doc["rssi"] = LoRa.packetRssi();
    doc["snr"] = LoRa.packetSnr();
    doc["received_at"] = millis();
    
    String enhancedData;
    serializeJson(doc, enhancedData);
    
    // Forward to API
    forwardToAPI(enhancedData);
}

void checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        wifiConnected = false;
        unsigned long currentTime = millis();
        
        if (currentTime - lastConnectionAttempt >= CONNECTION_RETRY_INTERVAL) {
            Serial.println("WiFi disconnected, attempting to reconnect...");
            setupWiFi();
            lastConnectionAttempt = currentTime;
        }
    } else {
        wifiConnected = true;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("CragCrowd Gateway Starting...");
    
    setupLoRa();
    setupWiFi();
    
    // Set up LoRa receive callback
    LoRa.onReceive(onLoRaReceive);
    LoRa.receive();
    
    Serial.println("Gateway setup complete. Listening for sensor data...");
}

void loop() {
    checkWiFiConnection();
    
    // The main work is done in the LoRa callback
    // Just need to keep the loop running and check connections
    delay(1000);
}