#include "main_window.h"

static Window *s_window;
static TextLayer *s_text_layer_h, *s_text_layer_m, *s_text_layer_d;
static GFont s_time_font, s_date_font;
static Layer *s_canvas_layer, *s_bt_layer;
static AppTimer *timer;

static void timer_callback(void *data) {
//  layer_remove_from_parent((Layer *)s_text_layer_d);
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
//  layer_insert_above_sibling((Layer *)s_text_layer_d, (Layer *)s_canvas_layer);
  timer = app_timer_register(3000, (AppTimerCallback)timer_callback, NULL);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tap");
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_buffer_hour[8];
  strftime(s_buffer_hour, sizeof(s_buffer_hour), clock_is_24h_style() ?
                                          "%H" : "%I", tick_time);
  static char s_buffer_mins[8];
  strftime(s_buffer_mins, sizeof(s_buffer_mins), "%M", tick_time);
  static char s_buffer_time[8];
  strftime(s_buffer_time, sizeof(s_buffer_time), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  static char s_buffer_date[8];
  strftime(s_buffer_date, sizeof(s_buffer_date), "%m %d", tick_time);
  
  text_layer_set_text(s_text_layer_h, s_buffer_hour);
  text_layer_set_text(s_text_layer_m, s_buffer_mins);

  if (settings.ShowDate) {
    text_layer_set_text(s_text_layer_d, s_buffer_date);
  }
}

void prv_window_update() {
  update_time();
  layer_mark_dirty(s_canvas_layer);
  layer_mark_dirty(s_bt_layer);
}

void bluetooth_callback(bool connected) {
  if (s_bt_layer && s_canvas_layer) {
    layer_set_hidden(s_bt_layer, connected);
    layer_set_hidden(s_canvas_layer, !connected);
  }

  if (!connected) {
    vibes_double_pulse();
  }
}


static void bt_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  uint32_t m; // multiplier
  if (settings.BrandOnly) {
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
      {16 * m, -32 * m}
    }
  };

  GPathInfo BT_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0, 40 * m},
      {0, -36 * m}
    }
  };

  GPathInfo BT_TOP_INFO = {
    .num_points = 7,
    .points = (GPoint[7]) {
      {-3, -32},
      {-8, -40},
      {-2, -36},
      {0, -46},
      {2, -36},
      {8, -40},
      {3, -32}
    }
  };

  GPath *s_bt_path = gpath_create(&BT_PATH_INFO);
  GPath *s_bt_centre = gpath_create(&BT_CENTRE_INFO);
  GPath *s_bt_top = gpath_create(&BT_TOP_INFO);

  gpath_move_to(s_bt_path, centre);
  gpath_move_to(s_bt_centre, centre);
  if (settings.BrandOnly) {
    gpath_move_to(s_bt_top, (GPoint){centre.x, centre.y/2+2});
  } else {
    gpath_move_to(s_bt_top, centre);
  }

  graphics_context_set_stroke_color(ctx, settings.BluetoothColour);
  graphics_context_set_fill_color(ctx, settings.BluetoothColour);
  if (settings.BrandOnly) {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH_LG);
  } else {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
  }
 
  gpath_draw_outline_open(ctx, s_bt_path);
  gpath_draw_outline_open(ctx, s_bt_centre);
  gpath_draw_filled(ctx, s_bt_top);

  gpath_destroy(s_bt_path);
  gpath_destroy(s_bt_centre);
  gpath_destroy(s_bt_top);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawn BT");
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  uint32_t m; // multiplier
  if (settings.BrandOnly) {
    m = 2;
  } else {
    m = 1;
  }

  GPathInfo BRAND_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-16 * m, -32 * m},
      {-25 * m, -24 *m},
      {0 * m, 0 * m},
      {-25 * m, 20 * m},
      // Middle
      {0 * m, 40 * m},
      // Right
      {25 * m, 20 * m},
      {0 * m, 0 * m},
      {25 * m, -24 * m},
      {16 * m, -32 * m}
    }
  };

  GPathInfo BRAND_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0 * m, 40 * m},
      {0 * m, -36 * m}
    }
  };

  GPathInfo BRAND_TOP_INFO = {
    .num_points = 7,
    .points = (GPoint[7]) {
      {-3, -32},
      {-8, -40},
      {-2, -36},
      {0, -46},
      {2, -36},
      {8, -40},
      {3, -32}
    }
  };

  GPath *s_brand_path = gpath_create(&BRAND_PATH_INFO);
  GPath *s_brand_centre = gpath_create(&BRAND_CENTRE_INFO);
  GPath *s_brand_top = gpath_create(&BRAND_TOP_INFO);

  gpath_move_to(s_brand_path, centre);
  gpath_move_to(s_brand_centre, centre);
  if (settings.BrandOnly) {
    gpath_move_to(s_brand_top, (GPoint){centre.x, centre.y/2+2});
  } else {
    gpath_move_to(s_brand_top, centre);
  } 

  graphics_context_set_stroke_color(ctx, settings.BrandColour);
  graphics_context_set_fill_color(ctx, settings.BrandColour);
  if (settings.BrandOnly) {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH_LG);
  } else {
    graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
  }
 
  gpath_draw_outline_open(ctx, s_brand_path);
  gpath_draw_outline(ctx, s_brand_centre);
  gpath_draw_filled(ctx, s_brand_top);

  gpath_destroy(s_brand_path);
  gpath_destroy(s_brand_centre);
  gpath_destroy(s_brand_top);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawn brand");
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);
  window_set_background_color(s_window, settings.BackgroundColour);

  s_time_font = fonts_load_custom_font(resource_get_handle(TIME_FONT));
  s_date_font = fonts_load_custom_font(resource_get_handle(DATE_FONT));

