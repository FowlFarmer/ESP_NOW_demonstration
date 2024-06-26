#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <driver/i2c_master.h>
#include "pcf8574.h"
#include "esp_log.h"

static const char *TAG = "tag";

pcf8574::pcf8574(){
    //default constructor here
}

void pcf8574::init(i2c_master_bus_handle_t bus_handle, uint8_t address, uint8_t timeout){
    _Timeout = timeout;
    if(i2c_master_probe(bus_handle, address, timeout) != ESP_OK){
        ESP_LOGI(TAG, "Error: unable to find / connect to i2c device");
        return;
    }
    i2c_device_config_t config;
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7,
    config.device_address = address,
    config.scl_speed_hz = 100000,
    
    i2c_master_bus_add_device(bus_handle, &config, &_Device);
}

void pcf8574::deinit(){
    i2c_master_bus_rm_device(_Device);
}

void pcf8574::read_bit_mask(uint8_t *readBuffer){
    if(readBuffer == nullptr){
        ESP_LOGI(TAG, "Error: readBuffer is null");
        return;
    }
    i2c_master_receive(_Device, readBuffer, 1, _Timeout);
}

bool pcf8574::read_bit(uint8_t pin){
    if(pin > 7){
        ESP_LOGI(TAG, "Error: read pin > 7");
        return 0;
    }
    uint8_t read{};
    i2c_master_receive(_Device, &read, 1, _Timeout);
    if((read&(0b00000001<<(pin))) == 0b00000000){
        return 0;
    }
    return 1;
}

void pcf8574::set_bit_mask(uint8_t setBuffer){
    i2c_master_transmit(_Device, &setBuffer, 1, _Timeout);
}

void pcf8574::set_bit(uint8_t pin, bool level){
    if(pin > 7){
        ESP_LOGI(TAG, "Error: set pin > 7");
        return;
    }
    uint8_t pin0b = 0b00000001<<(pin);
    uint8_t read{};
    read_bit_mask(&read);
    if(level){
        read |= pin0b;
    }
    else{
        read &= (~pin0b);
    }
    i2c_master_transmit(_Device, &read, 1, _Timeout);
}
