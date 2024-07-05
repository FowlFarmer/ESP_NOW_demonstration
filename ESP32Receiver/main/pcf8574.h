#include <driver/i2c_master.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <driver/i2c_master.h>

#ifndef PCF_ADDRESS_0
#define PCF_ADDRESS_0 0x20
#endif
#ifndef PCF_ADDRESS_1
#define PCF_ADDRESS_1 0x21
#endif
#ifndef PCF_ADDRESS_2
#define PCF_ADDRESS_2 0x22
#endif
#ifndef PCF_ADDRESS_3
#define PCF_ADDRESS_3 0x23
#endif
#ifndef PCF_ADDRESS_4
#define PCF_ADDRESS_4 0x24
#endif
#ifndef PCF_ADDRESS_5
#define PCF_ADDRESS_5 0x25
#endif
#ifndef PCF_ADDRESS_6
#define PCF_ADDRESS_6 0x26
#endif
#ifndef PCF_ADDRESS_7
#define PCF_ADDRESS_7 0x27
#endif

class pcf8574 {

    public:
        pcf8574();
        void init(i2c_master_bus_handle_t bus_handle, uint8_t address, uint8_t timeout);
        void deinit();
        void set_bit(uint8_t pin, bool level);
        void set_bit_mask(uint8_t setBuffer);
        void read_bit_mask(uint8_t *readBuffer);
        bool read_bit(uint8_t pin);
        

    private:
        i2c_master_dev_handle_t _Device;
        uint8_t _Timeout;
};