import serial
import struct

a = 27*8
#header 2byte, len 2byte, sum 1byte
data_format = '<HHBBBffffIBBBHB'
raw_format = '<B B B B {a}s'

def unpack_packet(packet):
    unpacked_data = struct.unpack(data_format, packet)
    
    # Extract BmsData from the unpacked data
    bms_data = {
        'packet_id': unpacked_data[4],
        'voltage': unpacked_data[5],
        'current': unpacked_data[6],
        'percentage': unpacked_data[7],
        'temperature': unpacked_data[8],
        'charge': unpacked_data[9],
        'power_supply_status': unpacked_data[10],
        'power_supply_health': unpacked_data[11],
        'power_supply_technology': unpacked_data[12]
    }
    
    # Extract PowerMonStatus data
    power_mon_status = {
        'header': unpacked_data[0],
        'length': unpacked_data[1],
        'checksum': unpacked_data[2],
        'packet_id': unpacked_data[3],
        'bms_data': bms_data,
        'aux_voltage': unpacked_data[13],
        'conn_health': unpacked_data[14] & 0b11,
        'aux_health': (unpacked_data[14] >> 2) & 0b11,
        'reserved': (unpacked_data[14] >> 4) & 0b1111
    }
    
    return power_mon_status
with serial.Serial('/dev/ttyUSB1', 115200, timeout = 1)  as ser:# open serial port
    ser.reset_input_buffer()
    while(1):
        ser.reset_input_buffer()
        data = unpack_packet(ser.read(33))
        print(data)
        #if(data['packet_id'] == 10):
        #    print(data['packet_id'])