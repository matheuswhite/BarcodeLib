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

#ifndef MWBarcode
#define MWBarcode

#include <stdint.h>

//Public defines. Change without restrictions
#define WHITE 0
#define BLACK 1
#define BAR_WIDTH  1
#define BAR_HEIGHT 38

//Internal defines. Don't change!
#define LEFT_QUIET_ZONE_LENGTH  11
#define RIGHT_QUIET_ZONE_LENGTH 7
#define CODE_LENGTH    13

//typedefs
typedef void (*MWBarcodeSetPixel)(uint16_t x, uint16_t y, uint8_t color);

//utils functions
void MWBarcodeString2Code(uint8_t code[CODE_LENGTH], const char str_code[CODE_LENGTH]);
void MWBarcodeCode2String(char str_code[CODE_LENGTH], const uint8_t code[CODE_LENGTH]);
void MWBarcodeRawcode2Code(uint8_t code[CODE_LENGTH], uint64_t rawcode);
void MWBarcodeRawcode2String(char str_code[CODE_LENGTH], uint64_t rawcode);

//main functions
void MWBarcodeInit(MWBarcodeSetPixel setPixelFunction);
void MWBarcodeDraw(uint16_t x, uint16_t y, uint8_t code[CODE_LENGTH]);

#endif //MWBarcode
