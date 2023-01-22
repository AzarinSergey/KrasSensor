#include "AppConfig.h"

const char *CONFIG_FILE_PATH = "/device_config.json";

bool _equal(const char * str1, const char * str2);
String _stateToString(unsigned char state);
unsigned char _convertState(const char *state);
const String _print(const char *wled_state, const char *msled_state, const char *ms_state);
StaticJsonDocument<200> _getDefaultConfigJson();

AppConfig::AppConfig()
{
    Value.WifiLedState = 1;
    Value.MotionSensorLedState = 1;
    Value.MotionSensorState = 0;
}

AppConfig::AppConfig(void (*handler)(const char *message))
{
    _messageHandler = handler;
    _fs_started = LittleFS.begin();

    StaticJsonDocument<200> doc = _getConfigJson();

    const char *wled_state = doc["wled"];
    const char *msled_state = doc["msled"];
    const char *ms_state = doc["ms"];

    Value.WifiLedState = _convertState(wled_state);
    Value.MotionSensorLedState = _convertState(msled_state);
    Value.MotionSensorState = _convertState(ms_state);

    _messageHandler(_print(wled_state, msled_state, ms_state).c_str());
}

void AppConfig::Save(unsigned char wifiStateLed, unsigned char motionSensorLedState, unsigned char motionSensorState)
{
    StaticJsonDocument<200> configData;
    configData["wled"] = _stateToString(wifiStateLed);
    configData["ms"] = _stateToString(motionSensorState);
    configData["msled"] = _stateToString(motionSensorLedState);

    File config = LittleFS.open(CONFIG_FILE_PATH, "w");

    if (!config)
    {
        _messageHandler("Failed to open config file for writing");
        return;
    }

    serializeJson(configData, config);
 
    String message = "It has written: \n";
    message += Print(Value.WifiLedState, Value.MotionSensorLedState, Value.MotionSensorState);
    _messageHandler(message.c_str());
}

const String AppConfig::Print(unsigned char wifiLedState, unsigned char msLedState, unsigned char msState)
{
    return _print(_stateToString(wifiLedState).c_str(), _stateToString(msLedState).c_str(), _stateToString(msState).c_str());
}

const String AppConfig::Print()
{
    StaticJsonDocument<200> doc = _getConfigJson();

    const char *wled_state = doc["wled"];
    const char *msled_state = doc["msled"];
    const char *ms_state = doc["ms"];

    return _print(wled_state, msled_state, ms_state);
}


StaticJsonDocument<200> AppConfig::_getConfigJson()
{
    if (!_fs_started)
    {
        _messageHandler("Failed to mount file system. Configuration won't be loaded.");
        return  _getDefaultConfigJson();
    }

    File config = LittleFS.open(CONFIG_FILE_PATH, "r");

    if (!config)
    {
        _messageHandler("No saved config. It's going to create default.");
        _restoreDefaultConfig();
        config.close();
    }

    config = LittleFS.open(CONFIG_FILE_PATH, "r");

    if (!config)
    {
        _messageHandler("Error! Written config didn't read! Try to reboot device or rewrite config.");
        return _getDefaultConfigJson();
    }

    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, config);
    if (error)
    {
        const char *errorMessage = error.c_str();
        String message = "Error! Failed to parse config file! Try to restore to default it.";
        message += errorMessage;
        _messageHandler(message.c_str());
        return _getDefaultConfigJson();
    }

    return doc;
}

const String _print(const char *wled_state, const char *msled_state, const char *ms_state)
{
    String message = "Configuration data: \n";
    message += "Wifi led: ";
    message += wled_state;
    message += "\n";
    message += "Motion sensor led: ";
    message += msled_state;
    message += "\n";
    message += "Motion sensor: ";
    message += ms_state;
    message += "\n";
    return message;
}

void AppConfig::_restoreDefaultConfig()
{
    File config = LittleFS.open(CONFIG_FILE_PATH, "w");

    if (!config)
    {
        _messageHandler("Failed to open config file for writing");
        return;
    }

    serializeJson(_getDefaultConfigJson(), config);
}

StaticJsonDocument<200> _getDefaultConfigJson()
{
    StaticJsonDocument<200> configData;
    configData["wled"] = "on";
    configData["ms"] = "off";
    configData["msled"] = "on";

    return configData;
}

String _stateToString(unsigned char state)
{
    return state > 0 ? "on" : "off";
}

unsigned char _convertState(const char *state)
{
    if (_equal(state, "off"))
        return 0;

    if (_equal(state, "on"))
        return 1;

    return 0;
}

bool _equal(const char * str1, const char * str2)
{
    unsigned int size = 0;
    while (str1[size] != '\0') size++;
    for (unsigned int i = 0; i < size; i++)
    {
        if(str1[i] != str2[i])
            return false;
    }

    return true;
}