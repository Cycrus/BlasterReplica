/* The script to control the working blaster model.
 * Essentially triggers LED and sound when button is pressed.
 * Also offers capabilities to change sound and color profiles.
 * 
 * Author: Cyril Marx (Cycrus)
 * Year: 2023
 *
 /**********************************************************/
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);

#define TRIGGER_BUTTON 8
#define SETTINGS_BUTTON 7

#define FRONT_LED_PIN 9
#define LEFT_LED_PIN 6
#define RIGHT_LED_PIN 5

#define FRONT_LED_NUM 4
#define SIDE_LED_NUM 30

/**********************************************************/
/**********************************************************/
class LedSystem
{
  public:

    /*************************************************
     * Constructor. Initializes LED devices.
     */
    LedSystem()
    {
      front_led = new Adafruit_NeoPixel(FRONT_LED_NUM, FRONT_LED_PIN, NEO_GRB + NEO_KHZ800);
      left_led = new Adafruit_NeoPixel(SIDE_LED_NUM, LEFT_LED_PIN, NEO_GRB + NEO_KHZ800);
      right_led = new Adafruit_NeoPixel(SIDE_LED_NUM, RIGHT_LED_PIN, NEO_GRB + NEO_KHZ800);

      front_led->begin();
      left_led->begin();
      right_led->begin();
    
      COLOR_LIST[0] = front_led->Color(255, 0, 0); // Red
      COLOR_LIST[1] = front_led->Color(0, 0, 255); // Blue
      COLOR_LIST[2] = front_led->Color(0, 255, 0); // Green
      COLOR_LIST[3] = front_led->Color(255, 255, 0); // Yellow
      COLOR_LIST[4] = front_led->Color(255, 255, 255); // White
      COLOR_LIST[5] = front_led->Color(255, 0, 255); // Purple

      doPerformAnimation = false;
      curr_color = 0;
      curr_animation_step = 0;
      global_step_counter = 0;

      clearAllLeds();
      showAllLeds();
    }

    /*************************************************
     * Rotates through all possible color profiles.
     */
    void nextColorProfile()
    {
      curr_color++;
      if(curr_color >= COLOR_NUM)
        curr_color = 0;
    }

    /*************************************************
     * Returns the current color value.
     * @return uint8_t The current color code.
     */
    uint8_t getCurrColorCode()
    {
      return curr_color;
    }

    /*************************************************
     * Returns the current animation step. Used to check
     * if configuration attempts are valid or not.
     * @return uint32_t The current step of the animation progress.
     */
    uint32_t getAnimationStep()
    {
      return curr_animation_step;
    }

    /*************************************************
     * Performs a single animation step. Must be called in every iteration of the controller.
     * @param step_delay The delay in between the individual animation steps measured in 
     *                   controller cycles. The higher this value, the slower the animation
     *                   is played.
     */
    void performAnimationStep(int step_delay)
    {
      if(!doPerformAnimation)
          return;

      if(global_step_counter % step_delay == 0)
      {
        clearAllLeds();

        animateFrontLed();
        animateSideLed(left_led);
        animateSideLed(right_led);

        curr_animation_step++;
        if(curr_animation_step > MAX_ANIMATION_STEP)
        {
          clearAllLeds();
          curr_animation_step = 0;
          doPerformAnimation = false;
        }
        showAllLeds();
      }

      global_step_counter++;
    }

    /*************************************************
     * Clears the color of all neopixel leds. 
     */
    void clearAllLeds()
    {
      clearLed(front_led);
      clearLed(right_led);
      clearLed(left_led);
    }

    /*************************************************
     * A wrapper to call the update function of the leds on all led lights. 
     */
    void showAllLeds()
    {
      front_led->show();
      left_led->show();
      right_led->show();
    }

    /*************************************************
     * Starts the animation by resetting all values.
     */
    void startAnimation()
    {
      doPerformAnimation = true;
      curr_animation_step = 1;
      global_step_counter = 0;
    }

  private:
    Adafruit_NeoPixel* front_led;
    Adafruit_NeoPixel* left_led;
    Adafruit_NeoPixel* right_led;

    bool doPerformAnimation;

    const uint8_t COLOR_NUM = 6;
    uint32_t COLOR_LIST[6];
    uint8_t curr_color;

    const uint32_t MAX_ANIMATION_STEP = 30;
    uint32_t curr_animation_step;

    int global_step_counter;

