#include <Arduino.h>

#include "webserver.h"
#include "http_client.h"
#include "sensor.h"
#include "hmi.h"
#include "nvm.h"

#define SENSOR_UPDATE_PERIOD 100
#define DATA_UPDATE_PERIOD 500

sensor_data_t data;
unsigned long prev_sensor_update_ticks, prev_data_update_ticks;
uint8_t device_state = 0;

void setup()
{
  Serial.begin(115200);

  hmi_init();
  nvm_read_data();

  http_client_init(get_wifi_credentials());

  device_state = DEVICE_STATE_PAIRING;
}

void loop()
{
  switch (device_state)
  {
  case DEVICE_STATE_PAIRING:
    // Check if WiFi is connected, Update state to active if connected
    if (http_client_status() == WIFI_CONNECTED)
    {
      Serial.println("Connected!");
      device_state = DEVICE_STATE_ACTIVE;
    }
    break;

  case DEVICE_STATE_ACTIVE:
    // Send sensor data every 500ms to webserver if connected, else set to pairing
    if (millis() - prev_data_update_ticks > DATA_UPDATE_PERIOD)
    {
      if (http_client_status() == WIFI_CONNECTED)
      {
        http_client_send_data(get_server_ip(), data, get_device_id());
      }
      else
      {
        Serial.println("WiFi Disconnected");
        device_state = DEVICE_STATE_PAIRING;
      }
      prev_data_update_ticks = millis();
    }
    break;

  case DEVICE_STATE_CONFIGMODE:
    // Host a webserver and save the config in NVM, set state to pairing after save
    if (webserver_handle_client() == 1)
    {
      http_client_init(get_wifi_credentials());
      Serial.println("Connecting to WiFi...");
      device_state = DEVICE_STATE_PAIRING;
    }
    break;

  default:
    break;
  }
  // Get sensor data at 10Hz (Every 100ms)
  if (millis() - prev_sensor_update_ticks > SENSOR_UPDATE_PERIOD)
  {
    data = sensor_read();
    prev_sensor_update_ticks = millis();
  }

  show_device_status(device_state);

  int event = detect_event();
  if (event == CONFIGMODE_EVENT)
  {
    device_state = DEVICE_STATE_CONFIGMODE;
    Serial.println("Entered CONFIG Mode");

    webserver_init();
    Serial.println("WiFi Server Started");
  }
}
