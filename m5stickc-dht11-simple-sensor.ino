#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <M5StickC.h>

#define DHTPIN 33
#define DHTTYPE DHT11

float humidity = 0.;
float temperature = 0.;

DHT_Unified dht(DHTPIN, DHTTYPE);

int getVlevel() {
  float vbat = M5.Axp.GetBatVoltage();
  int vlevel = ( vbat - 3.2 ) / 0.8 * 100;
  if ( vlevel < 0 ) {
    vlevel = 0;
  }
  if ( 100 < vlevel ) {
    vlevel = 100;
  }
 
  return vlevel;
}

void showVatteryVolume() {
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(135, 0);
  M5.Lcd.printf("%3d%%", getVlevel());
}

void setup() {
  Serial.begin(115200);

  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
}

void loop() {
  M5.update();

  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));

    M5.Lcd.setTextSize(2);  
    M5.Lcd.setCursor(40, 20);
    M5.Lcd.println(String(event.temperature) + " C");
  }

  dht.humidity().getEvent(&event);
  
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(40, 40);
    M5.Lcd.println(String(event.relative_humidity) + " %");
  }

  showVatteryVolume();
  delay(1000);
}