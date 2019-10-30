#include "tester.h"
#include <util/delay.h>
#include "io.h"
#include "uart.h"

// Use LE_LOW, LE_HIGH, or LE_CTRL
void latch_out(const uint8_t latch, const uint8_t data) {
	// Data bus as output
	DDRB |= DATA_MASK_PB;
	DDRD |= DATA_MASK_PD;

	// Put data on bus
	PORTB = (PORTB & ~DATA_MASK_PB) | (data & DATA_MASK_PB);
	PORTD = (PORTD & ~DATA_MASK_PD) | (data & DATA_MASK_PD);

	// Clock output latch
	PORTC |= _BV(latch);
	//_delay_us(1);
	PORTC &= ~_BV(latch);
	//_delay_us(1);

	// Data bus as input
	DDRB &= ~DATA_MASK_PB;
	DDRD &= ~DATA_MASK_PD;
}

void latch_in(uint8_t * buffer) {
	// Data bus as input
	DDRB &= ~DATA_MASK_PB;
	DDRD &= ~DATA_MASK_PD;

	// Clock input latches
	PORTC |= _BV(LE_IN);
	_delay_us(10);
	PORTC &= ~_BV(LE_IN);
	_delay_us(10);

	// Read low
	PORTC &= ~_BV(OE_LOW);
	_delay_us(10);
	*(buffer++) = (PINB & DATA_MASK_PB) | (PIND & DATA_MASK_PD);
	PORTC |= _BV(OE_LOW);

	// Read high
	PORTD &= ~_BV(OE_HIGH);
	_delay_us(10);
	*(buffer++) = (PINB & DATA_MASK_PB) | (PIND & DATA_MASK_PD);
	PORTD |= _BV(OE_HIGH);

	// Read ctrl
	PORTC &= ~_BV(OE_CTRL);
	_delay_us(10);
	*(buffer++) = (PINB & DATA_MASK_PB) | (PIND & DATA_MASK_PD);
	PORTC |= _BV(OE_CTRL);
}

// Returns 0 on pass
uint8_t c_test(const uint8_t v, const uint8_t pr) {
	uint8_t in_data[3];	// Low, high, ctrl

	latch_out(LE_LOW, v);
	latch_out(LE_HIGH, v);
	latch_out(LE_CTRL, v);

	// Output enable
	PORTD &= ~_BV(OE_OUT);

	_delay_us(100);
	latch_in(in_data);

	// Output disable
	PORTD |= _BV(OE_OUT);

	if ((in_data[0] == v) && (in_data[1] == v) && (in_data[2] == v)) {
		if (pr)
			serial_print(str_pass, 1);
		return 0;
	} else {
		if (pr) {
			serial_print(str_fail, 1);
			serial_print(str_bitmap, 1);
			serial_print(str_expected, 0);
			serial_binary(v);
			serial_put(' ');
			serial_binary(v);
			serial_put(' ');
			serial_binary(v);
			serial_crlf();
			serial_print(str_got, 0);
			serial_binary(in_data[0]);
			serial_put(' ');
			serial_binary(in_data[1]);
			serial_put(' ');
			serial_binary(in_data[2]);
			serial_crlf();
		}
		return 1;
	}
}

void p_shift(const uint16_t v) {
	latch_out(LE_HIGH, v >> 8);
	latch_out(LE_LOW, v & 0xFF);
	// Output enable
	PORTD &= ~_BV(OE_OUT);
	// CS high
	latch_out(LE_CTRL, 0b00000001);
	//_delay_us(1);
	// CS high, shift high
	latch_out(LE_CTRL, 0b00000101);
	//_delay_us(1);
	// CS high, shift low
	latch_out(LE_CTRL, 0b00000001);
	//_delay_us(1);
}

