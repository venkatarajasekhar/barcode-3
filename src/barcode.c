#include <pebble.h>

Window *window;

BitmapLayer *barcodeLayer;
GBitmap *bmp;

typedef enum {w, W, b, B} bar;

bar* charLookup[] = {
	['0'] = (bar[]){b,w,b,W,B,w,B,w,b},
	['1'] = (bar[]){B,w,b,W,b,w,b,w,B},
	['2'] = (bar[]){b,w,B,W,b,w,b,w,B},
	['3'] = (bar[]){B,w,B,W,b,w,b,w,b},
	['4'] = (bar[]){b,w,b,W,B,w,b,w,B},
	['5'] = (bar[]){B,w,b,W,B,w,b,w,b},
	['6'] = (bar[]){b,w,B,W,B,w,b,w,b},
	['7'] = (bar[]){b,w,b,W,b,w,B,w,B},
	['8'] = (bar[]){B,w,b,W,b,w,B,w,b},
	['9'] = (bar[]){b,w,B,W,b,w,B,w,b},
	['A'] = (bar[]){B,w,b,w,b,W,b,w,B},
	['B'] = (bar[]){b,w,B,w,b,W,b,w,B},
	['C'] = (bar[]){B,w,B,w,b,W,b,w,b},
	['D'] = (bar[]){b,w,b,w,B,W,b,w,B},
	['E'] = (bar[]){B,w,b,w,B,W,b,w,b},
	['F'] = (bar[]){b,w,B,w,B,W,b,w,b},
	['G'] = (bar[]){b,w,b,w,b,W,B,w,B},
	['H'] = (bar[]){B,w,b,w,b,W,B,w,b},
	['I'] = (bar[]){b,w,B,w,b,W,B,w,b},
	['J'] = (bar[]){b,w,b,w,B,W,B,w,b},
	['K'] = (bar[]){B,w,b,w,b,w,b,W,B},
	['L'] = (bar[]){b,w,B,w,b,w,b,W,B},
	['M'] = (bar[]){B,w,B,w,b,w,b,W,b},
	['N'] = (bar[]){b,w,b,w,B,w,b,W,B},
	['O'] = (bar[]){B,w,b,w,B,w,b,W,b},
	['P'] = (bar[]){b,w,B,w,B,w,b,W,b},
	['Q'] = (bar[]){b,w,b,w,b,w,B,W,B},
	['R'] = (bar[]){B,w,b,w,b,w,B,W,b},
	['S'] = (bar[]){b,w,B,w,b,w,B,W,b},
	['T'] = (bar[]){b,w,b,w,B,w,B,W,b},
	['U'] = (bar[]){B,W,b,w,b,w,b,w,B},
	['V'] = (bar[]){b,W,B,w,b,w,b,w,B},
	['W'] = (bar[]){B,W,B,w,b,w,b,w,b},
	['X'] = (bar[]){b,W,b,w,B,w,b,w,B},
	['Y'] = (bar[]){B,W,b,w,B,w,b,w,b},
	['Z'] = (bar[]){b,W,B,w,B,w,b,w,b},
	['-'] = (bar[]){b,W,b,w,b,w,B,w,B},
	['.'] = (bar[]){B,W,b,w,b,w,B,w,b},
	['s'] = (bar[]){b,W,B,w,b,w,B,w,b},
	['$'] = (bar[]){b,W,b,W,b,W,b,w,b},
	['/'] = (bar[]){b,W,b,W,b,w,b,W,b},
	['+'] = (bar[]){b,W,b,w,b,W,b,W,b},
	['%'] = (bar[]){b,w,b,W,b,W,b,W,b},
	['*'] = (bar[]){b,W,b,w,B,w,B,w,b}
};

char* drawBar(char *buf, char val) {
	for (int i = 0; i < bmp->row_size_bytes; i++) {
		buf[i] = val;
	}
	return buf + bmp->row_size_bytes;
}

char* drawChar(char *buf, bar *c) {
	for (int i = 0; i < 9; i++) {
		switch (c[i]) {
		case W:
			buf = drawBar(buf, 0xFF);
		case w:
			buf = drawBar(buf, 0xFF);
			break;
		case B:
			buf = drawBar(buf, 0x00);
		case b:
			buf = drawBar(buf, 0x00);
			break;
		}
	}
	// Narrow white delimeter.
	buf = drawBar(buf, 0xFF);
	return buf;
}

int margin = 10;

void window_load(Window *window) {
	Layer *windowLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(windowLayer);
	bounds.origin.y += margin;
	bounds.size.h -= 2 * margin;
	barcodeLayer = bitmap_layer_create(bounds);

	bmp = gbitmap_create_blank(bounds.size);
	
	// Width in bytes, aligned to multiples of 4.
	bmp->row_size_bytes = (bounds.size.w/8+3) & ~3;
	bmp->addr = malloc(bounds.size.h * bmp->row_size_bytes);
	memset(bmp->addr, 0xFF, bounds.size.h * bmp->row_size_bytes);
	
	char *buf = (char*)bmp->addr;

	buf = drawChar(buf, charLookup['*']);
	buf = drawChar(buf, charLookup['A']);
	buf = drawChar(buf, charLookup['*']);

	bitmap_layer_set_bitmap(barcodeLayer, bmp);
	layer_add_child(windowLayer, bitmap_layer_get_layer(barcodeLayer));
}

int main(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
	});
	window_set_background_color(window, GColorWhite);
	window_set_fullscreen(window, true);
	window_stack_push(window, true);
	
	app_event_loop();
}
