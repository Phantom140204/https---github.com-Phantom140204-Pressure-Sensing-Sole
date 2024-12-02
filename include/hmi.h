#pragma once

#include <stdint.h>

#define DEVICE_STATE_ACTIVE 0
#define DEVICE_STATE_PAIRING 1
#define DEVICE_STATE_CONFIGMODE 2

#define CONFIGMODE_EVENT 1

void hmi_init();
void show_device_status(const uint8_t device_status);
int detect_event();
