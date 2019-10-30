#include "tester.h"
#include <avr/pgmspace.h>

char str_hello[] PROGMEM = "Virtualtap tester v0.1";
char str_instructions[] PROGMEM = "Connect PCB and press any key";
char str_bitmap[] PROGMEM = "         76543210 FEDCBA98 S2HLABCV";
char str_pass[] PROGMEM = "PASS";
char str_fail[] PROGMEM = "FAIL";
char str_expected[] PROGMEM = "Expected ";
char str_got[] PROGMEM = "     Got ";
char str_conn_check[] PROGMEM = "Connectivity loop check";
char str_all_ones[] PROGMEM = "All ones: ";
char str_all_zeros[] PROGMEM = "All zeros: ";
char str_alt_1[] PROGMEM = "Alt. 1: ";
char str_alt_2[] PROGMEM = "Alt. 2: ";
char str_counter[] PROGMEM = "Counter: ";
char str_picture_check[] PROGMEM = "Picture check";
char str_checkerboard_1[] PROGMEM = "Checkerboard 1";
char str_checkerboard_2[] PROGMEM = "Checkerboard 2";
char str_dacs_check[] PROGMEM = "DACs check";
char str_completed[] PROGMEM = "Checks completed !";

//uint8_t lut_order[8] = { 0,2,4,6,7,5,3,1 };
uint8_t lut_order[8] = { 0,14,2,12,4,10,6,8 };

// Bit in byte to position in data word:
// 0: 0
// 1: 14
// 2: 2
// 3: 12
// 4: 4
// 5: 10
// 6: 6
// 7: 8

// 1133557766442200

// R,Y,G,C,B,M,W,I
uint8_t palette_letters[8 * 8] PROGMEM = {
	// R:
	0b01111111,
	0b00001001,
	0b00001001,
	0b00001001,
	0b00011001,
	0b00101001,
	0b01000110,
	0b00000000,

	// Y:
	0b00000011,
	0b00000100,
	0b00001000,
	0b01110000,
	0b00001000,
	0b00000100,
	0b00000011,
	0b00000000,

	// G:
	0b00011100,
	0b00100010,
	0b01000001,
	0b01000001,
	0b01010001,
	0b01010001,
	0b01110010,
	0b00000000,

	// C:
	0b00011100,
	0b00100010,
	0b01000001,
	0b01000001,
	0b01000001,
	0b01000001,
	0b00100010,
	0b00000000,

	// B:
	0b01111111,
	0b01001001,
	0b01001001,
	0b01001001,
	0b01001001,
	0b01001001,
	0b00110110,
	0b00000000,

	// M:
	0b01111111,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00000100,
	0b00000010,
	0b01111111,
	0b00000000,

	// W:
	0b00001111,
	0b00110000,
	0b01000000,
	0b00110000,
	0b01000000,
	0b00110000,
	0b00001111,
	0b00000000,

	// I:
	0b01000001,
	0b01000001,
	0b01000001,
	0b01111111,
	0b01000001,
	0b01000001,
	0b01000001,
	0b00000000,
};
