#include <FastBot.h> //https://github.com/GyverLibs/FastBot/tree/main/examples
#include "WifiConfigurer/WifiConfigurer.h"
#include "MotionSensor/MotionSensor.h"
#include "AppConfig/AppConfig.h"
#include <WEMOS_SHT3X.h>

WifiConfigurer wifi;
MotionSensor motionSensor;
AppConfig config;

#define TELEGRAM_CHAT_ID "-892123703"
#define BOT_TOKEN ""

FastBot bot(BOT_TOKEN);
SHT3X sht30(0x44);

void GetHelp(String *chatId)
{
  String message = ".h - Получить справку. \n";
  message += ".sht - Получить данные сенсора температуры/влажности. \n";
  message += ".wledoff - отключить индикатор коннекта к wifi. \n";
  message += ".wledon - включить индикатор коннекта к wifi. \n";
  message += ".mson - Включить уведомления по сенсору движения. \n";
  message += ".msoff - Отключить уведомления по сенсору движения. \n";
  message += ".msledon - Включить индикатор фиксации движения. \n";
  message += ".msledoff - Отключить индикатор фиксации движения. \n";
  message += ".cfg - Показать сохраненные настройки. \n";
  message += ".ccfg - Показать текущие настройки. \n";
  message += ".rcfg - Сохранить текущие настройки в энергонезависимую память. \n";
  bot.sendMessage(message, *chatId);
}

void GetShtData(String *chatId)
{
  String message;
  if (sht30.get() == 0)
  {
    message = "Tемпература: ";
    message += sht30.cTemp;
    message += " C";
    message += "\n";
    message += "Влажность: ";
    message += sht30.humidity;
    message += " %";
  }
  else
  {
    message = "Ошибка чтения!";
  }
  bot.sendMessage(message, *chatId);
}

void newMsg(FB_msg &msg)
{
  // message info
  Serial.println(msg.toString());

  if (msg.text[0] != '.')
    return;

  // not allowed
  if (msg.chatID != TELEGRAM_CHAT_ID)
  {
    bot.sendMessage("forbidden", msg.chatID);
    return;
  }

  if (msg.text == ".h")
    return GetHelp(&msg.chatID);

  if (msg.text == ".sht")
    return GetShtData(&msg.chatID);

  if (msg.text == ".wledoff")
  {
    wifi.DisableConnectionLed();
    config.Value.WifiLedState = wifi.LedState();
    bot.sendMessage("Applied", msg.chatID);
    return;
  }

  if (msg.text == ".wledon")
  {
      wifi.EnableConnectionLed();
      config.Value.WifiLedState = wifi.LedState();
      bot.sendMessage("Applied", msg.chatID);
      return;
  }

  if (msg.text == ".mson")
  {
    motionSensor.Enable();
    config.Value.MotionSensorState = motionSensor.Enabled();
    bot.sendMessage("Applied", msg.chatID);
    return;
  }

  if (msg.text == ".msoff")
  {
    motionSensor.Disable();
    config.Value.MotionSensorState = motionSensor.Enabled();
    bot.sendMessage("Applied", msg.chatID);
    return;
  }

  if (msg.text == ".msledon")
  {
    motionSensor.EnableLed();
    config.Value.MotionSensorLedState = motionSensor.LedState();
    bot.sendMessage("Applied", msg.chatID);
    return;
  }

  if (msg.text == ".msledoff")
   {
    motionSensor.DisableLed();
    config.Value.MotionSensorLedState = motionSensor.LedState();
    bot.sendMessage("Applied", msg.chatID);
    return;
  }
  
  if (msg.text == ".cfg")
  {
    bot.sendMessage(config.Print(), TELEGRAM_CHAT_ID);
    return;
  }

  if(msg.text == ".ccfg")
  {
    bot.sendMessage(config.Print(wifi.LedState(), motionSensor.LedState(), motionSensor.Enabled()), TELEGRAM_CHAT_ID);
    return;
  }

  if(msg.text == ".rcfg")
  {
    config.Save(wifi.LedState(), motionSensor.LedState(), motionSensor.Enabled());
    return;
  }

  bot.sendMessage("Unknown command", msg.chatID);
}

void ConfigCallback(const char* message)
{
  bot.sendMessage(message, TELEGRAM_CHAT_ID);
}

void ApplyConfig(Config *cfg)
{
  if(cfg->WifiLedState > 0)
    wifi.EnableConnectionLed();
  else
    wifi.DisableConnectionLed();

  if(cfg->MotionSensorLedState > 0)
    motionSensor.EnableLed();
  else
    motionSensor.DisableLed();

  if(cfg->MotionSensorState > 0)
    motionSensor.Enable();
  else 
    motionSensor.Disable();
}

void setup()
{
  Serial.begin(115200);
  wifi.Invoke();
  bot.attach(newMsg);

  config = AppConfig(ConfigCallback);

  ApplyConfig(&config.Value);
}

void MotionDetected()
{
  bot.sendMessage("Motion detected.", TELEGRAM_CHAT_ID);
}

void loop()
{
  delay(300);
  motionSensor.CheckMotion(MotionDetected);
  bot.tick();
}
