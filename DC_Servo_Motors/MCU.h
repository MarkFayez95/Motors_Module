/*
 * MCU.h
 *
 * Created: 18/03/2021 10:05:01 PM
 *  Author: Mark Fayez
 */ 


#ifndef MCU_H_
#define MCU_H_

#include "STD_DTypes.h"

/*************************************** Two-wire Serial Interface **********************************/

#define TWBR    (*(volatile uint8*)0x20)
#define TWSR    (*(volatile uint8*)0x21)
#define TWPS0   0
#define TWPS1   1
#define TWPS3   3
#define TWPS4   4
#define TWPS5   5
#define TWPS6   6
#define TWPS7   7

#define TWAR    (*(volatile uint8*)0x22)
#define TWGCE   0
#define TWA0    1
#define TWA1    2
#define TWA2    3
#define TWA3    4
#define TWA4    5
#define TWA5    6
#define TWA6    7

#define TWDR    (*(volatile uint8*)0x23)

/*************************************** ADC Registers **********************************/
#define ADCL	(*(volatile uint8*)0x24)
#define ADC_VAL	(*(volatile uint16*)0x24)
#define ADCH	(*(volatile uint8*)0x25)
#define ADCSRA	(*(volatile uint8*)0x26)
#define ADIE	3
#define ADIF	4
#define ADATE	5
#define ADSC	6
#define ADEN	7

#define ADMUX	(*(volatile uint8*)0x27)
#define ADLAR	5
#define REFS0	6

#define ACSR    (*(volatile uint8*)0x28)
#define ACIS0   0
#define ACIS1   1
#define ACIC    2
#define ACIE    3
#define ACI     4
#define ACO     5
#define ACBG    6
#define ADC     7

/*************************************** UART Registers **********************************/
#define UBRRL   (*(volatile uint8*)0x29)
#define UCSRB   (*(volatile uint8*)0x2A)
#define TXB8    0
#define RXB8    1
#define UCSZ2   2
#define TXEN    3
#define RXEN    4
#define UDRIE   5
#define TXCIE   6
#define RXCIE   7

#define UCSRA   (*(volatile uint8*)0x2B)
#define MPCM    0
#define U2X     1
#define PE      2
#define DOR     3
#define FE      4
#define UDRE    5
#define TXC     6
#define RXC     7

#define UDR     (*(volatile uint8*)0x2C)

/*************************************** SPI Registers **********************************/
// SPCR bits
#define SPR0    0
#define SPR1    1
#define CPHA    2
#define CPOL    3
#define MSTR    4
#define DORD    5
#define SPE     6
#define SPIE    7

// SPSR bits
#define SPI2X   0
#define WCOL    6
#define SPIF    7

typedef struct 
{
	uint8 SPCR;
	uint8 SPSR;
	uint8 SPDR;
}SPI_REG;

#define SPI ((volatile SPI_REG*) 0x2D)

/*************************************** DIO Ports Registers **********************************/

#define PIND    (*(volatile uint8*)0x30)
#define DDRD    (*(volatile uint8*)0x31)
#define PORTD   (*(volatile uint8*)0x32)

#define PINC    (*(volatile uint8*)0x33)
#define DDRC    (*(volatile uint8*)0x34)
#define PORTC   (*(volatile uint8*)0x35)

#define PINB    (*(volatile uint8*)0x36)
#define DDRB    (*(volatile uint8*)0x37)
#define PORTB   (*(volatile uint8*)0x38)

#define PINA    (*(volatile uint8*)0x39)
#define DDRA    (*(volatile uint8*)0x3A)
#define PORTA   (*(volatile uint8*)0x3B)

#define PIN0	0
#define PIN1	1
#define PIN2	2
#define PIN3	3
#define PIN4	4
#define PIN5	5
#define PIN6	6
#define PIN7	7

typedef struct
{
	volatile uint8 PIN;
	volatile uint8 DDR;
	volatile uint8 PORT;
}PORT_REG;

#define PORT_D	((volatile PORT_REG*)0x30)
#define PORT_C	((volatile PORT_REG*)0x33)
#define PORT_B	((volatile PORT_REG*)0x36)
#define PORT_A ((volatile PORT_REG*)0x39)

/*************************************** EEPROM Registers **********************************/

#define EECR        (*(volatile uint8*)0x3C)
#define EERE        0
#define EEWE        1
#define EEMWE       2
#define EERIE       3

#define EEDR        (*(volatile uint8*)0x3D)

#define EEAR_VAL    (*(volatile uint16*)0x3E)
#define EEARL       (*(volatile uint8*)0x3E)
#define EEARH       (*(volatile uint8*)0x3F)
#define EEAR8       0
#define EEAR9       1

/*************************************** USART Register **********************************/
#define UBRRH		(*(volatile uint8*)0x40)

