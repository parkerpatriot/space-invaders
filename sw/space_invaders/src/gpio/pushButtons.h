#ifndef PUSHBUTTONS_H_
#define PUSHBUTTONS_H_

#include <stdbool.h>

#include "xparameters.h"
#include "xgpio.h"

#define BTN_LEFT_MASK	0x08
#define BTN_RIGHT_MASK	0x02
#define BTN_DOWN_MASK	0x04
#define BTN_UP_MASK		0x10
#define BTN_CENTER_MASK	0x01

// Setup the PBs in HW
void pushButtons_init();

// Public functions to make life easy
bool pushButtons_LeftPressed();
bool pushButtons_RightPressed();
bool pushButtons_DownPressed();
bool pushButtons_UpPressed();
bool pushButtons_CenterPressed();

#endif /* PUSHBUTTONS_H_ */
