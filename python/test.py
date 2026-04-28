import serial

# CHANGE THIS PORT
PORT = '/dev/ttyUSB0'   # or /dev/ttyACM0
BAUD = 9600

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Connected to {PORT}\n")
except Exception as e:
    print("Error opening serial port:", e)
    exit()

print("Listening to Arduino...\n")

while True:
    try:
        line = ser.readline().decode(errors='ignore').strip()

        if not line:
            continue

        print("RAW:", line)

        # Optional: simple validation
        if line.startswith("ST:"):
            print("VALID DATA\n")
        else:
            print("IGNORED (noise)\n")

    except KeyboardInterrupt:
        print("\nStopped by user")
        break

    except Exception as e:
        print("Error:", e)