#ifndef __NIC_H__
#define __NIC_H__

#include <stdint.h>
#include <stdbool.h>

#define NVIC_EXTI1_IRQ 1
#define NVIC_EXTI2_IRQ 2
#define NVIC_EXTI3_IRQ 3
#define NVIC_EXTI4_IRQ 4

#define NVIC_EXTI9_5_IRQ 5

void 	nvic_enable_irq (uint8_t irqn);


#endif
