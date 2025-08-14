#include "sensors.h"
#include "my_config.h"
#include "network.h"
#include "devices.h"
#include <DHT.h>

DHT dht(DHT_PIN, DHT_TYPE);

uint64_t last_time_soil = 0, last_time_dht = 0;
const uint64_t soil_time = 5000, dht_time = 3000;
uint16_t soil;
float h, t, moisture;

void sensorInit() {
  pinMode(SOIL_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  
  dht.begin();
}

void getDataDHT(uint64_t now) {
  if (now - last_time_dht >= dht_time) {
    h = dht.readHumidity();
    t = dht.readTemperature();

    if (t < TEMP_BASE) {
      if (light_state != DEVICE_ON) {
        light_state = DEVICE_ON;
        postState("light", light_state);
      }

      if (fan_state != DEVICE_OFF) {
        fan_state = DEVICE_OFF;
        postState("fan", fan_state);
      }
    } else {
      if (light_state != DEVICE_OFF) {
        light_state = DEVICE_OFF;
        postState("light", light_state);
      }
      
      if (fan_state != DEVICE_ON) {
        fan_state = DEVICE_ON;
        postState("fan", fan_state);
      }
    }
    
    printDHT(h, t);

    last_time_dht = now;
  } else return;
}

void getDataSoil(uint64_t now) {
  if (now - last_time_soil >= soil_time) {
    soil = analogRead(SOIL_PIN);
    moisture = convertMoisture(soil);

    if (moisture < MOISTURE_BASE) {
      if (pump_state != DEVICE_ON) {
        pump_state = DEVICE_ON;
        postState("pump", pump_state);
      }
    } else if (moisture >= MOISTURE_BASE) {
      if (pump_state != DEVICE_OFF) {
        pump_state = DEVICE_OFF;
        postState("pump", pump_state);
      }
    }

    printMoisture(moisture);

    last_time_soil = now;
  } else return;
}

float convertMoisture(uint16_t soil) {
  float percent = ((1.0*(SOIL_DRY - soil))/(SOIL_DRY - SOIL_WET))*100;
  return percent;
}


void printDHT(float h, float t) {
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  if (fan_state == DEVICE_OFF) {
    Serial.println("Fan off!");
  } else {
    Serial.println("Fan on!");
  }
  if (light_state == DEVICE_OFF) {
    Serial.println("Light off!");
  } else {
    Serial.println("Light on!");
  }
}

void printMoisture(float moisture) {
  Serial.print("Raw: ");
  Serial.print(soil);
  Serial.print("\tMoisture: ");
  Serial.print(moisture);
  Serial.println("%");
  if (pump_state == DEVICE_OFF) {
    Serial.println("Pump off!");
  } else {
    Serial.println("Pump on!");
  }
}
