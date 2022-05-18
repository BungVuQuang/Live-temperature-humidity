#ifndef __ESP_UART_H
#define __ESP_UART_H
#include <stdint.h>
// dinh nghia 1 func poiter de xu ly du lieu nhan duoc tren uart
typedef void (*uart_data_handle_t)(uint8_t *data, uint16_t len);

void esp_uart_init(void);
void esp_uart_put(uint8_t *data, uint16_t len);
void esp_uart_set_callback_data(void *cb);
#endif