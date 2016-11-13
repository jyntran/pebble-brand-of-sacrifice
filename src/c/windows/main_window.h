#pragma once

#include <pebble.h>
#include "../modules/settings.h"

#define BRAND_STROKE_WIDTH 3
#define BRAND_STROKE_WIDTH_LG 3

#define OE 1
#define OE_TIME_FONT RESOURCE_ID_FONT_OLDEENGLISH_48
#define OE_DATE_FONT RESOURCE_ID_FONT_OLDEENGLISH_32
#define OE_TIME_AREA 48
#define OE_DATE_HEIGHT 34
#define OE_TIME_PADDING 22
#define OE_DATE_PADDING 15
#define OE_DATE_OFFSET 0

#define BF 2
#define BF_TIME_FONT RESOURCE_ID_FONT_BLACKFOREST_42
#define BF_DATE_FONT RESOURCE_ID_FONT_BLACKFOREST_28
#define BF_TIME_AREA 42
#define BF_DATE_HEIGHT 34
#define BF_TIME_PADDING 17
#define BF_DATE_PADDING 15
#define BF_DATE_OFFSET 4

#define NR 3
#define NR_TIME_FONT RESOURCE_ID_FONT_NEWROCKER_42
#define NR_DATE_FONT RESOURCE_ID_FONT_NEWROCKER_32
#define NR_TIME_AREA 42
#define NR_DATE_HEIGHT 34
#define NR_TIME_PADDING 12
#define NR_DATE_PADDING 0
#define NR_DATE_OFFSET 0

void prv_window_push();
void prv_window_update();
void bluetooth_callback(bool connected);
void accel_tap_handler(AccelAxisType axis, int32_t direction);
void battery_callback(BatteryChargeState state);
