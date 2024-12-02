#pragma once

#include <WString.h>
#include <stdint.h>

typedef struct
{
    String ssid;
    String password;
} wifi_credentials_t;

void nvm_read_data();

wifi_credentials_t get_wifi_credentials();
uint8_t get_device_id();
String get_server_ip();

void set_wifi_credentials(const wifi_credentials_t wifi_credentials);
void set_device_id(const uint8_t id);
void set_server_ip(const String ip);