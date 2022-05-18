set(pri_req )
idf_component_register(SRCS "esp_uart.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
