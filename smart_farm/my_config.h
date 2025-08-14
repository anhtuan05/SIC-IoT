#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#include <Arduino.h>

// Pin define
#define DHT_PIN 5 //D0
#define DHT_TYPE DHT11  
#define SOIL_PIN A0
#define PUMP_PIN 4 //D2
#define LIGHT_PIN 14  //D5
#define FAN_PIN 2 //D4

// Soil calibration
extern const uint16_t SOIL_WET;
extern const uint16_t SOIL_DRY;

// Thresholds
extern const float TEMP_BASE;
extern const float MOISTURE_BASE;

// Wifi
extern const char* ssid;
extern const char* password;

// Server endpoints
extern const char* serverNameSensor;
extern const char* serverNameControl;
extern const char* serverNameStateGet;

#endif