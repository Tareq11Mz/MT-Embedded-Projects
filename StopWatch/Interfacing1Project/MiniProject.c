#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define delay_time 5 // Delay time for multiplexing (adjust as needed)

// Timing variables
unsigned char seconds = 0; // Seconds counter
unsigned char minutes = 0; // Minutes counter
unsigned char hrs = 0;     // Hours counter

// Interrupt Service Routine for Timer1 Compare Match A to increment time counters
ISR(TIMER1_COMPA_vect) {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
    }
    if (minutes == 60) {
        minutes = 0;
        hrs++;
    }
    if (hrs == 24) {
        hrs = 0;
    }
}

// Interrupt Service Routine for INT0 to reset the timer and counters
ISR(INT0_vect) {
    seconds = 0;
    minutes = 0;
    hrs = 0;
    TCNT1 = 0;             // Reset the timer counter
    TCCR1B |= (1 << CS12); // Resume the clock
}

// Interrupt Service Routine for INT1 to stop the timer
ISR(INT1_vect) {
    TCCR1B &= ~(1 << CS12); // Stop the clock
}

// Interrupt Service Routine for INT2 to resume the timer
ISR(INT2_vect) {
    TCCR1B |= (1 << CS12); // Resume the clock
}

// Initialize Timer1
void TIMER1_init(void) {
    TCCR1A = (1 << FOC1A) | (1 << FOC1B); // Non-PWM
    TCCR1B = (1 << WGM12) | (1 << CS12);  // CTC mode and prescale value of 256
    TCNT1 = 0;                           // Start from 0
    OCR1A = 3907;                        // Top value for 1 second counting
    TIMSK = (1 << OCIE1A);               // Enable timer interrupt
}

// Initialize INT1 for stop
void INT1_init(void) {
    DDRD &= ~(1 << PD3);  // Set PD3 as input
    MCUCR |= (1 << ISC11) | (1 << ISC10); // Generate interrupt on rising edge
    GICR |= (1 << INT1);   // Enable INT1
}

// Initialize INT2 for resume
void INT2_init(void) {
    DDRB &= ~(1 << PB2);  // Set PB2 as input
    PORTB |= (1 << PB2);  // Enable internal pull-down resistor
    MCUCSR &= ~(1 << ISC2); // Generate interrupt on falling edge
    GICR |= (1 << INT2);   // Enable INT2
}

// Initialize INT0 for reset
void INT0_init(void) {
    DDRD &= ~(1 << PD2);  // Set PD2 as input
    PORTD |= (1 << PD2);  // Enable internal pull-down resistor
    MCUCR |= (1 << ISC01); // Generate interrupt on falling edge
    GICR |= (1 << INT0);   // Enable INT0
}

int main(void) {
    SREG = (1 << 7);      // Enable global interrupt (I-bit)
    TIMER1_init();         // Initialize the timer
    INT0_init();           // Initialize INT0 for reset
    INT1_init();           // Initialize INT1 for stop
    INT2_init();           // Initialize INT2 for resume

    DDRA = 0x3F;           // Set PORTA as output (0 - 5)
    PORTA = 0xFF;          // Enable all 7 segments

    DDRC = 0x0F;           // Set first four pins in PORTC as outputs
    PORTC = 0;             // Display 0 initially on 7 segments

    _delay_us(delay_time);     // Delay for delay_time

    while (1) {
        // Displayment
        PORTA = 0x01;       // Enable 1st 7 segment
        PORTC = seconds % 10; // Display 1st digit of seconds
        _delay_us(delay_time); // Wait for 5 us

        PORTA = (PORTA << 1); // Enable 2nd 7 segment
        PORTC = (seconds) / 10; // Display 1st digit of seconds
        _delay_us(delay_time);    // Wait for 5 us

        PORTA = (PORTA << 1); // Enable 3rd 7 segment
        PORTC = minutes % 10; // Display 1st digit of minutes
        _delay_us(delay_time);    // Wait for 5 us

        PORTA = (PORTA << 1); // Enable 4th 7 segment
        PORTC = (minutes) / 10; // Display 2nd digit of minutes
        _delay_us(delay_time);    // Wait for 5 us

        PORTA = (PORTA << 1); // Enable 5th 7 segment
        PORTC = hrs % 10;     // Display 1st digit of hours
        _delay_us(delay_time);    // Wait for 5 us

        PORTA = (PORTA << 1); // Enable 6th 7 segment
        PORTC = (hrs) / 10;   // Display 2nd digit of hours
        _delay_us(delay_time);    // Wait for 5 us
    }

    return 0;
}
