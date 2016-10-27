#include <pebble.h>
#include <string.h>

static Window *s_window;
static TextLayer *s_text_layer_h, *s_text_layer_m;

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

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

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
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer_h);
  text_layer_destroy(s_text_layer_m);
}

static void prv_init(void) {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
  window_set_background_color(s_window, GColorBlack);
  update_time();
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
