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

#include "tft.h"
#include "ili9163c.h"
#include "font-0.h"
#include "font-1.h"
#include "past.h"
#include "pastunits.h"


#include "logo.h"


#define TFT_HEIGHT  (128)
#define TFT_WIDTH   (128)

/** How ofter we update the measurements in the UI (ms) */
#define UI_UPDATE_INTERVAL_MS  (250)

/** Timeout for waiting for wifi connction (ms) */
#define WIFI_CONNECT_TIMEOUT  (10000)

/** Blit positions */
#define XPOS_WIFI     (4)
#define XPOS_LOCK    (30)

/** Constants describing how certain things on the screen flash when needed */
#define WIFI_CONNECTING_FLASHING_PERIOD  (1000)
#define WIFI_ERROR_FLASHING_PERIOD        (500)
#define WIFI_UPGRADING_FLASHING_PERIOD    (250)
#define LOCK_FLASHING_PERIOD              (250)
#define LOCK_FLASHING_COUNTER               (4)
#define TFT_FLASHING_PERIOD               (100)
#define TFT_FLASHING_COUNTER                (2)

static void ui_flash(void);
static void read_past_settings(void);
static void write_past_settings(void);
static void check_master_reset(void);

/** UI settings */
static uint16_t bg_color;
static uint32_t ui_width;
static uint32_t ui_height;

/** Used to make the screen flash */
static uint32_t tft_flashing_period;
static uint32_t tft_flash_counter;

/** Used for flashing the wifi icon */
static uint32_t wifi_status_flashing_period;
static bool wifi_status_visible;

/** Used for flashing the lock icon */
static uint32_t lock_flashing_period;
static bool lock_visible;
static uint32_t lock_flash_counter;

/** Current icon settings */
//static wifi_status_t wifi_status;
static bool is_locked;
static bool is_enabled;

/** Last settings written to past */
static bool     last_tft_inv_setting;

/** Our parameter storage */
static past_t g_past = {
    .blocks = {0x0800f800, 0x0800fc00}
};

/** The function UI displaying the current active function */
//static uui_t func_ui;
/** The main UI displaying input voltage and such */
//static uui_t main_ui;



/**
  * @brief Check if user wants master reset, resetting the past area
  * @retval none
  */
static void check_master_reset(void)
{
    if (hw_sel_button_pressed()) {
        dbg_printf("Master reset\n");
        if (!past_format(&g_past)) {
            /** @todo Handle past format errors */
            dbg_printf("Error: past formatting failed!\n");
        }
    }
}




/**
  * @brief Read settings from past
  * @retval none
  */
static void read_past_settings(void)
{
    bool     inverse_setting = false;
    uint32_t length;
    uint32_t *p = 0;
    if (past_read_unit(&g_past, past_tft_inversion, (const void**) &p, &length)) {
        if (p) {
            inverse_setting = !!(*p);
        }
    }
    tft_invert(inverse_setting);

#ifdef GIT_VERSION
    /** Update app git hash in past if needed */
    char *ver = 0;
    bool exists = past_read_unit(&g_past, past_app_git_hash, (const void**) &ver, &length);
    if (!exists || strncmp((char*) ver, GIT_VERSION, 32 /* probably never longer than 32 bytes */) != 0) {
        if (!past_write_unit(&g_past, past_app_git_hash, (void*) &GIT_VERSION, strlen(GIT_VERSION))) {
            /** @todo Handle past write errors */
            dbg_printf("Error: past write app git hash failed!\n");
        }
    }
#endif // GIT_VERSION
}







/**
  * @brief Initialize the UI
  * @retval none
  */
static void ui_init(void)
{
    bg_color = BLACK;
    ui_width = TFT_WIDTH;
    ui_height = TFT_HEIGHT;

//    uui_init(&func_ui, &g_past);
//    func_cv_init(&func_ui);
#ifdef CONFIG_CC_ENABLE
    func_cc_init(&func_ui);
#endif // CONFIG_CC_ENABLE
//    uui_activate(&func_ui);

//    uui_init(&main_ui, &g_past);
//    number_init(&input_voltage);
//    input_voltage.ui.x = 105;
//    input_voltage.ui.y = ui_height - font_0_height;
//    uui_add_screen(&main_ui, &main_screen);
//    uui_activate(&main_ui);
}



#ifdef CONFIG_SPLASH_SCREEN
/**
  * @brief Draw splash screen
  * @retval none
  */
static void ui_draw_splash_screen(void)
{
    tft_blit((uint16_t*) logo, logo_width, logo_height, (ui_width-logo_width)/2, (ui_height-logo_height)/2);
}
#endif // CONFIG_SPLASH_SCREEN


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
    hw_init();
    pwrctl_init(); // Must be after DAC init
//    Init_Protocol_handler();
//    event_init();
//
//#ifdef CONFIG_COMMANDLINE
//    dbg_printf("Welcome to OpenDPS!\n");
//    dbg_printf("Try 'help;' for, well, help (note the semicolon).\n");
//#endif // CONFIG_COMMANDLINE
//
    tft_init();
    delay_ms(50); // Without this delay we will observe some flickering
    tft_clear();
#ifdef DPS_EMULATOR
    dps_emul_init(&g_past, argc, argv);
#else // DPS_EMULATOR
    (void) argc;
    (void) argv;
    g_past.blocks[0] = 0x0800f800;
    g_past.blocks[1] = 0x0800fc00;
#endif // DPS_EMULATOR
    if (!past_init(&g_past)) {

        dbg_printf("Error: past init failed!\n");
        /** @todo Handle past init failure */
    }

    check_master_reset();
    read_past_settings();
    ui_init();
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
#ifdef CONFIG_SPLASH_SCREEN
    ui_draw_splash_screen();
    hw_enable_backlight();
    delay_ms(750);
    tft_clear();
#endif // CONFIG_SPLASH_SCREEN
//  //  event_handler();


    return 0;
}
