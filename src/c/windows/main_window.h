#pragma once

#include <pebble.h>
#include "../modules/settings.h"

#define BRAND_STROKE_WIDTH 3
#define BRAND_STROKE_WIDTH_LG 3
#define TIME_FONT RESOURCE_ID_FONT_OLDEENGLISH_48
#define DATE_FONT RESOURCE_ID_FONT_OLDEENGLISH_32
#define TIME_HEIGHT 48
#define DATE_HEIGHT 34
#define TIME_PADDING_TOP 24
#define DATE_PADDING_TOP 15

void prv_window_push();
void prv_window_update();
void bluetooth_callback(bool connected);
void accel_tap_handler(AccelAxisType axis, int32_t direction);
void battery_callback(BatteryChargeState state);
