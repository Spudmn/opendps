#include <gpio.h>
#include "spi_driver.h"













void gpio_set (uint32_t gpioport, uint8_t gpios)
{
	mainwindow_gpio_set (gpioport, gpios);
}


void 	gpio_clear (uint32_t gpioport, uint8_t gpios)
{
	mainwindow_gpio_clear (gpioport, gpios);
}




bool spi_dma_transceive(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len)
{
    return mainwindow_spi_dma_transceive(tx_buf,tx_len, rx_buf, rx_len);
}
