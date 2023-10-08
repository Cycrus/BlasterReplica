#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
  sound_player = new DFPlayerMiniControl(10, 11);
  Serial.println("Audio device online.");

  VOLUME_LIST[0] = 0;
  VOLUME_LIST[1] = 10;
  VOLUME_LIST[2] = 20;
  VOLUME_LIST[3] = 30;
  volume_setting = 3;
  sound_player->volume(VOLUME_LIST[volume_setting]);
  curr_sound_num = 0;
}

void SoundSystem::nextSoundProfile()
{
  curr_sound_num++;
  if(curr_sound_num >= SOUND_NUM)
    curr_sound_num = 0;
}

void SoundSystem::nextVolumeSetting()
{
  volume_setting++;
  if(volume_setting >= VOLUME_NUM)
    volume_setting = 0;
}

void SoundSystem::playWelcomeSound()
{
  sound_player->advert(WELCOME_SOUND_OFFSET);
  delay(2000);
}

void SoundSystem::playTriggerSound()
{
  //setVolumeShooting();
  sound_player->advert(curr_sound_num);
}

uint8_t SoundSystem::getCurrSoundCode()
{
  return curr_sound_num;
}

uint8_t SoundSystem::getCurrVolumeLevel()
{
  return volume_setting;
}

void SoundSystem::playNewSettingSound(uint8_t new_setting_type_id)
{
  //setVolumeSettings();
  sound_player->advert(new_setting_type_id + SETTING_TYPE_OFFSET);
}

void SoundSystem::playNewColorSound(uint8_t new_color_code)
{
  //setVolumeSettings();
  sound_player->advert(new_color_code + COLOR_TYPE_OFFSET);
}

void SoundSystem::playNewSoundSound(uint8_t new_sound_code)
{
  //setVolumeSettings();
  sound_player->advert(new_sound_code + SOUND_TYPE_OFFSET);
}

void SoundSystem::playNewVolumeSound(uint8_t new_volume_setting)
{
  //setVolumeSettings();
  sound_player->advert(new_volume_setting + VOLUME_TYPE_OFFSET);
}

void SoundSystem::keepSystemOnline(bool init_system)
{
  if(sound_player->trackEnded() || init_system)
  {
    sound_player->playMp3(254);
  }
}

void SoundSystem::setVolumeShooting()
{
  sound_player->volume(VOLUME_LIST[volume_setting]);
  delay(100);
}

void SoundSystem::setVolumeSettings()
{
  sound_player->volume(settings_volume);
  delay(100);
}