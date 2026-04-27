import serial
import firebase_admin
from firebase_admin import credentials, db

# ================= FIREBASE SETUP =================
cred = credentials.Certificate("key.json")

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-sensitive-cargo-vault-default-rtdb.firebaseio.com/'
})

vault_ref = db.reference('vault')
reset_ref = db.reference('reset')

# ================= SERIAL SETUP =================
ser = serial.Serial('COM3', 9600, timeout=1)

# ================= PARSER =================
def parse_line(line):
    try:
        parts = line.strip().split(',')
        data = {}

        for p in parts:
            k, v = p.split(':')
            data[k] = v

        return data
    except:
        return None

# ================= FILTER =================
def should_upload(data):
    return data and data.get("ST") in ["TRANSIT", "BREACH"]

# ================= MAIN LOOP =================
while True:
    try:
        line = ser.readline().decode(errors='ignore').strip()

        if not line:
            continue

        print("RAW:", line)

        data = parse_line(line)

        if should_upload(data):
            print("UPLOAD:", data)
            vault_ref.set(data)

        # ===== REMOTE RESET =====
        if reset_ref.get() == 1:
            print("RESET COMMAND SENT")
            ser.write(b'RESET\n')
            reset_ref.set(0)

    except Exception as e:
        print("ERROR:", e)
