#define __AVR_ATmega32U4__ 1

#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/* You could use cbi ie &= ~ or sbi ie |= but this makes code more readable. */
void digital_write(volatile uint8_t *data_port, int pin, int value) {
	if (0 == value) {
		*data_port &= ~(_BV(pin));
	} else {
		*data_port |= _BV(pin);
	}
}

void send_str(char source[]) {
	char* c = source;
	while (*c) {
		usb_serial_putchar(*c++);
	}
}

static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}

char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}

void init(void) {
	CPU_PRESCALE(0);
	usb_init();

	/* Make PORTB5 (Arduino digital 13) an output by setting bit in DDR. */
	DDRC |= _BV(PC7); // internal led

	DDRD |= _BV(PD5); // TX led
	DDRB |= _BV(PB0); // RX led
}

int main(void) {
	init();
	int i = 0;
	char str[10];

	for (;;) {
		if(usb_configured()){// begin USBSerial operation only when USB ready
			// discard anything that was received prior.  Sometimes the
			// operating system or other software will send a modem
			// "AT command", which can still be buffered.
			usb_serial_flush_input();

			// wait for the user to run their terminal emulator program
			// which sets DTR to indicate it is ready to receive.
			if ((usb_serial_get_control() & USB_SERIAL_DTR)) {
				send_str("Hello world: ");
				itoa_simple(str, i - 100);
				send_str(str);
				send_str("\r\n");
			}
		}

		digital_write(&PORTC, PC7, 1);
		_delay_ms(100);
		digital_write(&PORTC, PC7, 0);
		digital_write(&PORTD, PD5, 1);
		_delay_ms(100);
		digital_write(&PORTD, PD5, 0);
		digital_write(&PORTB, PB0, 1);
		_delay_ms(100);
		digital_write(&PORTB, PB0, 0);

		digital_write(&PORTC, PC7, 0);
		_delay_ms(900);

		i++;
	}

	return 0;

}
