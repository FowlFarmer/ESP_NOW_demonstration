template <typename PacketInfo>
class RobotConnectPacket {
public:
    union {
        uint8_t data[sizeof(PacketInfo)];
        PacketInfo packet;
    };
};

struct EspnowHidBridgePacket {
    uint8_t packet_id; //206 for this packet
    uint8_t device_type; //0 for pocket RFID scanner device (keyboard input). Other devices using keyboard/mouse can be used.
    uint32_t device_id;
    uint8_t data[64]; //generic data for modular use.
};
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

using AMRDockedVoltageStatus = RobotConnectPacket<DockedVoltageStatus>;
using EspnowHidBridgePacketUnion = RobotConnectPacket<EspnowHidBridgePacket>;
