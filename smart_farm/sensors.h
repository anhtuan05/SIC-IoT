#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

extern uint64_t last_time_soil, last_time_dht;
extern const uint64_t soil_time, dht_time;
extern uint16_t soil;
extern float h, t, moisture;

void sensorInit();
void getDataDHT(uint64_t now);
void getDataSoil(uint64_t now);
float convertMoisture(uint16_t soil);

void printDHT(float h, float t);
void printMoisture(float moisture);

#endif