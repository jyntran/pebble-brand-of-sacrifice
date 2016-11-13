#include "main_window.h"

static Window *s_window;
static TextLayer *s_hour_layer, *s_mins_layer, *s_day_layer, *s_mth_layer;
static GFont s_time_font, s_date_font;
static Layer *s_brand_layer, *s_bt_layer, *s_battery_layer;
static AppTimer *timer;
static int s_battery_level;

void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  uint32_t m; // multiplier
  if (settings.LargeBrand) {
    m = 2;
  } else {
    m = 1;
  }

  int height = (s_battery_level * (73*m)) / 100;

  GPathInfo BATT_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0, 38 * m},
      {0, -36 * m}
    }
  };

  GPathInfo BATT_BAR_INFO = {
    .num_points = 6,
    .points = (GPoint[6]) {
      {-1, 38 * m},
      {0, 39 * m},
      {1, 38 * m},
      {1, (39 * m)-height},
      {0, (38 * m)-height},
      {-1, (39 * m)-height}
    }
  };

  GPathInfo BATT_TOP_INFO = {
    .num_points = 7,
    .points = (GPoint[7]) {
      {-1, -30},
      {-5, -38},
      {-2, -36},
      {0, -42},
      {2, -36},
      {5, -38},
      {1, -30}
    }
  };

  GPath *s_batt_centre = gpath_create(&BATT_CENTRE_INFO);
  GPath *s_batt_bar = gpath_create(&BATT_BAR_INFO);
  GPath *s_batt_top = gpath_create(&BATT_TOP_INFO);

  gpath_move_to(s_batt_centre, centre);
  gpath_move_to(s_batt_bar, centre);
  if (settings.LargeBrand) {
    gpath_move_to(s_batt_top, (GPoint){centre.x, centre.y/2+2});
  } else {
    gpath_move_to(s_batt_top, centre);
  }

  // Draw background
  if (settings.LargeBrand) {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH_LG);
  } else {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
  }
  graphics_context_set_stroke_color(ctx, settings.BrandColour);
  graphics_context_set_fill_color(ctx, settings.BrandColour);
  gpath_draw_outline(ctx, s_batt_centre);
  gpath_draw_filled(ctx, s_batt_top);
  graphics_context_set_stroke_width(ctx, 1);
  gpath_draw_outline_open(ctx, s_batt_top);

  if (!settings.BatteryOutline) {
    graphics_context_set_stroke_color(ctx, settings.BatteryColour);
  }
  graphics_context_set_fill_color(ctx, settings.BatteryColour);

  // Draw bar
  gpath_draw_filled(ctx, s_batt_bar);
  gpath_draw_outline(ctx, s_batt_bar);
  
  if (s_battery_level == 100) {
    gpath_draw_filled(ctx, s_batt_top);
    gpath_draw_outline_open(ctx, s_batt_top);
  }
  
  gpath_destroy(s_batt_centre);
  gpath_destroy(s_batt_bar);
  gpath_destroy(s_batt_top);
}

static void timer_callback(void *data) {
  //  layer_remove_from_parent((Layer *)s_text_layer_d);
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  //layer_insert_above_sibling((Layer *)s_text_layer_d, (Layer *)s_canvas_layer);
  timer = app_timer_register(3000, (AppTimerCallback)timer_callback, NULL);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Tap");
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer_hour[4];
  strftime(s_buffer_hour, sizeof(s_buffer_hour), clock_is_24h_style() ?
                                          "%H" : "%I", tick_time);
  static char s_buffer_mins[4];
  strftime(s_buffer_mins, sizeof(s_buffer_mins), "%M", tick_time);

  text_layer_set_text(s_hour_layer, s_buffer_hour);
  text_layer_set_text(s_mins_layer, s_buffer_mins);

  if (settings.ShowDate) {
    static char s_buffer_day[4];
    strftime(s_buffer_day, sizeof(s_buffer_day), "%d", tick_time);
    static char s_buffer_mth[4];
    strftime(s_buffer_mth, sizeof(s_buffer_mth), "%m", tick_time);

    text_layer_set_text(s_day_layer, s_buffer_day);
    text_layer_set_text(s_mth_layer, s_buffer_mth);
  }
}

void prv_window_update() {
  update_time();
  layer_mark_dirty(s_brand_layer);
  layer_mark_dirty(s_bt_layer);
}

void bluetooth_callback(bool connected) {
  if (s_bt_layer && s_brand_layer) {
    layer_set_hidden(s_bt_layer, connected);
    layer_set_hidden(s_brand_layer, !connected);
  }

  if (!connected) {
    vibes_double_pulse();
  }
}


static void bt_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  uint32_t m; // multiplier
  if (settings.LargeBrand) {
    m = 2;
  } else {
    m = 1;
  }

  GPathInfo BT_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-25 * m, -24 *m},
      {0, 0},
      {-25 * m, 20 * m},
      {0, 0},
      // Middle
      {0, 40 * m},
      // Right
      {25 * m, 20 * m},
      {0, 0},
      {25 * m, -24 * m},
      {14 * m, -36 * m},
    }
  };

  GPath *s_bt_path = gpath_create(&BT_PATH_INFO);

  gpath_move_to(s_bt_path, centre);
 
  graphics_context_set_stroke_color(ctx, settings.BluetoothColour);
  graphics_context_set_fill_color(ctx, settings.BluetoothColour);
  if (settings.LargeBrand) {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH_LG);
  } else {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
  }
 
  gpath_draw_outline_open(ctx, s_bt_path);
 
  gpath_destroy(s_bt_path);
}

