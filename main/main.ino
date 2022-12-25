#include "SevSeg.h"
#include "ezButton.h"

#define VRX_PIN A1
#define VRY_PIN A0
#define FIRE_BUTTON A2
#define PORTAL_LEDS A3
#define FLUID_LED A4

#define LEFT_THRESHOLD 400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD 350
#define DOWN_THRESHOLD 850

#define COMMAND_NO 0x00
#define COMMAND_LEFT 0x01
#define COMMAND_RIGHT 0x02
#define COMMAND_UP 0x04
#define COMMAND_DOWN 0x08

#define REBOOT_DELETE 15

SevSeg sevseg;
ezButton fb(FIRE_BUTTON);

bool fbState = false;
int xValue = 0;
int yValue = 0;

unsigned long PORTAL_INTERVAL = 5 * 1000;
unsigned long PORTAL_PREV_MILLIS = 0;

unsigned long SELECTOR_INTERVAL = 275;
unsigned long SELECTOR_PREV_MILLIS = 0;

unsigned long ANIMATION_INTERVAL = 250;
unsigned long ANIMATION_PREV_MILLIS = 0;

int UNLOCK_CODE = 0;
bool ARMED = false;

int command = COMMAND_NO;

int UNIVERSE_INDEX = -1;
int PREV_UNIVERSE_INDEX = -1;
int CURR_UNIVERSE_INDEX = 0;

const int UNIVERSE_LEN = 11;
char* UNIVERSE[UNIVERSE_LEN] = { "C137", "35-C", "D-99", "304X", "C131", "C141", "C198", "C229", "C290", "C773", "H457" };

/* Animation Data - HGFEDCBA Map */
const uint8_t ANIMATION[57][4] = {
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 0
  { 0x6d, 0x79, 0x38, 0x71 },  // Frame 1
  { 0x79, 0x38, 0x71, 0x00 },  // Frame 2
  { 0x38, 0x71, 0x00, 0x5e },  // Frame 3
  { 0x71, 0x00, 0x5e, 0x79 },  // Frame 4
  { 0x00, 0x5e, 0x79, 0x6d },  // Frame 5
  { 0x5e, 0x79, 0x6d, 0x31 },  // Frame 6
  { 0x79, 0x6d, 0x31, 0x50 },  // Frame 7
  { 0x6d, 0x31, 0x50, 0x3e },  // Frame 8
  { 0x31, 0x50, 0x3e, 0x39 },  // Frame 9
  { 0x50, 0x3e, 0x39, 0x31 },  // Frame 10
  { 0x3e, 0x39, 0x31, 0x00 },  // Frame 11
  { 0x39, 0x31, 0x00, 0x00 },  // Frame 12
  { 0x31, 0x00, 0x00, 0x00 },  // Frame 13
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 14
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 15
  { 0x80, 0x00, 0x00, 0x00 },  // Frame 16
  { 0x80, 0x80, 0x00, 0x00 },  // Frame 17
  { 0x80, 0x80, 0x80, 0x00 },  // Frame 18
  { 0x80, 0x80, 0x80, 0x80 },  // Frame 19
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 20
  { 0x80, 0x00, 0x00, 0x00 },  // Frame 21
  { 0x80, 0x80, 0x00, 0x00 },  // Frame 22
  { 0x80, 0x80, 0x80, 0x00 },  // Frame 23
  { 0x80, 0x80, 0x80, 0x80 },  // Frame 24
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 25
  { 0x08, 0x00, 0x00, 0x00 },  // Frame 26
  { 0x18, 0x00, 0x00, 0x00 },  // Frame 27
  { 0x5c, 0x00, 0x00, 0x00 },  // Frame 28
  { 0xff, 0x10, 0x00, 0x00 },  // Frame 29
  { 0xff, 0xff, 0x00, 0x00 },  // Frame 30
  { 0xff, 0xff, 0xff, 0x18 },  // Frame 31
  { 0xff, 0xff, 0xff, 0xff },  // Frame 32
  { 0xd2, 0xe4, 0xd2, 0xe4 },  // Frame 33
  { 0xd4, 0xd4, 0xd4, 0xd4 },  // Frame 34
  { 0x62, 0x54, 0x62, 0x54 },  // Frame 35
  { 0x76, 0x76, 0x76, 0x76 },  // Frame 36
  { 0x7f, 0x7f, 0x7f, 0x7f },  // Frame 37
  { 0xff, 0xff, 0xff, 0xff },  // Frame 38
  { 0x80, 0x80, 0x80, 0x80 },  // Frame 39
  { 0x08, 0x08, 0x08, 0x08 },  // Frame 40
  { 0x00, 0x08, 0x08, 0x0c },  // Frame 41
  { 0x00, 0x00, 0x08, 0x0e },  // Frame 42
  { 0x00, 0x00, 0x00, 0x0f },  // Frame 43
  { 0x00, 0x00, 0x01, 0x07 },  // Frame 44
  { 0x00, 0x01, 0x01, 0x03 },  // Frame 45
  { 0x01, 0x01, 0x01, 0x01 },  // Frame 46
  { 0x21, 0x01, 0x01, 0x00 },  // Frame 47
  { 0x31, 0x01, 0x00, 0x00 },  // Frame 48
  { 0x39, 0x00, 0x00, 0x00 },  // Frame 49
  { 0x38, 0x00, 0x00, 0x00 },  // Frame 50
  { 0x18, 0x00, 0x00, 0x00 },  // Frame 51
  { 0x08, 0x00, 0x00, 0x00 },  // Frame 52
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 53
  { 0xff, 0xff, 0xff, 0xff },  // Frame 54
  { 0x00, 0x00, 0x00, 0x00 },  // Frame 55
  { 0xff, 0xff, 0xff, 0xff }   // Frame 56
};

