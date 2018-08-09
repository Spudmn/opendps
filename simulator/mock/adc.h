#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

extern uint32_t Mock_ACD_SR;

#define ADC_SR(x) Mock_ACD_SR
#define ADC1 1
#define ADC_SR_JEOC 1



uint32_t adc_read_injected	(	uint32_t 	adc,uint8_t 	reg )	;


#endif
