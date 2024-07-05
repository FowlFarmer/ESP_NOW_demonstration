#!/usr/bin/env python3

import serial
import struct
import time
import sys
import argparse
import threading

UART_PACKET_HEADER = 0x55
UART_PACKET_PAYLOAD_MAX = 256

def calculate_checksum(data):
    return sum(data) & 0xFF

def decode_packet(serial_port):
    state = 'HEADER'
    data_counter = 0
    packet = {'len': 0, 'sum': 0, 'payload': bytearray()}

    while True:
        byte = serial_port.read(1)
        #print(byte)
        if len(byte) == 0:
            continue

        if state == 'HEADER': #I guess what i said under this is wrong...
            print("header") #The state machine is prolly printing header all the tie because decode is so fking fast it returns to header instantly each time
            if byte[0] == UART_PACKET_HEADER:
                state = 'LENGTH'

        elif state == 'LENGTH':
            print("len")
            if byte[0] < UART_PACKET_PAYLOAD_MAX:
                packet['len'] = byte[0]
                packet['payload'] = bytearray()
                state = 'CHECKSUM'

        elif state == 'CHECKSUM':
            print("checksum")
            packet['sum'] = byte[0]
            state = 'DATA'

        elif state == 'DATA':
            packet['payload'].append(byte[0])
            data_counter += 1

            if data_counter >= packet['len']:
                state = 'HEADER'
                if packet['sum'] == calculate_checksum(packet['payload']):
                    print("Received message:", packet['payload'].decode())

                data_counter = 0

print('hi')
with serial.Serial('/dev/ttyUSB1', 921600, timeout = 1)  as ser:# open serial port
    while(1):
        decode_packet(ser)