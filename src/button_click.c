#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Exterminate");
	//call hhtp post to play sound one
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Destroy");
	//call hhtp post to play sound two
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "You may proceed");
	//call hhtp post to play sound three
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
 GRect bounds = layer_get_bounds(window_layer);
  s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DALEK_BACKGROUND);

  s_image_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_image_layer, s_image_bitmap);
  bitmap_layer_set_alignment(s_image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_image_layer));
	
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  //text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { 70, 120 } });
  text_layer_set_text(text_layer, "Dalek Control");
  //text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  bitmap_layer_destroy(s_image_layer);
  gbitmap_destroy(s_image_bitmap);

  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}