#include <Arduino.h>
#include <WiFi.h>
#include <ESP32WebServer.h>

#include "webserver.h"
#include "nvm.h"

ESP32WebServer server(80);

static uint8_t config_save_flag = 0;

static void handle_root()
{
    String html = "<!DOCTYPE html><html><head><title>Device Config</title></head><body>";

    html += "<h1>Device Config</h1>";
    html += "<form action=\"/config_save\" method=\"post\">";

    html += "<label for=\"ssid\">WiFi SSID: </label>";
    html += "<input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br>";

    html += "<label for=\"password\">WiFi Password: </label>";
    html += "<input type=\"password\" id=\"password\" name=\"password\"><br><br>";

    html += "<label for=\"device_id\">Device ID: </label>";
    html += "<select id=\"device_id\" name=\"device_id\">";
    html += "<option value=\"0\">LEFT</option>";
    html += "<option value=\"1\">RIGHT</option>";
    html += "</select><br><br>";

    html += "<label for=\"server_ip\">Server IP: </label>";
    html += "<input type=\"text\" id=\"server_ip\" name=\"server_ip\"><br><br>";

    html += "<input type=\"submit\" value=\"Save\">";

    html += "</form></body></html>";

    server.send(200, "text/html", html);
}

static void handle_config_save()
{
    wifi_credentials_t wifi;
    wifi.ssid = server.arg("ssid");
    wifi.password = server.arg("password");

    uint8_t id = server.arg("device_id").toInt();

    String ip = server.arg("server_ip");

    String html = "<!DOCTYPE html><html><head><title>Config Saved!</title></head><body>";

    html += "<h>Config Updated Successfully!</h></html>";

    server.send(200, "text/html", html);

    if ((wifi.ssid != "") && (wifi.password != ""))
    {
        set_wifi_credentials(wifi);
    }
    set_device_id(id);
    if (ip != "")
    {
        set_server_ip(ip);
    }

    config_save_flag = 1;
}

void webserver_init()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32", "12345678");

    server.on("/", handle_root);
    server.on("/config_save", handle_config_save);
    server.begin();

    config_save_flag = 0;

    Serial.println("Serving on IP - " + WiFi.softAPIP().toString());
}

int webserver_handle_client()
{
    server.handleClient();

    if (config_save_flag == 1)
    {
        Serial.println("Config Saved");
        return 1;
    }

    return 0;
}
