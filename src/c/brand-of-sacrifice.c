#include <pebble.h>
#include "windows/main_window.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  prv_window_update();
}

static void prv_unobstructed_change(AnimationProgress progress, void *context) {
  prv_window_update();
}

static void prv_init(void) {
  prv_load_settings();
  prv_window_push();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  battery_state_service_subscribe(battery_callback);
  battery_callback(battery_state_service_peek());

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  UnobstructedAreaHandlers handlers = {
    .change = prv_unobstructed_change
  };
  unobstructed_area_service_subscribe(handlers, NULL);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  unobstructed_area_service_unsubscribe();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
