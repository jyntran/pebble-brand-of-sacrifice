#include "main_window.h"

static Window *s_window;
static TextLayer *s_hour_layer,
                 *s_mins_layer;
static Layer *s_window_layer,
             *s_time_layer,
             *s_brand_layer,
             *s_bt_layer,
             *s_battery_layer;
static GFont s_time_font;

static int s_battery_level;

uint32_t TIME_AREA,
         TIME_HEIGHT,
         TIME_PADDING;


void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  layer_mark_dirty(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  int height = (s_battery_level * 78) / 100;

  GPathInfo BATT_CENTRE_INFO = {
    .num_points = 2,
    .points = (GPoint[2]) {
      {0, 38},
      {0, -36}
    }
  };

  GPathInfo BATT_BAR_INFO = {
    .num_points = 6,
    .points = (GPoint[6]) {
      {-1, 38},
      {0, 39},
      {1, 38},
      {1, 39-height},
      {0, 38-height},
      {-1, 39-height}
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
  gpath_move_to(s_batt_top, centre);

  // Draw background
  graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
 
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
}

static void time_layout(GRect bounds) {
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  GRect leftTime = grect_inset(
    GRect(
      0,
      centre.y-(TIME_HEIGHT/2)-TIME_PADDING,
      bounds.size.w/2,
      TIME_AREA
    ),
    (GEdgeInsets){
      .right=centre.x/4
  });

  GRect rightTime = grect_inset(
    GRect(
      centre.x,
      centre.y-(TIME_HEIGHT/2)-TIME_PADDING,
      bounds.size.w/2,
      TIME_AREA
    ),
    (GEdgeInsets){
      .left=centre.x/4
  });

  layer_set_frame(text_layer_get_layer(s_hour_layer), leftTime);
  layer_set_frame(text_layer_get_layer(s_mins_layer), rightTime);

  layer_set_frame(s_time_layer, bounds);

  update_time();
}

void prv_window_update() {
  GRect bounds = layer_get_bounds(s_window_layer);
  GRect unobstructed_bounds = layer_get_unobstructed_bounds(s_window_layer);
  int16_t obstruction_height = bounds.size.h - unobstructed_bounds.size.h;

  if (obstruction_height == 0) {
    time_layout(bounds);
  } else {
    time_layout(unobstructed_bounds);
  }
}

void bluetooth_callback(bool connected) {
  if (s_bt_layer && s_brand_layer) {
    layer_set_hidden(s_bt_layer, connected);
    layer_set_hidden(s_brand_layer, !connected);
  }

  if (settings.BluetoothVibration && !connected) {
    vibes_double_pulse();
  }
}

static void bt_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
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
      {14, -36},
    }
  };

  GPath *s_bt_path = gpath_create(&BT_PATH_INFO);

  gpath_move_to(s_bt_path, centre);
 
  graphics_context_set_stroke_color(ctx, settings.BluetoothColour);
  graphics_context_set_fill_color(ctx, settings.BluetoothColour);
  graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
 
  gpath_draw_outline_open(ctx, s_bt_path);
 
  gpath_destroy(s_bt_path);
}

static void brand_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint centre = GPoint(bounds.size.w/2, bounds.size.h/2);

  GPathInfo BRAND_PATH_INFO = {
    .num_points = 9,
    .points = (GPoint[9]) {
      // Left
      {-14, -36},
      {-20, -24},
      {0, 0},
      {-20, 20},
      // Middle
      {0, 40},
      // Right
      {20, 20},
      {0, 0},
      {20, -24},
      {14, -36}
    }
  };

  GPath *s_brand_path = gpath_create(&BRAND_PATH_INFO);

  gpath_move_to(s_brand_path, centre);

  graphics_context_set_stroke_color(ctx, settings.BrandColour);
  graphics_context_set_fill_color(ctx, settings.BrandColour);
  graphics_context_set_stroke_width(ctx, BRAND_STROKE_WIDTH);
 
  gpath_draw_outline_open(ctx, s_brand_path);

  gpath_destroy(s_brand_path);
}

static void prv_window_load(Window *window) {
  window_set_background_color(s_window, settings.BackgroundColour);

  s_window_layer = window_get_root_layer(window);
  GRect full_bounds = layer_get_bounds(s_window_layer);
  GRect bounds = layer_get_unobstructed_bounds(s_window_layer);

  s_time_layer = layer_create(bounds);
  layer_add_child(s_window_layer, s_time_layer);

  switch (settings.FontStyle) {
    case OE:
      s_time_font = fonts_load_custom_font(resource_get_handle(OE_TIME_FONT));
      TIME_AREA = OE_TIME_AREA;
      TIME_PADDING = OE_TIME_PADDING;
      break;
    case BF:
      s_time_font = fonts_load_custom_font(resource_get_handle(BF_TIME_FONT));
      TIME_AREA = BF_TIME_AREA;
      TIME_PADDING = BF_TIME_PADDING;
      break;
    case NR:
    default:
      s_time_font = fonts_load_custom_font(resource_get_handle(NR_TIME_FONT));
      TIME_AREA = NR_TIME_AREA;
      TIME_PADDING = NR_TIME_PADDING;
  }
  
  TIME_HEIGHT = TIME_AREA - TIME_PADDING;

  // Time layer
  s_hour_layer = text_layer_create(bounds);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, settings.TimeColour);
  text_layer_set_font(s_hour_layer, s_time_font);
  layer_add_child(s_time_layer, text_layer_get_layer(s_hour_layer));

  s_mins_layer = text_layer_create(bounds);
  text_layer_set_text_alignment(s_mins_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_mins_layer, GColorClear);
  text_layer_set_text_color(s_mins_layer, settings.TimeColour);
  text_layer_set_font(s_mins_layer, s_time_font);
  layer_add_child(s_time_layer, text_layer_get_layer(s_mins_layer));

  time_layout(bounds);

  // Bluetooth
  s_bt_layer = layer_create(full_bounds);
  layer_set_update_proc(s_bt_layer, bt_update_proc);
  layer_add_child(s_window_layer, s_bt_layer);
  layer_set_hidden(s_bt_layer, true);

  bluetooth_callback(connection_service_peek_pebble_app_connection());

  // Brand layer
  s_brand_layer = layer_create(full_bounds);
  layer_set_update_proc(s_brand_layer, brand_update_proc);
  layer_add_child(s_window_layer, s_brand_layer);

  // Battery
  s_battery_layer = layer_create(full_bounds);
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  layer_add_child(s_window_layer, s_battery_layer);

  prv_window_update();
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_mins_layer);
  layer_destroy(s_brand_layer);
  layer_destroy(s_bt_layer);
  layer_destroy(s_battery_layer);
  layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
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
