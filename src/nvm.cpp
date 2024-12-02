#include <Arduino.h>
#include <Preferences.h>

#include "nvm.h"

#define DEVICE_ID_LEFT 0
#define DEVICE_ID_RIGHT 1

static wifi_credentials_t wifi_credentials;
static uint8_t device_id;
static String server_ip;

void nvm_read_data()
{
    Preferences preferences;

    preferences.begin("nvm_data", false);

    wifi_credentials.ssid = preferences.getString("ssid");
    wifi_credentials.password = preferences.getString("password");

    device_id = preferences.getUChar("device_id");

    server_ip = preferences.getString("server_ip");

    preferences.end();

    Serial.println("Read credentials -\n\tSSID - " + wifi_credentials.ssid + "\n\tPassword - " + wifi_credentials.password);
    Serial.println("\tDevice ID - " + String(device_id == DEVICE_ID_LEFT ? "LEFT" : "RIGHT") + "\n\tServer IP - " + server_ip);
}

wifi_credentials_t get_wifi_credentials()
{
    return wifi_credentials;
}

uint8_t get_device_id()
{
    return device_id;
}

String get_server_ip()
{
    return server_ip;
}

void set_wifi_credentials(const wifi_credentials_t credentials)
{
    Preferences preferences;

    preferences.begin("nvm_data", false);
    preferences.putString("ssid", credentials.ssid);
    preferences.putString("password", credentials.password);

    preferences.end();

    wifi_credentials.ssid = credentials.ssid;
    wifi_credentials.password = credentials.password;

    Serial.println("Set Wifi Credentials -\n\tSSID - " + credentials.ssid + "\n\tPassword - " + credentials.password);
}

void set_device_id(const uint8_t id)
{
    Preferences preferences;

    preferences.begin("nvm_data", false);
    preferences.putUChar("device_id", id);

    preferences.end();

    device_id = id;

    Serial.println("Device ID set to - " + String((id == DEVICE_ID_LEFT) ? "LEFT" : "RIGHT"));
}

void set_server_ip(const String ip)
{
    Preferences preferences;

    preferences.begin("nvm_data", false);
    preferences.putString("server_ip", ip);

    preferences.end();

    server_ip = ip;

    Serial.println("Server IP set to - " + ip);
}
