#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const int serverPort = 5000;

const int sense_pin = 34;
float a0 = 0.0668152332f;
float a1 = 0.0008833165f;
float a2 = -2.2913611837e-08f;
float a3 = -9.1713465978e-13f;
const int ADC_THRESH = 30;
const float P_MAX = 20.68f; //bar
const float V_ZERO = 0.5f; //volts
const float V_FULL = 4.5f;
const float DIV_RATIO = 1.5f;

float adc_to_bar(int adc);


WiFiServer server(serverPort);
const char* appPassword = "login_bursttest";

void setup()
{
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(sense_pin,ADC_11db);

  WiFi.softAP(ssid,password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP: "); Serial.println(IP);
  server.begin();
  server.setNoDelay(true);
}

void loop()
{
  WiFiClient client = server.available();

  if (!client) return;

  unsigned long start = millis();
  String line = "";
  while (millis() - start < 5000) {  // 5s timeout
    if (client.available()) {
      line = client.readStringUntil('\n');
      line.trim();
      break;
    }
  }

  if (line != appPassword) {
    Serial.println("Wrong password");
    client.println("AUTH FAILED");
    client.stop();
    return;
  }

  client.println("AUTH OK");
  Serial.println("Client authenticated");



  while (client && client.connected())
  {
    client.println(adc_to_bar(analogRead(sense_pin)));
    delay(50);
  }
  client.stop();
  Serial.println("Client Disconnected");
  
}



float adc_to_bar(int adc)
{
    float Vsensor;

    if (adc < ADC_THRESH) 
    {
        Vsensor = 0.0f;
    } else 
    {
        float Vadc = ((a3 * adc + a2) * adc + a1) * adc + a0;
        Vsensor = Vadc * DIV_RATIO;
    }

    Serial.println(Vsensor);

    if (Vsensor <= V_ZERO)
        return 0.0f;
    if (Vsensor >= V_FULL)
        return P_MAX;

    float bar = (Vsensor - V_ZERO) / (V_FULL - V_ZERO) * P_MAX;
    if (bar < 0.0f) bar = 0.0f;

    return bar;
}
