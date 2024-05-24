#!/usr/bin/python3

# This script is used to log strain gauge, wind vane, and wind anemometer data
# that is sent by arduino over serial/USB.

import csv,time
import serial, io, datetime 
from serial import Serial

def readData():
    buffer = ""
    while True:
        oneByte = ser.read(1)
        if oneByte == b"\n":   # method should return bytes
            return buffer
        else:
            buffer += oneByte.decode("unicode_escape")


timestr = time.strftime("%d%b%Y_%H%M%p")

folder_path = r'/home/ruteomic/RUTE/Weather-Data/Code'
file_prefix = r'Log'
file_type = '.csv'
file_name = file_prefix + '_' + timestr + file_type
file_path = folder_path + '/' + file_name

# Include the timestamps in the data
include_timestamp_boolean = True

# Show the serial data in the Python console
show_serial_data_boolean = True

addr = "/dev/ttyUSB0" # serial port to read data from (note that error will be outputted if arduino/ESP32 is not plugged in, as no usb is detected, but still runs when microcontroller is plugged in) 
baud = 9600 # baud rate for three instruments

ser = serial.Serial(
        port = addr, \
        baudrate = baud, \
        parity = serial.PARITY_NONE, \
        stopbits = serial.STOPBITS_ONE, \
        bytesize = serial.EIGHTBITS, \
        timeout = 0)

print('Start time: ' + timestr)
print("Connected to: " + ser.portstr)
print('Now collecting data...')

start_time = time.time()

try: 


    with open(file_path, 'w', newline='', encoding="unicode_escape") as csvfile:

        spamwriter = csv.writer(csvfile, delimiter=',')

        while True:
            line = readData();
            if show_serial_data_boolean:
                print(line)
            

            if include_timestamp_boolean != True:
                to_write = [str(line)]
            else:
                to_write = [str(time.time()-start_time) + ',' + str(line)]

            spamwriter.writerow(to_write)



except KeyboardInterrupt:

    print('Cntl+C press detected - ending script.')

    print()
    print('Log data svaed to:')
    print(file_name)

    ser.close()



