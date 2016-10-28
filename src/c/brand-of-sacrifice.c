#include <pebble.h>
#include <string.h>

static Window *s_window;
static TextLayer *s_text_layer_h, *s_text_layer_m;
static BitmapLayer *s_background_layer, *s_bt_layer;
static GBitmap *s_bitmap_brand, *s_bitmap_bt;

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
  layer_set_hidden(bitmap_layer_get_layer(s_bt_layer), connected);

  if (!connected) {
    vibes_double_pulse();
  }
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Canvas drawing goes here
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_bitmap_brand = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BRAND_70);
  s_background_layer = bitmap_layer_create(
      GRect(0, 0, bounds.size.w, bounds.size.h));
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_background_layer, s_bitmap_brand);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  s_text_layer_h = text_layer_create(GRect(0, 72, bounds.size.w, 28));
  text_layer_set_background_color(s_text_layer_h, GColorClear);
  text_layer_set_text_color(s_text_layer_h, GColorRed);
  text_layer_set_font(s_text_layer_h, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_text_layer_h, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_h));

  s_text_layer_m = text_layer_create(GRect(0, 72, bounds.size.w, 28));
  text_layer_set_background_color(s_text_layer_m, GColorClear);
  text_layer_set_text_color(s_text_layer_m, GColorRed);
  text_layer_set_font(s_text_layer_m, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_text_layer_m, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer_m));

  // Bluetooth
  s_bitmap_bt = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_120);
  s_bt_layer = bitmap_layer_create(
      GRect(0, 0, bounds.size.w, bounds.size.h));
  bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bt_layer, s_bitmap_bt);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bt_layer));

  update_time();

  bluetooth_callback(connection_service_peek_pebble_app_connection());

/*
  // Canvas layer
  static Layer *s_canvas_layer;
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);

  layer_mark_dirty(s_canvas_layer);
*/
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer_h);
  text_layer_destroy(s_text_layer_m);
  gbitmap_destroy(s_bitmap_brand);
  gbitmap_destroy(s_bitmap_bt);
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_bt_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
  window_set_background_color(s_window, GColorBlack);

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
