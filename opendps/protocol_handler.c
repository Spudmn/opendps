/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Johan Kanflo (github.com/kanflo)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <usart.h>
#include <string.h>
#include <stdlib.h>
#include <scb.h>
#include "dbg_printf.h"
#include "hw.h"
#include "uui.h"
#include "pwrctl.h"
#include "uframe.h"
#include "protocol.h"
#include "serialhandler.h"
#include "bootcom.h"
#include "uframe.h"
#include "opendps.h"
#include "scpi/scpi.h"  //Needs to be before Winsock2.h
#include "scpi-def.h"


#include "scpi_commands.h"


#define SCPI_COMMAND(P, C) scpi_result_t C(scpi_t * context);
SCPI_COMMANDS
#undef SCPI_COMMAND


#define SCPI_COMMAND(P, C) {P, C},
static const scpi_command_t scpi_commands[] = {
    SCPI_COMMANDS
    SCPI_CMD_LIST_END
};



scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;


#ifdef DPS_EMULATOR
 extern void dps_emul_send_frame(uint8_t *frame, uint32_t length);
#endif // DPS_EMULATOR



/**
  * @brief Send a frame on the uart
  * @param frame the frame to send
  * @param length length of frame
  * @retval None
  */
static void send_frame(uint8_t *frame, uint32_t length)
{
#ifdef DPS_EMULATOR
    dps_emul_send_frame(frame, length);
#else // DPS_EMULATOR
    do {
        usart_send_blocking(USART1, *frame);
        frame++;
    } while(--length);
#endif // DPS_EMULATOR
  }



size_t SCPI_Write(scpi_t * context, const char * data, size_t len) {
    (void) context;
    send_frame(data, len);
//    dbg_printf("SCPI_Write\n");
    return fwrite(data, 1, len, stdout);
}

scpi_result_t SCPI_Flush(scpi_t * context) {
    (void) context;
    dbg_printf("SCPI_Flush\n");
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) {
    (void) context;

    dbg_printf(stderr, "**ERROR: %d, \"%s\"\r\n", (int16_t) err, SCPI_ErrorTranslate(err));
    return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    (void) context;

    if (SCPI_CTRL_SRQ == ctrl) {
      dbg_printf(stderr, "**SRQ: 0x%X (%d)\r\n", val, val);
    } else {
      dbg_printf(stderr, "**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
    }
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
    (void) context;

    dbg_printf(stderr, "**Reset\r\n");
    return SCPI_RES_OK;
}





void Init_Protocol_handler(void)
{
  dbg_printf("Init_Protocol_handler \n");

  SCPI_Init(&scpi_context,
          scpi_commands,
          &scpi_interface,
          scpi_units_def,
          SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
          scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
          scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);


}


/**
  * @brief Handle received character
  * @param c well, the received character
  * @retval None
  */
void serial_handle_rx_char(char c)
{
  SCPI_Input(&scpi_context, &c, 1);
}
