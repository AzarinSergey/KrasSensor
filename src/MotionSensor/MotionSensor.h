#ifndef _MOTION_SENSOR_h
#define _MOTION_SENSOR_h

#ifndef MOTION_SENSOR_LED_D6 
#define MOTION_SENSOR_LED_D6 12
#endif

#ifndef MOTION_SENSOR_INPUT_D3 
#define MOTION_SENSOR_INPUT_D3 0
#endif

#include <Arduino.h>

class MotionSensor{
public:
	MotionSensor();
	
    void CheckMotion(void (*handler)(void));
    
    void Enable();
    void Disable();
    
    void DisableLed();
    void EnableLed();

    unsigned char LedState();
    unsigned char Enabled();
    
private:
	unsigned char motionSensorStatePrevious = 0;
    bool _motionSensorLedEnabled = true;
    bool _motionSensorEnabled = false;
};

#endif