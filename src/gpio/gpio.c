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
#include <stdbool.h>
#include "gpio.h"
#include "board.h"
#include "pin_mux.h"

void gpio_setPowerLED(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_LED_PWR_GPIO_PORT, 1u << BOARD_LED_PWR_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_LED_PWR_GPIO_PORT, 1u << BOARD_LED_PWR_GPIO_PIN);
    }
}

void gpio_setCommLED(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_LED_COMM_GPIO_PORT, 1u << BOARD_LED_COMM_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_LED_COMM_GPIO_PORT, 1u << BOARD_LED_COMM_GPIO_PIN);
    }
}

void gpio_setStatusLED(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_LED_STATUS_GPIO_PORT, 1u << BOARD_LED_STATUS_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_LED_STATUS_GPIO_PORT, 1u << BOARD_LED_STATUS_GPIO_PIN);
    }
}

void gpio_setMilliAmpEn(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_mA_nEN_GPIO_PORT, 1u << BOARD_mA_nEN_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_mA_nEN_GPIO_PORT, 1u << BOARD_mA_nEN_GPIO_PIN);
    }
}

void gpio_setMicroAmpEn(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_uA_nEN_GPIO_PORT, 1u << BOARD_uA_nEN_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_uA_nEN_GPIO_PORT, 1u << BOARD_uA_nEN_GPIO_PIN);
    }
}

void gpio_setNanoAmpEn(bool val) {
    if( val ) {
        GPIO_PortClear(GPIO, BOARD_nA_nEN_GPIO_PORT, 1u << BOARD_nA_nEN_GPIO_PIN);
    }
    else {
        GPIO_PortSet(GPIO, BOARD_nA_nEN_GPIO_PORT, 1u << BOARD_nA_nEN_GPIO_PIN);
    }
}