static void brand_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  uint32_t m; // multiplier
  if (settings.LargeBrand) {
    m = 2;
  } else {
    m = 1;
  }

  GPathInfo BRAND_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-14 * m, -36 * m},
      {-20 * m, -24 *m},
      {0, 0},
      {-20 * m, 20 * m},
      // Middle
      {0, 40 * m},
      // Right
      {20 * m, 20 * m},
      {0, 0},
      {20 * m, -24 * m},
      {14 * m, -36 * m}
    }
  };

  GPath *s_brand_path = gpath_create(&BRAND_PATH_INFO);

  gpath_move_to(s_brand_path, centre);

  graphics_context_set_stroke_color(ctx, settings.BrandColour);
  graphics_context_set_fill_color(ctx, settings.BrandColour);
  if (settings.LargeBrand) {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH_LG);
  } else {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
  }
 
  gpath_draw_outline_open(ctx, s_brand_path);

  gpath_destroy(s_brand_path);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);
  window_set_background_color(s_window, settings.BackgroundColour);

  uint32_t TIME_PADDING = settings.FontStyle == OLDE ? OLDE_TIME_PADDING : NEUE_TIME_PADDING;
  //uint32_t DATE_PADDING = settings.FontStyle == OLDE ? OLDE_DATE_PADDING : NEUE_DATE_PADDING;
  uint32_t DATE_OFFSET = settings.FontStyle == OLDE ? OLDE_DATE_OFFSET : NEUE_DATE_OFFSET;

  if (settings.FontStyle == OLDE) {
    s_time_font = fonts_load_custom_font(resource_get_handle(OLDE_TIME_FONT));
    s_date_font = fonts_load_custom_font(resource_get_handle(OLDE_DATE_FONT)); 
  } else {
    s_time_font = fonts_load_custom_font(resource_get_handle(NEUE_TIME_FONT));
    s_date_font = fonts_load_custom_font(resource_get_handle(NEUE_DATE_FONT));
  }

  GRect leftTime = grect_inset(
    GRect(
      0,
      centre.y-(TIME_HEIGHT/4)-TIME_PADDING,
      bounds.size.w/2,
      TIME_HEIGHT
    ),
    (GEdgeInsets){
      .right=centre.x/3
  });

  GRect rightTime = grect_inset(
    GRect(
      centre.x,
      centre.y-(TIME_HEIGHT/4)-TIME_PADDING,
      bounds.size.w/2,
      TIME_HEIGHT
    ),
    (GEdgeInsets){
      .left=centre.x/3
  });

  s_hour_layer = text_layer_create(leftTime);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, settings.TimeColour);
  text_layer_set_font(s_hour_layer, s_time_font);
  layer_add_child(window_layer, text_layer_get_layer(s_hour_layer));

  s_mins_layer = text_layer_create(rightTime);
  text_layer_set_text_alignment(s_mins_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_mins_layer, GColorClear);
  text_layer_set_text_color(s_mins_layer, settings.TimeColour);
  text_layer_set_font(s_mins_layer, s_time_font);
  layer_add_child(window_layer, text_layer_get_layer(s_mins_layer));

  if (settings.ShowDate) {
    GRect leftDate = grect_inset(
      GRect(
        centre.x/2,
        settings.LargeBrand ? centre.y+(DATE_HEIGHT/2)+DATE_OFFSET : 0,
        bounds.size.w/4,
        DATE_HEIGHT
      ),
      (GEdgeInsets){
        .left=4,
        .right=2
    });

    GRect rightDate = grect_inset(
      GRect(
        centre.x,
        settings.LargeBrand ? centre.y+(DATE_HEIGHT/2)+DATE_OFFSET : 0,
        bounds.size.w/4,
        DATE_HEIGHT
      ),
      (GEdgeInsets){
        .left=2,
        .right=4
    });

    if (settings.DayMonthFormat) {
      s_day_layer = text_layer_create(leftDate);
      s_mth_layer = text_layer_create(rightDate);
    } else {
      s_mth_layer = text_layer_create(leftDate);
      s_day_layer = text_layer_create(rightDate);
    }

    text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_day_layer, GColorClear);
    text_layer_set_text_color(s_day_layer, settings.DateColour);
    text_layer_set_font(s_day_layer, s_date_font);
    layer_add_child(window_layer, text_layer_get_layer(s_day_layer));

    text_layer_set_text_alignment(s_mth_layer, GTextAlignmentCenter);
    text_layer_set_background_color(s_mth_layer, GColorClear);
    text_layer_set_text_color(s_mth_layer, settings.DateColour);
    text_layer_set_font(s_mth_layer, s_date_font);
    layer_add_child(window_layer, text_layer_get_layer(s_mth_layer));
  }

  // Bluetooth
  s_bt_layer = layer_create(bounds);
  layer_set_update_proc(s_bt_layer, bt_update_proc);
  layer_add_child(window_layer, s_bt_layer);
  layer_set_hidden(s_bt_layer, true);

  bluetooth_callback(connection_service_peek_pebble_app_connection());

  // Brand layer
  s_brand_layer = layer_create(bounds);
  layer_set_update_proc(s_brand_layer, brand_update_proc);
  layer_add_child(window_layer, s_brand_layer);

  // Battery
  s_battery_layer = layer_create(bounds);
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  layer_add_child(window_layer, s_battery_layer);

  prv_window_update();
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_mins_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_mth_layer);
  layer_destroy(s_brand_layer);
  layer_destroy(s_bt_layer);
  layer_destroy(s_battery_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  if (s_window) { window_destroy(s_window); }
}

void prv_window_push() {
  if (!s_window) {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
      .load = prv_window_load,
      .unload = prv_window_unload,
    });
  }
  window_stack_push(s_window, true);
}
