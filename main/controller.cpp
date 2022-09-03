#include <SwitchControlLibrary.h>
#include "main.h"
#include "config.h"


// "Button" is already in the global namespace from SwitchControlLibrary
// We use increments rather than bitmasks to use them as array indexes
enum Pad {
    START = 0,
    LBOOSTER_LEFT = 1,
    LBOOSTER_DOWN = 2,
    LBOOSTER_UP = 3,
    LBOOSTER_RIGHT = 4,
    LBOOSTER_CLICK = 5,
    RBOOSTER_LEFT = 6,
    RBOOSTER_DOWN = 7,
    RBOOSTER_UP = 8,
    RBOOSTER_RIGHT = 9,
    RBOOSTER_CLICK = 10,
    BUTTONS_COUNT = 11,
};


struct ButtonPinPair {
    Pad button;
    int pin;
};


struct ButtonInfo {
    bool state = false;
    bool raw_state = false;
    long raw_time = 0;
};


static const ButtonPinPair PINS_MAP[] = {
    {START,          PIN_START},
    {LBOOSTER_LEFT,  PIN_LBOOSTER_LEFT},
    {LBOOSTER_DOWN,  PIN_LBOOSTER_DOWN},
    {LBOOSTER_UP,    PIN_LBOOSTER_UP},
    {LBOOSTER_RIGHT, PIN_LBOOSTER_RIGHT},
    {LBOOSTER_CLICK, PIN_LBOOSTER_CLICK},
    {RBOOSTER_LEFT,  PIN_RBOOSTER_LEFT},
    {RBOOSTER_DOWN,  PIN_RBOOSTER_DOWN},
    {RBOOSTER_UP,    PIN_RBOOSTER_UP},
    {RBOOSTER_RIGHT, PIN_RBOOSTER_RIGHT},
    {RBOOSTER_CLICK, PIN_RBOOSTER_CLICK},
};


static auto controller = SwitchControlLibrary();

static unsigned long last_update = 0;

static ButtonInfo buttons[BUTTONS_COUNT];


void hold_button_a(unsigned long time) {
    controller.pressButtonA();
    delay(time);
    controller.releaseButtonA();
}


void init_pins() {
    for (unsigned int i = 0; i < BUTTONS_COUNT; i += 1)
        pinMode(PINS_MAP[i].pin, INPUT);
}


/*
   Switch controller layout:

+-----------------------+
|    ZL            ZR   |
|    L              R   |
|                       |
|    UP             X   |
|LEFT  RIGHT      Y   A |
|   DOWN            B   |
|                       |
|MINUS              PLUS|
|SCREENSHOT         HOME|
+-----------------------+
*/

class HatManager {
    public:
        static bool left;
        static bool down;
        static bool up;
        static bool right;

        static Hat getHat() {
            if (up && left)
                return Hat::TOP_LEFT;
            if (up && right)
                return Hat::TOP_RIGHT;
            if (down && right)
                return Hat::BOTTOM_RIGHT;
            if (down && left)
                return Hat::BOTTOM_RIGHT;
            if (left)
                return Hat::LEFT;
            if (down)
                return Hat::BOTTOM;
            if (up)
                return Hat::TOP;
            if (right)
                return Hat::RIGHT;
            return Hat::CENTER;
        }

        static uint8_t getHatInt() {
            return static_cast<uint8_t>(getHat());
        }

        static void set_left(bool state) {
            left = state;
        }

        static void set_down(bool state) {
            down = state;
        }

        static void set_up(bool state) {
            up = state;
        }

        static void set_right(bool state) {
            right = state;
        }
};
bool HatManager::left = false;
bool HatManager::down = false;
bool HatManager::up = false;
bool HatManager::right = false;

void press(Pad button) {
    switch (button) {
        case START:
            return controller.pressButtonPlus();
        case LBOOSTER_LEFT:
            return HatManager::set_left(true);
        case LBOOSTER_DOWN:
            return HatManager::set_down(true);
        case LBOOSTER_UP:
            return HatManager::set_up(true);
        case LBOOSTER_RIGHT:
            return HatManager::set_right(true);
        case LBOOSTER_CLICK:
            return controller.pressButtonL();
        case RBOOSTER_LEFT:
            return controller.pressButtonY();
        case RBOOSTER_DOWN:
            return controller.pressButtonB();
        case RBOOSTER_UP:
            return controller.pressButtonX();
        case RBOOSTER_RIGHT:
            return controller.pressButtonA();
        case RBOOSTER_CLICK:
            return controller.pressButtonR();
    }
}