    /*************************************************
     * The animation logic of the front led collection.
     */
    void animateFrontLed()
    {
      if(animationProgressLowerBound(10))
      {
        lightLed(front_led, 4, 0);
        front_led->setBrightness(255);
      }

      if(animationProgressLowerBound(50))
      {
        uint32_t max_brightness_step = getAnimationStepPercentage(50);
        uint32_t min_brightness_step = getAnimationStepPercentage(100);
        float step_range = min_brightness_step - max_brightness_step;

        float norm_curr_animation_step = (float)(((float)curr_animation_step - (float)max_brightness_step) / (float)step_range);
        float inv_norm_curr_animation_step = 1 - norm_curr_animation_step;

        uint32_t curr_brightness = inv_norm_curr_animation_step * 255;
        lightLed(front_led, 4, 0);
        front_led->setBrightness(curr_brightness);
      }
    }

    /*************************************************
     * The animation logic of the side led collections.
     * @param led A pointer to the led collection to animate.
     */
    void animateSideLed(Adafruit_NeoPixel* led)
    {
      uint32_t min_slide_step = getAnimationStepPercentage(0);
      uint32_t max_slide_step = getAnimationStepPercentage(50);
      uint32_t width = 3;

      float slide_range = max_slide_step - min_slide_step;
      float norm_pixel_start = (float)(((float)curr_animation_step - (float)min_slide_step) / (float)slide_range);
      uint32_t proj_pixel_start = norm_pixel_start * SIDE_LED_NUM;
      
      lightLed(led, width, proj_pixel_start);
    }

    /*************************************************
     * Returns the absolute step number of the animation given a percentage (0-100).
     * @param percentage The percentage to translate.
     * @return uint32_t The absolute step number translated.
     */
    uint32_t getAnimationStepPercentage(uint8_t percentage)
    {
      if(percentage > 100)
        percentage = 100;

      return ((float)MAX_ANIMATION_STEP / (float)100) * (float)percentage;
    }

    /*************************************************
     * Returns the current progress of the animation in a percentage number (0-100).
     * @return uint32_t The percentage of the current animation progress.
     */
    uint32_t getCurrentAnimationProgressPercentage()
    {
      return ((float)MAX_ANIMATION_STEP * (float)100 / (float)curr_animation_step);
    }

    /*************************************************
     * Lights a led collection with a given number of pixels starting at a certain position.
     * As a color it takes the currently active color.
     * @param led A pointer to the led collection to light up.
     * @param pixel_count The number of pixels to light up.
     * @param start The id of the first pixel to light up.
     */
    void lightLed(Adafruit_NeoPixel* led, uint8_t pixel_count, uint8_t start)
    {
      led->fill(COLOR_LIST[curr_color], start, pixel_count);
    }

    /*************************************************
     * Clears the color of a certain led collection.
     * @param led A pointer to the led to clear.
     */
    void clearLed(Adafruit_NeoPixel* led)
    {
      led->setBrightness(255);
      led->clear();
    }

    /*************************************************
     * Checks if the animation progress is at least the given percentage.
     * @param percentage The percentage of animation progress to check for.
     * @return bool True if the animation progress is at least the given percentage.
     */
    bool animationProgressLowerBound(uint8_t percentage)
    {
      if(percentage > 100)
        percentage = 100;

      return ((curr_animation_step * 100) / (MAX_ANIMATION_STEP)) >= percentage;
    }

    /*************************************************
     * Checks if the animation progress is below the given percentage.
     * @param percentage The percentage of animation progress to check for.
     * @return bool True if the animation progress is below the given percentage.
     */
    bool animationProgressUpperBound(uint8_t percentage)
    {
      if(percentage > 100)
        percentage = 100;

      return ((curr_animation_step * 100) / (MAX_ANIMATION_STEP)) < percentage;
    }
};

/**********************************************************/
/**********************************************************/
class SoundSystem
{
  public:

    /*************************************************
     * Constructor. Initializes the software serial audio
     * driver.
     */
    SoundSystem()
    {
      softSerial.begin(9600);
      if(!sound_player.begin(softSerial, true, true))
      {
        Serial.println("Cannot start audio device.");
        while(true)
          delay(0);
      }
      Serial.println("Audio device online.");

      VOLUME_LIST[0] = 0;
      VOLUME_LIST[1] = 10;
      VOLUME_LIST[2] = 20;
      VOLUME_LIST[3] = 30;
      volume_setting = 1;
      sound_player.volume(VOLUME_LIST[volume_setting]);
    }


    /*************************************************
     * Rotates through all possible sound profiles.
     */
    void nextSoundProfile()
    {
      curr_sound_num++;
      if(curr_sound_num >= SOUND_NUM)
        curr_sound_num = 0;
    }

    /*************************************************
     * Rotates through all possible volume settings.
     */
    void nextVolumeSetting()
    {
      volume_setting++;
      if(volume_setting >= VOLUME_NUM)
        volume_setting = 0;
    }
	
