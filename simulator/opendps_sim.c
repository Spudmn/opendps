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
#include <string.h>


/**
  * @brief Ye olde main
  * @retval preferably none
  */
#ifdef DPS_EMULATOR
int sim_main(int argc, char const *argv[])
#else
int main(int argc, char const *argv[])
#endif // DPS_EMULATOR
{
//    hw_init();
//    pwrctl_init(); // Must be after DAC init
//    Init_Protocol_handler();
//    event_init();
//
//#ifdef CONFIG_COMMANDLINE
//    dbg_printf("Welcome to OpenDPS!\n");
//    dbg_printf("Try 'help;' for, well, help (note the semicolon).\n");
//#endif // CONFIG_COMMANDLINE
//
//    tft_init();
//    delay_ms(50); // Without this delay we will observe some flickering
//    tft_clear();
//#ifdef DPS_EMULATOR
//    dps_emul_init(&g_past, argc, argv);
//#else // DPS_EMULATOR
//    (void) argc;
//    (void) argv;
//    g_past.blocks[0] = 0x0800f800;
//    g_past.blocks[1] = 0x0800fc00;
//#endif // DPS_EMULATOR
//    if (!past_init(&g_past)) {
//        dbg_printf("Error: past init failed!\n");
//        /** @todo Handle past init failure */
//    }
//
//    check_master_reset();
//    read_past_settings();
//    ui_init();
//
//#ifdef CONFIG_WIFI
//    /** Rationale: the ESP8266 could send this message when it starts up but
//      * the current implementation spews wifi/network related messages on the
//      * UART meaning this message got lost. The ESP will however send the
//      * wifi_connected status when it connects but if that does not happen, the
//      * ui module will turn off the wifi icon after 10s to save the user's
//      * sanity
//      */
//    opendps_update_wifi_status(wifi_connecting);
//#endif // CONFIG_WIFI
//
//#ifdef CONFIG_SPLASH_SCREEN
//    ui_draw_splash_screen();
//    hw_enable_backlight();
//    delay_ms(750);
//    tft_clear();
//#endif // CONFIG_SPLASH_SCREEN
//  //  event_handler();
    return 0;
}
