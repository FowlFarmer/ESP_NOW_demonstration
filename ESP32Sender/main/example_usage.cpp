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


static const char *TAG = "Sender"; //needed for ESP_LOGI

uint8_t receiver_mac[6]{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
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


template <typename PacketInfo>
class RobotConnectPacket {
public:
    union {
        uint8_t data[sizeof(PacketInfo)];
        PacketInfo packet;
    };
};
using AMRDockedVoltageStatus = RobotConnectPacket<DockedVoltageStatus>;

extern "C"{ //mangling will make app_main() unrecognizeable to esp32 microcontroller so add syntax to not mangle (interesting topic)


void app_main(void) {
    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
    .rx_flow_ctrl_thresh = 122,
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 9, 8, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here hmmm i see :)
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, \
        uart_buffer_size, 10, &uart_queue, 0));
    ESP_LOGI(TAG, "boot UART");
    // Initialize NVS //NVS required for 
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

    //uint8_t read{};
    //uint8_t time{0};
    while(1){
        ESP_LOGI(TAG, "pass");
        //uart_read_bytes(uart_num, &read, 1, 1000);
        //if(read != NULL){
            //////////////////
        //ESP_ERROR_CHECK(
        AMRDockedVoltageStatus status{};
        BmsData data{};
        
        status.packet.packet_id = 10;
        status.packet.device_id = 0;
        status.packet.aux_voltage = 27000;
        status.packet.device_name[19] = 'a';
        data.charge = 69;
        data.packet_id = 69;
        data.power_supply_health = 69;
        data.voltage = 420;
        data.current = 421;
        data.percentage = 5;
        data.temperature = 18;
        data.charge = 32;
        status.packet.bms_data = data;


        ESP_ERROR_CHECK(esp_now_send(receiver_mac, status.data, sizeof(status)));
        ESP_LOGI(TAG, "%d : %f", sizeof(status), status.packet.bms_data.temperature); //%lu is long unsigned
        vTaskDelay(pdMS_TO_TICKS(200));
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




