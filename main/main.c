
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp_driver.h"

extern temp_sensor_t TMP102;

void app_main(void) {
    printf("TMP102 demo startar...\n");

    if (TMP102.init() != 0) {
        printf("Fel: kunde inte initiera TMP102/I2C\n");
        return;
    }
    printf("TMP102 init OK.\n");

    while (1) {
        int16_t raw = 0;
        int r = TMP102.read_raw(&raw);
        if (r == 0) {
            float c = TMP102.raw_to_celsius(raw);
            printf("Temp: raw=%d, %.2f C\n", raw, c);
        } else if (r == -1) {
            printf("Fel: I2C timeout\n");
        } else if (r == -2) {
            printf("Fel: I2C NACK/busfel\n");
        } else {
            printf("Fel: okänt I2C-fel (%d)\n", r);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
