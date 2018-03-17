#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB = _BV(PB0); // LED connected to attiny2313 pin12 (PB0)

	for (;;) {
		PORTB |= _BV(PB0);
		_delay_ms(20);
		PORTB &= ~_BV(PB0);
		_delay_ms(1980);
	}

	return 1;
}
