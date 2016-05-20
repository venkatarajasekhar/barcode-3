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

static char* charLookup[] = {
	['0'] = "bwbWBwBwb",
	['1'] = "BwbWbwbwB",
	['2'] = "bwBWbwbwB",
	['3'] = "BwBWbwbwb",
	['4'] = "bwbWBwbwB",
	['5'] = "BwbWBwbwb",
	['6'] = "bwBWBwbwb",
	['7'] = "bwbWbwBwB",
	['8'] = "BwbWbwBwb",
	['9'] = "bwBWbwBwb",
	['A'] = "BwbwbWbwB",
	['B'] = "bwBwbWbwB",
	['C'] = "BwBwbWbwb",
	['D'] = "bwbwBWbwB",
	['E'] = "BwbwBWbwb",
	['F'] = "bwBwBWbwb",
	['G'] = "bwbwbWBwB",
	['H'] = "BwbwbWBwb",
	['I'] = "bwBwbWBwb",
	['J'] = "bwbwBWBwb",
	['K'] = "BwbwbwbWB",
	['L'] = "bwBwbwbWB",
	['M'] = "BwBwbwbWb",
	['N'] = "bwbwBwbWB",
	['O'] = "BwbwBwbWb",
	['P'] = "bwBwBwbWb",
	['Q'] = "bwbwbwBWB",
	['R'] = "BwbwbwBWb",
	['S'] = "bwBwbwBWb",
	['T'] = "bwbwBwBWb",
	['U'] = "BWbwbwbwB",
	['V'] = "bWBwbwbwB",
	['W'] = "BWBwbwbwb",
	['X'] = "bWbwBwbwB",
	['Y'] = "BWbwBwbwb",
	['Z'] = "bWBwBwbwb",
	['-'] = "bWbwbwBwB",
	['.'] = "BWbwbwBwb",
	['s'] = "bWBwbwBwb",
	['$'] = "bWbWbWbwb",
	['/'] = "bWbWbwbWb",
	['+'] = "bWbwbWbWb",
	['%'] = "bwbWbWbWb",
	['*'] = "bWbwBwBwb",
};

static char* drawBar(char *buf, char val) {
	for (int i = 0; i < bmp->row_size_bytes; i++) {
		buf[i] = val;
	}
	return buf + bmp->row_size_bytes;
}

static char* drawChar(char *buf, char *c) {
	for (int i = 0; i < 9; i++) {
		switch (c[i]) {
		case 'W':
			buf = drawBar(buf, 0xFF);
		case 'w':
			buf = drawBar(buf, 0xFF);
			break;
		case 'B':
			buf = drawBar(buf, 0x00);
		case 'b':
			buf = drawBar(buf, 0x00);
			break;
		}
	}
	// Narrow white delimeter.
	buf = drawBar(buf, 0xFF);
	return buf;
}

static int charWidth = 13; // Each char is 13px wide. 6 narrow, 3 wide, 1 delimeter.

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