void (*resetFunc)(void) = 0;

void display(char* text) {
  sevseg.setChars(text);
  sevseg.refreshDisplay();
}

void destruct() {
  int duration = sizeof(ANIMATION) / sizeof(ANIMATION[0]) + random(5, 21);

  for (int i = 0; i < duration;) {
    sevseg.refreshDisplay();
    if ((unsigned long)(millis() - ANIMATION_PREV_MILLIS) >= ANIMATION_INTERVAL) {
      ANIMATION_PREV_MILLIS = millis();
      sevseg.setSegments(ANIMATION[i]);
      i++;
    }
  }
  sevseg.blank();
  digitalWrite(FLUID_LED, LOW);
  delay(REBOOT_DELETE * 1000);
  resetFunc();
}

void run() {

  if (UNLOCK_CODE == -10 && UNIVERSE_INDEX == 2) {
    UNIVERSE_INDEX = -50;
  }

  if (UNLOCK_CODE == -2) {
    UNIVERSE_INDEX = -10;
  }

  switch (UNIVERSE_INDEX) {
    case -100:
      ARMED = false;
      destruct();
      break;
    case -50:
      ARMED = true;
      display("UNLC");
      break;
    case -10:
      ARMED = false;
      display(" LCD");
      break;
    case -1:
      ARMED = false;
      display("----");
      break;
    default:
      if (UNIVERSE_INDEX > UNIVERSE_LEN - 1) {
        ARMED = false;
        display("----");
        UNIVERSE_INDEX = -1;
      } else if (UNIVERSE_INDEX < -1) {
        ARMED = false;
        display("----");
        UNIVERSE_INDEX = UNIVERSE_LEN - 1;
      } else {
        ARMED = true;
        display(UNIVERSE[UNIVERSE_INDEX]);
      }
      break;
  }


  if (fb.isPressed() && ARMED) {
    PORTAL_PREV_MILLIS = millis();
    PREV_UNIVERSE_INDEX = CURR_UNIVERSE_INDEX;
    CURR_UNIVERSE_INDEX = UNIVERSE_INDEX;
    UNLOCK_CODE = 0;
    ARMED = false;
    if (UNIVERSE_INDEX == -50) {
      UNIVERSE_INDEX = -100;
    } else {
      UNIVERSE_INDEX = -1;
      digitalWrite(PORTAL_LEDS, HIGH);
    }
  }

  if ((unsigned long)(millis() - PORTAL_PREV_MILLIS) >= PORTAL_INTERVAL) {
    PORTAL_PREV_MILLIS = millis();
    digitalWrite(PORTAL_LEDS, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  byte numDigits = 4;
  byte digitPins[] = { 2, 3, 4, 5 };
  byte segmentPins[] = { 6, 7, 8, 9, 10, 11, 12, 13 };
  bool resistorsOnSegments = 0;
  // variable above indicates that 4 resistors were placed on the digit pins.
  // set variable to 1 if you want to use 8 resistors on the segment pins.
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(10);

  pinMode(PORTAL_LEDS, OUTPUT);
  pinMode(FLUID_LED, OUTPUT);

  digitalWrite(FLUID_LED, HIGH);
  digitalWrite(PORTAL_LEDS, LOW);

  fb.setDebounceTime(50);
}

void loop() {
  fb.loop();

  fbState = !fb.getState();
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  command = COMMAND_NO;

  // check left/right commands
  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;


  if ((unsigned long)(millis() - SELECTOR_PREV_MILLIS) >= SELECTOR_INTERVAL) {
    SELECTOR_PREV_MILLIS = millis();
    if (command & COMMAND_LEFT) {
      // Serial.println("COMMAND LEFT");
      UNIVERSE_INDEX--;
    }

    if (command & COMMAND_RIGHT) {
      // Serial.println("COMMAND RIGHT");
      UNIVERSE_INDEX++;
    }

    if (command & COMMAND_UP) {
      // Serial.println("COMMAND UP");
      UNIVERSE_INDEX = CURR_UNIVERSE_INDEX;
      UNLOCK_CODE = 0;
    }

    if (command & COMMAND_DOWN) {
      // Serial.println("COMMAND DOWN");
      UNIVERSE_INDEX = PREV_UNIVERSE_INDEX;
      UNLOCK_CODE--;
    }
  }

  run();

  // Serial.print(UNIVERSE_INDEX);
  // Serial.print("\t");
  // Serial.println(UNLOCK_CODE);
  // Serial.print(xValue);
  // Serial.print("\t");
  // Serial.println(yValue);
}