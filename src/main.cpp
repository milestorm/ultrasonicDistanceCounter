#include <Arduino.h>
#include <TM1637Display.h>
#include <NewPing.h>
#include <MedianFilter.h>

// pin definitions
#define USONIC_TRIGGER 5
#define USONIC_ECHO    4
#define MAX_DISTANCE   400 // max countable distance in cm
#define LCD_CLK 3
#define LCD_DIO 2
#define LED_ALARM 7 // pin for LED light
#define LED_ALARM_ON_VALUE 50 // if it is 90 or higher, turn on the led

#define MIN 50  // minimal distance in cm for 0%
#define MAX 250 // maximal distance in cm for 100%

const uint8_t CLEARDATA[4] = { 0x00, 0x00, 0x00, 0x00 };
TM1637Display display(LCD_CLK, LCD_DIO); // initiates LCD

MedianFilter median(51, 0);

NewPing sonar(USONIC_TRIGGER, USONIC_ECHO, MAX_DISTANCE); // initializes the Ultrasonic module

unsigned int distance, percent;
int medianedPercent;

// =========================================================

/**
 * gets distance in percent. nearest is 0, farest is 100
 */
int getPercentageDistance(int dist, int min, int max) {
  int range = max - min;
  int correctedStartValue = dist - min;
  int percentage = (correctedStartValue * 100) / range;
  if (percentage > 100) percentage = 100;
  if (percentage < 0) percentage = 0;

  return percentage;
};

int getInversedPercentageDistance(int dist, int min, int max) {
  return 100 - getPercentageDistance(dist, min, max);
}

void setup() {
  pinMode(LED_ALARM, OUTPUT);
  // cleard LCD
  display.setBrightness(0x0f);
	display.setSegments(CLEARDATA);
  display.showNumberDecEx(0, 0, false);
}

void loop() {
  for (size_t i = 0; i < 51; i++) {
    distance = sonar.ping_cm();
    percent = getInversedPercentageDistance(distance, MIN, MAX);
    median.in(percent);
    delay(100);
  }
  
  medianedPercent = median.out();

  if (medianedPercent >= LED_ALARM_ON_VALUE) {
    digitalWrite(LED_ALARM, HIGH);
  } else {
    digitalWrite(LED_ALARM, LOW);
  }
  
  display.showNumberDecEx(medianedPercent, 0, false);
}