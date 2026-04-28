# Pin Configuration

## 1. Overview

This document defines the pin mapping for both:

* Simulation (Wokwi)
* Hardware implementation

---

## 2. Pin Mapping Table

| Component       | Pin | Type          |
| --------------- | --- | ------------- |
| Pushbutton      | D2  | Digital Input |
| Tilt Switch     | D3  | Digital Input |
| LED (PWM)       | D9  | PWM Output    |
| Temperature     | A0  | Analog Input  |
| Gas Sensor      | A1  | Analog Input  |
| Distance Sensor | A2  | Analog Input  |

---

## 3. Wiring Details

### Pushbutton

* One terminal → GND
* Other terminal → D2
* Uses INPUT_PULLUP

---

### LED

* Anode (+) → D9
* Cathode (-) → GND
* PWM used for brightness control

---

### Potentiometers (Simulation)

Each potentiometer:

* VCC → 5V
* GND → GND
* Signal → Analog pin

| Function    | Pin |
| ----------- | --- |
| Temperature | A0  |
| Gas         | A1  |
| Distance    | A2  |

---

### Tilt Switch

* One terminal → GND
* Other terminal → D3
* LOW signal indicates tilt

---

## 4. Simulation Mapping

| Real Sensor        | Simulation Equivalent |
| ------------------ | --------------------- |
| Temperature Sensor | Potentiometer         |
| MQ2 Gas Sensor     | Potentiometer         |
| Ultrasonic Sensor  | Potentiometer         |
| Tilt Sensor        | Slide Switch          |

---

## 5. Electrical Notes

* All components share common ground
* INPUT_PULLUP is used for button and tilt
* Analog values range from 0 to 1023

---

## 6. Diagram Reference

See:

```text id="9nqv0s"
images/circuit.png
```

---

## 7. Summary

The system uses a simplified pin configuration suitable for both simulation and real-world deployment, ensuring consistent behavior across environments.
