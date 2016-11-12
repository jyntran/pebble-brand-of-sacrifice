#pragma once

#include <pebble.h>
#include "../modules/settings.h"

#define BRAND_STROKE_WIDTH 3
#define BRAND_STROKE_WIDTH_LG 3.5
#define TIME_FONT RESOURCE_ID_FONT_SMALLTYPE_40
#define TIME_HEIGHT 42

void prv_window_push();
void prv_window_update();
void bluetooth_callback(bool connected);

