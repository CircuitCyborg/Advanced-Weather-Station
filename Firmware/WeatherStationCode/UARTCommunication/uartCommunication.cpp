#include "UARTCommunication/UARTCommunication.h"

bool UARTCommunication::initialize(){
    stdio_init_all();
    uart_init(uart0, 9600);
    gpio_set_function(pinConfig.UART_TX_1, GPIO_FUNC_UART);
    gpio_set_function(pinConfig.UART_RX_1, GPIO_FUNC_UART);
    return true;
};

bool UARTCommunication::sendData(const std::string& data){
    uart_puts(uart0, data.c_str());
    return true;
}

std::string UARTCommunication::receiveData(){
    static char buffer[128];
    int i = 0;
    while(uart_is_readable(uart0) && i < sizeof(buffer) - 1)
    {
        buffer[i++] = uart_getc(uart0);
    }
    buffer[i] = '\0';
    return buffer:
}
