#include "my_config.h"

// Soil calibration
const uint16_t SOIL_WET = 492;
const uint16_t SOIL_DRY = 1024;

// Thresholds
const float TEMP_BASE = 34.0;
const float MOISTURE_BASE = 50.0;

// Wifi
const char* ssid = "RiandRi";
const char* password = "Btduo1314";

// Server endpoints
const char* serverNameSensor = "http://anhtuan0503.pythonanywhere.com/api/sensor";
const char* serverNameControl = "http://anhtuan0503.pythonanywhere.com/api/control";
const char* serverNameStateGet = "http://anhtuan0503.pythonanywhere.com/api/control/status";