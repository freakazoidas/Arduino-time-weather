#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>

const char* ssid = "Cgates_BF90";
const char* password = "74097494";
const char* ntpServer = "pool.ntp.org";
const long utcOffsetInSeconds = 7200;

const char* weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=Vilnius,LT&appid=2481208bf97323e600991b6841f7fbc6&units=metric";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);
unsigned long lastNTPUpdate = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  timeClient.begin();
  timeClient.update();
  setTime(timeClient.getEpochTime()); // set the time using NTPClient's epoch time

  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.setCursor(6, 0);

  char dateStr[11]; // Buffer to hold formatted date string
  sprintf(dateStr, "%04d-%02d-%02d", year(), month(), day());
  lcd.print(dateStr);

  lcd.setCursor(0, 1);
  lcd.print("Time:   ");
  lcd.setCursor(6, 1);
  lcd.print(timeClient.getFormattedTime());

  lcd.setCursor(0, 2);
  lcd.print("Weather: ");

  lcd.setCursor(0, 3);
  lcd.print("Temp:   ");
}


void loop() {
  if (millis() - lastNTPUpdate >= 3600000) {
    timeClient.update();
    lastNTPUpdate = millis();
  }

  lcd.setCursor(6, 0);
  char dateStr[11];
  sprintf(dateStr, "%04d-%02d-%02d", year(), month(), day());
  lcd.print(dateStr);

  lcd.setCursor(6, 1);
  lcd.print(timeClient.getHours()); // print the hours
  lcd.print(":");
  if (timeClient.getMinutes() < 10) {
    lcd.print("0");
  }
  lcd.print(timeClient.getMinutes()); // print the minutes
  lcd.print(":");
  if (timeClient.getSeconds() < 10) {
    lcd.print("0");
  }
  lcd.print(timeClient.getSeconds()); // print the seconds

  WiFiClient client;
  HTTPClient http;
  http.begin(client, weatherURL);

  int httpCode = http.GET();
  String payload = http.getString();

  float temp = 0.0;
  String description = "";
  if (httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (!error) {
      temp = doc["main"]["temp"].as<float>();
      description = doc["weather"][0]["description"].as<String>();

      lcd.setCursor(9, 2);
      lcd.print(description);

      lcd.setCursor(6, 3);
      lcd.print(String(temp, 1));
      lcd.print(" C    ");
    }
  }

  delay(1000);
}
