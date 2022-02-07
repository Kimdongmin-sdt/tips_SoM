/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "print_color.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed-client-cli/ns_cmdline.h"
#include <cstring>

#define TRACE_GROUP         "MAIN"
#define CMD_MAX_SIZE        50
#define TEST_MAN            "Command-line tool for testing SDT tips thread\n"   \
                            "SDT_Thread_cmd <Commands>\r\n"                     \
                            "example : SDT_Thread_cmd thread_run0\r\n"          \
                            "Commands:                        desc\r\n"         \
                            "thread_run0                      panid 0x1234\n"   \
                            "thread_run1                      channel 12\n"     \
                            "thread_run2                      ifconfig up\n"    \
                            "thread_run3                      thread start\n"   \
                            "thread_run4                      state\n"

static UnbufferedSerial     debug_port(MBED_CONF_TARGET_STDIO_UART_TX, MBED_CONF_TARGET_STDIO_UART_RX);
static UnbufferedSerial     wb55_port(MBED_CONF_APP_WB55_UART_TX, MBED_CONF_APP_WB55_UART_RX);
static Mutex                SerialOutMutex;
static char                 cmd_buf[CMD_MAX_SIZE + 1];

void serial_out_mutex_wait()
{
    SerialOutMutex.lock();
}
void serial_out_mutex_release()
{
    SerialOutMutex.unlock();
}

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

void thread_run0()
{
#define THREAD_RUN_0_CMD "panid 0x1234\r\n"
    tr_debug("thread run0!!\n");
    memset(cmd_buf, 0x00, sizeof(cmd_buf));
    strncpy(cmd_buf, THREAD_RUN_0_CMD, strlen(THREAD_RUN_0_CMD));
    wb55_port.write(cmd_buf, strlen(cmd_buf));
}

void thread_run1()
{
#define THREAD_RUN_1_CMD "channel 12\r\n"
    tr_debug("thread run1!!\n");

    memset(cmd_buf, 0x00, sizeof(cmd_buf));
    strncpy(cmd_buf, THREAD_RUN_1_CMD, strlen(THREAD_RUN_1_CMD));
    wb55_port.write(cmd_buf, strlen(cmd_buf));
}

void thread_run2()
{
#define THREAD_RUN_2_CMD "ifconfig up\r\n"
    tr_debug("thread run2!!\n");

    memset(cmd_buf, 0x00, sizeof(cmd_buf));
    strncpy(cmd_buf, THREAD_RUN_2_CMD, strlen(THREAD_RUN_2_CMD));
    wb55_port.write(cmd_buf, strlen(cmd_buf));
}

void thread_run3()
{
#define THREAD_RUN_3_CMD "thread start\r\n"
    tr_debug("thread run3!!\n");
    memset(cmd_buf, 0x00, sizeof(cmd_buf));
    strncpy(cmd_buf, THREAD_RUN_3_CMD, strlen(THREAD_RUN_3_CMD));
    wb55_port.write(cmd_buf, strlen(cmd_buf));
}

void thread_run4()
{
#define THREAD_RUN_4_CMD "state\r\n"
    tr_debug("thread run4!!\n");
    memset(cmd_buf, 0x00, sizeof(cmd_buf));
    strncpy(cmd_buf, THREAD_RUN_4_CMD, strlen(THREAD_RUN_4_CMD));
    wb55_port.write(cmd_buf, strlen(cmd_buf));
}

#define CMD_POS 1
int callback_Thread_app_cmd(int argc, char *argv[])
{
    int ret;
    int size;
    if (ret = cmd_parameter_index(argc, argv, "thread_run0") == CMD_POS) {
         tr_debug("find cmd=%s(%d)\n", argv[ret], ret);
         tr_debug("path=%s\n", argv[ret + 2]);
        thread_run0();
        cmd_printf("%s\r\n", TEST_MAN);
        return CMDLINE_RETCODE_SUCCESS;
    } else if (ret = cmd_parameter_index(argc, argv, "thread_run1") == CMD_POS) {
         tr_debug("find cmd=%s(%d)\n", argv[ret], ret);
         tr_debug("path=%s\n", argv[ret + 2]);
        thread_run1();
        cmd_printf("%s\r\n", TEST_MAN);
        return CMDLINE_RETCODE_SUCCESS;
    } else if (ret = cmd_parameter_index(argc, argv, "thread_run2") == CMD_POS) {
         tr_debug("find cmd=%s(%d)\n", argv[ret], ret);
         tr_debug("path=%s\n", argv[ret + 2]);
        thread_run2();
        cmd_printf("%s\r\n", TEST_MAN);
        return CMDLINE_RETCODE_SUCCESS;
    } else if (ret = cmd_parameter_index(argc, argv, "thread_run3") == CMD_POS) {
         tr_debug("find cmd=%s(%d)\n", argv[ret], ret);
         tr_debug("path=%s\n", argv[ret + 2]);
        thread_run3();
        cmd_printf("%s\r\n", TEST_MAN);
        return CMDLINE_RETCODE_SUCCESS;
    } else if (ret = cmd_parameter_index(argc, argv, "thread_run4") == CMD_POS) {
         tr_debug("find cmd=%s(%d)\n", argv[ret], ret);
         tr_debug("path=%s\n", argv[ret + 2]);
        thread_run4();
        cmd_printf("%s\r\n", TEST_MAN);
        return CMDLINE_RETCODE_SUCCESS;
    }
}

void Thread_app_cmd()
{
    // Initialize cmd library
    cmd_init(0);
    // Register callback used to lock serial out mutex
    cmd_mutex_wait_func(&serial_out_mutex_wait);
    // Register callback used to release serial out mutex
    cmd_mutex_release_func(&serial_out_mutex_release);

    cmd_add("SDT_Thread_cmd", callback_Thread_app_cmd,
            "SDT Thread app",
            TEST_MAN);

    cmd_printf("Mbed os Version %d.%d.%d\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    cmd_printf("%s\r\n", TEST_MAN);
    cmd_output();

    while (true) {
        char c = getchar();
        if (EOF != c) {
            cmd_char_input(c);
        }
    }
}

int main(void)
{
    ThisThread::sleep_for(5s);

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
    printf("** Start TIPS Monitor\n");
    printf("** Build Date: %s: %s \n", __DATE__,__TIME__);
    printf("**********************************************************************\n\n");

    init_port();

    register_rx_irq_callback();

    FileHandle *_console = mbed_file_handle(STDIN_FILENO);
    char c = 0;
    while (true) {
        if (_console->readable()) {
            tr_debug("input for Thread command\n");

            _console->read(&c, sizeof(c));
            tr_debug("read contents : %s\n", c);
            tr_debug("read from console!\n");
#if 1
            wb55_port.write(&c, 1);
#endif
            Thread_app_cmd();
        }
    }

    ThisThread::sleep_for(1s);
}