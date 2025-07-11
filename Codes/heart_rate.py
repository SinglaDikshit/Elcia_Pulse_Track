import serial
import csv
from datetime import datetime

# Configuration
PORT = 'COM8'               # port setting
BAUD_RATE = 9600
CSV_FILE = 'heart_data_log.csv' #defininf csv

# Open CSV file and write header
csv_file = open(CSV_FILE, mode='w', newline='')
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["Timestamp", "IR", "BPM", "HR"])  # header row

# Start Serial Communication 
try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=2)
    print(f"Connected to {PORT} at {BAUD_RATE} baud.")
except serial.SerialException as e:
    print("Error opening serial port:", e)
    exit()

# Read and write data
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

                if all(k in data for k in ['IR', 'BPM', 'HR']):
                    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")  
                    row = [timestamp, data['IR'], data['BPM'], data['HR']]    #terminal view
                    csv_writer.writerow(row)
                    print("✅ Logged:", row)
                    csv_file.flush()  # Making sure it's written immediately
                else:
                    print("Skipped - Missing keys:", data)   #detect error

            except Exception as e:
                print("Parsing error:", e, "| Line:", line)

#defining end 
except KeyboardInterrupt:
    print("\n Logging stopped by user.")
    ser.close()
    csv_file.close()
    print(f" CSV saved to '{CSV_FILE}'")  #saving the data