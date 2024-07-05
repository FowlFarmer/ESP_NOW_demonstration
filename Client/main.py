import serial
import struct

a = 27*8
#header 2byte, len 2byte, sum 1byte
data_format = '<HHBBI20sBffIIIBBBHB'
#raw_format = '<B B B B {a}s'


"""
struct BmsData {
    uint8_t packet_id;
    float voltage;
    float current;
    float percentage;
    float temperature;
    uint32_t charge;
    uint8_t power_supply_status;
    uint8_t power_supply_health;
    uint8_t power_supply_technology;
} __attribute__((__packed__));

struct DockedVoltageStatus {
    uint8_t packet_id;
    uint32_t device_id;
    char device_name[20];
    BmsData bms_data;
    uint16_t aux_voltage;     // External auxiliary voltage (in millivolts)
    uint8_t conn_health : 2;  // Connection health (00 = poor, 01 = good, 10 = fair, 11 = excellent)
    uint8_t aux_health : 2;   // Connection health [aux] (00 = poor, 01 = good, 10 = fair, 11 = excellent)
    uint8_t reserved : 4;     // Reserved for future use
} __attribute__((__packed__));
"""

def unpack_packet(packet):
    unpacked_data = struct.unpack(data_format, packet)
    

    
    # Extract PowerMonStatus data
    power_mon_status = {
        'header': hex(unpacked_data[0]),
        'length': unpacked_data[1],
        'checksum': unpacked_data[2],
        'packet_id': unpacked_data[3],
        'device_id': unpacked_data[4],
        'device_name': unpacked_data[5],
        'bms_data_packet_id': unpacked_data[6],
        'voltage': unpacked_data[7],
        'current': unpacked_data[8],
        'percentage': bytes(unpacked_data[9]),
        'temperature': bytes(unpacked_data[10]),
        'charge': unpacked_data[11],
        'power_supply_status': unpacked_data[12],
        'power_supply_health': unpacked_data[13],
        'power_supply_technology': unpacked_data[14],
        'aux_voltage': unpacked_data[15],
        'conn_health': unpacked_data[16] & 0b11,
        'aux_health': (unpacked_data[16] >> 2) & 0b11,
        'reserved': (unpacked_data[16] >> 4) & 0b1111
    }
    
    return power_mon_status
with serial.Serial('/dev/ttyUSB1', 921600, timeout = 1)  as ser:# open serial port
    ser.reset_input_buffer()
    while(1):
        ser.reset_input_buffer()
        data = unpack_packet(ser.read(57))
        if(data['packet_id'] == 10):
            print(data)
        #if(data['packet_id'] == 10):
        #    print(data['packet_id'])