    /*************************************************
     * Plays a welcome message whenever the blaster is
     * started.
     */
    void playWelcomeSound()
    {
      sound_player.play(WELCOME_SOUND_OFFSET);
      delay(2000);
    }

    /*************************************************
     * Plays the currently selected trigger sound.
     */
    void playTriggerSound()
    {
      sound_player.volume(VOLUME_LIST[volume_setting]);
      sound_player.play(curr_sound_num);
    }

    /*************************************************
     * Returns the currently selected sound code.
     * @return uint8_t The current sound code.
     */
    uint8_t getCurrSoundCode()
    {
      return curr_sound_num;
    }

    uint8_t getCurrVolumeLevel()
    {
      return volume_setting;
    }

    /*************************************************
     * Plays the selection soudn of a new color.
     */
    void playNewColorSound(uint8_t new_color_code)
    {
      sound_player.pause();
      sound_player.volume(10);
      sound_player.play(new_color_code + COLOR_TYPE_OFFSET);
    }

    /*************************************************
     * Plays the selection sound of a new sound.
     */
    void playNewSoundSound(uint8_t new_sound_code)
    {
      sound_player.pause();
      sound_player.volume(10);
      sound_player.play(new_sound_code + SOUND_TYPE_OFFSET);
    }

    /*************************************************
     * Plays the selection sound of a new volume
     */
    void playNewVolumeSound(uint8_t new_volume_setting)
    {
      sound_player.pause();
      sound_player.volume(10);
      sound_player.play(new_volume_setting + VOLUME_TYPE_OFFSET);
    }

  private:

    DFRobotDFPlayerMini sound_player;

    const int SOUND_NUM = 9;
    uint8_t curr_sound_num;

    const int SOUND_TYPE_OFFSET = 200;
    const int WELCOME_SOUND_OFFSET = 150;
    const int COLOR_TYPE_OFFSET = 100;
    const int VOLUME_TYPE_OFFSET = 50;

    const uint8_t VOLUME_NUM = 4;
    uint8_t volume_setting;
    uint8_t VOLUME_LIST[4];
};

/**********************************************************/
/**********************************************************/
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
    ButtonSystem()
    {
      pinMode(TRIGGER_BUTTON, INPUT);
      pinMode(SETTINGS_BUTTON, INPUT);

      trigger_pressed = false;
      settings_pressed = false;
      curr_setting_type = SettingsType::COLOR;
    }

    /*************************************************
     * Checks if the trigger is currently pressed.
     * @return bool True if the button has been pressed.
     */
    bool isTriggerPressed()
    {
      bool trigger = checkButtonDown(TRIGGER_BUTTON, &trigger_pressed);
      return trigger;
    }

    SettingsButtonState isSettingsPressed(uint32_t short_time, uint32_t long_time)
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

    void changeSettingsType()
    {
        if(curr_setting_type == SettingsType::COLOR)
        {
          Serial.println("Settings: SOUND");
          curr_setting_type = SettingsType::SOUND;
        }
        else if(curr_setting_type == SettingsType::SOUND)
        {
          Serial.println("Settings: VOLUME");
          curr_setting_type = SettingsType::VOLUME;
        }
        else if(curr_setting_type == SettingsType::VOLUME)
        {
          Serial.println("Settings: COLOR");
          curr_setting_type = SettingsType::COLOR;
        }
    }

    SettingsType getCurrSettingType()
    {
      return curr_setting_type;
    }

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
    bool checkButtonDown(uint8_t pin, bool* button_state)
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

    uint32_t checkButtonUp(uint8_t pin, bool* button_state, uint32_t* button_down_timer)
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
};

/**********************************************************/
/**********************************************************/
LedSystem* led;
SoundSystem* sound;
ButtonSystem* buttons;

void setup()
{
  buttons = new ButtonSystem();
  led = new LedSystem();
  //sound = new SoundSystem();
  //sound->playWelcomeSound();
  Serial.begin(9600);
}

void loop()
{
	if(buttons->isTriggerPressed())
  {
    //sound->playTriggerSound();
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
        //sound->playNewColorSound();
      }
      else if(buttons->getCurrSettingType() == SettingsType::SOUND)
      {
        Serial.println("Next sound profile");
        sound->nextSoundProfile();
        //sound->playNewSoundSound();
      }
      else if(buttons->getCurrSettingType() == SettingsType::VOLUME)
      {
        Serial.println("Next volume setting");
        sound->nextVolumeSetting();
        //sound->playNewVolumeSound();
      }
      else
      {
        Serial.println("Error: Wrong settings type detected.");
      }
    }
    else if(settingsState == SettingsButtonState::LONG)
    {
      buttons->changeSettingsType();
    }
  }
  led->performAnimationStep(1);
  delay(5);
}
