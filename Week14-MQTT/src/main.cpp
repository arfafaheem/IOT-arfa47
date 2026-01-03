/****************************************************
 * ESP32 + MQTT Subscriber + OLED (Temp only)
 * Subscribes: home/lab2/temp
 * Displays temperature on SSD1306 OLED
 ****************************************************/

#include <WiFi.h>
#include <PubSubClient.h> 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- WiFi ----------
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ---------- MQTT ----------
const char* mqtt_server = "10.13.18.69"; // Change to your MQTT broker IP
const int mqtt_port = 1883;

const char* TOPIC_TEMP = "home/lab2/temp"; // Topic for temperature

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- MQTT ----------
WiFiClient espClient;
PubSubClient mqtt(espClient);

// latest temperature
String lastTemp = "--"; // initial display value

// Function to update OLED display

void showTemp() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MQTT Temp Monitor");
  display.println(TOPIC_TEMP);
  display.println("----------------");

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print("Temp: ");
  display.print(lastTemp);
  display.println("C");

  display.display();
}

// MQTT callback: runs when message arrives
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  if (String(topic) == TOPIC_TEMP) {
    lastTemp = msg;   // store received value
    Serial.print("Temp received: ");
    Serial.println(lastTemp);
    showTemp();       // update OLED
  }
}

void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// Connect to MQTT broker as subscriber
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting MQTT...");
    if (mqtt.connect("subscriber-1")) {
      Serial.println("connected");
      mqtt.subscribe(TOPIC_TEMP);
      showTemp();
    } else {
      Serial.print("failed rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // OLED init (ESP32 default: SDA=21, SCL=22)
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true) delay(3);
  }
  display.clearDisplay();
  display.display();

  showTemp();       // initial screen
  connectWiFi();

  mqtt.setServer(mqtt_server, mqtt_port); // MQTT broker
  mqtt.setCallback(callback);
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop(); 
}


//topic subscriber wala, mqtt out

/****************************************************
 * ESP32 + DHT22 + MQTT (PUBLISHER ONLY)
 * Topic:
 *   home/lab1/temp
 *   home/lab1/hum
 ****************************************************/

// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include "DHT.h"

// // ---------- WiFi ----------
// char ssid[] = "Wokwi-GUEST";
// char pass[] = "";

// // ---------- MQTT ----------
// const char* mqtt_server = "10.13.18.69";  // Mosquitto / Cloud broker
// const int   mqtt_port   = 1883;

// // Topics
// const char* TOPIC_TEMP = "home/lab1/temp";
// const char* TOPIC_HUM  = "home/lab1/hum";

// // ---------- DHT ----------
// #define DHTPIN  23
// #define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // ---------- MQTT Client ----------
// WiFiClient espClient;
// PubSubClient mqtt(espClient);

// // ---------- Functions ----------
// void connectWiFi() {
//   Serial.print("Connecting to WiFi");
//   WiFi.begin(ssid, pass);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected");
// }

// void connectMQTT() {
//   while (!mqtt.connected()) {
//     Serial.print("Connecting to MQTT...");
//     if (mqtt.connect("ESP32_Publisher")) {
//       Serial.println("connected");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.println(mqtt.state());
//       delay(2000);
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);

//   dht.begin();
//   connectWiFi();

//   mqtt.setServer(mqtt_server, mqtt_port);
//   connectMQTT();
// }

// void loop() {
//   if (!mqtt.connected()) {
//     connectMQTT();
//   }
//   mqtt.loop();

//   float temperature = dht.readTemperature();
//   float humidity    = dht.readHumidity();

//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("DHT read failed");
//     delay(2000);
//     return;
//   }

//   char tBuf[8], hBuf[8];
//   dtostrf(temperature, 4, 2, tBuf);
//   dtostrf(humidity,    4, 2, hBuf);

//   mqtt.publish(TOPIC_TEMP, tBuf);
//   mqtt.publish(TOPIC_HUM,  hBuf);

//   Serial.print("Published -> Temp: ");
//   Serial.print(tBuf);
//   Serial.print(" C | Hum: ");
//   Serial.print(hBuf);
//   Serial.println(" %");

//   delay(5000);  // publish every 5 seconds
// }
// /****************************************************
//  * ESP32 + DHT22 + MQTT (PUBLISHER ONLY)
//  * Topic:
//  *   home/lab1/temp
//  *   home/lab1/hum
//  ****************************************************/

// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include "DHT.h"

// // ---------- WiFi ----------
// char ssid[] = "Wokwi-GUEST";
// char pass[] = "";

// // ---------- MQTT ----------
// const char* mqtt_server = "192.168.81.241";  // Mosquitto / Cloud broker
// const int   mqtt_port   = 1883;

// // Topics
// const char* TOPIC_TEMP = "homea/lab1a/tempa";
// const char* TOPIC_HUM  = "homea/lab1a/huma";

// // ---------- DHT ----------
// #define DHTPIN  23
// #define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // ---------- MQTT Client ----------
// WiFiClient espClient;
// PubSubClient mqtt(espClient);

// // ---------- Functions ----------
// void connectWiFi() {
//   Serial.print("Connecting to WiFi");
//   WiFi.begin(ssid, pass);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected");
// }

// void connectMQTT() {
//   while (!mqtt.connected()) {
//     Serial.print("Connecting to MQTT...");
//     if (mqtt.connect("ESP32_arfa")) {
//       Serial.println("connected");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.println(mqtt.state());
//       delay(2000);
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);

//   dht.begin();
//   connectWiFi();

//   mqtt.setServer(mqtt_server, mqtt_port);
//   connectMQTT();
// }

// void loop() {
//   if (!mqtt.connected()) {
//     connectMQTT();
//   }
//   mqtt.loop();

//   float temperature = dht.readTemperature();
//   float humidity    = dht.readHumidity();

//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("DHT read failed");
//     delay(2000);
//     return;
//   }

//   char tBuf[8], hBuf[8];
//   dtostrf(temperature, 4, 2, tBuf);
//   dtostrf(humidity,    4, 2, hBuf);

//   mqtt.publish(TOPIC_TEMP, tBuf);
//   mqtt.publish(TOPIC_HUM,  hBuf);

//   Serial.print("Published -> Temp: ");
//   Serial.print(tBuf);
//   Serial.print(" C | Hum: ");
//   Serial.print(hBuf);
//   Serial.println(" %");

//   delay(5000);  // publish every 5 seconds
// }