#define UCSRC		(*(volatile uint8*)0x40)
#define UCPOL           0
#define UCSZ0           1
#define UCSZ1           2
#define USBS            3
#define UPM0            4
#define UPM1            5
#define UMSEL           6
#define UCSRC_URSEL     7


/*************************************** WDT Register **********************************/

#define WDTCR           (*(volatile uint8*)0x41)
#define WDP0            0
#define WDP1            1
#define WDP2            2
#define WDE             3
#define WDTOE           4

/*************************************** Timer 2 Registers **********************************/

#define ASSR            (*(volatile uint8*)0x42)
#define TCR2UB          0
#define OCR2UB          1
#define TCN2UB          2
#define AS2             3

#define OCR2            (*(volatile uint8*)0x43)
#define TCNT2           (*(volatile uint8*)0x44)
#define TCCR2           (*(volatile uint8*)0x45)
#define CS20            0
#define CS21            1
#define CS22            2
#define WGM21           3
#define COM20           4
#define COM21           5
#define WGM20           6
#define FOC2            7

/*************************************** Timer 1 Registers **********************************/

#define ICR1_VAL    (*(volatile uint16*)0x46)
#define ICR1L       (*(volatile uint8*)0x46)
#define ICR1H       (*(volatile uint8*)0x47)

#define OCR1B_VAL   (*(volatile uint16*)0x48)
#define OCR1BL      (*(volatile uint8*)0x48)
#define OCR1BH      (*(volatile uint8*)0x49)

#define OCR1A_VAL   (*(volatile uint16*)0x4A)
#define OCR1AL      (*(volatile uint8*)0x4A)
#define OCR1AH      (*(volatile uint8*)0x4B)

#define TCNT1_VAL   (*(volatile uint16*)0x4C)
#define TCNT1L      (*(volatile uint8*)0x4C)
#define TCNT1H      (*(volatile uint8*)0x4D)

#define TCCR1B      (*(volatile uint8*)0x4E)
#define CS10        0
#define CS11        1
#define CS12        2
#define WGM12       3
#define WGM13       4
#define ICES1       6
#define ICNC1       7

#define TCCR1A      (*(volatile uint8*)0x4F)
#define WGM10       0
#define WGM11       1
#define FOC1B       2
#define FOC1A       3
#define COM1B0      4
#define COM1B1      5
#define COM1A0      6
#define COM1A1      7

/*************************************** Special Function Register **********************************/
#define SFIOR	        (*(volatile uint8*) 0x50)
#define PSR10			0
#define PSR2			1
#define PUD				2
#define ACME			3
#define SFIOR_Res_Bit	4
#define ADTS			5


#define OCDR        (*(volatile uint8*)0x51)
#define OSCCAL      (*(volatile uint8*)0x51)

/*************************************** Timer 0 Registers **********************************/

#define TCNT0	(*(volatile uint8*)0x52)

#define TCCR0	(*(volatile uint8*)0x53)
#define FOC0	7
#define WGM00	6
#define COM01	5
#define COM00	4
#define WGM01	3
#define CS02	2
#define CS01	1
#define CS00	0

#define MCUCSR	(*(volatile uint8*)0x54)
#define MCUCR	(*(volatile uint8*)0x55)
#define TWCR    (*(volatile uint8*)0x56)
#define TWIE    0
#define TWEN    2
#define TWWC    3
#define TWSTO   4
#define TWSTA   5
#define TWEA    6
#define TWINT   7

#define SPMCR   (*(volatile uint8*)0x57)
#define SPMEN   0
#define PGERS   1
#define PGWRT   2
#define BLBSET  3
#define RWWSRE  4
#define RWWSB   6
#define SPMIE   7

#define OCR0	(*(volatile uint8*)0x5C)

/***************************Timer Interrupt Mask Register***************************/
#define TIFR	(*(volatile uint8*)0x58)
#define TOV0	0
#define OCF0	1
#define TOV1	2
#define OCF1B	3
#define OCF1A	4
#define ICF1	5
#define	TOV2	6
#define OCF2	7

#define TIMSK	(*(volatile uint8*)0x59)
#define TOIE0	0
#define OCIE0	1
#define TOIE1	2
#define OCIE1B	3
#define OCIE1A	4
#define TICIE1	5
#define TOIE2	6
#define OCIE2	7

/*************************************** SP Register **********************************/

#define SP_VAL      (*(volatile uint16*)0x5D)
#define SPL         (*(volatile uint8*)0x5D)
#define SPH         (*(volatile uint8*)0x5E)

/*********************************** Interrupts Registers ********************************/

#define GIFR	(*(volatile uint8*)0x5A)
#define GICR	(*(volatile uint8*)0x5B)

#define SREG	(*(volatile uint8*)0x5F)
#define SREG_I	7

#endif /* MCU_H_ */