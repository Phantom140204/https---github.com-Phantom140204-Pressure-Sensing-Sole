#include <Arduino.h>
#include "hmi.h"

#define STATUS_LED_PIN 2
#define PAIRING_BLINK_PERIOD 1000
#define INIT_FAILURE_BLINK_PERIOD 200

#define CONFIGMODE_ON_PERIOD 200
#define CONFIGMODE_OFF_PERIOD 1500
#define CONFIGMODE_BUTTON_PIN 16

#define DEBOUNCE_PERIOD 2000

void configmode_event_isr();

static unsigned long prev_blink_ticks = millis(), configmode_button_ticks = millis();
static uint8_t led_state = 0, configmode_event = 0;

void hmi_init()
{
    pinMode(STATUS_LED_PIN, OUTPUT);
    pinMode(CONFIGMODE_BUTTON_PIN, INPUT);

    attachInterrupt(CONFIGMODE_BUTTON_PIN, configmode_event_isr, FALLING);
}

void show_device_status(const uint8_t device_status)
{
    switch (device_status)
    {
    case DEVICE_STATE_ACTIVE:
        led_state = HIGH;
        break;

    case DEVICE_STATE_PAIRING:
        if (millis() - prev_blink_ticks > PAIRING_BLINK_PERIOD)
        {
            led_state ^= 0x01;
            prev_blink_ticks = millis();
        }
        break;

    case DEVICE_STATE_CONFIGMODE:
        if (led_state == 1)
        {
            if (millis() - prev_blink_ticks > CONFIGMODE_ON_PERIOD)
            {
                led_state = 0;
                prev_blink_ticks = millis();
            }
        }
        else
        {
            if (millis() - prev_blink_ticks > CONFIGMODE_OFF_PERIOD)
            {
                led_state = 1;
                prev_blink_ticks = millis();
            }
        }
        break;

    default:
        break;
    }

    digitalWrite(STATUS_LED_PIN, led_state);
}

int detect_event()
{
    if (configmode_event)
    {
        configmode_event = 0;
        return CONFIGMODE_EVENT;
    }
    else
    {
        return 0;
    }
}

void configmode_event_isr()
{
    if (millis() - configmode_button_ticks > DEBOUNCE_PERIOD)
    {
        configmode_button_ticks = millis();
        configmode_event = 1;
    }
}
