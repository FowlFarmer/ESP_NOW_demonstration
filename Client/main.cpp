#include <iostream>
#include <termios.h>
#include <cstring>
#include <fcntl.h> //file descriptior manipulation functions.
#include <unistd.h> //also file descriptor stuff (POSIX API)


int main(){
    int serial_port = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY | O_NDELAY); // Open serial port, read and write, not 
    //control terminal, non bocking mode (R/W prereturns value instead of holding up the system)
    if (serial_port == -1) {
        std::cerr << serial_port << "Failed to open serial port\n";
        return 1;
    }
    struct termios uart_config; //configure the structure
    memset(&uart_config, 0, sizeof(uart_config)); // Clear the struct for new settings

    if (tcgetattr(serial_port, &uart_config) != 0) {
        std::cerr << "Failed to get UART attributes." << std::endl; //failure msg
        close(serial_port);
        return 1;
    }
    cfsetispeed(&uart_config, B115200); //baud rates
    cfsetospeed(&uart_config, B115200);
        // Configure 8N1 (8 data bits, no parity, 1 stop bit)
    uart_config.c_cflag &= ~PARENB; // No parity
    uart_config.c_cflag &= ~CSTOPB; // 1 stop bit
    uart_config.c_cflag &= ~CSIZE;  // Clear the current character size mask
    uart_config.c_cflag |= CS8;     // 8 data bits
    uart_config.c_cflag |= (CLOCAL | CREAD);
    uart_config.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    uart_config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    // Set non-canonical mode
    uart_config.c_iflag &= ~(IXON | IXOFF | IXANY);    // Disable software flow control
    uart_config.c_oflag &= ~OPOST;    // Set raw output mode
    if (tcsetattr(serial_port, TCSANOW, &uart_config) != 0) { //set configuration settings, wrapped in error condition
            std::cerr << "Failed to set UART attributes." << std::endl;
            close(serial_port);
            return 1;
        }

    while(true){
    char type_byte{}; //write
    std::cin >> type_byte;
    int bytes_written = write(serial_port, &type_byte, 1);
    std::cout << "pass" << std::endl;
}}