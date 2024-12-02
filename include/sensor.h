#pragma once

typedef struct
{
    int fsr_toe;
    int fsr_middle;
    int fsr_heel;
} sensor_data_t;

sensor_data_t sensor_read();