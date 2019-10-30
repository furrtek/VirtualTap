#include "tester.h"
#include <avr/pgmspace.h>

void serial_put(const char c) {
	while((UCSRA&(1<<UDRE)) == 0);
	UDR = c;
}

char serial_get() {
	while((UCSRA&(1<<RXC)) == 0);
	return UDR;
}

void serial_crlf() {
	serial_put(0x0D);
	serial_put(0x0A);
}

void serial_print(char * str, const uint8_t crlf) {
	char c;
	
	while ((c = pgm_read_byte(str++)))
		serial_put(c);
	
	if (crlf)
		serial_crlf();
}

void serial_hex_digit(uint8_t v) {
	v &= 15;
	if (v > 9)
		v += ('A' - 10);
	else
		v += '0';
	serial_put(v);
}

void serial_binary(const uint8_t v) {
	for (uint8_t bit = 0; bit < 8; bit++) {
		if ((v << bit) & 0x80)
			serial_put('1');
		else
			serial_put('0');
	}
}
