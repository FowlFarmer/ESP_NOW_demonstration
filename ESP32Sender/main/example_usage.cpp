#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
//#include <driver/i2c_master.h>
//#include "pcf8574.h"
#include "esp_log.h"
#include <driver/uart.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include "packets.h"

static const char *TAG = "Sender"; //needed for ESP_LOGI

uint8_t receiver_mac[6]{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //broadcast address

extern "C"{ //mangling will make app_main() unrecognizeable to esp32 microcontroller so add syntax to not mangle (interesting topic)

void espnowInit(void){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the Wi-Fi stack
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(14, WIFI_SECOND_CHAN_NONE));
    ESP_LOGI(TAG, "boot WIFI");

    ESP_ERROR_CHECK(esp_now_init());
    
    esp_now_peer_info_t peerInfo = {}; //YOU NEED THIS STUFF EVEN IF U ONLY HAVE 1 ESP NOW!!
    memcpy(peerInfo.peer_addr, receiver_mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo)); //THIS IS TO ADD IT TO PEER LIST, CAN JUST PUT ADDRESS IF 1 ESP DEVICE TO TALK TO


}
void app_main(void) {

    espnowInit();
    EspnowHidBridgePacketUnion packet;
    packet.packet.packet_id = 206;
    uint8_t buffer[23] = {16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16, 18, 16};
    memcpy(packet.packet.data, buffer, 23);

    while(1){
        ESP_LOGI(TAG, "pass");



        //ESP_ERROR_CHECK(esp_now_send(receiver_mac, status.data, sizeof(status)));
        //ESP_LOGI(TAG, "%d : %f", sizeof(status), status.packet.bms_data.temperature); //%lu is long unsigned
        vTaskDelay(pdMS_TO_TICKS(3000));
        esp_now_send(receiver_mac, packet.data, sizeof(packet));
        //uart_write_bytes(uart_num, &status, sizeof(status));
            //////////////////
        //ESP_LOGI(TAG, "%d", read);

        //uart_write_bytes(uart_num, &read, 1);
        }/*
        else{
            ESP_ERROR_CHECK(esp_now_send(&receiver_mac[0], &time, 1));
            ++time;
            //////////////////
        }*/
        //read = NULL;
 //   }
}
}

    

//     uint8_t read{};
//     while(1){
//             uart_read_bytes(uart_num, &read, 1, 1000);
//             if(read != NULL){
//                 //////////////////
//             esp_now_send(peer, &read, 1)
//                 //////////////////
//             uart_write_bytes(UART_NUM_1, &read, 1);
//             ESP_LOGI(TAG, "%d", read);
//             }
//             read = NULL;
//         }
//     }




