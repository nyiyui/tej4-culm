#include <EEPROM.h>

int light_leftThreshold = 720;
int light_rightThreshold = 790;
bool light_left = false;
bool light_right = false;
#define EEPROM_LIGHT_LEFT_THRESHOLD 0x10
#define EEPROM_LIGHT_RIGHT_THRESHOLD 0x11

// left
//   blank = 550
//   black = 840
//   white pla-board = 900
// right
//   blank = 490
//   black = 750
//   white pla-board = 900

void light_read() {
  int leftRaw = analogRead(LIGHT_LEFT_PIN);
  int rightRaw = analogRead(LIGHT_RIGHT_PIN);
  // Serial.print("l = ");
  // Serial.print(leftRaw);
  // Serial.print(" ; r = ");
  Serial.println(rightRaw);
  light_left = leftRaw >= light_leftThreshold;
  light_right = rightRaw >= light_rightThreshold;
}

void light_saveThresholds(int left, int right) {
  EEPROM.write(EEPROM_LIGHT_LEFT_THRESHOLD, light_leftThreshold);
  EEPROM.write(EEPROM_LIGHT_RIGHT_THRESHOLD, light_rightThreshold);
}

void light_loadThresholds() {
  light_leftThreshold = EEPROM.read(EEPROM_LIGHT_LEFT_THRESHOLD);
  light_rightThreshold = EEPROM.read(EEPROM_LIGHT_RIGHT_THRESHOLD);
}