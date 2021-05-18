/*
 * DIO.c
 *
 * Created: 20/03/2021 11:00:23 AM
 *  Author: Mark Fayez
 */ 
#include "DIO.h"

void DIO_SetPortDir(uint8 Port, uint8 Dir)
{
	switch (Port)
	{
		case DIO_PORTA:
			DDRA = Dir;
		break;
		
		case DIO_PORTB:
			DDRB = Dir;
		break;
		
		case DIO_PORTC:
			DDRC = Dir;
		break;
		
		case DIO_PORTD:
			DDRD = Dir;
		break;
		
		default:
		break;
	}
}
void DIO_SetPinDir(uint8 Port, uint8 Pin, uint8 Dir)
{
	if(Dir == DIO_PIN_OUTPUT )
	{
		switch (Port)
		{
			case DIO_PORTA:
			SetBit(DDRA , Pin);
			break;
			
			case DIO_PORTB:
			SetBit(DDRB , Pin);
			break;
			
			case DIO_PORTC:
			SetBit(DDRC , Pin);
			break;
			
			case DIO_PORTD:
			SetBit(DDRD , Pin);
			break;
			
			default:
			break;
		}
	}
	else if(Dir == DIO_PIN_INPUT)
	{
		switch (Port)
		{
			case DIO_PORTA:
			ClearBit(DDRA , Pin);
			break;
			
			case DIO_PORTB:
			ClearBit(DDRB , Pin);
			break;
			
			case DIO_PORTC:
			ClearBit(DDRC , Pin);
			break;
			
			case DIO_PORTD:
			ClearBit(DDRD , Pin);
			break;
			
			default:
			break;
		}
	}
}

void DIO_SetPortValue(uint8 Port, uint8 Value)
{
	switch (Port)
	{
		case DIO_PORTA:
		PORTA = Value;
		break;
		
		case DIO_PORTB:
		PORTB = Value;
		break;
		
		case DIO_PORTC:
		PORTC = Value;
		break;
		
		case DIO_PORTD:
		PORTD = Value;
		break;
		
		default:
		break;
	}
}
void DIO_SetPinValue(uint8 Port, uint8 Pin, uint8 Value)
{
	if(Value == DIO_PIN_HIGH )
	{
		switch (Port)
		{
			case DIO_PORTA:
			SetBit(PORTA , Pin);
			break;
			
			case DIO_PORTB:
			SetBit(PORTB , Pin);
			break;
			
			case DIO_PORTC:
			SetBit(PORTC , Pin);
			break;
			
			case DIO_PORTD:
			SetBit(PORTD , Pin);
			break;
			
			default:
			break;
		}
	}
	else if(Value == DIO_PIN_LOW)
	{
		switch (Port)
		{
			case DIO_PORTA:
			ClearBit(PORTA , Pin);
			break;
			
			case DIO_PORTB:
			ClearBit(PORTB , Pin);
			break;
			
			case DIO_PORTC:
			ClearBit(PORTC , Pin);
			break;
			
			case DIO_PORTD:
			ClearBit(PORTD , Pin);
			break;
			
			default:
			break;
		}
	}
}

void DIO_ReadPortValue(uint8 Port, uint8* Value)
{
	switch (Port)
	{
		case DIO_PORTA:
		*Value = PINA;
		break;
		
		case DIO_PORTB:
		*Value = PINB;
		break;
		
		case DIO_PORTC:
		*Value = PINC;
		break;
		
		case DIO_PORTD:
		*Value = PIND;
		break;
		
		default:
		break;
	}
}
void DIO_ReadPinValue(uint8 Port, uint8 Pin, uint8* Value)
{
	switch (Port)
	{
		case DIO_PORTA:
		*Value = GetBit(PINA,Pin);
		break;
		
		case DIO_PORTB:
		*Value = GetBit(PINB,Pin);
		break;
		
		case DIO_PORTC:
		*Value = GetBit(PINC,Pin);
		break;
		
		case DIO_PORTD:
		*Value = GetBit(PIND,Pin);
		break;
		
		default:
		break;
	}
}

void DIO_TogglePort(uint8 Port)
{
	switch (Port)
	{
		case DIO_PORTA:
		PORTA ^= 0xFF;
		break;
		
		case DIO_PORTB:
		PORTB ^= 0xFF;
		break;
		
		case DIO_PORTC:
		PORTC ^= 0xFF;
		break;
		
		case DIO_PORTD:
		PORTD ^= 0xFF;
		break;
		
		default:
		break;
	}
}
void DIO_TogglePin(uint8 Port, uint8 Pin)
{
	switch (Port)
	{
		case DIO_PORTA:
		ToggleBit(PORTA , Pin);
		break;
		
		case DIO_PORTB:
		ToggleBit(PORTB , Pin);
		break;
		
		case DIO_PORTC:
		ToggleBit(PORTC , Pin);
		break;
		
		case DIO_PORTD:
		ToggleBit(PORTD , Pin);
		break;
		
		default:
		break;
	}
}

void DIO_SetPortPullup(uint8 Port)
{
	switch (Port)
	{
		case DIO_PORTA:
		PORTA = DIO_PORT_HIGH;
		break;
		
		case DIO_PORTB:
		PORTB = DIO_PORT_HIGH;
		break;
		
		case DIO_PORTC:
		PORTC = DIO_PORT_HIGH;
		break;
		
		case DIO_PORTD:
		PORTD = DIO_PORT_HIGH;
		break;
		
		default:
		break;
	}
}
void DIO_SetPinPullup(uint8 Port, uint8 Pin)
{
	switch (Port)
	{
		case DIO_PORTA:
			SetBit(PORTA,Pin);
		break;
		
		case DIO_PORTB:
			SetBit(PORTB,Pin);
		break;
		
		case DIO_PORTC:
			SetBit(PORTC,Pin);
		break;
		
		case DIO_PORTD:
			SetBit(PORTD,Pin);
		break;
		
		default:
		break;
	}
}