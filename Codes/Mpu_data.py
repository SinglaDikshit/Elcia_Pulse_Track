import serial
import csv
from datetime import datetime


PORT = 'COM8'               # port definition
BAUD_RATE = 9600
CSV_FILE = 'mpu6050_data_log.csv' #including csv

# Open CSV file and writing header
csv_file = open(CSV_FILE, mode='w', newline='')
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["Timestamp", "Ax", "Ay", "Az", "Rx", "Ry", "Rz"])

# Start Serial Communication
try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=2)
    print(f"Connected to {PORT} at {BAUD_RATE} baud.")
except serial.SerialException as e:
    print("Error opening serial port:", e)
    csv_file.close()
    exit()

# Read and log data
try:
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()

        if line.startswith("<") and line.endswith(">"):
            line = line[1:-1]  # remove < and >

            try:
                # Build dictionary from key:value pairs
                data = {}
                for item in line.split(','):
                    if ':' in item:
                        key, value = item.split(':')
                        data[key.strip()] = float(value.strip())

                expected_keys = ['Ax', 'Ay', 'Az', 'Rx', 'Ry', 'Rz']
                if all(k in data for k in expected_keys):
                    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    row = [
                        timestamp,
                        data['Ax'], data['Ay'], data['Az'],   # terminal view
                        data['Rx'], data['Ry'], data['Rz']
                    ]
                    csv_writer.writerow(row)
                    csv_file.flush()
                    print(" Logged:", row)     #getting values
                else:
                    print("Missing some values:", data)   #to debug and for errors

            except Exception as e:
                print(" Parsing error:", e, "| Line:", line)

# defining emd
except KeyboardInterrupt:
    print("\n Logging stopped by user.")
    ser.close()
    csv_file.close()
    print(f" CSV saved to '{CSV_FILE}'") #saving data