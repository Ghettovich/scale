#include "HX711.h"
#include <SPI.h>
#include <EthernetENC.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROMex.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Scale arduino");

  hx711Setup();
  Serial.println("Bin scale setup ready");

  setupMqttClient();
  Serial.println("Mqtt client Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:
  hx711Loop();
  mqttLoop();
}
