# Smart Sensitive Cargo Vault

## Overview

Smart Sensitive Cargo Vault is an IoT-based monitoring system designed to ensure the safety of sensitive cargo during transport. The system detects environmental anomalies and physical tampering using multiple sensors and operates through a finite state machine.

The implementation consists of three layers:

* Embedded system (Arduino) for sensing and control
* Python middleware for communication and data processing
* Firebase Realtime Database for cloud storage and remote control

---

## System Architecture

![Architecture](images/architecture.png)

The system follows an event-driven pipeline:

* Arduino collects sensor data and evaluates system state
* Data is sent via Serial to Python
* Python processes and uploads data to Firebase
* Firebase can send a reset command back through Python

---

## Features

* Finite State Machine (SECURE, TRANSIT, BREACH)
* Multi-sensor monitoring (temperature, gas, distance, tilt)
* PWM-based thermal response using LED
* EEPROM-based breach counter persistence
* Event-driven serial communication
* Firebase integration for real-time data storage
* Remote reset functionality

---

## Project Structure

```text
Sensitive_Cargo_Vault/
├── arduino/
│   ├── sketch.ino
│   └── diagram.json
├── python/
│   ├── main.py
│   ├── test.py
│   ├── 
│   └── requirements.txt
├── firebase/
│   ├── sample_db.json
│   └── key.json (excluded via .gitignore)
├── docs/
│   ├── architecture.md
│   ├── data-flow.md
│   ├── state-machine.md
│   ├── pin-diagram.md
│   └── setup-guide.md
├── images/
│   ├── architecture.png
│   └── circuit.png
└── README.md
```

---

## State Machine

The system operates in three states:

| State   | Description                            |
| ------- | -------------------------------------- |
| SECURE  | Idle state, waiting for authentication |
| TRANSIT | Active monitoring mode                 |
| BREACH  | Alarm triggered due to anomaly         |

Transitions:

* SECURE → TRANSIT → Digital key (long press + short press)
* TRANSIT → BREACH → Gas / Tilt / Distance anomaly
* BREACH → SECURE → RESET command

---

## Data Format

Arduino sends structured data via Serial:

```text
ST:<STATE>,T:<TEMP>,G:<GAS>,D:<DIST>,B:<BREACH_COUNT>
```

Example:

```text
ST:BREACH,T:30,G:400,D:60,B:2
```

---

## Firebase Data Model

```json
{
  "vault": {
    "ST": "BREACH",
    "T": "30",
    "G": "400",
    "D": "60",
    "B": "2"
  },
  "reset": 0
}
```

---

## Simulation (Wokwi)

This project can be fully tested without hardware.

### Components Used

* Arduino Uno
* Pushbutton
* LED + Resistor
* 3 Potentiometers (simulate sensors)
* Slide Switch (tilt simulation)

---

## Pin Configuration

```text
D2  → Pushbutton
D3  → Tilt Switch
D9  → LED (PWM)

A0  → Temperature (Potentiometer)
A1  → Gas (Potentiometer)
A2  → Distance (Potentiometer)
```

---

## Running the Project

### 1. Simulation (Recommended)

* Open Wokwi
* Load `diagram.json` and `sketch.ino`
* Start simulation

---

### 2. Python Middleware

Install dependencies:

```bash
pip install -r python/requirements.txt
```

Run:

```bash
python python/main.py
```

---

### 3. Firebase Setup

* Create a Firebase project
* Enable Realtime Database
* Set rules:

```json
{
  ".read": true,
  ".write": true
}
```

* Place service account key in:

```text
firebase/key.json
```

---

## Testing Workflow

1. System starts in SECURE
2. Enter TRANSIT using digital key
3. Adjust sensors:

   * Temperature → LED brightness changes
   * Gas → triggers BREACH
   * Distance → triggers BREACH
   * Tilt → triggers BREACH
4. Send RESET via Firebase or Serial

---

## Important Notes

* Sensors are simulated using potentiometers
* Wokwi is used due to lack of hardware
* Serial Monitor must be closed when running Python
* Firebase credentials are excluded for security

---

## Limitations

* No real sensor calibration
* Wokwi cannot directly integrate with Python serial
* Network latency not simulated

---

## Future Improvements

* GPS-based tracking
* Web dashboard for visualization
* Alert system (notifications)
* Hardware deployment with real sensors

---

## License

MIT License
