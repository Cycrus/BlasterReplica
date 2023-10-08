#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#include "DFPlayerMiniControl.h"

class SoundSystem
{
  public:

    /*************************************************
     * Constructor. Initializes the software serial audio
     * driver.
     */
    SoundSystem();


    /*************************************************
     * Rotates through all possible sound profiles.
     */
    void nextSoundProfile();

    /*************************************************
     * Rotates through all possible volume settings.
     */
    void nextVolumeSetting();
	
    /*************************************************
     * Plays a welcome message whenever the blaster is
     * started.
     */
    void playWelcomeSound();

    /*************************************************
     * Plays the currently selected trigger sound.
     */
    void playTriggerSound();

    /*************************************************
     * Returns the currently selected sound code.
     * @return uint8_t The current sound code.
     */
    uint8_t getCurrSoundCode();

    /*************************************************
     * Returns the currently selected volume level id.
     * @return uint8_t The current volume level id.
     */
    uint8_t getCurrVolumeLevel();

    /*************************************************
     * Plays the selection sound of a new setting type
     */
    void playNewSettingSound(uint8_t new_setting_type_id);

    /*************************************************
     * Plays the selection sound of a new color.
     */
    void playNewColorSound(uint8_t new_color_code);

    /*************************************************
     * Plays the selection sound of a new sound.
     */
    void playNewSoundSound(uint8_t new_sound_code);

    /*************************************************
     * Plays the selection sound of a new volume
     */
    void playNewVolumeSound(uint8_t new_volume_setting);

    void keepSystemOnline(bool init_system);

    void setVolumeShooting();

    void setVolumeSettings();

  private:

    DFPlayerMiniControl *sound_player;

    const int SOUND_NUM = 9;
    uint8_t curr_sound_num;

    const int SETTING_TYPE_OFFSET = 230;
    const int SOUND_TYPE_OFFSET = 200;
    const int WELCOME_SOUND_OFFSET = 150;
    const int COLOR_TYPE_OFFSET = 100;
    const int VOLUME_TYPE_OFFSET = 50;

    const uint8_t VOLUME_NUM = 4;
    uint8_t volume_setting;
    uint8_t VOLUME_LIST[4];
    const uint8_t settings_volume = 15;
};
