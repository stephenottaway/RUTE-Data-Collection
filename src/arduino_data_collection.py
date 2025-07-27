#!/usr/bin/python3

# The above shebang statement specifies the path to the Python interpreter.

# This script is used to log strain gauge, wind vane, and wind anemometer data
# that is sent by arduino over serial/USB.

import csv,time
import serial, io, datetime 
from serial import Serial


# This method reads raw bytes coming through the serial port.
def readData():
    buffer = ""
    while True:
        oneByte = ser.read(1) # Read and return one byte read from serial port.
        if oneByte == b"\n":   # We return the buffer once we read a new-line character. 
            return buffer
        else:
            buffer += oneByte.decode('unicode_escape') # Append to the buffer the byte read converted to a string (in unicode_escape encoding).


# Method returns the current time (when we started the script) formatted as a string. 
# Format is [day][month][year]_[hour][minute][AM/PM].
timestr = time.strftime("%d%b%Y_%H%M%p")

devices = ['Acer', 'RUTE']
curr_device = devices[0] # Modify this depending on which system I'm using.

folder_path = ''
if (curr_device == 'Acer'):
    folder_path = '/home/steph/RUTE/RUTE-Data-Collection/src/logs'
else:
    folder_path = '/home/ruteomic/RUTE/RUTE-Data-Collection/src/logs'

file_prefix = 'Log'
file_type = '.csv'
file_name = file_prefix + '_' + timestr + file_type
file_path = folder_path + '/' + file_name

# Include the timestamps in the data
include_timestamp = True 

# Show the serial data in the Python console
show_serial_data = True

addr = "/dev/ttyUSB0" # Serial port to read data from.
baud = 9600 # Baud rate for three instruments.

# Serial class initialization. Serial port is immediately opened upon object creation since addr is passed to port.
ser = serial.Serial(
        port = addr, \
        baudrate = baud, \
        parity = serial.PARITY_NONE, \
        stopbits = serial.STOPBITS_ONE, \
        bytesize = serial.EIGHTBITS, \
        timeout = 0)

print('Start time: ' + timestr)
print("Connected to: " + ser.name) # ser.name returns the device name.
print('Now collecting data...')
print()

try: 

    # Create and open the file_path in write mode, newline='' setting allows csv to handle newlines itself.
    with open(file_path, 'w', newline='') as csvfile:

        # Returns a writer object to convert data into delimited strings on the csvfile file object.
        spamwriter = csv.writer(csvfile, delimiter=',')

        while True:
            line = readData().strip() # .strip() with no argument gets rid of leading and trailing whitespaces.
            if show_serial_data:
                print("'" + line + "'")
            
            if include_timestamp == False:
                to_write = [line] 
            else:
                to_write = [time.strftime("%H:%M:%S") + ',' + str(line)] 
         
            # Write the row to the writers csvfile object.
            spamwriter.writerow(to_write)



except KeyboardInterrupt:

    print()
    print('Cntl+C press detected - ending script.')

    print()
    print('Log data saved to:')
    print(file_name)

    # Close (stop listening to) serial port immediately.
    ser.close()



