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

#ifndef _UI_H_
#define _UI_H_

typedef enum {
    UI_ELE_COMM_LED = 0x00,
    UI_ELE_STAT_LED,
    UI_ELE_PWR_LED,
    UI_ELE__MAX__
} ui_ele_enum;

typedef enum {
    UI_MODE_OFF = 0x00,
    UI_MODE_ON,
    UI_MODE_FLASH_SLOW,
    UI_MODE_FLASH_FAST,
    UI_MODE__MAX__
} ui_mode_enum;

void ui_init();
void ui_update();
void ui_setState(ui_ele_enum ele, ui_mode_enum mode);

#endif // _UI_H_