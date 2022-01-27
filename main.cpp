/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "print_color.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "MAIN"

// Create a DigitalOutput object to toggle an LED whenever data is received.
DigitalOut led(LED1);

// Create a UnbufferedSerial object with a default baud rate.
UnbufferedSerial debug_port(MBED_CONF_TARGET_STDIO_UART_TX, MBED_CONF_TARGET_STDIO_UART_RX);
UnbufferedSerial wb55_port(MBED_CONF_APP_WB55_UART_TX, MBED_CONF_APP_WB55_UART_RX);

void init_port()
{
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
}

void on_rx_interrupt()
{
    char c;
    // Read the data to clear the receive interrupt.
    if (wb55_port.read(&c, 1)) {
        // Echo the input back to the terminal.
        debug_port.write(&c, 1);
    }
}

void register_rx_irq_callback()
{
    wb55_port.attach(&on_rx_interrupt, SerialBase::RxIrq);
}

int main(void)
{
    mbed_trace_init();
    printf(ANSI_COLOR_RESET);
    printf(ANSI_COLOR_MAGENTA"                                                      oMMMMM/  yhhhhh              \n");
    printf(ANSI_COLOR_MAGENTA"                                                      oMMMMM/  yhhhhh              \n");
    printf(ANSI_COLOR_MAGENTA"                                                      oMMMMM/  yhhhhh              \n");
    printf(ANSI_COLOR_MAGENTA"                                                      oMMMMM/  yhhhhh              \n");
    printf(ANSI_COLOR_MAGENTA"    :++++++++++++++++++++++++/    `ossssssssssssssssssdMMMMM/  yhhhhh+++++++++.    \n");
    printf(ANSI_COLOR_MAGENTA"   /hhhhhhhhhhhhhhhhhhhhhhhhhs   .dMMMMMMMMMMMMMMMMMMMMMMMMM/  yhhhhhhhhhhhhhh-    \n");
    printf(ANSI_COLOR_MAGENTA"  ohhhhhhhhhhhhhhhhhhhhhhhhhhs  -mMMMMMMMMMMMMMMMMMMMMMMMMMM/  yhhhhhhhhhhhhhh-    \n");
    printf(ANSI_COLOR_MAGENTA"  :::::::::::::::::::::::::::-  +MMMMMm+//////////////hMMMMM/  :::::::::::::::`    \n");
    printf(ANSI_COLOR_MAGENTA"                                +MMMMM+               oMMMMM/                      \n");
    printf(ANSI_COLOR_MAGENTA"  hhhhhhhhhhhhhhhhhhhhhhhhhhhs  +MMMMM+               oMMMMM/  yhhhhh              \n");
    printf(ANSI_COLOR_MAGENTA"  MMMMMMMMMMMMMMMMMMMMMMMMMMMd  +MMMMM+               oMMMMM/  mMMMMN              \n");
    printf(ANSI_COLOR_MAGENTA"  NNNNNNNNNNNNNNNNNNNNNNMMMMMd  +MMMMM+               oMMMMM/  mMMMMN              \n");
    printf(ANSI_COLOR_MAGENTA"  ---------------------:MMMMMd  +MMMMM+               oMMMMM/  mMMMMN              \n");
    printf(ANSI_COLOR_MAGENTA"  `````````````````````/MMMMMd  +MMMMM+```````````````hMMMMM/  mMMMMN``````-yyyyyy \n");
    printf(ANSI_COLOR_MAGENTA"  mmmmmmmmmmmmmmmmmmmmmNMMMMMh  +MMMMMNmmmmmmmmmmmmmmNMMMMMN:  mMMMMMmmmmmmNMMMMMN \n");
    printf(ANSI_COLOR_MAGENTA"  MMMMMMMMMMMMMMMMMMMMMMMMMMd.  +MMMMMMMMMMMMMMMMMMMMMMMMMN+   mMMMMMMMMMMMMMMMMm- \n");
    printf(ANSI_COLOR_MAGENTA"  MMMMMMMMMMMMMMMMMMMMMMMMMy    +MMMMMMMMMMMMMMMMMMMMMMMMN:    mMMMMMMMMMMMMMMMh`  \n");
    printf(ANSI_COLOR_RESET);

    printf("\n**********************************************************************\n");
    printf("** Start Mooving Monitor...\n");
    printf("** Build Date: %s: %s \n", __DATE__,__TIME__);
    printf("**********************************************************************\n\n");

    init_port();

    register_rx_irq_callback();

#if 1
    FileHandle *_console = mbed_file_handle(STDIN_FILENO);
    int cnt = 10;
    while (cnt--)
    {
        if (_console->readable())
        {
            char c = 0;
            _console->read(&c, 1);
            tr_debug("read from console!\n");

            wb55_port.write(&c, 1);
            tr_debug("write to wb55!\n");

            cnt=0;
        }

       ThisThread::sleep_for(1s);
       tr_debug("cnt : %d\n", cnt);
    }
#endif

}