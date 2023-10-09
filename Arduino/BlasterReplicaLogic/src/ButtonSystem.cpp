#include "ButtonSystem.h"

ButtonSystem::ButtonSystem()
{
  pinMode(TRIGGER_BUTTON, INPUT);
  pinMode(SETTINGS_BUTTON, INPUT);

  trigger_pressed = false;
  settings_pressed = false;
  curr_setting_type = SettingsType::COLOR;
}

bool ButtonSystem::isTriggerPressed()
{
  bool trigger = checkButtonDown(TRIGGER_BUTTON, &trigger_pressed);
  return trigger;
}

SettingsButtonState ButtonSystem::isSettingsPressed(uint32_t short_time, uint32_t long_time)
{
  SettingsButtonState button_state = SettingsButtonState::NONE;
  uint32_t button_pressed_time = checkButtonUp(SETTINGS_BUTTON, &settings_pressed, &settings_press_time);
  if(button_pressed_time >= long_time)
    button_state = SettingsButtonState::LONG;
  else if(button_pressed_time >= short_time)
    button_state = SettingsButtonState::SHORT;
  else
    button_state = SettingsButtonState::NONE;
  return button_state;
}

void ButtonSystem::changeSettingsType()
{
    if(curr_setting_type == SettingsType::COLOR)
    {
      Serial.println("Settings: SOUND");
      curr_setting_type = SettingsType::SOUND;
    }
    else if(curr_setting_type == SettingsType::SOUND)
    {
      Serial.println("Settings: COLOR");
      curr_setting_type = SettingsType::COLOR;
    }
    /*else if(curr_setting_type == SettingsType::VOLUME)
    {
      Serial.println("Settings: COLOR");
      curr_setting_type = SettingsType::COLOR;
    }*/
}

uint8_t ButtonSystem::getCurrSettingType()
{
  return curr_setting_type;
}

void ButtonSystem::delayButtonPress()
{
  delay(400);
}

bool ButtonSystem::checkButtonDown(uint8_t pin, bool* button_state)
{
  if(digitalRead(pin) == HIGH)
  {
    if(!*button_state)
    {
      *button_state = true;
      return true;
    }
  }
  else
  {
    *button_state = false;
  }

  return false;
}

uint32_t ButtonSystem::checkButtonUp(uint8_t pin, bool* button_state, uint32_t* button_down_timer)
{
  bool register_button_down = digitalRead(pin) == HIGH && !*button_state;
  bool register_button_up = digitalRead(pin) == LOW && *button_state;

  if(register_button_down)
  {
    Serial.println("Button down");
    *button_state = true;
    *button_down_timer = millis();
  }

  else if(register_button_up)
  {
    Serial.println("Button up");
    *button_state = false;
    return millis() - *button_down_timer;
  }

  return 0;
}