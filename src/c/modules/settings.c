#include "settings.h"

struct ClaySettings settings;

static void prv_default_settings() {
  settings.BackgroundColour = GColorBlack;
  settings.BrandColour = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  settings.TimeColour = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  settings.BluetoothColour = PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
  settings.ShowTimeAtTop = false;
  settings.ShowDate = false;
  settings.BrandOnly = false;
}

static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *bg_colour_t = dict_find(iter, MESSAGE_KEY_BackgroundColour);
  if (bg_colour_t) {
    settings.BackgroundColour = GColorFromHEX(bg_colour_t->value->int32);
  }

  Tuple *bd_colour_t = dict_find(iter, MESSAGE_KEY_BrandColour);
  if (bd_colour_t) {
    settings.BrandColour = GColorFromHEX(bd_colour_t->value->int32);
  }

  Tuple *tm_colour_t = dict_find(iter, MESSAGE_KEY_TimeColour);
  if (tm_colour_t) {
    settings.TimeColour = GColorFromHEX(tm_colour_t->value->int32);
  }

  Tuple *bt_colour_t = dict_find(iter, MESSAGE_KEY_BluetoothColour);
  if (bt_colour_t) {
    settings.BluetoothColour = GColorFromHEX(bt_colour_t->value->int32);
  }

  Tuple *ttop_bool_t = dict_find(iter, MESSAGE_KEY_ShowTimeAtTop);
  if (ttop_bool_t) {
    settings.ShowTimeAtTop = ttop_bool_t->value->int32 == 1;
  }

  Tuple *dt_bool_t = dict_find(iter, MESSAGE_KEY_ShowDate);
  if (dt_bool_t) {
    settings.ShowDate = dt_bool_t->value->int32 == 1;
  }

  Tuple *bo_bool_t = dict_find(iter, MESSAGE_KEY_BrandOnly);
  if (bo_bool_t) {
    settings.BrandOnly = bo_bool_t->value->int32 == 1;
  }

  prv_save_settings();
}

void prv_load_settings() {
  prv_default_settings();
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(64, 64);
}