void release(Pad button) {
    switch (button) {
        case START:
            return controller.releaseButtonPlus();
        case LBOOSTER_LEFT:
            return HatManager::set_left(false);
        case LBOOSTER_DOWN:
            return HatManager::set_down(false);
        case LBOOSTER_UP:
            return HatManager::set_up(false);
        case LBOOSTER_RIGHT:
            return HatManager::set_right(false);
        case LBOOSTER_CLICK:
            return controller.releaseButtonL();
        case RBOOSTER_LEFT:
            return controller.releaseButtonY();
        case RBOOSTER_DOWN:
            return controller.releaseButtonB();
        case RBOOSTER_UP:
            return controller.releaseButtonX();
        case RBOOSTER_RIGHT:
            return controller.releaseButtonA();
        case RBOOSTER_CLICK:
            return controller.releaseButtonR();
    }
}

void handle_hat() {
    controller.moveHat(HatManager::getHatInt());
}


void update_input(const ButtonPinPair &button_pin, unsigned long delta) {
    auto &info = buttons[button_pin.button];
    bool state = digitalRead(button_pin.pin);

    if (REVERSE_PIN_SIGNALS)
        state = !state;

    if (info.raw_state != state) {
        info.raw_time = 0;
        info.raw_state = state;
    }

    info.raw_time += delta;

    if (info.raw_state == info.state)
        return; // nothing to do if nothing changed

    if (
        (info.raw_state && info.raw_time >= MINIMUM_PRESS_TIME)
        || (!info.raw_state && info.raw_time >= MINIMUM_RELEASE_TIME)
    )
        info.state = info.raw_state;
}


unsigned long get_delta() {
    auto now = millis();
    auto delta = now - last_update;
    last_update = now;
    return delta;
}

void update_inputs() {
    auto delta = get_delta();
    for (unsigned int i = 0; i < BUTTONS_COUNT; i += 1)
        update_input(PINS_MAP[i], delta);
}


void handle_macros() {
    if (!buttons[START].state)
        return;
    // every macros use the START button

    if (buttons[LBOOSTER_CLICK].state) {
        controller.pressButtonHome();
        delay(100);
        controller.releaseButtonHome();
    }

    if (buttons[RBOOSTER_CLICK].state) {
        controller.pressButtonRClick();
        delay(100);
        controller.releaseButtonRClick();
    }

    if (buttons[RBOOSTER_LEFT].state && buttons[RBOOSTER_UP].state)
        controller.pressButtonCapture();
    else
        controller.releaseButtonCapture();
}

void handle_boosters() {
    if (buttons[RBOOSTER_CLICK].raw_state) {
        buttons[RBOOSTER_LEFT].state = false;
        buttons[RBOOSTER_DOWN].state = false;
        buttons[RBOOSTER_UP].state = false;
        buttons[RBOOSTER_RIGHT].state = false;
    }
    if (buttons[LBOOSTER_CLICK].raw_state) {
        buttons[LBOOSTER_LEFT].state = false;
        buttons[LBOOSTER_DOWN].state = false;
        buttons[LBOOSTER_UP].state = false;
        buttons[LBOOSTER_RIGHT].state = false;
    }
}


void update_button(Pad button) {
    if (buttons[button].state)
        press(button);
    else
        release(button);
}


void update_buttons() {
    Pad button;

    handle_macros();
    handle_boosters();

    for (unsigned int i = 0; i < BUTTONS_COUNT; i += 1) {
        button = PINS_MAP[i].button;
        if (buttons[button].state)
            press(button);
        else
            release(button);
    }

    handle_hat();
    controller.sendReport();
}


static unsigned long last_report = 0;

void log_report() {
    auto now = millis();
    auto delta = now - last_report;

    if (delta <= 1000)
        return;
    last_report = now;
    Serial.print("======================\n");
    Serial.print("Sending report\n");
    Serial.print("Time: ");
    Serial.print(now);
    Serial.print("\n");
    for (unsigned int i = 0; i < BUTTONS_COUNT; i += 1) {
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(" raw: ");
        Serial.print(buttons[i].raw_state);
        Serial.print(" | state: ");
        Serial.print(buttons[i].state);
        Serial.print("\n");
    }
    Serial.print("======================\n");
}
