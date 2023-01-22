#include "MotionSensor.h"

MotionSensor::MotionSensor()
{
  pinMode(MOTION_SENSOR_INPUT_D3, INPUT);
  pinMode(MOTION_SENSOR_LED_D6, OUTPUT);
}

void MotionSensor::CheckMotion(void (*handler)(void))
{
  if (!_motionSensorEnabled)
    return;

  int motionSensorState = digitalRead(MOTION_SENSOR_INPUT_D3);

  if (motionSensorState > 0 && motionSensorStatePrevious == 0) // detected
  {
    if (_motionSensorLedEnabled)
      digitalWrite(MOTION_SENSOR_LED_D6, HIGH);

    if (*handler) 
        handler();
  }

  if (motionSensorState == 0 && motionSensorStatePrevious > 0) // did not detect
  {
    digitalWrite(MOTION_SENSOR_LED_D6, LOW);
  }

  motionSensorStatePrevious = motionSensorState;
}

void MotionSensor::Enable()
{
    _motionSensorEnabled = true;
}

void MotionSensor::Disable()
{
    _motionSensorEnabled = false;
}

void MotionSensor::DisableLed()
{
    _motionSensorLedEnabled = false;
    digitalWrite(MOTION_SENSOR_LED_D6, LOW);
}

void MotionSensor::EnableLed()
{
    _motionSensorLedEnabled = true;
}

unsigned char MotionSensor::LedState()
{
    return _motionSensorLedEnabled;
}

unsigned char MotionSensor::Enabled()
{
    return _motionSensorEnabled;
}
