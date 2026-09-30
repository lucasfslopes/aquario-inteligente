#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

extern WiFiManager wm;

constexpr char *versaoFirmware = "v1.0";
constexpr char *wifi_ssid = "Aqua-Smart";
constexpr char *wifi_password = "a1b2c3d4";

void wificonnect();

void configWiFi();

void resetWiFi();

#endif