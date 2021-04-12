/******************************************************************************
MIT License

Copyright (c) 2021 Glassboard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include <stdint.h>
#include "ui.h"
#include "board.h"
#include "gpio.h"

#define LED_UPDATE_RATE_FAST_uS  100000
#define LED_UPDATE_RATE_SLOW_uS  500000

typedef struct {
    ui_mode_enum mode;
    bool state;
    uint32_t refTime;
} ui_config_str;

ui_config_str ui_elements[UI_ELE__MAX__] = {
    {.mode = UI_MODE_OFF, .state = 0, .refTime = 0x0000},
    {.mode = UI_MODE_OFF, .state = 0, .refTime = 0x0000},
    {.mode = UI_MODE_OFF, .state = 0, .refTime = 0x0000},
};

void ui_init() {
    ui_setState(UI_ELE_COMM_LED, UI_MODE_OFF);
    ui_setState(UI_ELE_STAT_LED, UI_MODE_OFF);
    ui_setState(UI_ELE_PWR_LED, UI_MODE_ON);
}

void ui_update() {
    // Loop through all of our UI elements and see if they are set for a 'dynamic' config
    // and in need of updating
    for(uint8_t i = 0; i < UI_ELE__MAX__; i++) {
        if( (ui_elements[i].mode == UI_MODE_FLASH_FAST) || (ui_elements[i].mode  == UI_MODE_FLASH_SLOW) ) {

            // Check if the LED is due for an update
            if( (ui_elements[i].refTime + (ui_elements[i].mode == UI_MODE_FLASH_FAST ? LED_UPDATE_RATE_FAST_uS : LED_UPDATE_RATE_SLOW_uS)) < BOARD_GetTick() ) {
                // Store the LEDs new ref time
                ui_elements[i].refTime = BOARD_GetTick();
                // Since we are flashing, invert the LED state
                ui_elements[i].state = !ui_elements[i].state;
                // Write the new state via the GPIO module
                switch(i) {
                    case UI_ELE_COMM_LED:
                        gpio_setCommLED(ui_elements[i].state);
                        break;

                    case UI_ELE_STAT_LED:
                        gpio_setStatusLED(ui_elements[i].state);
                        break;

                    case UI_ELE_PWR_LED:
                        gpio_setPowerLED(ui_elements[i].state);
                        break;

                    default:
                        break;
                }
            }

        }
        else {
            // This element is either solid on or off.. we don't need to do anything.
        }
    }
}

void ui_setState(ui_ele_enum ele, ui_mode_enum mode) {

    // Check if the element index given is in range
    if( ele >= UI_ELE__MAX__ )
        return;

    // Check if the state index given is in range
    if( mode >= UI_MODE__MAX__ ) {
        // The index given was out of range. Set the state to OFF.
        ui_elements[ele].mode = UI_MODE_OFF;
    }
    else {
        // Set the new config for the given element
        ui_elements[ele].mode = mode;
    }

    // Check if the new mode is ON or OFF
    if( (ui_elements[ele].mode == UI_MODE_OFF) || (ui_elements[ele].mode == UI_MODE_ON) ) {
        // If the new mode is a "static" mode, then write the value now
        // via the GPIO module.
        switch(ele) {
            case UI_ELE_COMM_LED:
                gpio_setCommLED((mode == UI_MODE_OFF ? false : true));
                break;

            case UI_ELE_STAT_LED:
                gpio_setStatusLED((mode == UI_MODE_OFF ? false : true));
                break;

            case UI_ELE_PWR_LED:
                gpio_setPowerLED((mode == UI_MODE_OFF ? false : true));
                break;

            default:
                break;
        }
    }
}
