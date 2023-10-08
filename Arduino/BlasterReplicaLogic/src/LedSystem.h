#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define FRONT_LED_PIN 9
#define LEFT_LED_PIN 6
#define RIGHT_LED_PIN 5

#define FRONT_LED_NUM 4
#define SIDE_LED_NUM 30

class LedSystem
{
  public:

    /*************************************************
     * Constructor. Initializes LED devices.
     */
    LedSystem();

    /*************************************************
     * Rotates through all possible color profiles.
     */
    void nextColorProfile();

    /*************************************************
     * Returns the current color value.
     * @return uint8_t The current color code.
     */
    uint8_t getCurrColorCode();

    /*************************************************
     * Returns the current animation step. Used to check
     * if configuration attempts are valid or not.
     * @return uint32_t The current step of the animation progress.
     */
    uint32_t getAnimationStep();

    /*************************************************
     * Performs a single animation step. Must be called in every iteration of the controller.
     * @param step_delay The delay in between the individual animation steps measured in 
     *                   controller cycles. The higher this value, the slower the animation
     *                   is played.
     */
    void performAnimationStep(int step_delay);

    /*************************************************
     * Clears the color of all neopixel leds. 
     */
    void clearAllLeds();

    /*************************************************
     * A wrapper to call the update function of the leds on all led lights. 
     */
    void showAllLeds();

    /*************************************************
     * Starts the animation by resetting all values.
     */
    void startAnimation();

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
    void animateFrontLed();

    /*************************************************
     * The animation logic of the side led collections.
     * @param led A pointer to the led collection to animate.
     */
    void animateSideLed(Adafruit_NeoPixel* led);

    /*************************************************
     * Returns the absolute step number of the animation given a percentage (0-100).
     * @param percentage The percentage to translate.
     * @return uint32_t The absolute step number translated.
     */
    uint32_t getAnimationStepPercentage(uint8_t percentage);

    /*************************************************
     * Returns the current progress of the animation in a percentage number (0-100).
     * @return uint32_t The percentage of the current animation progress.
     */
    uint32_t getCurrentAnimationProgressPercentage();

    /*************************************************
     * Lights a led collection with a given number of pixels starting at a certain position.
     * As a color it takes the currently active color.
     * @param led A pointer to the led collection to light up.
     * @param pixel_count The number of pixels to light up.
     * @param start The id of the first pixel to light up.
     */
    void lightLed(Adafruit_NeoPixel* led, uint8_t pixel_count, uint8_t start);

    /*************************************************
     * Clears the color of a certain led collection.
     * @param led A pointer to the led to clear.
     */
    void clearLed(Adafruit_NeoPixel* led);

    /*************************************************
     * Checks if the animation progress is at least the given percentage.
     * @param percentage The percentage of animation progress to check for.
     * @return bool True if the animation progress is at least the given percentage.
     */
    bool animationProgressLowerBound(uint8_t percentage);

    /*************************************************
     * Checks if the animation progress is below the given percentage.
     * @param percentage The percentage of animation progress to check for.
     * @return bool True if the animation progress is below the given percentage.
     */
    bool animationProgressUpperBound(uint8_t percentage);
};
