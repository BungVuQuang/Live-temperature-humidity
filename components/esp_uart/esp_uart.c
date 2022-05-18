#include "esp_uart.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "uart_esp";

/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

//#define EX_UART_NUM UART_NUM_2
const uart_port_t EX_UART_NUM = UART_NUM_2;
//#define PATTERN_CHR_NUM (3) /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart2_queue;

static uart_data_handle_t uart_data_handle = NULL; // khỏi tạo con trỏ, đầu tiên con trỏ chưa trỏ đến đâu
static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    // size_t buffered_size;
    uint8_t *dtmp = (uint8_t *)malloc(RD_BUF_SIZE);
    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(uart2_queue, (void *)&event, (portTickType)portMAX_DELAY))
        {
            bzero(dtmp, RD_BUF_SIZE);
            // ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch (event.type)
            {

            case UART_DATA:

                uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                // ESP_LOGI(TAG, "[UART DATA]: %d", (uint8_t)dtmp);
                uart_data_handle(dtmp, event.size); // con trỏ hàm này sẽ đưa dữ liệu nhận được đến hàm uart_data_recveie_callback
                // ESP_LOGI(TAG, "[DATA EVT]:");
                // uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);
                break;

            // Others
            default:
                ESP_LOGI(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void esp_uart_init(void)
{

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart2_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);

    // Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}

void esp_uart_put(uint8_t *data, uint16_t len)
{
    uart_write_bytes(EX_UART_NUM, (const char *)data, len);
}

void esp_uart_set_callback_data(void *cb)
{ // hàm xét nó trỏ đến 1 hàm bất kỳ

    if (cb)
    {
        uart_data_handle = cb;
    }
}