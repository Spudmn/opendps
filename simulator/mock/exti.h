#ifndef __EXTI_H__
#define __EXTI_H__

#define EXTI1 1
#define EXTI2 2
#define EXTI3 3
#define EXTI4 4
#define EXTI5 5
#define EXTI8 8
#define EXTI9 9



/* Trigger types */
 enum exti_trigger_type {
         EXTI_TRIGGER_RISING,
         EXTI_TRIGGER_FALLING,
         EXTI_TRIGGER_BOTH,
 };




void exti_set_trigger(uint32_t extis, enum exti_trigger_type trig);
void exti_enable_request(uint32_t extis);
void exti_disable_request(uint32_t extis);
void exti_reset_request(uint32_t extis);
void exti_select_source(uint32_t exti, uint32_t gpioport);
uint32_t exti_get_flag_status(uint32_t exti);


#endif
