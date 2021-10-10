// Update these with values suitable for your network.
byte mac[] = {0xFE, 0xA7, 0x3D, 0x80, 0xB4, 0xC2};
IPAddress ip(192, 168, 178, 22);
IPAddress server(192, 168, 178, 251);

const char recipeDataTopic[] = "recipe/data";
const char tareScaleTopic[] = "tare/scale";

EthernetClient ethClient;
PubSubClient client(ethClient);

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
}

void callback(char* topic, byte* payload, unsigned int len) {
  Serial.println("callback function called.");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient3")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(recipeDataTopic, "hello world");

      // ... and resubscribe
      client.subscribe(tareScaleTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
