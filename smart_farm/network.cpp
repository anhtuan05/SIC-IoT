#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "my_config.h"
#include "network.h"
#include "devices.h"
#include "sensors.h"

HTTPClient http;
WiFiClient client;

uint64_t last_post_time = 0, last_get_time = 0;
const uint64_t post_time = 5000, get_time = 2000;

void WifiInit() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void postData(uint64_t now)  {
  if (now - last_post_time >= post_time) {
    if (WiFi.status() == WL_CONNECTED) {
      http.begin(client, serverNameSensor);
      http.addHeader("Content-Type", "application/json");

      String jsonPayload = "{\"temperature\":";
      jsonPayload += String(t, 2);
      jsonPayload += ",\"humidity_air\":";
      jsonPayload += String(h, 1);
      jsonPayload += ",\"humidity_soil\":";
      jsonPayload += String(moisture, 1);
      jsonPayload += "}";

      int httpResponseCode = http.POST(jsonPayload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    last_post_time = now;
  }
}

void postState(String device, uint8_t device_state)  {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverNameControl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"device\":";
    if (device == "pump") {
      jsonPayload += "\"pump\",";
    } else if (device == "light") {
      jsonPayload += "\"light\",";
    } else if (device == "fan") {
      jsonPayload += "\"fan\",";
    }
    jsonPayload += "\"state\":";
    if (device_state == DEVICE_ON) {
      jsonPayload += "true";
    } else if (device_state == DEVICE_OFF) {
      jsonPayload += "false";
    }
    jsonPayload += "}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void getState(uint64_t now) {
  if (now - last_get_time >= get_time) {
    if (WiFi.status() == WL_CONNECTED) {
      http.begin(client, serverNameStateGet);

      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println("Raw JSON: " + payload);

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          fan_state = doc["data"]["fan"]["state"];
          light_state = doc["data"]["light"]["state"];
          pump_state = doc["data"]["pump"]["state"];
        } else {
          Serial.println("JSON parse failed: " + String(error.c_str()));
        }
      } else {
        Serial.println("Error on GET: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    last_post_time = now;
  }
}