#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>

const char* const SSID = "Cgates_BF90";
const char* const PASSWORD = "74097494";
const char* const NTP_SERVER = "pool.ntp.org";
const long UTC_OFFSET = 7200;
const char* const WEATHER_URL = "http://api.openweathermap.org/data/2.5/weather?q=Vilnius,LT&appid=2481208bf97323e600991b6841f7fbc6&units=metric";
const uint8_t LCD_COLS = 20;
const uint8_t LCD_ROWS = 4;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, UTC_OFFSET);
unsigned long lastNTPUpdate = 0;

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

void setup() {
  lcd.init();
  lcd.backlight();

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  timeClient.begin();
  timeClient.update();
  setTime(timeClient.getEpochTime()); // set the time using NTPClient's epoch time
}

void printDateTime() {
  char dateStr[11]; // Buffer to hold formatted date string
  sprintf(dateStr, "%04d-%02d-%02d", year(), month(), day());
  lcd.setCursor(0, 0);
  lcd.print(dateStr);

  lcd.setCursor(0, 1);
  lcd.print(timeClient.getFormattedTime());
}


void printWeather() {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, WEATHER_URL);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, http.getString());

    float temp = doc["main"]["temp"].as<float>();
    String description = doc["weather"][0]["description"].as<String>();

    lcd.setCursor(0, 2);
    lcd.print(description);

    lcd.setCursor(0, 3);
    lcd.print(String(temp, 1) + " C    ");
  }
}

void loop() {
  if (millis() - lastNTPUpdate >= 3600000) {
    timeClient.update();
    lastNTPUpdate = millis();
  }

  printDateTime();
  printWeather();

  delay(1000);
}
