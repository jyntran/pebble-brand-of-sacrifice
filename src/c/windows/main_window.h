#pragma once

#include <pebble.h>
#include "../modules/settings.h"

#define BRAND_STROKE_WIDTH 3
#define BRAND_STROKE_WIDTH_LG 5

#define OLDE 1
#define NEUE 2
#define OLDE_TIME_FONT RESOURCE_ID_FONT_OLDEENGLISH_48
#define NEUE_TIME_FONT RESOURCE_ID_FONT_BLACKFOREST_42
#define OLDE_DATE_FONT RESOURCE_ID_FONT_OLDEENGLISH_32
#define NEUE_DATE_FONT RESOURCE_ID_FONT_BLACKFOREST_28
#define TIME_HEIGHT 48
#define DATE_HEIGHT 34
#define OLDE_TIME_PADDING 26
#define NEUE_TIME_PADDING 18
#define OLDE_DATE_PADDING 15
#define NEUE_DATE_PADDING 15
#define OLDE_DATE_OFFSET 0
#define NEUE_DATE_OFFSET 4

void prv_window_push();
void prv_window_update();
void bluetooth_callback(bool connected);
void accel_tap_handler(AccelAxisType axis, int32_t direction);
void battery_callback(BatteryChargeState state);
