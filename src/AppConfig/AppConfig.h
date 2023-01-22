#ifndef _APP_CONFIG_h
#define _APP_CONFIG_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>

struct Config
{
    unsigned char WifiLedState; // 1
    unsigned char MotionSensorLedState; // 1
    unsigned char MotionSensorState; //0
};

class AppConfig
{
public:
    AppConfig();
	AppConfig(void (*handler)(const char* message));
    
    void Save(unsigned char wifiStateLed, unsigned char motionSensorLedState, unsigned char motionSensorState);

    const String Print(unsigned char wifiLedState, unsigned char msLedState, unsigned char msState);
    const String Print();

    Config Value;

private:
    bool _fs_started;
    void (*_messageHandler)(const char* message) = nullptr;

    void _restoreDefaultConfig();
    StaticJsonDocument<200> _getConfigJson();
};

#endif