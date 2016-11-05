#pragma once

#include <pebble.h>
#define SETTINGS_KEY 1

typedef struct ClaySettings {
  GColor BackgroundColour;
  GColor BrandColour;
  GColor TimeColour;
  GColor BluetoothColour;
} ClaySettings;

struct ClaySettings settings;

void prv_load_settings();

