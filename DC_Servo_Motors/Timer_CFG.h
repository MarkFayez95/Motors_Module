/*
 * Timer_CFG.h
 *
 * Created: 16/05/2021 4:04:53 PM
 *  Author: Mark Fayez
 * 
 * Pre-Build configurations for Timer Driver
 */ 


#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

#include "Bit_math.h"
#include "MCU.h"
#include "STD_DTypes.h"

#define CLK_FREQ	16 // Clock Frequency in Mega-Hertz

#define PWM1_TOP_VALUE	1250

#define TIMER0_GEN_INT		DISABLE
#define TIMER0_OVF_INT		DISABLE
#define TIMER0_COMP_INT		DISABLE

#define TIMER1_GEN_INT		DISABLE
#define TIMER1_OVF_INT		DISABLE
#define TIMER1_COMP_A_INT	DISABLE
#define TIMER1_COMP_B_INT	DISABLE

#define TIMER2_GEN_INT	ENABLE
#define TIMER2_OVF_INT	ENABLE
#define TIMER2_COMP_INT	ENABLE

#define PWM0_PORT_DIR_REG	(PORT_B->DDR)
#define PWM0_OC0_PIN		PIN3

#define PWM1_A_PORT_DIR_REG	(PORT_D->DDR)
#define PWM1_A_OC1A_PIN		PIN5
#define PWM1_B_PORT_DIR_REG	(PORT_D->DDR)
#define PWM1_B_OC1B_PIN		PIN4

#define PWM2_PORT_DIR_REG	(PORT_D->DDR)
#define PWM2_OC2_PIN		PIN7

#endif /* TIMER_CFG_H_ */