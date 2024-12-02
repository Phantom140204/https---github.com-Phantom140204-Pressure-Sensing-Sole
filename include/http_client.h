#pragma once

#define WIFI_CONNECTED 1

#include <WString.h>
#include <stdint.h>
#include "nvm.h"
#include "sensor.h"

int http_client_init(wifi_credentials_t credentials);
void http_client_send_data(String server_ip, sensor_data_t data, uint8_t device_id);
int http_client_status();