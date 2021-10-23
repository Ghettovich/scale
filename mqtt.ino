// Update these with values suitable for your network.
byte mac[] = {0xFE, 0xA7, 0x3D, 0x80, 0xB4, 0xC2};
IPAddress ip(192, 168, 178, 23);
IPAddress server(192, 168, 178, 251);

const char recipeDataTopic[] = "recipe/data";
const char calibrateScaleTopic[] = "scale/calibrate";
const char tareScaleTopic[] = "scale/tare";

EthernetClient ethClient;
PubSubClient client(ethClient);

bool delayPublishScaleData = true;
unsigned long delayPublishScaleStart = 0;
const int intervalPublishData = 3000;

bool flagPublishData = true;

void setupMqttClient() {
  client.setBufferSize(512);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void mqttLoop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (flagPublishData && delayPublishScaleData &&
      (millis() - delayPublishScaleStart >= intervalPublishData)) {
    publishScaleData();
    delayPublishScaleStart = millis();
    Serial.println("published recipe data");
  }
}

void callback(char* topic, byte* payload, unsigned int len) {
  if (strcmp(calibrateScaleTopic, topic) == 0) {
    Serial.println("calibrating scale");

    DynamicJsonDocument doc(len);
    deserializeJson(doc, payload);

    int scaleId = doc["id"];
    float weight = doc["weight"];
    bool confirm = doc["confirm"];

    if (confirm) {
      setWeightPlaced(true);
    }
    else {
      setScaleIdToCalibrate(scaleId);
      setCalibrationWeight(weight);

      tareScaleHx711();
    }
  }
  if (strcmp(tareScaleTopic, topic) == 0) {

    DynamicJsonDocument doc(len);
    deserializeJson(doc, payload);

    int scaleId = doc["id"];

    if (scaleId) {
      Serial.println("tare scale");

      tareScaleHx711();
    }
  }
}

void publishScaleData() {
  char payload[64];
  DynamicJsonDocument doc(64);

  doc["scaleId"] = 1;
  doc["weight"] = getCurrentWeightScale1();

  serializeJson(doc, payload);

  client.publish(recipeDataTopic, payload);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient3")) {
      client.subscribe(tareScaleTopic);
      delay(250);
      client.subscribe(calibrateScaleTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
