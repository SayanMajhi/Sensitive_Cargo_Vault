import firebase_admin
from firebase_admin import credentials, db
import time

# ===== INIT =====
cred = credentials.Certificate("key.json")

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://smart-sensitive-cargo-vault-default-rtdb.firebaseio.com/'
})

vault_ref = db.reference('vault')
reset_ref = db.reference('reset')

# ===== TEST WRITE =====
print("Writing test data...")

vault_ref.set({
    "ST": "TEST",
    "T": "25",
    "G": "100",
    "D": "40",
    "B": "0"
})

print("✅ Data written to Firebase")

# ===== TEST READ LOOP =====
print("Waiting for reset signal...")

while True:
    val = reset_ref.get()
    print("Reset value:", val)

    if val == 1:
        print("✅ RESET detected from Firebase")

        # simulate sending to Arduino
        print("Pretend: sending RESET to Arduino")

        reset_ref.set(0)
        print("Reset cleared")

    time.sleep(2)
