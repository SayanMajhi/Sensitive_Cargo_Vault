# 🚚 Smart Sensitive Cargo Vault

An IoT-based secure logistics monitoring system using Arduino, Python, and Firebase.

---

## 🔥 Features

- Finite State Machine (SECURE / TRANSIT / BREACH)
- Multi-sensor monitoring (Temperature, Gas, Distance, Tilt)
- Thermal PWM control
- EEPROM-based breach persistence
- Event-driven serial communication
- Firebase real-time logging
- Remote reset via cloud

---

## 🧠 System Architecture
Sensors → Arduino FSM → Serial → Python → Firebase
↑
Remote Reset
---

## 📂 Project Structure

- `arduino/` → Embedded system code
- `python/` → Middleware + Firebase integration
- `docs/` → Architecture & design
- `firebase/` → DB config examples

---

## ⚙️ Setup Instructions

### 1. Arduino
- Upload `cargo_vault.ino` using Arduino IDE

### 2. Python
```bash
pip install -r requirements.txt

3. Firebase
Create project
Enable Realtime DB
Download key.json
Place inside firebase/
▶️ Run System
python python/main.py
🧪 Testing
Enter TRANSIT via button
Trigger sensors → BREACH
Check Firebase updates
Set "reset": 1 → system resets
📊 Sample Data
{
  "ST": "BREACH",
  "T": "30",
  "G": "400",
  "D": "60",
  "B": "2"
}
🚀 Future Improvements
GPS tracking
Dashboard UI
Mobile alerts
🏷️ Tags

arduino, iot, firebase, embedded-systems, security


---


