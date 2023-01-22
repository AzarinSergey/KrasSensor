
#ifndef _WIFI_CONNECTION_h
#define _WIFI_CONNECTION_h

#ifndef WIFI_STATE_LED 
#define WIFI_STATE_LED 13
#endif

#ifndef WIFI_RESET_CONFIG
#define WIFI_RESET_CONFIG 3
#endif

#define WM_DEBUG_LEVEL DEBUG_MAX

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          
#include <Ticker.h>

class WifiConfigurer
{
 public:
	 void Invoke();
	 void DisableConnectionLed();
	 void EnableConnectionLed();

	 unsigned char LedState();
};

#endif
