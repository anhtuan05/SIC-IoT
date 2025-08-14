#ifndef DEVICES_H
#define DEVICES_H

#include <Arduino.h>

enum DEVICE_STATE {
  DEVICE_OFF = 0,
  DEVICE_ON
};

extern uint8_t pump_state, light_state, fan_state;

void deviceInit();
void fanTurnOn();
void fanTurnOff();
void lightTurnOn();
void lightTurnOff();
void pumpTurnOn();
void pumpTurnOff();

#endif