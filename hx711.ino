HX711 scale;

const int HX711_dout = 18;
const int HX711_sck = 49;

const int interval = 750, timeOutInterval = 30000;
unsigned long delayStartPublishRecipe = 0, startTimeOut = 0;

bool timeOutOccured = false, delayTimeOut = false;
bool delayRunning = false, isTareActive = false;

int prevWeight = 0;
float currentWeight = -1.00f, tareWeight = 1000.0f;
float scaleFactorAddress = 10;


void hx711Setup() {
  scale.begin(HX711_dout, HX711_sck);
  scale.set_scale(scaleFactorAddress);
  
  delayStartPublishRecipe = millis();
  delayRunning = true;
}

void setIsTareActive(bool active) {
  isTareActive = active;
}

void hx711Loop() {

  if (delayRunning && (millis() - delayStartPublishRecipe) >= interval) {

    scale.power_up();

    if (isTareActive) {
      scale.callibrate_scale(1000, 5);
    }
    
    currentWeight = scale.get_units(3);
    Serial.println(currentWeight);
    
    delayStartPublishRecipe = millis();    
  }
}
