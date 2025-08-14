#include "my_config.h"
#include "network.h"
#include "devices.h"
#include "sensors.h"
#include "control.h"

enum CONTROL_TYPE {
  AUTO = 0,
  MANUAL
};

uint64_t now_time;

void setup() {
  Serial.begin(9600);
  deviceInit();
  sensorInit();
  WifiInit();

  delay(2000);
}

void loop() {
  now_time = millis();

  getDataDHT(now_time);
  getDataSoil(now_time);
  postData(now_time);
  getState(now_time);

  applyDeviceStates();
  checkSensorErrors();
}

