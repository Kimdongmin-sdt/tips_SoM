/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Create a DigitalOutput object to toggle an LED whenever data is received.
DigitalOut led(LED1);

// Create a UnbufferedSerial object with a default baud rate.
UnbufferedSerial debug_port(MBED_CONF_TARGET_STDIO_UART_TX, MBED_CONF_TARGET_STDIO_UART_RX);
UnbufferedSerial wb55_port(MBED_CONF_APP_WB55_UART_TX, MBED_CONF_APP_WB55_UART_RX);

void on_rx_interrupt()
{
    char c;
    // Read the data to clear the receive interrupt.
    if (wb55_port.read(&c, 1)) {
        // Echo the input back to the terminal.
        debug_port.write(&c, 1);
    }
}

int main(void)
{
    printf("main start!!\n");

    debug_port.baud(115200);
    debug_port.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    wb55_port.baud(115200);
    wb55_port.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    // Register a callback to process a Rx (receive) interrupt.
    wb55_port.attach(&on_rx_interrupt, SerialBase::RxIrq);
}