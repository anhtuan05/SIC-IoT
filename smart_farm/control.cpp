#include "control.h"
#include "devices.h"
#include "sensors.h"
#include "network.h"

extern float h, t;
extern uint8_t pump_state, fan_state, light_state;

void applyDeviceStates() {
  (pump_state == DEVICE_OFF) ? pumpTurnOff() : pumpTurnOn();
  (fan_state  == DEVICE_OFF) ? fanTurnOff()  : fanTurnOn();
  (light_state == DEVICE_OFF) ? lightTurnOff() : lightTurnOn();
}

void checkSensorErrors() {
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }
}