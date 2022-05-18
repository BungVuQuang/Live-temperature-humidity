set(pri_req protocol_examples_common mqtt nvs_flash esp_uart json_generator json_parser esp_output)
idf_component_register(SRCS "app_main.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req}
                    )