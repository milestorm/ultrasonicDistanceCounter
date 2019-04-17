#include <Arduino.h>
#include <TM1637Display.h>
#include <NewPing.h>

// pin definitions
#define USONIC_TRIGGER 8
#define USONIC_ECHO    9
#define MAX_DISTANCE   400 // max countable distance in cm
#define LCD_CLK 4
#define LCD_DIO 5

#define MIN 10  // minimal distance of percentage count
#define MAX 200 // maximal distance of percentage count

const uint8_t CLEARDATA[4] = { 0x00, 0x00, 0x00, 0x00 };
TM1637Display display(LCD_CLK, LCD_DIO); // initiates LCD

NewPing sonar(USONIC_TRIGGER, USONIC_ECHO, MAX_DISTANCE); // initializes the Ultrasonic module

unsigned int distance, percent;

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
  // cleard LCD
  display.setBrightness(0x0f);
	display.setSegments(CLEARDATA);
}

void loop() {
  distance = sonar.ping_cm();
  percent = getInversedPercentageDistance(distance, MIN, MAX);

  display.showNumberDecEx(percent, 0, false);

  delay (1000); // updates every second
}