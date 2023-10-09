/* The script to control the working blaster model.
 * Essentially triggers LED and sound when button is pressed.
 * Also offers capabilities to change sound and color profiles.
 * 
 * Author: Cyril Marx (Cycrus)
 * Year: 2023
 *
 /**********************************************************/

#include "src/LedSystem.h"
#include "src/SoundSystem.h"
#include "src/ButtonSystem.h"

LedSystem* led;
SoundSystem* sound;
ButtonSystem* buttons;

void setup()
{
  Serial.begin(9600);
  buttons = new ButtonSystem();
  led = new LedSystem();
  sound = new SoundSystem();
  delay(200);
  sound->keepSystemOnline(true);
  delay(1000);
  sound->playWelcomeSound();
}

void loop()
{
  sound->keepSystemOnline(false);

	if(buttons->isTriggerPressed())
  {
    sound->playTriggerSound();
    delay(200);
    Serial.println("Trigger!");
    led->startAnimation();
  }

  SettingsButtonState settingsState = buttons->isSettingsPressed(1, 500);

  if(led->getAnimationStep() == 0)
  {
    if(settingsState == SettingsButtonState::SHORT)
    {
      if(buttons->getCurrSettingType() == SettingsType::COLOR)
      {
        Serial.println("Next color profile");
        led->nextColorProfile();
        sound->playNewColorSound(led->getCurrColorCode());
        buttons->delayButtonPress();
      }
      else if(buttons->getCurrSettingType() == SettingsType::SOUND)
      {
        Serial.println("Next sound profile");
        sound->nextSoundProfile();
        sound->playNewSoundSound(sound->getCurrSoundCode());
        buttons->delayButtonPress();
      }
      else if(buttons->getCurrSettingType() == SettingsType::VOLUME)
      {
        Serial.println("Next volume setting");
        sound->nextVolumeSetting();
        sound->playNewVolumeSound(sound->getCurrVolumeLevel());
        buttons->delayButtonPress();
      }
      else
      {
        Serial.println("Error: Wrong settings type detected.");
      }
    }
    else if(settingsState == SettingsButtonState::LONG)
    {
      buttons->changeSettingsType();
      sound->playNewSettingSound(buttons->getCurrSettingType());
      buttons->delayButtonPress();
    }
  }
  led->performAnimationStep(1);
  delay(5);
}
