#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>

#define GPIOA 0x00
#define GPIOB 0x01


#define GPIO2 2
#define GPIO3 3
#define GPIO1 1
#define GPIO8 8
#define GPIO9 9
#define GPIO14 14
#define GPIO12 12
#define GPIO14 14
#define GPIO12 12
#define GPIO14 14


#define GPIO12 12
#define GPIO14 14



#define GPIO_IDR(port) port

void 	gpio_toggle (uint32_t gpioport, uint8_t gpios);
// 	Toggle a Group of Pins. More...

uint8_t 	gpio_read (uint32_t gpioport, uint8_t gpios);
// 	Get status of a Group of Pins (atomic) More...

void 	gpio_write (uint32_t gpioport, uint8_t gpios, uint8_t data);
// 	Set level of a Group of Pins (atomic) More...

void 	gpio_set (uint32_t gpioport, uint8_t gpios);
// 	Set a Group of Pins (atomic) More...

void 	gpio_clear (uint32_t gpioport, uint8_t gpios);
// 	Clear a Group of Pins (atomic) More...

uint8_t 	gpio_port_read (uint32_t gpioport);
// 	Read level of all pins from a port (atomic) More...

void 	gpio_port_write (uint32_t gpioport, uint8_t data);
// 	Set level of of all pins from a port (atomic) More...


uint16_t gpio_get(	uint32_t 	gpioport,uint16_t 	gpios );


void mainwindow_gpio_set (uint32_t gpioport, uint8_t gpios);
void mainwindow_gpio_clear (uint32_t gpioport, uint8_t gpios);
bool mainwindow_spi_dma_transceive(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len);





#endif
