/* A script to control the working blaster model.
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

SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);

#define TRIGGER_BUTTON 10
#define COLOR_BUTTON 11
#define SOUND_BUTTON 12

#define FRONT_LED_PIN 14
#define LEFT_LED_PIN 15
#define RIGHT_LED_PIN 16

#define FRONT_LED_NUM 4
#define LEFT_LED_NUM 12
#define RIGHT_LED_NUM 12

/**********************************************************/
class LedSystem
{
  public:

    LedSystem()
    {
      front_led = Adafruit_NeoPixel(FRONT_LED_NUM, FRONT_LED_PIN, NEO_GRB + NEO_KHZ800);
      left_led = Adafruit_NeoPixel(LEFT_LED_NUM, LEFT_LED_PIN, NEO_GRB + NEO_KHZ800);
      right_led = Adafruit_NeoPixel(RIGHT_LED_NUM, RIGHT_LED_PIN, NEO_GRB + NEO_KHZ800);
    
      COLOR_LIST[0] = front_led.Color(255, 0, 0); // Red
      COLOR_LIST[1] = front_led.Color(0, 255, 0); // Green
      COLOR_LIST[2] = front_led.Color(0, 0, 255); // Blue

      doPerformAnimation = false;
      curr_color = 0;
      curr_animation_step = 0;
      global_step_counter = 0;
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
     * @return uint32_t The current color value.
     */
    uint8_t getCurrColorCode()
    {
      return curr_color;
    }

    uint32_t getAnimationStep()
    {
      return curr_animation_step;
    }

    /*************************************************
     * Performs a single animation step. Must be called in every iteration of the controller.
     */
    void performAnimationStep(int step_delay)
    {
      if(!doPerformAnimation)
          return;

      if(global_step_counter % step_delay == 0)
      {
        // TODO: Animation comes here!

        curr_animation_step++;
        if(curr_animation_step > MAX_ANIMATION_STEP)
        {
          curr_animation_step = 0;
          doPerformAnimation = false;
        }
      }

      global_step_counter++;
    }

    void startAnimation()
    {
      doPerformAnimation = 0;
      curr_animation_step = 0;
      global_step_counter = 0;
    }

  private:
    Adafruit_NeoPixel front_led;
    Adafruit_NeoPixel left_led;
    Adafruit_NeoPixel right_led;

    bool doPerformAnimation;

    const uint8_t COLOR_NUM = 3;
    uint32_t COLOR_LIST[3];
    uint8_t curr_color;

    const uint32_t MAX_ANIMATION_STEP = 20;
    uint32_t curr_animation_step;

    int global_step_counter;
};

/**********************************************************/
class SoundSystem
{
  public:

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
      sound_player.volume(20);
    }

    void nextSoundProfile()
    {
      curr_sound_num++;
      if(curr_sound_num >= SOUND_NUM)
        curr_sound_num = 0;
    }

    void playTriggerSound()
    {
      sound_player.play(curr_sound_num);
    }

    uint8_t getCurrSoundCode()
    {
      return curr_sound_num;
    }

    void playNewColorSound(uint8_t new_color_code)
    {
      sound_player.play(new_color_code + COLOR_TYPE_OFFSET);
    }

    void playNewSoundSound(uint8_t new_sound_code)
    {
      sound_player.play(new_sound_code + SOUND_TYPE_OFFSET);
    }

  private:

  DFRobotDFPlayerMini sound_player;

  const int SOUND_NUM = 9;
  uint8_t curr_sound_num;

  const int SOUND_TYPE_OFFSET = 200;
  const int COLOR_TYPE_OFFSET = 100;
};

/**********************************************************/
class ButtonSystem
{
  public:

  ButtonSystem()
  {
    pinMode(TRIGGER_BUTTON, INPUT);
    pinMode(COLOR_BUTTON, INPUT);
    pinMode(SOUND_BUTTON, INPUT);

    trigger_pressed = false;
    color_pressed = false;
    sound_pressed = false;
  }

  bool isTriggerPressed()
  {
    checkButton(TRIGGER_BUTTON, &trigger_pressed);
  }

  bool isColorPressed()
  {
    checkButton(COLOR_BUTTON, &color_pressed);
  }

  bool isSoundPressed()
  {
    checkButton(SOUND_BUTTON, &sound_pressed);
  }

  private:
    bool trigger_pressed;
    bool color_pressed;
    bool sound_pressed;

    bool checkButton(uint8_t pin, bool* button_state)
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
};

/**********************************************************/
LedSystem* led;
SoundSystem* sound;
ButtonSystem* buttons;

void setup()
{
  buttons = new ButtonSystem();
  led = new LedSystem();
  sound = new SoundSystem();
}

void loop()
{
	if(buttons->isTriggerPressed())
  {
    sound->playTriggerSound();
    led->startAnimation();
  }

  if(led->getAnimationStep() == 0)
  {
    if(buttons->isColorPressed())
    {
      led->nextColorProfile();
      sound->playNewColorSound(led->getCurrColorCode());
    }

    if(buttons->isSoundPressed())
    {
      sound->nextSoundProfile();
      sound->playNewSoundSound(sound->getCurrSoundCode());
    }
  }

  led->performAnimationStep(10);
}