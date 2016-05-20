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

char* charLookup[] = {
	"11011001100",
	"11001101100",
	"11001100110",
	"10010011000",
	"10010001100",
	"10001001100",
	"10011001000",
	"10011000100",
	"10001100100",
	"11001001000",
	"11001000100",
	"11000100100",
	"10110011100",
	"10011011100",
	"10011001110",
	"10111001100",
	"10011101100",
	"10011100110",
	"11001110010",
	"11001011100",
	"11001001110",
	"11011100100",
	"11001110100",
	"11101101110",
	"11101001100",
	"11100101100",
	"11100100110",
	"11101100100",
	"11100110100",
	"11100110010",
	"11011011000",
	"11011000110",
	"11000110110",
	"10100011000",
	"10001011000",
	"10001000110",
	"10110001000",
	"10001101000",
	"10001100010",
	"11010001000",
	"11000101000",
	"11000100010",
	"10110111000",
	"10110001110",
	"10001101110",
	"10111011000",
	"10111000110",
	"10001110110",
	"11101110110",
	"11010001110",
	"11000101110",
	"11011101000",
	"11011100010",
	"11011101110",
	"11101011000",
	"11101000110",
	"11100010110",
	"11101101000",
	"11101100010",
	"11100011010",
	"11101111010",
	"11001000010",
	"11110001010",
	"10100110000",
	"10100001100",
	"10010110000",
	"10010000110",
	"10000101100",
	"10000100110",
	"10110010000",
	"10110000100",
	"10011010000",
	"10011000010",
	"10000110100",
	"10000110010",
	"11000010010",
	"11001010000",
	"11110111010",
	"11000010100",
	"10001111010",
	"10100111100",
	"10010111100",
	"10010011110",
	"10111100100",
	"10011110100",
	"10011110010",
	"11110100100",
	"11110010100",
	"11110010010",
	"11011011110",
	"11011110110",
	"11110110110",
	"10101111000",
	"10100011110",
	"10001011110",
};

static char* startA = "11010000100";
static char* stop = "1100011101011";

static char* drawBar(char *buf, char val) {
	for (int i = 0; i < bmp->row_size_bytes; i++) {
		buf[i] = val;
	}
	return buf + bmp->row_size_bytes;
}

static char* drawChar(char *buf, char *c) {
	while (*c != '\0') {
		switch (*c) {
		case '0':
			buf = drawBar(buf, 0xFF);
			break;
		case '1':
			buf = drawBar(buf, 0x00);
			break;
		}
		c++;
	}
	return buf;
}

static int charWidth = 11;

int drawCode128(char *c) {
	int count = 0, sum = 103;
	char *buf = (char*)bmp->addr;
	memset(bmp->addr, 0xFF, bmp->bounds.size.h * bmp->row_size_bytes);
	
	buf = drawChar(buf, startA);
	while (*c != '\0') {
		buf = drawChar(buf, charLookup[*c-32]);
		count++;
		sum += count * (*c-32);
		c++;
	}
	sum %= 103;
	buf = drawChar(buf, charLookup[sum]);
	buf = drawChar(buf, stop);
	
	return (count+3)*charWidth +1;
}
