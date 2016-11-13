#pragma once

#include <pebble.h>
#define SETTINGS_KEY 1

typedef struct ClaySettings {
  GColor BackgroundColour;
  GColor BrandColour;
  GColor TimeColour;
  GColor DateColour;
  GColor BatteryColour;
  bool BatteryOutline;
  GColor BluetoothColour;
  bool ShowDate;
  bool DayMonthFormat;
  bool LargeBrand;
  int FontStyle;
} ClaySettings;

struct ClaySettings settings;

void prv_load_settings();

