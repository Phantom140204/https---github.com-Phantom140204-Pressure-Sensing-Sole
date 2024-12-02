#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "http_client.h"
#include "nvm.h"
#include "sensor.h"

int http_client_init(wifi_credentials_t credentials)
{
    return WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());
}

int http_client_status()
{
    return (WiFi.status() == WL_CONNECTED);
}

void http_client_send_data(String server_ip, sensor_data_t data, uint8_t device_id)
{
    HTTPClient http;

    http.begin("http://" + server_ip + "/sensor_data");
    http.addHeader("Content-Type", "application/json");

    String post_message = "{\"device_id\": " + String(device_id);
    post_message += ", \"fsr_toe\": " + String(data.fsr_toe);
    post_message += ", \"fsr_middle\": " + String(data.fsr_middle);
    post_message += ", \"fsr_heel\": " + String(data.fsr_heel);
    post_message += "}";

    int http_code = http.POST(post_message.c_str());

    if (http_code > 0)
    {
        Serial.print("Sent HTTP POST with response code -");
        Serial.println(http_code);
        Serial.println(http.getString());
    }
    else
    {
        Serial.println("HTTP Request failed");
    }
}
