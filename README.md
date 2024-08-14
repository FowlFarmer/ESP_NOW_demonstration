# ESP_NOW_demonstration
Demonstration of c++ project to communicate between two ESP32-C3 microcontrollers through WIFI protocol and ESP-NOW. This project has been expanded to facilitate testing of a Rapyutian ESP-NOW pipeline for communication between an autonomous battery exchange robot and an Automated Storage and Retrieval (ASRS) Autonomous Mobile Robot (AMR).

Utilizes: ESP-NOW, WIFI, NVS, UART, I2C, Termios & Pyserial, ESP-IDF

Computer inputs --Via UART--> Sender ESP32-C3 --Via ESP-NOW--> Receiver ESP32-C3 --Via I2C--> GPIO Expander --> LEDs
                                                                                 --Via Serial--> IDF Monitor

Expanded to send and receive specific test packets for various projects.
