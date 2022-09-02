#include "main.h"
#include "config.h"

void setup() {
    init_pins();
    delay(1000);
    hold_button_a(500);
}


void loop() {
    update_buttons();
    update_inputs();
    if (SERIAL_LOG_DEBUG)
        log_report();
}
