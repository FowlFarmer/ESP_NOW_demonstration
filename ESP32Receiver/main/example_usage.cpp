#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <driver/i2c_master.h>
#include "pcf8574.h"
#include "esp_log.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_event.h>


static const char *TAG = "Receiver"; //needed for ESP_LOGI

enum PacketID{
    BEC_DOCKED_VOLTAGE_STATUS = 90,
};

struct DockedVoltageStatus {
    uint8_t packet_id; // RBT_DOCKED_VOLTAGE_STATUS
    uint32_t device_id;
    uint16_t aux_voltage; // millivolt current voltage
};

DockedVoltageStatus status;

pcf8574 gpioexpander; //move this global so cb can use it

//uint8_t sender_mac[6]{0x84, 0xf7, 0x03, 0x05, 0xa6, 0x1c};
void cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int len){
    if(*data == BEC_DOCKED_VOLTAGE_STATUS){
        memcpy(&status, data, len);
        ESP_LOGI(TAG, "Received: %d", status.aux_voltage);
    }
    return;
}

extern "C" void app_main()
{
        // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    i2c_master_bus_config_t conf;
    conf.clk_source = I2C_CLK_SRC_DEFAULT;
    conf.i2c_port = I2C_NUM_0;
    conf.sda_io_num = GPIO_NUM_18;         // select GPIO specific to your project |||||||||||| numbers work in c due to less strict typed rules.
    conf.scl_io_num = GPIO_NUM_19;         // select GPIO specific to your project
    conf.glitch_ignore_cnt = 7;
    conf.intr_priority = 2;
    conf.trans_queue_depth = 0;
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&conf, &bus_handle)); 
    
    gpioexpander.init(bus_handle, PCF_ADDRESS_0, (uint8_t)1000); //all i2c

    ESP_LOGI(TAG, "boot 1");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "boot WIFI");

    ESP_ERROR_CHECK(esp_now_init());


    ESP_LOGI(TAG, "boot 2");
    ESP_ERROR_CHECK(esp_now_register_recv_cb(cb));
    //while(1){
        //if(read != NULL){
            // gpioexpander.set_bit_mask(read);
            // ESP_LOGI(TAG, "%d", read); //we can move all of this into the callback function.
          //  read = NULL;
            //}
      //  }
    }




