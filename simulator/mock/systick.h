#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>
#include <stdbool.h>

#define STK_CSR_CLKSOURCE_AHB_DIV8 100

void systick_set_clocksource(uint32_t);

void systick_set_reload(uint32_t);

void systick_interrupt_enable(void);
void systick_counter_enable(void);

void sys_tick_handler(void);

void mainwindow_processEvents();

#endif
