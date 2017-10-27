/*
 * Author - Matheus Tenório dos Santos
 * Date   - 27/10/2017
 * Email  - tenoriomatheus0@gmail.com
 */

/*
 * MIT License
 *
 * Copyright (c) 2017 Matheus Tenorio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "MWBarcode.h"

static MWBarcodeSetPixel __setPixelFunction;
//1 = L | 0 = G
static const uint8_t __firstGroup[10] = {0b111111, 0b110100, 0b110010, 0b110001, 0b101100,
										 0b100110, 0b100011, 0b101010, 0b101001, 0b100101};
static const uint8_t __lCode[10] = {0b0001101, 0b0011001, 0b0010011, 0b0111101, 0b0100011,
									0b0110001, 0b0101111, 0b0111011, 0b0110111, 0b0001011};

//utils functions
void MWBarcodeString2Code(uint8_t code[CODE_LENGTH], const char str_code[CODE_LENGTH]) {
	for (uint8_t i = 0; i < CODE_LENGTH; ++i) {
		code[i] = str_code[i] - '0';
	}
}

void MWBarcodeCode2String(char str_code[CODE_LENGTH], const uint8_t code[CODE_LENGTH]) {
	for (uint8_t i = 0; i < CODE_LENGTH; ++i) {
		str_code[i] = code[i] + '0';
	}
}

void MWBarcodeRawcode2Code(uint8_t code[CODE_LENGTH], uint64_t rawcode) {
	uint64_t quotient  = 0;
	uint64_t remainder = rawcode;
	uint64_t divisor = 1e13;

	for (unt8_t i = 1; i <= CODE_LENGTH; ++i) {
		quotient = remainder / divisor;
		remainder = rawcode % divisor;
		code[CODE_LENGTH-i] = quotient;
		quotient = remainder;
		divisor /= 10;
	}
}

void MWBarcodeRawcode2String(char str_code[CODE_LENGTH], uint64_t rawcode) {
	uint64_t quotient  = 0;
	uint64_t remainder = rawcode;
	uint64_t divisor = 1e13;

	for (unt8_t i = 1; i <= CODE_LENGTH; ++i) {
		quotient = remainder / divisor;
		remainder = rawcode % divisor;
		str_code[CODE_LENGTH-i] = quotient + '0';
		quotient = remainder;
		divisor /= 10;
	}
}

//private functions
void MWBarcodeDrawBar(uint16_t *x, uint16_t y, uint8_t color) {
	for (uint8_t i = 0; i < BARCODE_WIDTH; ++i) {
		for (uint8_t j = 0; j < BAR_HEIGHT; ++j) {
			__setPixelFunction((*x)+i, y+j, color);
		}
	}
	*x += BAR_WIDTH;
}

uint8_t MWBarcodeGetGCode(uint8_t lCode) {
	lCode = (lCode & 0xF0) >> 4 | (lCode & 0x0F) << 4;
	lCode = (lCode & 0xCC) >> 2 | (lCode & 0x33) << 2;
	lCode = (lCode & 0xAA) >> 1 | (lCode & 0x55) << 1;
	lCode = lCode >> 1;
	lCode = ~lCode;

	return lCode;
}

//main functions
void MWBarcodeInit(MWBarcodeSetPixel setPixelFunction) {
	__setPixelFunction = setPixelFunction;
}

void MWBarcodeDraw(uint16_t x, uint16_t y, uint8_t code[CODE_LENGTH]) {
	uint8_t firstDigit = code[CODE_LENGTH];
	uint8_t mask;
	uint8_t mask2;
	uint8_t digit;

	//write left quite zone
	for (uint8_t i = 0; i < LEFT_QUIET_ZONE_LENGTH; ++i) {
		MWBarcodeDrawBar(&x, y, WHITE);
	}

	//write left mark
	MWBarcodeDrawBar(&x, y, BLACK);
	MWBarcodeDrawBar(&x, y, WHITE);
	MWBarcodeDrawBar(&x, y, BLACK);

	//write first group
	mask2 = 0b00100000;
	for (int8_t i = 11; i >= 6; --i, mask2 >>= 1) {
		digit = code[i];
		mask = 0b01000000;
		for (uint8_t j = 0; j < 7; ++j, mask >>= 1) {
			if ((__firstGroup[firstDigit] & mask2) != 0) {
				//L
				MWBarcodeDrawBar(&x, y, (mask & __lCode[digit]) == 0 ? WHITE : BLACK);
			}
			else {
				//G
				MWBarcodeDrawBar(&x, y, (mask & MWBarcodeGetGCode(__lCode[digit])) == 0 ? WHITE : BLACK);
			}
		}
	}

	//write center mark
	MWBarcodeDrawBar(&x, y, WHITE);
	MWBarcodeDrawBar(&x, y, BLACK);
	MWBarcodeDrawBar(&x, y, WHITE);
	MWBarcodeDrawBar(&x, y, BLACK);
	MWBarcodeDrawBar(&x, y, WHITE);

	//write second group
	for (int8_t i = 5; i >= 0; --i) {
		digit = code[i];
		mask = 0b01000000;
		for (uint8_t j = 0; j < 7; ++j, mask >>= 1) {
			MWBarcodeDrawBar(&x, y, (mask & ~__lCode[digit]) == 0 ? WHITE : BLACK);
		}
	}

	//write right mark
	MWBarcodeDrawBar(&x, y, BLACK);
	MWBarcodeDrawBar(&x, y, WHITE);
	MWBarcodeDrawBar(&x, y, BLACK);

	//write right quite zone
	for (uint8_t i = 0; i < RIGHT_QUIET_ZONE_LENGTH; ++i) {
		MWBarcodeDrawBar(&x, y, WHITE);
	}
}
