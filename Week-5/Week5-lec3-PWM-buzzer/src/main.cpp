#include <Arduino.h>

// LED Pins
#define LED1_PIN 18
#define LED2_PIN 19

// Buzzer Pin
#define BUZZER_PIN 27

// LED PWM settings
#define LED_PWM_CH1 0
#define LED_PWM_CH2 1
#define LED_FREQ 2000
#define LED_RES 10  // 10-bit resolution (0-1023)

// Buzzer PWM settings
#define BUZZER_PWM_CH 2
#define BUZZER_FREQ 2000
#define BUZZER_RES 10

int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
int melodyLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
  // Setup LEDs
  ledcSetup(LED_PWM_CH1, LED_FREQ, LED_RES);
  ledcAttachPin(LED1_PIN, LED_PWM_CH1);
  ledcSetup(LED_PWM_CH2, LED_FREQ, LED_RES);
  ledcAttachPin(LED2_PIN, LED_PWM_CH2);

  // Setup Buzzer
  ledcSetup(BUZZER_PWM_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_PWM_CH);
}

void loop() {
  // --- Smooth LED Fade In/Out ---
  for (int duty = 0; duty <= 1023; duty += 4) {
    ledcWrite(LED_PWM_CH1, duty);
    ledcWrite(LED_PWM_CH2, 1023 - duty);
    delay(5);
  }
  for (int duty = 1023; duty >= 0; duty -= 4) {
    ledcWrite(LED_PWM_CH1, duty);
    ledcWrite(LED_PWM_CH2, 1023 - duty);
    delay(5);
  }

  // --- Buzzer Beep Pattern ---
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(BUZZER_PWM_CH, 2000 + i * 400);
    delay(150);
    ledcWrite(BUZZER_PWM_CH, 0);
    delay(150);
  }

  // --- Frequency Sweep ---
  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(BUZZER_PWM_CH, f);
    delay(20);
  }
  ledcWrite(BUZZER_PWM_CH, 0);
  delay(500);

  // --- Short Melody ---
  for (int i = 0; i < melodyLength; i++) {
    ledcWriteTone(BUZZER_PWM_CH, melody[i]);
    delay(250);
  }
  ledcWrite(BUZZER_PWM_CH, 0);
}
