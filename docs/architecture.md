# Smart Sensitive Cargo Vault

An IoT-based secure logistics monitoring system that detects environmental anomalies and physical tampering during cargo transport. The system combines embedded sensing, event-driven communication, and cloud-based control.

---

## Overview

This project implements a multi-layer architecture:

* Embedded system (Arduino) for real-time monitoring and control
* Python middleware for data parsing and communication
* Firebase Realtime Database for cloud storage and remote commands

The system operates using a finite state machine with three modes:

* SECURE (Idle)
* TRANSIT (Active Monitoring)
* BREACH (Alarm State)

---

## System Architecture

```mermaid
flowchart LR
    Sensors --> Arduino
    Arduino -->|Serial (Event Data)| Python
    Python --> Firebase
    Firebase -->|Reset Command| Python
    Python -->|Serial Command| Arduino
```

Detailed design is available in `docs/architecture.md`.

---

## Features

* Finite State Machine-based control
* Multi-sensor monitoring (temperature, gas, distance, tilt)
* PWM-based thermal response
* Event-driven serial communication
* EEPROM-based breach persistence
* Cloud integration using Firebase RTDB
* Remote system reset capability

---

## Project Structure

```text
arduino/     Embedded firmware (FSM, sensors, EEPROM)
python/      Middleware (serial parsing, Firebase integration)
firebase/    Sample configuration (no secrets)
docs/        System design and architecture
images/      Diagrams and demo assets
```

---

## Setup Instructions

### 1. Arduino (or Simulation via Wokwi)

* Upload `arduino/cargo_vault.ino`
* For simulation, use Wokwi with potentiometers replacing sensors

---

### 2. Python Environment

Install dependencies:

```bash
pip install -r python/requirements.txt
```

---

### 3. Firebase Setup

1. Create a Firebase project
2. Enable Realtime Database
3. Set rules:

```json
{
  ".read": true,
  ".write": true
}
```

4. Download service account key
5. Place it at:

```text
firebase/key.json
```

---

## Running the System

Start middleware:

```bash
python python/main.py
```

System flow:

1. Arduino sends event-based sensor data
2. Python parses and uploads to Firebase
3. Firebase stores current system state
4. Remote reset triggers command back to Arduino

---

## Data Format

Arduino sends structured data:

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

## Simulation Notes

* Sensors are simulated using potentiometers in Wokwi
* Tilt is simulated using a switch
* System logic remains identical to hardware implementation

---

## Security Note

Firebase credentials (`key.json`) are excluded from version control via `.gitignore`.

---

## Future Work

* GPS-based tracking
* Web dashboard for visualization
* Notification system (alerts)
* Secure authentication layer

---

## License

This project is licensed under the MIT License.
