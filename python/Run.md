# Running the Firebase Integration

This guide explains how to run the Smart Sensitive Cargo Vault system using a real Arduino connected to Firebase.

---

## 1. Prerequisites

Install required packages:

```bash
sudo apt update
sudo apt install python3 python3-pip python3-venv
```

---

## 2. Setup Python Environment

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r python/requirements.txt
```

---

## 3. Firebase Setup

1. Go to Firebase Console
2. Create a project
3. Enable **Realtime Database**
4. Set rules:

```json
{
  ".read": true,
  ".write": true
}
```

5. Download service account key and place it here:

```text
firebase/key.json
```

6. Update database URL in `python/main.py`:

```python
'databaseURL': 'https://smart-sensitive-cargo-vault-default-rtdb.firebaseio.com/'
```

---

## 4. Connect Arduino

### Upload code

* Open Arduino IDE
* Upload `sketch.ino` to your Arduino

---

### Find serial port

```bash
ls /dev/tty*
```

Typical ports:

* `/dev/ttyUSB0`
* `/dev/ttyACM0`

---

### Update port in `main.py`

```python
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
```

---

## 5. Run the System

! Close Arduino Serial Monitor before running

```bash
python python/main.py
```

---

## 6. Expected Output

```text
RAW: ST:TRANSIT,T:28,G:120,D:45,B:0
UPLOAD: {'ST': 'TRANSIT', 'T': '28', 'G': '120', 'D': '45', 'B': '0'}
```

---

## 7. Verify Firebase

Open Firebase → Realtime Database

```json
{
  "vault": {
    "ST": "TRANSIT",
    "T": "28",
    "G": "120",
    "D": "45",
    "B": "0"
  }
}
```

---

## 8. Remote Reset

Set in Firebase:

```json
"reset": 1
```

Expected behavior:

* Python sends RESET to Arduino
* Arduino returns to SECURE state
* Firebase resets value to 0

---

## 9. Troubleshooting

| Issue              | Cause            | Fix                          |
| ------------------ | ---------------- | ---------------------------- |
| No data            | Wrong port       | Check `/dev/tty*`            |
| Permission denied  | No access        | Use `sudo` or add to dialout |
| Serial busy        | Arduino IDE open | Close Serial Monitor         |
| No Firebase update | Wrong URL        | Verify database URL          |

---

## 10. System Flow

```text
Arduino → Serial → Python → Firebase → Reset → Arduino
```

---
