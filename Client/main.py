import serial
with serial.Serial('/dev/ttyUSB2', 115200, timeout = 1)  as ser:# open serial port
    while (1):
        ser.write(input().encode('utf-8'))
        print(str(ser.read(1)))