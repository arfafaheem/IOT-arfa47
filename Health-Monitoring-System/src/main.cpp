#define BLYNK_TEMPLATE_ID "TMPL6lWpKA16D"     //identifies blynk project with template id
#define BLYNK_TEMPLATE_NAME "Health Monitoring System"    //project name on blynk cloud
#define BLYNK_AUTH_TOKEN "zmtY11y0b9sfY8m7SzdOs2cnrgcVBhe_"   //unique key to connect esp32 to blynk dashboard
#define BLYNK_PRINT Serial  //prints blynk connection messages on serial monitor

//libraries used throughout the project
#include <Wire.h>  //for I2C communication
#include <OneWire.h>

#include <MAX30100_PulseOximeter.h>  //for heart rate and SpO2 sensor
// #include <DallasTemperature.h> //for body temperature sensor

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //for OLED display

#include <WiFi.h>
#include <BlynkSimpleEsp32.h> //for wifi and blynk cloud connection

//Wifi credentials
char ssid[]="WirelessNet";
char pass[]="eeeeeeee";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define REPORTING_PERIOD_MS 3000

PulseOximeter pox;// Creates sensor object pox for MAX30100(heart rate and SpO2)
uint32_t tsLastReport = 0;   //for udating data every second

// Thresholds
#define HR_MAX 75  // Maximum heart rate threshold for alert
#define BUZZER_PIN 14 
int melody[] = {523, 587, 659, 698};  // C5, D5, E5, F5
int noteDurations[] = {200, 200, 200, 400}; // ms

int melodyIndex = 0;
unsigned long noteStartTime = 0;
bool playingMelody = false;

// bool hrAlertSent = false;  
unsigned long lastAlertTime = 0;
const unsigned long ALERT_INTERVAL = 30000; // 30 sec between repeated alerts


//Called when heartbeat is detected and prints message on serial monitor, callback function
void onBeatDetected() {
  Serial.println("Beat Detected!");
}

void setup() {
  Serial.begin(115200);

  //Blynk Initialization
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  //connect esp32 to wifi and blynk cloud
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // buzzer off initially

  //OLED Initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

 //MAX30100 Initialization
  if (!pox.begin()) { //checks if sensor (pox is sensor's object) is connected properly
    Serial.println("MAX30100 FAILED");
    display.setCursor(0, 10);
    display.println("MAX30100 FAILED");
    display.display();
    while (true);
  } else {
    Serial.println("MAX30100 SUCCESS");
    display.setCursor(0, 10);
    display.println("MAX30100 SUCCESS");
    display.display();
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_11MA);  //setting the current for the IR LED inside the MAX30100 pulse oximeter sensor
  pox.setOnBeatDetectedCallback(onBeatDetected); //if connected then links the callback function to heartbeat detection
}

void loop() {
  Blynk.run();       // keeps Blynk cloud connection alive
  pox.update();      // IMPORTANT, must run continuously and read sensor data

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {  // Updates data every 3 sec
    float hr = pox.getHeartRate();   // reads heart rate value
    float spo2 = pox.getSpO2();      // reads SpO2 value

    // Print data on Serial Monitor
    Serial.print("Heart rate: ");
    Serial.print(hr);
    Serial.print(" bpm / SpO2: ");
    Serial.print(spo2);
    Serial.println(" %");

    // Send sensors' data to Blynk
    Blynk.virtualWrite(V0, hr);
    Blynk.virtualWrite(V1, spo2);

    // Sensor values display on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("HMS");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("HR: ");
    display.print((int)hr);
    display.println(" bpm");
    display.setCursor(0, 45);
    display.print("SpO2: ");
    display.print((int)spo2);
    display.println(" %");
    display.display();

    //for buzzer and email alert on high heart rate
    unsigned long currentMillis = millis();

    // HR Alert
    if (hr > HR_MAX) {
        // Play melody 
        if (!playingMelody) {
            noteStartTime = currentMillis;
            playingMelody = true;
        }
        if (playingMelody) {
            if (currentMillis - noteStartTime >= noteDurations[melodyIndex]) {
                tone(BUZZER_PIN, melody[melodyIndex], noteDurations[melodyIndex]);
                noteStartTime = currentMillis;
                melodyIndex++;
                if (melodyIndex >= sizeof(melody)/sizeof(melody[0])) {
                    melodyIndex = 0;
                    playingMelody = false;
                }
            }
        }

       // Send Blynk event email every ALERT_INTERVAL
    if (millis() - lastAlertTime >= ALERT_INTERVAL) {
        Blynk.logEvent(
            "hr_alert", 
            "⚠️ High Heart Rate Detected!\nHR: " + String((int)hr) + " bpm"
        );
        lastAlertTime = millis();
    }

    } else {
        noTone(BUZZER_PIN); //stop buzzer
        playingMelody = false;
        melodyIndex = 0;
    }
    tsLastReport = currentMillis;  
  } 
}
