#include <Arduino.h>

#include "sensor.h"

#define FSR_TOE_PIN 32
#define FSR_MIDDLE_PIN 34
#define FSR_HEEL_PIN 35

sensor_data_t sensor_read()
{
    sensor_data_t data = {0};

    data.fsr_toe = analogRead(FSR_TOE_PIN);
    data.fsr_middle = analogRead(FSR_MIDDLE_PIN);
    data.fsr_heel = analogRead(FSR_HEEL_PIN);

    return data;
}
