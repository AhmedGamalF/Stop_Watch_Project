/****************************************************************************************
 *   Name: Ahmed Gamal Abdelrahman Nasr
 *
 *   Describtion : Stop_Watch by using Timers & Interrupts
 ***************************************************************************************/


# include <avr/io.h>
#include <avr/interrupt.h>
# include <util/delay.h> // for delay function

/* Global variables to increment the time and display */
unsigned char sec_enable1 = 0;
unsigned char sec_enable2 = 0;
unsigned char min_enable1 = 0;
unsigned char min_enable2 = 0;
unsigned char h_enable1 = 0;
unsigned char h_enable2 = 0;
/*Dispaly function*/
void Display(void);
/* INT0 Intialization Function */
void INT0_Inti (void)
{
	MCUCR |=(1<<ISC01) ; // tigger with the falling edge
	GICR |= (1<<INT0) ; // Enable INT0
	DDRD &= ~(1<<PD2);  // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2); //Activate the internal pull up resistor at PD2
}
/* ISR for INT0  */
ISR(INT0_vect)
{
	sec_enable1 = 0;
	sec_enable2 = 0;
	min_enable1 = 0;
	min_enable2 = 0;
	h_enable1 = 0;
	h_enable2 = 0;
	TCNT1 = 0; // to make sure that timer will recount from 0
	/* Only when the INT1 excuted INT0 will reset the Stop Watch
	 * To resume the Stop Watch  Enable the Clock */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
}
/* INT1 Intialization Function */
void INT1_Inti (void)
{
	MCUCR |=(1<<ISC01)|(1<<ISC11) ; // tigger with the raising edge
	GICR |= (1<<INT1) ; // Enable INT1
	DDRD &= ~(1<<PD3);  // Configure INT1/PD3 as input pin
}
/* ISR for INT1 */
ISR(INT1_vect)
{
	/* To Stop the Stop Watch  Disable the Clock */
	TCCR1B = 0;
}
/* INT2 Intialization Function */
void INT2_Inti (void)
{
	DDRB &= ~(1<<PB2);  // Configure INT2/PB2 as input pin
	PORTB |= (1<<PB2); //Activate the internal pull up resistor at PB2
	MCUCSR &= ~(1<<ISC2) ; // tigger with the falling edge
	GICR |= (1<<INT2) ; // Enable INT2

}
/* ISR for INT2  */
ISR(INT2_vect)
{
	/* To resume the Stop Watch  Enable the Clock */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
}

/*Timer1 Intialization Function*/
void Timer1_Init (void)
{
	TCNT1 = 0 ; /* Set timer1 initial count to zero */
	OCR1A = 1000 ; /* Set the Compare value  */
	TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
	 */
	TCCR1A = (1<<FOC1A);
	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Prescaler = F_CPU/1024 CS10=1 CS11=0 CS12=1
	 */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
}

/* ISR for timer1 compare mode */
ISR(TIMER1_COMPA_vect)
{

	if(sec_enable1 == 9)
	{
		if(sec_enable2 == 5)
		{
			if(min_enable1 == 9)
			{
				if(min_enable2 == 5)
				{
					if(h_enable1 == 9)
					{
						if(h_enable2 == 5)
						{
							/* if stop watch reaches its max stop timer1 */
							TCCR1B = 0;
						}
					} // end the fifth if (for Frist digit of hours )
					else
					{
						h_enable1++;
					}
					min_enable2 = 0;
				}// end fourth if (for second digit of minuites )
				else
				{
					min_enable2++;
				}
				min_enable1 = 0;
			} // end third if (for Frist digit of minuites )
			else
			{
				min_enable1++;
			}
			sec_enable2 = 0 ;
		} // end secnod if (for second digit of seconds )
		else
		{
			sec_enable2++;
		}
		sec_enable1 = 0 ;
	} // end frist if (for Frist digit of seconds )
	else
	{
		sec_enable1++;
	}
}
/*************************************** Main_Function**********************************/
int main()
{
	DDRC |= 0x0F ; // configuer pins PC0 --> PC3 as output pins
	DDRA |= 0x3F ; // configuer pins PA0 --> PA5 as output pins
	PORTC &= 0xF0 ; // intal value (0) to frist 4 pins
	PORTA &= 0xC0 ; // intail values to 6 pins
	SREG |= (1<<7); /* Enable I-Bit */

	Timer1_Init(); /* Start the timer */
	INT0_Inti(); /* Restart the Stop Watch */
	INT1_Inti(); /* Paused the Stop Watch*/
	INT2_Inti(); /* Resume the Stop Watch*/
	while(1)
	{
		Display(); // Display the time
	}
}
/* function to select the 7-segment to dispaly */
void Display(void)
{
	PORTA = ((PORTA & 0xC0) | (1<<PA0)) ; // Enable 1st 7-SEG
	PORTC = ((PORTC & 0xF0)|(sec_enable1 & 0x0F)); //display
	_delay_ms(2);

	PORTA = ((PORTA & 0xC0) | (1<<PA1)) ; // Enable 2nd 7-SEG
	PORTC = ((PORTC & 0xF0)|(sec_enable2 & 0x0F)); //display
	_delay_ms(2);

	PORTA = ((PORTA & 0xC0) | (1<<PA2)) ; // Enable 3rd 7-SEG
	PORTC = ((PORTC & 0xF0)|(min_enable1 & 0x0F)); //display
	_delay_ms(2);

	PORTA = ((PORTA & 0xC0) | (1<<PA3)) ; // Enable 4th 7-SEG
	PORTC = ((PORTC & 0xF0)|(min_enable2 & 0x0F)); //display
	_delay_ms(2);

	PORTA = ((PORTA & 0xC0) | (1<<PA4)) ; // Enable 5th 7-SEG
	PORTC = ((PORTC & 0xF0)|(h_enable1 & 0x0F)); //display
	_delay_ms(2);

	PORTA = ((PORTA & 0xC0) | (1<<PA5)) ; // Enable 6th 7-SEG
	PORTC = ((PORTC & 0xF0)|(h_enable2 & 0x0F)); //display
	_delay_ms(2);
}

