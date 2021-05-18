/*
 * Motors_CFG.h
 *
 * Created: 16/05/2021 3:48:48 PM
 *  Author: Mark Fayez
 *
 *	Pre-Build Configurations for Motors Module
 */ 


#ifndef MOTORS_CFG_H_
#define MOTORS_CFG_H_

#include "MCU.h"
#include "Bit_math.h"
#include "STD_DTypes.h"

#define MOTOR_A_SEL DISABLE
#define MOTOR_B_SEL ENABLE

#define SERVO_MOTOR_SEL ENABLE

typedef enum{
	NO_CURRENT_OUTPUT,
	CLOCK_WISE_DIR,
	ANTI_CLOCK_WISE_DIR,
	DC_MOTOR_STOP
}DCMotor_Dir;

#define H_BRIDGE_DIR_DIO_PORT	DIO_PORTC
#define H_BRIDGE_DIR_PINS_PORT	PORT_C
 
#define H_BRIDGE_DIR_PIN_1	PIN3
#define H_BRIDGE_DIR_PIN_2	PIN4
#define H_BRIDGE_DIR_PIN_3	PIN5
#define H_BRIDGE_DIR_PIN_4	PIN6

#define H_BRIDGE_DIR_MOTOR_A_MASK	0x18
#define H_BRIDGE_DIR_MOTOR_B_MASK	0x60
 
#define SERVO_DIR_DIO_PORT	DIO_PORTD
#define SERVO_PIN			PIN7

#define SERVO_OP_FREQ			50
#define SERVO_ANGLE_OFFSET		90 // Servo operational from -90 degrees till +90 degrees
#define SERVO_SLOPE				(float64)(0.0277777777777778) // slope is duty cycle difference divided by degree difference 5/180
#define SERVO_MIN_DUTY_CYCLE	5 // 5% is the Minimum duty cycle for degrees = -90 

#define SERVO_SIGNAL_PERIODIC_TIME	(uint32)((float64)1000/(2*SERVO_OP_FREQ))

#endif /* MOTORS_CFG_H_ */