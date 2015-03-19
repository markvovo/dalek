#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static BitmapLayer *s_image_layer;
static GBitmap *s_image_bitmap;

char location_buffer[64], temperature_buffer[32], time_buffer[32];

enum {
	KEY_LOCATION = 0,
	KEY_TEMPERATURE = 1,
};

void send_int(uint8_t key, uint8_t cmd)
{
	DictionaryIterator *iter;
 	app_message_outbox_begin(&iter);
 	
 	Tuplet value = TupletInteger(key, cmd);
 	dict_write_tuplet(iter, &value);
 	
 	app_message_outbox_send();
}

void process_tuple(Tuple *t)
{
	//Get key
	int key = t->key;

	//Get integer value, if present
	int value = t->value->int32;

	//Get string value, if present
	char string_value[32];
	strcpy(string_value, t->value->cstring);

	//Decide what to do
	switch(key) {
		case KEY_LOCATION:
			//Location received
			//snprintf(location_buffer, sizeof("Location: couldbereallylongname"), "Location: %s", string_value);
			//text_layer_set_text(location_layer, (char*) &location_buffer);
			break;
		case KEY_TEMPERATURE:
			//Temperature received
			//snprintf(temperature_buffer, sizeof("Temperature: XX \u00B0C"), "Temperature: %d \u00B0C", value);
			//text_layer_set_text(temperature_layer, (char*) &temperature_buffer);
			break;
	}

	//Set time this update came in
	time_t temp = time(NULL);	
	struct tm *tm = localtime(&temp);
	//strftime(time_buffer, sizeof("Last updated: XX:XX"), "Last updated: %H:%M", tm);
	//text_layer_set_text(time_layer, (char*) &time_buffer);
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Exterminate");
	//call hhtp post to play sound one
	send_int(1, 1);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Destroy");
	//call hhtp post to play sound two
	send_int(1, 2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "You may proceed");
	//call hhtp post to play sound three
	send_int(1, 3);
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

static void in_received_handler(DictionaryIterator *iter, void *context) 
{
	(void) context;
	
	//Get data
	Tuple *t = dict_read_first(iter);
	while(t != NULL)
	{
		process_tuple(t);
		
		//Get next
		t = dict_read_next(iter);
	}
}


static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
    //Register AppMessage events
	app_message_register_inbox_received(in_received_handler);					 
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());		//Largest possible input and output buffer sizes

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