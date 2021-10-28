#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#endif


const char* ssid = "######";
const char* password = "q1234!@#$";
String serverName = "http://api.app.........#####cloud:8000/pir/";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

const int pir = 12;     //pir
const int led = 22;     //led

int ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  int pir_stat = digitalRead(pir);
  //  boolean requested = false;

  if (pir_stat == HIGH) {                       //requested == !requested
    if ((millis() - lastTime) > timerDelay) {
      if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(led, HIGH);
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);

        http.addHeader("Content-Type", "application/json");
        StaticJsonDocument<200> doc;
        doc["sensor"] = "PIR";
//        doc["time"] = 20210824;
        JsonArray data = doc.createNestedArray("MacAddress");
        data.add(WiFi.macAddress());

        String request;
        serializeJsonPretty(doc, request);

        int httpResponseCode = http.POST(request);

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
      lastTime = millis();
    }
    //    delay(8000);
    //    requested = true;
  }
  else {
    Serial.println("motion undetected_NO PIR API REQUESTED");
    //    requested = false;
    digitalWrite(led, LOW);
  }
  delay(3000);
}
