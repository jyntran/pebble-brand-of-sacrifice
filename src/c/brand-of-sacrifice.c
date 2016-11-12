#include <pebble.h>
#include "windows/main_window.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  prv_window_update();
}

static void prv_init(void) {
  prv_load_settings();

  prv_window_push();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
