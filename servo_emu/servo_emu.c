// VirtualBoy servo board emulator
// For ATTiny25, internal RC osc, CKDIV8
// Part of the VirtualTap project
// 2018 Furrtek

// MCU     VB
// PB3(2): Pin 4 (clk)
// PB4(3): Pin 5 (data)
// GND(4): Pin 1
// PB0(5): Pin 8 (eye A sync feedback)
// PB1(6): Pin 9 (eye B sync feedback)
// PB2(7): Pin 6 (main 50Hz sync)
// VCC(8): Pin 3

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMING_DATA 0xA4	// Approx. value when oscillation is stable

uint8_t volatile data_step = 0xFF, data_sr;
uint16_t volatile tmr = 0;

// Interrupt occurs every 50us
ISR(TIM0_COMPA_vect) {
	tmr++;

	// Generate sync signals with appropriate phases
	// Measured from real servo board
	// Eyes: 8ms low, 12ms high
	// Sync: 10ms low, 10ms high
	if (tmr == 40) {
		PORTB &= ~2;	// Eye B feedback low
	} else if (tmr == 180) {
		PORTB |= 4;		// Main sync high
	} else if (tmr == 200) {
		PORTB |= 2;		// Eye B feedback high
	} else if (tmr == 210) {
		data_step = 0;	// Start sending frame duration
		data_sr = TIMING_DATA;
	} else if (tmr == 240) {
		PORTB &= ~1;	// Eye A feedback low
	} else if (tmr == 252) {
		data_step = 0;	// Start sending frame duration
		data_sr = TIMING_DATA;
	} else if (tmr == 380) {
		PORTB &= ~4;	// Main sync low
	} else if (tmr == 400) {
		PORTB |= 1;		// Eye A feedback high
		tmr = 0;		// Whole cycle is 400*50us = 20000us = 20ms (50Hz)
	}

	if (data_step != 0xFF) {
		// Send 8-bit frame duration

		// Clock
		if (data_step & 1) {
			PORTB |= 8;
		} else {
			PORTB &= ~8;
			// Data
			if (data_sr & 0x80)
				PORTB |= 16;
			else
				PORTB &= ~16;
			data_sr <<= 1;
		}

		if (data_step == 16)
			data_step = 0xFF;	// Done
		else
			data_step++;
	}
}

int main(void) {
	MCUSR &= ~(1<<WDRF);	// Watchdog sleepies -_-
	WDTCR = (1<<WDCE) | (1<<WDE);
	WDTCR = 0x00;

	DDRB = 0b00011111;		// PB0~PB4 as outputs
	PORTB = 0b00000000;

	TCCR0A = 0b00000010;	// CTC
	TCCR0B = 0b00000001;	// Prescaler = 1
	OCR0A = 49;				// 1M/1/(x+1) = 20kHz (50us)
	TIMSK = 0b00010000;		// Match A interrupt

	sei();

	for (;;) { }

    return 0;
}
