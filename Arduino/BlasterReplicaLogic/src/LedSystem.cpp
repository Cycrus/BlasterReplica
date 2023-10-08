#include "LedSystem.h"

LedSystem::LedSystem()
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

void LedSystem::nextColorProfile()
{
  curr_color++;
  if(curr_color >= COLOR_NUM)
    curr_color = 0;
}

uint8_t LedSystem::getCurrColorCode()
{
  return curr_color;
}

uint32_t LedSystem::getAnimationStep()
{
  return curr_animation_step;
}

void LedSystem::performAnimationStep(int step_delay)
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

void LedSystem::clearAllLeds()
{
  clearLed(front_led);
  clearLed(right_led);
  clearLed(left_led);
}

void LedSystem::showAllLeds()
{
  front_led->show();
  left_led->show();
  right_led->show();
}

void LedSystem::startAnimation()
{
  doPerformAnimation = true;
  curr_animation_step = 1;
  global_step_counter = 0;
}

void LedSystem::animateFrontLed()
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

void LedSystem::animateSideLed(Adafruit_NeoPixel* led)
{
  uint32_t min_slide_step = getAnimationStepPercentage(0);
  uint32_t max_slide_step = getAnimationStepPercentage(50);
  uint32_t width = 3;

  float slide_range = max_slide_step - min_slide_step;
  float norm_pixel_start = (float)(((float)curr_animation_step - (float)min_slide_step) / (float)slide_range);
  uint32_t proj_pixel_start = norm_pixel_start * SIDE_LED_NUM;
  
  lightLed(led, width, proj_pixel_start);
}

uint32_t LedSystem::getAnimationStepPercentage(uint8_t percentage)
{
  if(percentage > 100)
    percentage = 100;

  return ((float)MAX_ANIMATION_STEP / (float)100) * (float)percentage;
}

uint32_t LedSystem::getCurrentAnimationProgressPercentage()
{
  return ((float)MAX_ANIMATION_STEP * (float)100 / (float)curr_animation_step);
}

void LedSystem::lightLed(Adafruit_NeoPixel* led, uint8_t pixel_count, uint8_t start)
{
  led->fill(COLOR_LIST[curr_color], start, pixel_count);
}

void LedSystem::clearLed(Adafruit_NeoPixel* led)
{
  led->setBrightness(255);
  led->clear();
}

bool LedSystem::animationProgressLowerBound(uint8_t percentage)
{
  if(percentage > 100)
    percentage = 100;

  return ((curr_animation_step * 100) / (MAX_ANIMATION_STEP)) >= percentage;
}


bool LedSystem::animationProgressUpperBound(uint8_t percentage)
{
  if(percentage > 100)
    percentage = 100;

  return ((curr_animation_step * 100) / (MAX_ANIMATION_STEP)) < percentage;
}
