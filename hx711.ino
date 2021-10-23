HX711 scale;

const int HX711_dout = 18;
const int HX711_sck = 49;

const int interval = 750, timeOutInterval = 30000;
unsigned long delayStartReadScale = 0;

bool timeOutOccured = false, delayTimeOut = false;
bool delayRunning = false, isTareActive = false, weightPlaced = false;

int prevWeight = 0;
float currentWeightScale1 = -1.00f, calibrationWeight = 1000.0f;

float scaleFactor = 0;
int scaleFactorAddress;

int scaleId;

void hx711Setup() {
  scaleFactor = EEPROM.readFloat(scaleFactorAddress);
  Serial.print("scale factor value (eeprom) ");
  Serial.println(scaleFactor);

  scale.begin(HX711_dout, HX711_sck);
  
  scale.tare();
  scale.set_scale(scaleFactor);

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  delayStartReadScale = millis();
  delayRunning = true;
}

void setTareActive(bool active) {
  isTareActive = active;
}

void setWeightPlaced(bool _placed) {
  weightPlaced = _placed;
}

void setCalibrationWeight(float weight) {
  calibrationWeight = weight;
}

void setScaleIdToCalibrate(int id) {
  scaleId = id;
}

void tareScaleHx711() {
  scale.tare();
}

float getCurrentWeightScale1() {
  return currentWeightScale1;
}

void calibrateScale() {
  scale.calibrate_scale(calibrationWeight, 5);
  
  float output = scale.get_scale();
  Serial.print("Scale factor value = ");
  Serial.println(output);

  EEPROM.updateFloat(scaleFactorAddress, output);
  scale.set_scale(output);

  Serial.print("calibrated scale with weight : ");
  Serial.println(calibrationWeight);

  Serial.print("scale factor is ");
  Serial.println(EEPROM.readFloat(scaleFactorAddress));
}

void hx711Loop() {

  if (delayRunning && (millis() - delayStartReadScale) >= interval) {

    if (weightPlaced) {
      calibrateScale();
      weightPlaced = false;
    }

    scale.power_up();
    currentWeightScale1 = scale.get_units(3);
    delayStartReadScale = millis();
  }
}
