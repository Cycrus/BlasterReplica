#pragma once

#include <stdint.h>
#include <Arduino.h>

#define TRIGGER_BUTTON 8
#define SETTINGS_BUTTON 7

enum SettingsButtonState
{
  NONE,
  SHORT,
  LONG
};

enum SettingsType
{
  COLOR,
  SOUND,
  VOLUME
};

class ButtonSystem
{
  public:

    /*************************************************
    * Constructor. Initializes the buttons as digital inputs.
    */
    ButtonSystem();

    /*************************************************
     * Checks if the trigger is currently pressed.
     * @return bool True if the button has been pressed.
     */
    bool isTriggerPressed();

    SettingsButtonState isSettingsPressed(uint32_t short_time, uint32_t long_time);

    /*************************************************
     * Rotates through the different settings typese and assigns the next valid one.
     */
    void changeSettingsType();

    /*************************************************
     * Returns the id of the current setting type as an uint8.
     * @return uint8_t The id of the current setting type.
     */
    uint8_t getCurrSettingType();

    /*************************************************
     * Convenience method to delay the controller after
     * a valid setting button press.
     */
    void delayButtonPress();

  private:
    bool trigger_pressed;
    bool settings_pressed;
    uint32_t settings_press_time;
    SettingsType curr_setting_type;

    /*************************************************
     * A general method to check if a certain button was pressed.
     * @param pin The pin the button is located on.
     * @param button_state A pointer to the state representation of the button.
     *                     Used to discard consequtive button pressed.
     * @return bool True if the button has been pressed.
     */
    bool checkButtonDown(uint8_t pin, bool* button_state);

    /*************************************************
     * Checks if a button is released and how long it has been pressed.
     * @param pin The pin the button is located on.
     * @param button_state A pointer to a bool variable representing if the button is currently pressed.
     * @param button_down_timer A pointer to a variable used as a timer how long the button has been pressed.
     * @return uint32_t The time the button was pressed before being released.
     */
    uint32_t checkButtonUp(uint8_t pin, bool* button_state, uint32_t* button_down_timer);
};
