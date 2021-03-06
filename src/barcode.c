/*
Copyright 2016 Salman Aljammaz

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <pebble.h>

#include "barcode.h"

Window *window;
BitmapLayer *barcode;
TextLayer *label;

int currentBarcode = 0;
int nBarcodes = 2;
typedef struct Barcode {
	char* label;
	char* code;
	Symbology sym;
} Barcode;

extern Barcode barcodes[];

void display() {
	switch (barcodes[currentBarcode].sym) {
	case CODE39:
		bmp->bounds.size.h = drawCode39(barcodes[currentBarcode].code);
		break;
	case CODE128:
		bmp->bounds.size.h = drawCode128(barcodes[currentBarcode].code);
		break;
	}
	bitmap_layer_set_bitmap(barcode, bmp);
	
	text_layer_set_text(label, barcodes[currentBarcode].label);
}

int margin = 4;

void window_load(Window *window) {
	Layer *windowLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(windowLayer);
	bounds.origin.y += margin;
	bounds.size.h -= 2 * margin;
	barcode = bitmap_layer_create(bounds);
	
	bitmap_layer_set_alignment(barcode, GAlignCenter);
	layer_add_child(windowLayer, bitmap_layer_get_layer(barcode));

	bmp = gbitmap_create_blank(bounds.size);
	
	// Width in bytes, aligned to multiples of 4.
	bmp->row_size_bytes = (bounds.size.w/8+3) & ~3;
	bmp->addr = malloc(bounds.size.h * bmp->row_size_bytes);
	
	label = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 16 } });
	text_layer_set_text(label, "Barcodes!");
	text_layer_set_text_alignment(label, GTextAlignmentCenter);
	layer_add_child(windowLayer, text_layer_get_layer(label));
	
	display();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	currentBarcode = (currentBarcode+1) % nBarcodes;
	display();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	currentBarcode--;
	if (currentBarcode < 0) {
		currentBarcode = nBarcodes-1;
	}
	display();
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

int main(void) {
	window = window_create();
	window_set_click_config_provider(window, click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
	});
	window_set_background_color(window, GColorWhite);
	window_set_fullscreen(window, true);
	window_stack_push(window, true);
	
	app_event_loop();
}