/*
  s_text_layer_hm = text_layer_create(grect_inset(GRect(0, 0, bounds.size.w, TIME_HEIGHT), (GEdgeInsets){ .top=0 }));
  text_layer_set_text_alignment(s_text_layer_hm, GTextAlignmentCenter);
  text_layer_set_background_color(s_text_layer_hm, GColorClear);
  text_layer_set_text_color(s_text_layer_hm, PBL_IF_COLOR_ELSE(GColorRed, GColorWhite));
  text_layer_set_font(s_text_layer_hm, s_time_font);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_hm));
*/

  s_text_layer_h = text_layer_create(grect_inset(GRect(0, centre.y-TIME_PADDING_TOP-(TIME_HEIGHT/4), bounds.size.w/2, TIME_HEIGHT), (GEdgeInsets){ .right=centre.x/3}));
  text_layer_set_text_alignment(s_text_layer_h, GTextAlignmentRight);
  text_layer_set_background_color(s_text_layer_h, GColorClear);
  text_layer_set_text_color(s_text_layer_h, settings.TimeColour);
  text_layer_set_font(s_text_layer_h, s_time_font);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_h));

  s_text_layer_m = text_layer_create(grect_inset(GRect(centre.x, centre.y-TIME_PADDING_TOP-(TIME_HEIGHT/4), bounds.size.w/2, TIME_HEIGHT), (GEdgeInsets){ .left=centre.x/3}));
  text_layer_set_text_alignment(s_text_layer_m, GTextAlignmentLeft);
  text_layer_set_background_color(s_text_layer_m, GColorClear);
  text_layer_set_text_color(s_text_layer_m, settings.TimeColour);
  text_layer_set_font(s_text_layer_m, s_time_font);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_m));

  if (settings.ShowDate) {
  s_text_layer_d = text_layer_create(grect_inset(GRect(0, centre.y+(DATE_HEIGHT/2), bounds.size.w, DATE_HEIGHT), (GEdgeInsets){ .top=0 }));
  text_layer_set_text_alignment(s_text_layer_d, GTextAlignmentCenter);
  text_layer_set_background_color(s_text_layer_d, GColorClear);
  text_layer_set_text_color(s_text_layer_d, settings.DateColour);
  text_layer_set_font(s_text_layer_d, s_date_font);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_d));
  }

  // Bluetooth
  s_bt_layer = layer_create(bounds);
  layer_set_update_proc(s_bt_layer, bt_update_proc);
  layer_add_child(window_layer, s_bt_layer);
  layer_set_hidden(s_bt_layer, true);

  bluetooth_callback(connection_service_peek_pebble_app_connection());

  // Canvas layer
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  prv_window_update();
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer_h);
  text_layer_destroy(s_text_layer_m);
  text_layer_destroy(s_text_layer_d);
  layer_destroy(s_canvas_layer);
  layer_destroy(s_bt_layer);
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
