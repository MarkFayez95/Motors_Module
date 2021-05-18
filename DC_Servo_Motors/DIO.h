/*
 * DIO.h
 *
 * Created: 20/03/2021 11:00:11 AM
 *  Author: Mark Fayez
 */ 

#ifndef DIO_H_
#define DIO_H_

#include "STD_DTypes.h"
#include "Bit_math.h"
#include "MCU.h"

#define DIO_PORTA 10
#define DIO_PORTB 20
#define DIO_PORTC 30
#define DIO_PORTD 40

#define DIO_PIN_0 0
#define DIO_PIN_1 1
#define DIO_PIN_2 2
#define DIO_PIN_3 3
#define DIO_PIN_4 4
#define DIO_PIN_5 5
#define DIO_PIN_6 6
#define DIO_PIN_7 7

#define DIO_PORT_OUTPUT 0xFF
#define	DIO_PORT_INPUT	0X00

#define DIO_PIN_OUTPUT 1
#define DIO_PIN_INPUT 0

#define DIO_PORT_HIGH 0xFF
#define	DIO_PORT_LOW	0X00

#define DIO_PIN_HIGH 1
#define DIO_PIN_LOW 0

void DIO_SetPortDir(uint8 Port, uint8 Dir);
void DIO_SetPinDir(uint8 Port, uint8 Pin, uint8 Dir);

void DIO_SetPortValue(uint8 Port, uint8 Value);
void DIO_SetPinValue(uint8 Port, uint8 Pin, uint8 Value);

void DIO_ReadPortValue(uint8 Port, uint8* Value);
void DIO_ReadPinValue(uint8 Port, uint8 Pin, uint8* Value);

void DIO_TogglePort(uint8 Port);
void DIO_TogglePin(uint8 Port, uint8 Pin);

void DIO_SetPortPullup(uint8 Port);
void DIO_SetPinPullup(uint8 Port, uint8 Pin);

#endif /* DIO_H_ */