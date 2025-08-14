#include "devices.h"
#include "my_config.h"

uint8_t pump_state = DEVICE_OFF, light_state = DEVICE_OFF, fan_state = DEVICE_OFF;

void deviceInit() {
  pumpTurnOff();
  fanTurnOff();
  lightTurnOff();
}

void fanTurnOn() {
  digitalWrite(FAN_PIN, HIGH);
}

void fanTurnOff() {
  digitalWrite(FAN_PIN, LOW);
}

void lightTurnOn() {
  digitalWrite(LIGHT_PIN, HIGH);
}

void lightTurnOff() {
  digitalWrite(LIGHT_PIN, LOW);
}

void pumpTurnOn() {
  digitalWrite(PUMP_PIN, HIGH);
}

void pumpTurnOff() {
  digitalWrite(PUMP_PIN, LOW);
}