#include <pebble.h>
#include <string.h>

static Window *s_window;
static TextLayer *s_text_layer_h, *s_text_layer_m;

static Layer *s_canvas_layer, *s_bt_layer;


static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_buffer_hour[8];
  strftime(s_buffer_hour, sizeof(s_buffer_hour), clock_is_24h_style() ?
                                          "%H" : "%I", tick_time);
  static char s_buffer_mins[8];
  strftime(s_buffer_mins, sizeof(s_buffer_mins), "%M", tick_time);
  
  text_layer_set_text(s_text_layer_h, s_buffer_hour);
  text_layer_set_text(s_text_layer_m, s_buffer_mins);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void bluetooth_callback(bool connected) {
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

  GPathInfo BT_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-20, -24},
      {0, 0},
      {-20, 20},
      {0, 0},
      // Middle
      {0, 40},
      // Right
      {20, 20},
      {0, 0},
      {20, -24},
      {15, -40}
    }
  };

  GPathInfo BT_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0, 40},
      {0, -40}
    }
  };

  GPathInfo BT_TOP_INFO = {
    .num_points = 7,
    .points = (GPoint[7]) {
      {-4, -36},
      {-8, -44},
      {-3, -40},
      {0, -54},
      {3, -40},
      {8, -44},
      {4, -36}
    }
  };

  GPath *s_bt_path = gpath_create(&BT_PATH_INFO);
  GPath *s_bt_centre = gpath_create(&BT_CENTRE_INFO);
  GPath *s_bt_top = gpath_create(&BT_TOP_INFO);

  gpath_move_to(s_bt_path, centre);
  gpath_move_to(s_bt_centre, centre);
  gpath_move_to(s_bt_top, centre);

  graphics_context_set_stroke_color(ctx, GColorRed);
  graphics_context_set_fill_color(ctx, GColorRed);
  graphics_context_set_stroke_width(ctx, 4);

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

  GPathInfo BRAND_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-15, -40},
      {-20, -24},
      {0, 0},
      {-20, 20},
      // Middle
      {0, 40},
      // Right
      {20, 20},
      {0, 0},
      {20, -24},
      {15, -40}
    }
  };

  GPathInfo BRAND_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0, 40},
      {0, -40}
    }
  };

  GPathInfo BRAND_TOP_INFO = {
    .num_points = 7,
    .points = (GPoint[7]) {
      {-4, -36},
      {-8, -44},
      {-3, -40},
      {0, -54},
      {3, -40},
      {8, -44},
      {4, -36}
    }
  };

  GPath *s_brand_path = gpath_create(&BRAND_PATH_INFO);
  GPath *s_brand_centre = gpath_create(&BRAND_CENTRE_INFO);
  GPath *s_brand_top = gpath_create(&BRAND_TOP_INFO);

  gpath_move_to(s_brand_path, centre);
  gpath_move_to(s_brand_centre, centre);
  gpath_move_to(s_brand_top, centre);

  graphics_context_set_stroke_color(ctx, GColorRed);
  graphics_context_set_fill_color(ctx, GColorRed);
  graphics_context_set_stroke_width(ctx, 4);

  gpath_draw_outline_open(ctx, s_brand_path);
  gpath_draw_outline_open(ctx, s_brand_centre);
  gpath_draw_filled(ctx, s_brand_top);

  gpath_destroy(s_brand_path);
  gpath_destroy(s_brand_centre);
  gpath_destroy(s_brand_top);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawn brand");
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorBlack);

  s_text_layer_h = text_layer_create(grect_inset(GRect(0, bounds.size.h/2 - 20, bounds.size.w, 28), (GEdgeInsets){ .left=8, .right=8}));
  text_layer_set_background_color(s_text_layer_h, GColorClear);
  text_layer_set_text_color(s_text_layer_h, GColorRed);
  text_layer_set_font(s_text_layer_h, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_text_layer_h, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_h));

  s_text_layer_m = text_layer_create(grect_inset(GRect(0, bounds.size.h/2 - 20, bounds.size.w, 28), (GEdgeInsets){ .left=8, .right=8}));
  text_layer_set_background_color(s_text_layer_m, GColorClear);
  text_layer_set_text_color(s_text_layer_m, GColorRed);
  text_layer_set_font(s_text_layer_m, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_text_layer_m, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_m));

  // Bluetooth
  s_bt_layer = layer_create(bounds);
  layer_set_update_proc(s_bt_layer, bt_update_proc);
  layer_add_child(window_layer, s_bt_layer);

  bluetooth_callback(connection_service_peek_pebble_app_connection());

  // Canvas layer
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer_h);
  text_layer_destroy(s_text_layer_m);
  layer_destroy(s_canvas_layer);
  layer_destroy(s_bt_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
