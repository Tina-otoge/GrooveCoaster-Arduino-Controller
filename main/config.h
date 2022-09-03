#pragma once


#define REVERSE_PIN_SIGNALS false

// Sends report messages about each button every seconds
#define SERIAL_LOG_DEBUG false

// Any input on the boosters click will disable all directions inputs on it
#define CLICK_DISABLES_DIRECTION true


// DEBOUNCING
#define MINIMUM_PRESS_TIME   5
#define MINIMUM_RELEASE_TIME 20


// PIN MAPPING
// middle square button
#define PIN_START 7

// left booster
#define PIN_LBOOSTER_LEFT  11
#define PIN_LBOOSTER_DOWN  10
#define PIN_LBOOSTER_UP     8
#define PIN_LBOOSTER_RIGHT  9
#define PIN_LBOOSTER_CLICK  5

// right booster
#define PIN_RBOOSTER_LEFT  1
#define PIN_RBOOSTER_DOWN  2
#define PIN_RBOOSTER_UP    3
#define PIN_RBOOSTER_RIGHT 4
#define PIN_RBOOSTER_CLICK 6
