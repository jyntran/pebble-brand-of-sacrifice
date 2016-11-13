#pragma once

#include <pebble.h>
#include "../modules/settings.h"

#define BRAND_STROKE_WIDTH 3

#define NR 1
#define NR_TIME_FONT RESOURCE_ID_FONT_NEWROCKER_42
#define NR_TIME_AREA 42
#define NR_TIME_PADDING 12

#define OE 2
#define OE_TIME_FONT RESOURCE_ID_FONT_OLDEENGLISH_48
#define OE_TIME_AREA 48
#define OE_TIME_PADDING 22

#define BF 3
#define BF_TIME_FONT RESOURCE_ID_FONT_BLACKFOREST_42
#define BF_TIME_AREA 42
#define BF_TIME_PADDING 17

void prv_window_push();
void prv_window_update();
void bluetooth_callback(bool connected);
void battery_callback(BatteryChargeState state);
