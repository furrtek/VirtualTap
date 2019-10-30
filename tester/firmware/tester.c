// Virtualtap tester firmware v1
// 10/2018 furrtek
// ATMega8 16MHz

// TODO: Test 5V in and 1.65V in ?
// TODO: Test palette switch debounce ?
// TODO: Test buffer mode with rapid frame blinks ?

#include "tester.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io.h"
#include "uart.h"
#include "hw.h"

int main(void) {
	uint8_t b, c, p;
	uint16_t x, y, va, vb;

	WDTCR = _BV(WDCE) | _BV(WDE);	// Watchdog off
	WDTCR = 0x00;

	DDRB = 0b00000000;
	DDRC = 0b00111111;
	DDRD = 0b00111110;

	PORTB = 0;
	PORTC = _BV(OE_LOW) | _BV(OE_CTRL);
	PORTD = _BV(OE_OUT) | _BV(OE_HIGH);

    #define BAUD 38400
    #include <util/setbaud.h>
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
    	UCSRA |= _BV(U2X);
    #else
    	UCSRA &= ~_BV(U2X);
    #endif
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
	UCSRB = _BV(TXEN) | _BV(RXEN); 

	serial_print(str_hello, 1);

	for(;;) {
		serial_print(str_instructions, 1);
		serial_get();
		serial_print(str_conn_check, 1);

		serial_print(str_all_ones, 0);
		c_test(0xFF, 1);
		serial_print(str_all_zeros, 0);
		c_test(0x00, 1);
		serial_print(str_alt_1, 0);
		c_test(0x55, 1);
		serial_print(str_alt_2, 0);
		c_test(0xAA, 1);
		serial_print(str_counter, 0);
		for (c = 0; c < 255; c++) {
			if ((c & 0x0F) == 0x00)
				serial_hex_digit(c >> 4);
			if (c_test(c, 0))
				break;
		}
		serial_crlf();

		serial_print(str_picture_check, 1);

		serial_print(str_checkerboard_1, 1);
		for (b = 0; b < 4; b++) {
			for (x = 0; x < 384; x++) {
				for (y = 0; y < 28; y++) {
					if (x & 8) {
						if (y & 1)
							p_shift(0xFFFF);
						else
							p_shift(0x0000);
					} else {
						if (y & 1)
							p_shift(0x0000);
						else
							p_shift(0xFFFF);
					}
				}
			}
			latch_out(LE_CTRL, 0b00000000);	// CS low, shift low
			_delay_ms(1);
		}
		//serial_get();
		_delay_ms(500);

		serial_print(str_checkerboard_2, 1);
		for (b = 0; b < 4; b++) {
			for (x = 0; x < 384; x++) {
				for (y = 0; y < 28; y++) {
					if (x & 8) {
						if (y & 1)
							p_shift(0x0000);
						else
							p_shift(0xFFFF);
					} else {
						if (y & 1)
							p_shift(0xFFFF);
						else
							p_shift(0x0000);
					}
				}
			}
			latch_out(LE_CTRL, 0b00000000);	// CS low, shift low
			_delay_ms(1);
		}
		//serial_get();
		_delay_ms(500);

		serial_print(str_counter, 1);
		for (b = 0; b < 4; b++) {
			for (x = 0; x < (384 * 28); x++)
				p_shift(x);
			latch_out(LE_CTRL, 0b00000000);	// CS low, shift low
			_delay_ms(1);
		}
		//serial_get();
		_delay_ms(500);

		serial_print(str_dacs_check, 1);
		for (p = 0; p < 8; p++) {
			serial_hex_digit(p);
			for (b = 0; b < 4; b++) {
				for (x = 0; x < 384; x++) {
					for (y = 0; y < 7; y++)
						p_shift(0xFFFF);		// 11 3/3 brightness
					for (y = 0; y < 7; y++)
						p_shift(0x55AA);		// 10 2/3 brightness
					for (y = 0; y < 8; y++)
						p_shift(0xAA55);		// 01 1/3 brightness
					// 1bpp to 2bpp
					c = pgm_read_byte(&palette_letters[((x >> 1) & 7) + (p << 3)]);
					va = 0;
					for (y = 0; y < 4; y++) {
						if (c & (1 << y)) {
							va |= (3 << lut_order[y*2]);
							va |= (3 << lut_order[y*2 + 1]);
						}
					}
					vb = 0;
					for (y = 0; y < 4; y++) {
						if (c & (16 << y)) {
							vb |= (3 << lut_order[y*2]);
							vb |= (3 << lut_order[y*2 + 1]);
						}
					}
					for (y = 0; y < 3; y++) {
						p_shift(va);
						p_shift(vb);
					}
				}
				latch_out(LE_CTRL, 0b00000000);	// CS low, shift low
				_delay_ms(1);
			}
			//serial_get();
			_delay_ms(500);

			// Tick palette
			PORTD |= _BV(PAL);
			_delay_ms(20);
			PORTD &= ~_BV(PAL);
			_delay_ms(20);
		}
		// Output disable
		PORTD |= _BV(OE_OUT);

		serial_crlf();
		serial_print(str_completed, 1);
		serial_crlf();
	}

    return 0;
}
