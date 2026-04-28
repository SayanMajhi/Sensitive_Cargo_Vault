#include <EEPROM.h>

// ---------------- PINS ----------------
#define BUTTON_PIN 2
#define TILT_PIN 3
#define LED_PIN 9

#define TEMP_PIN A0
#define GAS_PIN A1
#define DIST_PIN A2

// ---------------- STATES ----------------
enum State { SECURE, TRANSIT, BREACH };
State currentState = SECURE;

// ---------------- GLOBALS ----------------
float baselineDist = 0;
int baselineTemp = 25;
int breachAddr = 0;

// ---------------- EEPROM ----------------
int getBreachCount() {
  return EEPROM.read(breachAddr);
}

void incrementBreach() {
  int count = EEPROM.read(breachAddr);
  if (count < 255) count++;
  EEPROM.write(breachAddr, count);
}

// ---------------- TEMP ----------------
int readTemp() {
  int val = analogRead(TEMP_PIN);
  return map(val, 0, 1023, 20, 50);
}

void handleTemp() {
  int temp = readTemp();
  int pwm = map(temp, baselineTemp, baselineTemp + 20, 0, 255);
  pwm = constrain(pwm, 0, 255);
  analogWrite(LED_PIN, pwm);
}

// ---------------- GAS ----------------
bool gasLeak() {
  return analogRead(GAS_PIN) > 500;
}

// ---------------- DIST ----------------
float getDistance() {
  int val = analogRead(DIST_PIN);
  return map(val, 0, 1023, 0, 100);
}

bool cargoShifted() {
  float d = getDistance();
  return abs(d - baselineDist) > 5;
}

// ---------------- TILT ----------------
bool isTilted() {
  // LOW only when switch is ON
  return digitalRead(TILT_PIN) == LOW;
}

// ---------------- SERIAL ----------------
void sendData(String st) {
  Serial.print("ST:");
  Serial.print(st);
  Serial.print(",T:");
  Serial.print(readTemp());
  Serial.print(",G:");
  Serial.print(analogRead(GAS_PIN));
  Serial.print(",D:");
  Serial.print(getDistance());
  Serial.print(",B:");
  Serial.println(getBreachCount());
}

// ---------------- BREACH ----------------
void triggerBreach(String reason) {
  if (currentState != BREACH) {
    currentState = BREACH;

    incrementBreach();

    Serial.print("BREACH: ");
    Serial.println(reason);

    analogWrite(LED_PIN, 255);

    sendData("BREACH");
  }
}

// ---------------- DIGITAL KEY ----------------
bool detectKey() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    unsigned long t1 = millis();

    while (digitalRead(BUTTON_PIN) == LOW);

    unsigned long duration = millis() - t1;

    if (duration > 1500) {
      delay(300);

      if (digitalRead(BUTTON_PIN) == LOW) {
        unsigned long t2 = millis();

        while (digitalRead(BUTTON_PIN) == LOW);

        if ((millis() - t2) < 500) {
          return true;
        }
      }
    }
  }
  return false;
}

// ---------------- RESET ----------------
void checkSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    if (cmd == "RESET") {
      currentState = SECURE;
      analogWrite(LED_PIN, 0);
      sendData("SECURE");
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TILT_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  if (EEPROM.read(breachAddr) == 255) {
    EEPROM.write(breachAddr, 0);
  }
  delay(500);
  sendData("SECURE");
}
// ---------------- LOOP ----------------
void loop() {
  checkSerial();
  switch (currentState) {
    case SECURE:
      analogWrite(LED_PIN, 0);
      if (detectKey()) {
        currentState = TRANSIT;

        delay(200);
        baselineDist = getDistance();

        sendData("TRANSIT");
      }
      break;

    case TRANSIT:
      handleTemp();

      if (gasLeak()) triggerBreach("GAS");
      if (cargoShifted()) triggerBreach("SHIFT");
      if (isTilted()) triggerBreach("TILT");

      break;

    case BREACH:
      break;
  }
}
