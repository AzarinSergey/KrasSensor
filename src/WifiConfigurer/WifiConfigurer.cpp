#include "WifiConfigurer.h"

Ticker *ticker;

void WifiConfigurer::DisableConnectionLed()
{
	if(!ticker->active())
	{
		digitalWrite(WIFI_STATE_LED, 0);
	}
}

void WifiConfigurer::EnableConnectionLed()
{
	if(!ticker->active())
	{
		digitalWrite(WIFI_STATE_LED, 1);
	}
}

unsigned char WifiConfigurer::LedState()
{
    return digitalRead(WIFI_STATE_LED);
}

void WifiConfigurer::Invoke()
{
	//blink 'ON' while not connected to Wifi network
	pinMode(WIFI_STATE_LED, OUTPUT);
	ticker = new Ticker();
	ticker->attach(0.6, []() {
		int state = digitalRead(WIFI_STATE_LED);
		digitalWrite(WIFI_STATE_LED, !state);
	});

	//configure connection in auto or manual throught portal
	WiFiManager wifiManager;

	//reset if requested
	pinMode(WIFI_RESET_CONFIG, INPUT);
    int resetRequested = !digitalRead(WIFI_RESET_CONFIG);
    
	Serial.println("resetRequested");
	Serial.println(resetRequested);

	
	if(resetRequested)
        wifiManager.resetSettings();
    

	while (!wifiManager.autoConnect("Croat", NULL)) {
		Serial.println("failed to connect and hit timeout");
		ESP.reset();
		delay(1000);
	}

	Serial.println("Connected to endpoint...");

	//stop blink and turn on light
	ticker->detach();
	ticker->~Ticker();
	digitalWrite(WIFI_STATE_LED, HIGH);
}