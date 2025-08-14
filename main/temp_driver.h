
#ifndef TEMP_DRIVER_H
#define TEMP_DRIVER_H

#include <stdint.h>

typedef struct {
    int   (*init)(void);
    int   (*read_raw)(int16_t *raw);
    float (*raw_to_celsius)(int16_t raw);
} temp_sensor_t;

#endif // TEMP_DRIVER_H
