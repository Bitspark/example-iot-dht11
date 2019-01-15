#include "definitions.h"

DHT dht(D3, DHT11);

unsigned long lastMeasure = 0;
unsigned long lastLight = 0;

void setup() {
  Serial.begin(9600);

  dht.begin();

  ledSetup();
  mqttSetup();

  lastMeasure = millis();
}

void loop() {
  mqttLoop();

  if (lastLight != 0 && millis() - lastLight > 2000) {
    lastLight = millis();
    ledsOff();
  }

  if (millis() - lastMeasure > 10000) {
    lastMeasure = millis();

    ledOn(LED_BLUE);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    ledOff(LED_BLUE);

    if (isnan(humidity) && isnan(temperature)) {
      ledOn(LED_RED);
    } else {
      ledOn(LED_GREEN);
      char str[16];
      sprintf(str, "%.0f %.0f", temperature, humidity);
      mqttPublish("/bitspark/office/plant/coffee1/dht", str);
    }

    lastLight = millis();
  }
}

void messageReceived(String &topic, String &payload) {}

void subscribe() {}
