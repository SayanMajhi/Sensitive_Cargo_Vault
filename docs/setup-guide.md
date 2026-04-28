# Setup Guide

## 1. Overview

This guide describes how to run the Smart Sensitive Cargo Vault system using either:

* Simulation (Wokwi)
* Local execution (Python + Firebase)

Hardware is optional; simulation is sufficient to validate system logic.

---

## 2. Prerequisites

### Software

* Arduino IDE (optional if using Wokwi)
* Python 3.8+
* Firebase account

---

## 3. Simulation Setup (Wokwi)

### Components Used

* Arduino Uno
* Pushbutton
* LED
* 3 × Potentiometer (simulate sensors)
* Slide Switch (tilt simulation)

---

### Pin Mapping

```text id="k3q7bw"
D2  → Button
D3  → Tilt Switch
D9  → LED (PWM)

A0  → Temperature (Potentiometer)
A1  → Gas (Potentiometer)
A2  → Distance (Potentiometer)
```

---

### Steps

1. Open Wokwi
2. Create a new Arduino Uno project
3. Replace:

   * `diagram.json`
   * `sketch.ino`
4. Click "Start Simulation"

---

### Testing

| Action                | Expected Result        |
| --------------------- | ---------------------- |
| Button (long → short) | TRANSIT state          |
| Rotate A0             | LED brightness changes |
| Increase A1           | BREACH (gas)           |
| Change A2             | BREACH (shift)         |
| Toggle switch         | BREACH (tilt)          |
| Serial input `RESET`  | Back to SECURE         |

---

## 4. Python Setup

Navigate to project root:

```bash id="w0d9kp"
pip install -r python/requirements.txt
```

---

### Configure Serial Port

Update in `python/main.py`:

```python id="x9m3y7"
ser = serial.Serial('COM3', 9600, timeout=1)
```

* Windows: COM3 / COM4
* Linux: /dev/ttyUSB0

---

## 5. Firebase Setup

1. Create a Firebase project
2. Enable Realtime Database
3. Set rules:

```json id="h4r8pv"
{
  ".read": true,
  ".write": true
}
```

4. Download service account key
5. Place file at:

```text id="2g6l1s"
firebase/key.json
```

---

### Database Structure

```json id="5yk6vt"
{
  "vault": {},
  "reset": 0
}
```

---

## 6. Running the System

Start Python middleware:

```bash id="8y7x6j"
python python/main.py
```

---

### System Execution Flow

1. Arduino sends event data via Serial
2. Python parses and uploads to Firebase
3. Firebase stores latest state
4. Reset command triggers reverse flow

---

## 7. Remote Reset

Set in Firebase:

```json id="p4t1xe"
"reset": 1
```

Result:

* Python sends RESET command
* Arduino transitions to SECURE
* System updates state

---

## 8. Notes

* Serial Monitor must be closed when running Python
* Sensors are simulated using analog inputs
* Ensure correct Firebase database URL

---

## 9. Troubleshooting

| Issue              | Cause         | Fix                          |
| ------------------ | ------------- | ---------------------------- |
| No Firebase data   | Wrong URL     | Verify databaseURL           |
| Serial not working | Port conflict | Close Arduino Serial Monitor |
| No state change    | Button wiring | Check INPUT_PULLUP logic     |

---

## 10. Summary

The system can be fully tested without hardware using Wokwi, while Python and Firebase provide the cloud integration layer.
