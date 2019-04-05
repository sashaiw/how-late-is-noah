#include <pebble.h>
#define NOAH_TIME 438 // seconds in Noah's commute

static Window *s_window;
static TextLayer *s_text_layer;

static void update_text() {
  static char s_noah_text[] = "Noah is 00m00s early for class.";

  // now in epoch
  time_t now_e = time(NULL);
  
  // nearest 1h alignment in epoch
  time_t nh_e = time(NULL);
  nh_e -= (nh_e % 3600) - 3600;
  
  // calculate lateness in seconds
  int l8_sec = nh_e - now_e - NOAH_TIME / 2;

  // format output
  if (l8_sec > 0) {
    int l8_min = l8_sec / 60;
    l8_sec %= 60;
    snprintf(s_noah_text, sizeof(s_noah_text), "Noah is %um%us early for class.", l8_min, l8_sec);
  } else {
    int l8_min = -l8_sec / 60;
    l8_sec %= 60;
    snprintf(s_noah_text, sizeof(s_noah_text), "Noah is %um%us early for class.", l8_min, l8_sec);
  }


  text_layer_set_text(s_text_layer, s_noah_text);
}

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  update_text();
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(10, 72, bounds.size.w - 20, 40));
  text_layer_set_text(s_text_layer, "AWAITING NOAH");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
