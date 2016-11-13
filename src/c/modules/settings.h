#pragma once

#include <pebble.h>
#define SETTINGS_KEY 1

typedef struct ClaySettings {
  int FontStyle;
  GColor BackgroundColour;
  GColor BrandColour;
  GColor TimeColour;
  GColor BatteryColour;
  bool BatteryOutline;
  GColor BluetoothColour;
  bool BluetoothVibration;
} ClaySettings;

struct ClaySettings settings;

void prv_load_settings();

