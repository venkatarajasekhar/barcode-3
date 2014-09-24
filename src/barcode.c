#include <pebble.h>

Window *window;

BitmapLayer *barcodeLayer;
GBitmap *bmp;

void setLine(int i, char val) {
	char *buf = (char*)bmp->addr;
	for (int j = 0; j < bmp->row_size_bytes; j++) {
		buf[i*bmp->row_size_bytes + j] = val;
	}
}

void window_load(Window *window) {
	Layer *windowLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(windowLayer);
	barcodeLayer = bitmap_layer_create(bounds);

	bmp = gbitmap_create_blank(bounds.size);
	bmp->bounds = bounds;
	// Width in bytes, aligned to multiples of 4.
	bmp->row_size_bytes = (bounds.size.w/8+3) & ~3;
	bmp->addr = malloc(bounds.size.h * bmp->row_size_bytes);
	
	char *buf = (char*)bmp->addr;
	
	for (int i = 0; i < bounds.size.h; i+=2) {
		setLine(i, 0xFF);
	}

	bitmap_layer_set_bitmap(barcodeLayer, bmp);
	layer_add_child(windowLayer, bitmap_layer_get_layer(barcodeLayer));
}

int main(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
	});
	window_stack_push(window, true);
	
	app_event_loop();
}
