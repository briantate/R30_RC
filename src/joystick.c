/*
 * joystick.c
 *
 * Created: 5/23/2016 5:41:28 PM
 *  Author: brian.tate
 */

#include "joystick.h"

#include <string.h>

// macro's for 8b ADC data processing
#define GUARD_BAND 20
#define CENTER_VAL 128
#define FORWARD_VAL CENTER_VAL + GUARD_BAND
#define BACKWARD_VAL CENTER_VAL - GUARD_BAND
#define MAX_VAL 127
#define POSITIVE true
#define NEGATIVE false

// private prototypes
void joystick_filter(joystickPtr joystickInstance);

struct JOYSTICK {
  uint8_t xRaw;            // raw horizontal value
  uint8_t xFiltered;       // filtered horizontal value
  ADC_ReadVal_t readXadc;  // pointer to func to read horizontal adc value
  bool xPolarity;          // right = positive, left = negative
  uint8_t yRaw;            // raw vertical value
  uint8_t yFiltered;       // filtered vertical value
  ADC_ReadVal_t readYadc;  // pointer to func to read vertical adc value
  bool yPolarity;          // up = positive, down = negative
};

#define MAX_JOYSTICKS 2  // arbitrary limit -- my game controller has joysticks
static struct JOYSTICK JoystickPool[MAX_JOYSTICKS];
static uint8_t numberOfJoysticks = 0;

joystickPtr Joystick_Create(ADC_ReadVal_t readHorz, ADC_ReadVal_t readVert) {
  joystickPtr joystick = NULL;
  if (numberOfJoysticks < MAX_JOYSTICKS) {
    joystick = &JoystickPool[numberOfJoysticks++];
    // initialize the object
    joystick->xRaw = CENTER_VAL;
    joystick->yRaw = CENTER_VAL;
    joystick->readXadc = readHorz;
    joystick->readYadc = readVert;
  }

  return joystick;
}

void Joystick_Delete(joystickPtr joystickInstance) {
  // to do -- shouldn't really ever need to delete a joystick...
}

uint8_t Joystick_GetX(joystickPtr joystickInstance) {
  return joystickInstance->xFiltered;
}

bool Joystick_GetXpolarity(joystickPtr joystickInstance) {
  return joystickInstance->xPolarity;
}

uint8_t Joystick_GetY(joystickPtr joystickInstance) {
  return joystickInstance->yFiltered;
}

bool Joystick_GetYpolarity(joystickPtr joystickInstance) {
  return !(
      joystickInstance->yPolarity);  // Inverting because this JS +Y is reverse
}

void Joystick_Measure(joystickPtr joystickInstance) {
  // call injected ADC functions
  joystickInstance->xRaw = joystickInstance->readXadc();
  joystickInstance->yRaw = joystickInstance->readYadc();

  joystick_filter(joystickInstance);
}

/************************************************************************/
/*			Private Functions */
/************************************************************************/

void joystick_filter(joystickPtr joystickInstance) {
  // filter vertical
  if (joystickInstance->yRaw > FORWARD_VAL) {
    joystickInstance->yFiltered = joystickInstance->yRaw - FORWARD_VAL;
    joystickInstance->yPolarity = POSITIVE;
  } else if (joystickInstance->yRaw < BACKWARD_VAL) {
    joystickInstance->yFiltered = BACKWARD_VAL - joystickInstance->yRaw;
    joystickInstance->yPolarity = NEGATIVE;
  } else {
    joystickInstance->yFiltered = 0;
    joystickInstance->yPolarity = POSITIVE;
  }

  // make sure it doesn't overflow...
  if (joystickInstance->yFiltered > MAX_VAL) {
    joystickInstance->yFiltered = MAX_VAL;
  }

  joystickInstance->yFiltered =
      2 *
      joystickInstance
          ->yFiltered;  // multiply by 2 since we only get half the resolution

  // filter horizontal
  if (joystickInstance->xRaw > FORWARD_VAL) {
    joystickInstance->xFiltered = joystickInstance->xRaw - FORWARD_VAL;
    joystickInstance->xPolarity = POSITIVE;
  } else if (joystickInstance->xRaw < BACKWARD_VAL) {
    joystickInstance->xFiltered = BACKWARD_VAL - joystickInstance->xRaw;
    joystickInstance->xPolarity = NEGATIVE;
  } else {
    joystickInstance->xFiltered = 0;
    joystickInstance->xPolarity = POSITIVE;
  }

  // make sure it doesn't overflow...
  if (joystickInstance->xFiltered > MAX_VAL) {
    joystickInstance->xFiltered = MAX_VAL;
  }

  joystickInstance->xFiltered =
      2 *
      joystickInstance
          ->xFiltered;  // multiply by 2 since we only get half the resolution
}
