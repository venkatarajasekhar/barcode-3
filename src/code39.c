#include <pebble.h>

#include "barcode.h"

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

int charWidth = 13; // Each char is 13px wide. 6 narrow, 3 wide, 1 delimeter.

int drawCode39(char *word) {
	// TODO check the size of the word against the max width for the bitmap.
	int count = 0;
	char *buf = (char*)bmp->addr;
	memset(bmp->addr, 0xFF, bmp->bounds.size.h * bmp->row_size_bytes);
	buf = drawChar(buf, charLookup['*']);
	while (*word != '\0') {
		buf = drawChar(buf, charLookup[(int)*word]);
		word++;
		count++;
	}
	buf = drawChar(buf, charLookup['*']);
	return (count+2)*charWidth;
}
