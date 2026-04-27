// ================== PIN CONFIG ==================

#include <EEPROM.h>

#define BUTTON_PIN 2
#define TILT_PIN 3
#define TRIG_PIN 6
#define ECHO_PIN 7

#define TEMP_PIN A0
#define MQ2_PIN A1

#define LED_PIN 9
#define BUZZER 8

// ================== STATE MACHINE ==================
enum State {
  SECURE,
  TRANSIT,
  BREACH
};

State currentState = SECURE;

// ================== GLOBALS ==================
int breachAddr = 0;

int getBreachCount() {
  return EEPROM.read(breachAddr);
}

void incrementBreach() {
  int count = EEPROM.read(breachAddr);
  count++;
  EEPROM.write(breachAddr, count);
}
int baselineTemp = 25;
float baselineDist = 0;

// ================== DIGITAL KEY ==================
bool detectKey() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    unsigned long t1 = millis();

    while (digitalRead(BUTTON_PIN) == LOW) {
      // wait for release
    }
    unsigned long duration = millis() - t1;

    if (duration > 1500) {
      delay(300);

      if (digitalRead(BUTTON_PIN) == LOW) {
        unsigned long t2 = millis();

        while (digitalRead(BUTTON_PIN) == LOW) {
          // wait for release
        }

        if ((millis() - t2) < 500) {
          return true;
        }
      }
    }
  }
  return false;
}

// ================== TEMPERATURE ==================
int readTemp() {
  int val = analogRead(TEMP_PIN);
  float voltage = val * (5.0 / 1023.0);
  return voltage * 100; // LM35
}

void handleTemp() {
  int temp = readTemp();

  int pwm = map(temp, baselineTemp, baselineTemp + 20, 0, 255);
  pwm = constrain(pwm, 0, 255);

  analogWrite(LED_PIN, pwm);
}

// ================== GAS SENSOR ==================
#define GAS_THRESHOLD 300

bool gasLeak() {
  int val = analogRead(MQ2_PIN);
  return val > GAS_THRESHOLD;
}

// ================== ULTRASONIC ==================
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) {
    return -1.0;
  }
  return duration * 0.034 / 2;
}

bool cargoShifted() {
  float d = getDistance();
  if (d < 0) {
    return false;
  }
  return fabs(d - baselineDist) > 5.0;
}

// ================== TILT ==================
bool isTilted() {
  return digitalRead(TILT_PIN) == HIGH;
}

// ================== BREACH ==================
void triggerBreach(String reason) {
  if (currentState != BREACH) {
    currentState = BREACH;

    incrementBreach();   // 🔥 EEPROM update

    digitalWrite(BUZZER, HIGH);
    analogWrite(LED_PIN, 255);

    Serial.print("REASON:");
    Serial.println(reason);
    sendData("BREACH");  // 🔥 structured output
  }
}

void checkSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "RESET") {
      currentState = SECURE;

      digitalWrite(BUZZER, LOW);
      analogWrite(LED_PIN, 0);

      sendData("SECURE");
    }
  }
}
void sendData(String stateName) {
  int temp = readTemp();
  int gas = analogRead(MQ2_PIN);
  float dist = getDistance();
  int breach = getBreachCount();

  Serial.print("ST:");
  Serial.print(stateName);
  Serial.print(",T:");
  Serial.print(temp);
  Serial.print(",G:");
  Serial.print(gas);
  Serial.print(",D:");
  Serial.print(dist);
  Serial.print(",B:");
  Serial.println(breach);
}

// ================== SETUP ==================
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TILT_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  Serial.println("System in SECURE mode");
  sendData("SECURE");
}

// ================== LOOP ==================
void loop() {
checkSerial();
  switch (currentState) {

    // ===== SECURE MODE =====
    case SECURE:
      digitalWrite(BUZZER, LOW);
      analogWrite(LED_PIN, 0);

      if (detectKey()) {
        currentState = TRANSIT;
        baselineTemp = readTemp();
        baselineDist = getDistance();

        sendData("TRANSIT");   // 🔥 log event
      }
      break;

    // ===== TRANSIT MODE =====
    case TRANSIT:
      handleTemp();

      if (gasLeak()) {
        triggerBreach("GAS LEAK");
      }

      if (cargoShifted()) {
        triggerBreach("CARGO SHIFT");
      }

      if (isTilted()) {
        triggerBreach("TILT DETECTED");
      }
      break;

    // ===== BREACH MODE =====
    case BREACH:
      // Stay here until reset (next part)
      break;
  }
}
